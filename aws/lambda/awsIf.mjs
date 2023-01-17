import * as AWS from "@aws-sdk/client-dynamodb";
import { DynamoDBDocumentClient } from "@aws-sdk/lib-dynamodb";
import { IoTDataPlane } from '@aws-sdk/client-iot-data-plane';


var ddb;

export async function SendMqttResponse(topicName, data)
{ 
    const IoTClient = new IoTDataPlane({
      region: 'ap-south-1',
      endpoint: 'https://avruhytgmbwix-ats.iot.ap-south-1.amazonaws.com'
    });

    var mqttParams = {
        topic: topicName,
        payload: data,
        qos: 1
    };

    await IoTClient.publish(mqttParams);
}

export async function GetNodeData(projectName, deviceID) {
    
    var rawData;
    var data = {};
    
    // Create the DynamoDB service object
    ddb = new AWS.DynamoDB({apiVersion: '2012-10-08'});

    var params = {
      TableName: 'iotData',
      Key: {
       "prj" : {S: projectName},
       "id": { S: deviceID }
      }
    };

    try {
        rawData = await getItemDDB(params);

        rawData = rawData.Item;
        
        for (var key in rawData) {
            data[key] = rawData[key][Object.keys(rawData[key])[0]]; // Get the first object key value
            
            if(key == "iotdata") {
                // JSON Parse the string
                if(typeof data[key] === "string") // data is Json string
                {
                    data[key] = JSON.parse(data[key]);
                }
            }
        }
        
    } catch (e) {
        console.log(e);
        data = null;
    }
    
    return data;
}

function getItemDDB(params) {
    
    // return ddb.get(params).promise();
    
    return new Promise((resolve, reject) => {
        // Do Async Job
        ddb.getItem(params, function(err, data) {
          if (err) reject(err); // an error occurred
          else     resolve(data);           // successful response
        });
    });
}

export async function GetMultiNodeData(projectName, nodeIDList) {
    
    var rawData;
    let retval = [];
    
    // Create the DynamoDB service object
    ddb = new AWS.DynamoDB({apiVersion: '2012-10-08'});

    var keys = [];
    for(var index in nodeIDList) {
        var key = {};
        key.prj = { S: projectName };
        key.id = { S: nodeIDList[index] };
        keys.push(key);
    }

    var params = {
      RequestItems: {
        'iotData' : {
          Keys: keys
        }
      }
    };

    
    try {
        rawData = await getBatchItemDDB(params);

        rawData = rawData.Responses.FFU_New;
        
        for(var i in rawData) {
            var obj = rawData[i];

            var data = {};
            for (var key in obj) {
                data[key] = obj[key][Object.keys(obj[key])[0]]; // Get the first object key value
                
                if(key == "iotdata") {
                    // JSON Parse the string
                    if(typeof data[key] === "string")
                    {
                        data[key] = JSON.parse(data[key]);
                    }
                }
            }
            
            retval.push(data);
        }
    } catch (e) {
        console.log(e);
        retval = null;
    }
 
    return retval;
}

function getBatchItemDDB(params) {
    
    // return ddb.get(params).promise();
    
    return new Promise((resolve, reject) => {
        // Do Async Job
        ddb.batchGetItem(params, function(err, data) {
          if (err) reject(err); // an error occurred
          else     resolve(data);           // successful response
        });
    });
}

export async function SetNodeData(CurrentNodeData, UpdatedNodeData) {
    
    var rawData;
    var data = {};

    // Create the DynamoDB service object
    ddb = new AWS.DynamoDB({apiVersion: '2012-10-08'});
    
    var updateExpression = "set ";
    var expressionAttributes = {};
    
    var len = Object.keys(UpdatedNodeData).length;

    var i = 0;
    for (var key in UpdatedNodeData) {
        
        var ValidKey = true;
        
        // JSON Attributes
        if(key == "iotdata") {
            if(typeof UpdatedNodeData[key] === "object")
            {
                // If already parsed data was available, then stringify it
                updateExpression += key + " = :" + key;
                expressionAttributes[":" + key] = { "S" : JSON.stringify(UpdatedNodeData[key]) };
            }
            else if(typeof UpdatedNodeData["key"] == "string")
            {
                updateExpression += key + " = :" + key;
                expressionAttributes[":" + key] = { "S" : UpdatedNodeData[key] };
            }
        }
        else {
            // Do Nothing if the key is ID (This is the primary key and will not needs to be included)
            ValidKey = false;
        }
        
        if((i < (len - 1)) && (ValidKey)) {
            updateExpression += ", ";
        }
        
        i++;
    }

    var params = {
        TableName: "iotData",
        Key:{
            "prj" : {S: UpdatedNodeData.prj},
            "id": { S: UpdatedNodeData.id }
        },
        UpdateExpression: updateExpression,
        ExpressionAttributeValues: expressionAttributes,
        ReturnValues:"ALL_NEW"
    };
    
    try {
        rawData = await updateItemDDB(params);
        
        rawData = rawData.Attributes;
        
        for (var key in rawData) {
            data[key] = rawData[key][Object.keys(rawData[key])[0]]; // Get the first object key value
            
            if(key == "iotdata") {
                // JSON Parse the string
                if(typeof data[key] === "string")
                {
                    data[key] = JSON.parse(data[key]);
                }
            }
        }
        
    } catch (e) {
        console.log(e);
        data = null;
    }
    
    return data;
}

function updateItemDDB(params) {
    return new Promise((resolve, reject) => {
        // Do Async Job
        ddb.updateItem(params, function(err, data) {
          if (err){
              reject(err); // an error occurred
          } 
          else {
              resolve(data); // successful response
          }
        });
        
    });
}

export async function ScanDatabase(projectName)
{
    // If projectName is null, then scan all items in the table
    
    let retval = [];
    
    // Create the DynamoDB service object
    const docClient = new AWS.DynamoDBClient({
        apiVersion: '2012-10-08',
        sslEnabled: false,
        paramValidation: false,
        convertResponseTypes: false
    });

    var params;

    if(projectName != null) {
        params = {
            TableName: "iotData",
            FilterExpression: "#prj = :prj_val",
            ExpressionAttributeNames: {
                "#prj": "prj",
            },
            ExpressionAttributeValues: { ":prj_val": { "S" : projectName } }
        };
    }
    else {
        params = {
          TableName: 'iotData' // Table name is enough for scanning the entire table
        };
    }

    try {
        
        var rawData;
        
        do
        {
            const command = new AWS.ScanCommand(params);
            
            console.log(params);
            
            rawData = await docClient.send(command);
            
            if(rawData.Items != null)
            {
                rawData.Items.forEach(item => {
                    
                    var data = {};
                    
                    for (var key in item) {

                        data[key] = item[key];
                        
                        if(key == "iotdata") {
                            // JSON Parse the string
                            if(typeof data[key] === "string") // data is Json string
                            {
                                data[key] = JSON.parse(data[key]);
                            }
                        }
                    }
                    
                    retval.push(data);
                });
            }
            
            params.ExclusiveStartKey  = rawData.LastEvaluatedKey;
        }
        while(typeof rawData.LastEvaluatedKey !== "undefined");
        
    } catch (e) {
        console.log(e);
        retval = null;
    }
    
    return retval;
}

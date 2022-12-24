var AWS = require('aws-sdk');

var ddb;


module.exports = {
    
    GetNodeData        : getDBContents,
    GetMultiNodeData   : getDBContentsInBatch,
    SetNodeData        : updateDBContents,
};

async function getDBContents(projectName, deviceID) {
    
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

async function getDBContentsInBatch(projectName, nodeIDList) {
    
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

async function updateDBContents(CurrentNodeData, UpdatedNodeData) {
    
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

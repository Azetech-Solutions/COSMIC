/* Imported Modules */
var Globals = require('./Globals.js');

var AWS = require('aws-sdk');
var iotdata = new AWS.IotData({ endpoint: 'avruhytgmbwix-ats.iot.ap-south-1.amazonaws.com' });

var NodeData;

exports.handler = async (event, context, callback) => {

    /**
     * This lambda function will be invoked from the AWS Rule whenever the device sends the MQTT message
     * to it's pub topic. (<prj>/<deviceId>/pub) or the App sends it's out topic (<prj>/<deviceId>/out)
     * 
     * The payloads will be encoded along with the topic name and sent to this function in JSON format for parsing.
     * E.g.:
        {
          "data": {"m1":1, ....},
          "topic": "aze/<deviceId>/<pub/out>"
        }
        
        This has to be parsed and decoded in this function and act accordingly
        
        Once the data is parsed, the then this function shall store the info in the DynamoDB and route it back to other end.
        If the data is received from uC via pub topic (<prj>/<deviceId>/pub), then it shall route it to app via in topic (<prj>/<deviceId>/in)
        If the data is received from app via out topic (<prj>/<deviceId>/out), then it shall route it to uC via sub topic (<prj>/<deviceId>/sub)
     **/


    var topic = event.topic;
    var rawdata = event.data;
    
    var UpdatedNodeData = {};

    /*********************************************/
    /* For MQTT Message handling from the device */
    /*********************************************/
    
    if((topic != null) && (rawdata != null))
    {
        var topicArray = topic.split('/');
        
        if(topicArray.length == 3) // Only if the topic is valid
        {
            var projectName = topicArray[0];
            var deviceID = topicArray[1];
            var relayTopic = "";
            
            if(topicArray[2] == "pub")
            {
                relayTopic = projectName + "/" + deviceID + "/in"
            }
            else if(topicArray[2] == "out")
            {
                relayTopic = projectName + "/" + deviceID + "/sub"
            }
            else
            {
                // Invalid Topic
            }
            
            if((deviceID != null) && (relayTopic != ""))
            {
                // Update the content to DynamoDB
                // Get the Node ID Contents from the table
                NodeData = await Globals.GetNodeData(projectName, deviceID);
            
                if(NodeData != null)
                {
                    var inputData;
                    
                    if(typeof rawdata === "string") // data is Json string
                    {
                        inputData = JSON.parse(rawdata);
                    }
                    else
                    {
                        inputData = rawdata;
                    }
                    
                    if(inputData != null)
                    {
                        // TODO: Parse the Data field into JSON and correct it
                        UpdatedNodeData["prj"]  = projectName;
                        UpdatedNodeData["id"]   = deviceID;
                        UpdatedNodeData["iotdata"] = inputData;
                    
                        // If there were anything needs to be stored in the DB, then store it. !Important: DO NOT USE UpdatedNodeData after this call
                        let result = await Globals.SetNodeData(NodeData, UpdatedNodeData);
                        
                        if(result == null)
                        {
                            callback(new Error("Unable to Store the data in DynamoDB"));
                        }
                    }

                    // Relay the data to the other side even if the storage fails
                    return sendMqttResponse(relayTopic, rawdata);
                }
                else
                {
                    callback(new Error("Device not found in the server database!"));
                }
            }
            else
            {
                callback(new Error("Invalid Topic!"));
            }
        }
    }
    
    /*********************************************/
    /* For HTTP Requests handling from the tools */
    /*********************************************/
    else
    {
        // If the Lambda function is invoked by the API Gateway by the tool, app or web portal
        
        var RequestType = event.httpMethod;

        if(RequestType == "GET") {
            
            
            // Get the REST API paths
            // path will be like: "/<prj>/<device>" so, second element will be the parent path

            if(event.pathParameters == null)
            {
                // If there are no REST API Path, then the user might be requesting for all projects.
                // Scan all the items and send it.
                
                NodeData = await Globals.ScanDatabase(null);
                
                sendCallbackResponse(JSON.stringify(NodeData), callback);
            }
            else
            {
                var projectName = event.pathParameters["prj"];
                
                if(projectName != null)
                {
                    // Check if Device is also passed
                    var deviceName = event.pathParameters["device"];
                    
                    if(deviceName == null)
                    {
                        // If Device is not passed, then list all the devices in the project
                        NodeData = await Globals.ScanDatabase(projectName);
                
                        sendCallbackResponse(JSON.stringify(NodeData), callback);
                    }
                    else
                    {
                        // If a single device data is requested, then perform operations in such a way
                        NodeData = await Globals.GetNodeData(projectName, deviceName);
                        
                        sendCallbackResponse(JSON.stringify(NodeData), callback);
                    }
                }
                else
                {
                    callback(new Error("Unsuported API Path!"));
                }
            }
            
            
        }
        else if(RequestType == "POST") {
            
            // Do not do anything for now! Since it is un-authenticated access, there could be a security breach
            
            // Parse the Request Body and update the DB
            if(event.body != null) {
            
            }
            
            callback(new Error("POST API is not supported!"));
        }
        else
        {
            callback(new Error("Unsuported API!"));
        }
    }
};


function sendMqttResponse(topicName, data) {
    
    var mqttParams = {
        topic: topicName,
        payload: JSON.stringify(data),
        qos: 1
    };

    const request = iotdata.publish(mqttParams);
    request
        .on('success', () => console.log("Successfully published Mqtt Data to " + topicName))
        .on('error', () => console.log("Unable to Publish Mqtt Data"))
    
    return new Promise(() => request.send());
}

function sendCallbackResponse(data, callback) {
    
    var response = {};
    
    response = {
        "statusCode": 200,
        "isBase64Encoded": false,
        "headers" : {
                        "Access-Control-Allow-Origin" : "*", // Required for CORS support to work
                        "Access-Control-Allow-Credentials" : true // Required for cookies, authorization headers with HTTPS 
                    }
    };
    
    response.body = data; // This data must be a string
    
    callback(null, response);
}
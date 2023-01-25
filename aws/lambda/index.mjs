// Import ComIf
import * as ComIf from './ComIf.mjs';
import * as Globals from './awsIf.mjs';

// Import Rx Messages
import * as StatusData from './msg/StatusData.mjs';
import * as CommandData from './msg/CommandData.mjs';


var RxMessages = [StatusData, CommandData];
var TxMessages = [CommandData];

var NodeData;

var ComIfDataToBeSent = "";

const INVALID_DATA_FORMAT = -1;
const COMIF_DATA_FORMAT = 0;
const JSON_DATA_FORMAT = 1;

var receivedDataFormat = INVALID_DATA_FORMAT;

/*******************************************************************/
/* ComIf Functions */

/**
 * This function will be called from the Transmit requests of the Tx Messages.
 */
function ComIfChannelTransmit(Length, str)
{
    ComIfDataToBeSent += str;
}

function ComIfChannelErrorNotification(Debug0, Debug1)
{
    console.log(`--> Error : ${Debug0} 0x${Debug1.toString(16)}`);
}

/*******************************************************************/

export const handler = async(event) => {

    /**
     * This lambda function will be invoked from the AWS Rule whenever the device sends the MQTT message
     * to it's pub topic. (<prj>/<deviceId>/pub) or the App sends it's out topic (<prj>/<deviceId>/out)
     * 
     * Since the payloads are just the string, those will be encoded along with the topic name
     * and sent to this function in JSON format for parsing.
     * E.g.:
        {
          "data": "7;5=020000007=",              ---> ComIf Message format (base64 encoded)
          "data": {"m1":1, ....}                 ---> Direct JSON format (base64 encoded)
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

    ComIfDataToBeSent = "";
    
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
            var feedbackTopic = "";
            
            if(topicArray[2] == "pub")
            {
                relayTopic = projectName + "/" + deviceID + "/in"
                feedbackTopic = projectName + "/" + deviceID + "/sub"
            }
            else if(topicArray[2] == "out")
            {
                relayTopic = projectName + "/" + deviceID + "/sub"
                feedbackTopic = projectName + "/" + deviceID + "/in"
            }
            else
            {
                // Invalid Topic
            }
            
            /* If the data is received from the IoT Rule, then the data will always be encoded, irrespective of COMIF or JSON formats */
            // So, decode it before processing
            
            rawdata = new Buffer.from(rawdata, 'base64').toString('ascii');
            
            if(typeof rawdata === "string") // if the data is Json string
            {
                if(rawdata.startsWith("7;") && rawdata.endsWith("7=")) // --> ComIf Message
                {
                    receivedDataFormat = COMIF_DATA_FORMAT;
                }
                else if(rawdata.startsWith("{") && rawdata.endsWith("}")) // --> JSON String
                {
                    receivedDataFormat = JSON_DATA_FORMAT;
                    
                    rawdata = JSON.parse(rawdata);
                }
                else
                {
                    // Invalid Data format
                }
            }
            else
            {
                receivedDataFormat = JSON_DATA_FORMAT;
            }
            
            if((deviceID != null) && (relayTopic != "") && (receivedDataFormat != INVALID_DATA_FORMAT))
            {
                // Update the content to DynamoDB
                // Get the Node ID Contents from the table
                NodeData = await Globals.GetNodeData(projectName, deviceID);
            
                if(NodeData != null)
                {
                    if(receivedDataFormat == COMIF_DATA_FORMAT)
                    {
                        // Set the Topic name as Channel name to identify the user in later point in time
                        var channel = new ComIf.Channel(topic, ComIf.StringChannel, ComIfChannelTransmit, ComIfChannelErrorNotification);
    
                        // Registering all Rx Messages
                        for(var i = 0; i < RxMessages.length; i++) {
                            var rxMessage = new ComIf.RxMessage(RxMessages[i].ID, RxMessages[i].DLC, RxMessages[i].RxCbk);
                            channel.RegisterRxMessage(rxMessage);
                        }
                        
                        // Registering all Tx Messages
                        for(var i = 0; i < TxMessages.length; i++) {
                            var txMessage = new ComIf.TxMessage(TxMessages[i].ID, TxMessages[i].DLC, TxMessages[i].TxCbk);
                            channel.RegisterTxMessage(txMessage);
                        }
                        
                        // Set the Node Data
                        var myNodeData = NodeData["iotdata"];
                        channel.SetNodeData(myNodeData);
                        
                        channel.RxIndication_String(rawdata);
                        
                        UpdatedNodeData["prj"]  = projectName;
                        UpdatedNodeData["id"]   = deviceID;
                        UpdatedNodeData["iotdata"] = myNodeData;
                    
                        // If there were anything needs to be stored in the DB, then store it. !Important: DO NOT USE UpdatedNodeData after this call
                        if(channel.RequiresDatabaseUpdate) {
                            let result = await Globals.SetNodeData(NodeData, UpdatedNodeData);
                        
                            if(result == null)
                            {
                                return (new Error("Unable to Store the data in DynamoDB"));
                            }
                        }
                        
                        // Relay the data to the other side even if the storage fails.
                        
                        if(channel.TriggerTransmitForScheduledMessages())
                        {
                            // If there is atleast one message to be scheduled, then send that.
                            await Globals.SendMqttResponse(feedbackTopic, ComIfDataToBeSent);
                        }
                        
                        // If there are no data to be sent out newly, then simply relay the received data to the client
                        return Globals.SendMqttResponse(relayTopic, rawdata);
                    }
                    else // if(receivedDataFormat == JSON_DATA_FORMAT)
                    {
                        if(rawdata != null)
                        {
                            // TODO: Parse the Data field into JSON and correct it
                            UpdatedNodeData["prj"]  = projectName;
                            UpdatedNodeData["id"]   = deviceID;
                            UpdatedNodeData["iotdata"] = rawdata;
                        
                            // If there were anything needs to be stored in the DB, then store it. !Important: DO NOT USE UpdatedNodeData after this call
                            let result = await Globals.SetNodeData(NodeData, UpdatedNodeData);
                            
                            if(result == null)
                            {
                                return (new Error("Unable to Store the data in DynamoDB"));
                            }
                        }
    
                        // Relay the data to the other side even if the storage fails
                        return Globals.SendMqttResponse(relayTopic, JSON.stringify(rawdata));
                    }
                }
                else
                {
                    return (new Error("Device not found in the server database!"));
                }
            }
            else
            {
                return (new Error("Invalid Topic!"));
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

        if(RequestType == "GET")
        {
            // Get the REST API paths
            // path will be like: "/<prj>/<device>" so, second element will be the parent path

            if(event.pathParameters == null)
            {
                // If there are no REST API Path, then the user might be requesting for all projects.
                // Scan all the items and send it.
                
                NodeData = await Globals.ScanDatabase(null);
                
                return sendPositiveResponse(JSON.stringify(NodeData));
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
                
                        return sendPositiveResponse(JSON.stringify(NodeData));
                    }
                    else
                    {
                        // If a single device data is requested, then perform operations in such a way
                        NodeData = await Globals.GetNodeData(projectName, deviceName);
                        
                        return sendPositiveResponse(JSON.stringify(NodeData));
                    }
                }
                else
                {
                    return (new Error("Unsuported API Path!"));
                }
            }
            
            
        }
        else if(RequestType == "POST") {
            
            // Do not do anything for now! Since it is un-authenticated access, there could be a security breach
            
            // Parse the Request Body and update the DB
            if(event.body != null) {
            
            }
            
            return (new Error("POST API is not supported!"));
        }
        else
        {
            return (new Error("Unsuported API!"));
        }
    }
};

function sendPositiveResponse(data) {
    
    const response = {
        statusCode: 200,
        isBase64Encoded: false,
        headers : {
                        "Access-Control-Allow-Origin" : "*", // Required for CORS support to work
                        "Access-Control-Allow-Credentials" : true // Required for cookies, authorization headers with HTTPS 
                    },
        body : data
    };
    
    return response;
}

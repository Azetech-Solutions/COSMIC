/* Mandatory Message Attributes */
export const ID = 0x5D;

export const DLC = 2;

export function RxCbk(Length, Data, channel) {
    // Update the channel.NodeData based on the Data dn Length received from the channel    
    // If the received message needs an update to be stored in the database, then set the flag channel.RequiresDatabaseUpdate to true
    
    // 255 = The device is booting, 1 = the device is requesting the command data
    if((Data[0] == 255) || (Data[0] == 1)) {
        // If the uC sent this value, it means the uC is booting, so send the command data
        var CmdDataMsg = channel.GetTxMessageInstance(0xCD);
        if(CmdDataMsg != null) {
            CmdDataMsg.IsTxScheduled = true;
        }
    }
}

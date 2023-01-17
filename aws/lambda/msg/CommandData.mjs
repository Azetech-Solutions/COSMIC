/* Mandatory Message Attributes */
export const ID = 0xCD;

export const DLC = 8;

/**
 * Data Structure
 * --------------
 * 
 *               Bit-7       Bit-6       Bit-5       Bit-4       Bit-3       Bit-2       Bit-1       Bit-0
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 0  |           |           |           |           |           |           |           |     m1    |
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 1  |           |           |           |           |           |           |           |           |
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 2  |           |           |           |           |           |           |           |           |
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 3  |           |           |           |           |           |           |           |           |
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 4  |           |           |           |           |           |           |           |           |
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 5  |           |           |           |           |           |           |           |           |
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 6  |           |           |           |           |           |           |           |           |
 *           +-----------------------------------------------------------------------------------------------+
 * Byte - 7  |           |           |           |           |           |           |           |           |
 *           +-----------------------------------------------------------------------------------------------+
 **/


export function RxCbk(Length, Data, channel) {
    // Update the channel.NodeData based on the Data dn Length received from the channel    
    // If the received message needs an update to be stored in the database, then set the flag channel.RequiresDatabaseUpdate to true

    if((Data[0] & 0x01) != channel.NodeData.m1) {
        channel.NodeData.m1 = (Data[0] & 0x01);
        channel.RequiresDatabaseUpdate = true;
    }
}

export function TxCbk(Data, channel) {
    
    // Update the data bytes based on the channel.NodeData
    
    Data[0] = channel.NodeData.m1;
}

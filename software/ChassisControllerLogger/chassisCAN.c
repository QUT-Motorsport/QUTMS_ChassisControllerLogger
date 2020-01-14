/**
 * @file chassisCAN.c
 * @author Jonn Dillon
 * @date 16/4/2018
 * @brief Handles functions, calls and utilities related to the CANBUS's
 **/

#include "chassisCAN.h"
#include "MCP2515.h"

// Serves as a wrapper for MCP2515_TX in MCP2515_TX.h - for more info refer to that.
void CAN_send(uint8_t CANbus, uint8_t numBytes, uint8_t* data, uint32_t ID) {
    MCP2515_TX(CANbus, MCP2515_findFreeTxBuffer(CANbus), numBytes, data, ID);
}

// Construct a unique CAN packet identifier
uint32_t CAN_ID_constructor(uint32_t sendingID, unsigned char type, unsigned char address, uint32_t status) {
    return (
               sendingID |                 // Sending ID, ( First 8 bits, define the device to send to)
               ((uint32_t)type << 18) |    // What sort of command to send
               ((uint32_t)address << 13) | // Specific address
               status                      // SubID?
           );
}

// Serves as a wrapper for the MCP2515_PullCanPacket(..) in MCP2515.h - For more info refer to that.
void CAN_pull_packet(uint8_t CANbus, uint8_t* numBytes, uint8_t* data, uint32_t* ID) {
    // Receive the status of the buffers RXB0 and RXB1
    uint8_t status = MCP2515_check_receive_status(CANbus);
    // Check which receive buffer contains the data (or if both contain) by checking bits 7:6
    switch(status >> 6) {
    case 1:	// Message in RXB0
        MCP2515_PullCanPacket(CANbus, MCP2515_RXB0SIDH, numBytes, data, ID);
        break;
    case 2: // Message in RXB1=
        MCP2515_PullCanPacket(CANbus, MCP2515_RXB1SIDH, numBytes, data, ID);
        break;
    case 3: // Message in both buffers
        MCP2515_PullCanPacket(CANbus, MCP2515_RXB0SIDH, numBytes, data, ID);
        MCP2515_PullCanPacket(CANbus, MCP2515_RXB1SIDH, numBytes, data, ID);
        break;
    default:
        break;
    }
}
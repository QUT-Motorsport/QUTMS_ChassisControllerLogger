#define F_CPU 16000000
#include "main.h"

uint16_t ignitionStateDebounceCount = 0;
uint8_t ignitionStateLock = 0;
uint8_t ignitionState = 0;


uint8_t shutdownState = 0;

volatile uint8_t testError = 0;

char out[96] = {'\0'}; // CAN 1 SUFF
uint8_t inverterArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t PDMarray[5] = {0, 0, 0, 0, 0};
uint8_t WheelArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};

volatile uint8_t ouft = 0;

int main(void) {

    // Set Up
    firmware_init();
    timer_init();

    // Grab the state of the shutdown circuity
    shutdownState = 0;
    _delay_ms(500);
    // Enable Interupts
    sei();
    // Main Poll
    // ------------------------------------------------------------------------
    while(1) {

        //if(isCharAvailable() == 1)uart_process_byte(receiveChar());
        //uart1_puts("Hello World!\n");
    }

    // return 0; // Never return from an embedded program, does bad things if this ever triggers
}

/**
 * @brief Core system's timer, 1ms, core of the pseudo RTOS system
 */
void oneKHzTimer(void)
{

}

// -------------------------------------------------- Interrupt Service Routines --------------------------------------------------

/**
 * @brief Called whenever the 1Khz timer triggers
 */
ISR(TIMER0_COMPA_vect)
{
    oneKHzTimer();
}

ISR(TIMER1_COMPA_vect)
{

}

/**
 * @brief Called whenever CANBUS 1 interupt is triggered
 *        * When ever there is data waiting in CAN 1
 */
ISR(INT1_vect) {
    // Details about the message we're attempting to pull from the CAN bus
    uint8_t data[8];
    uint32_t ID;
    uint8_t numBytes;
    // Get the data from the CAN bus and process it
    CAN_pull_packet(TRACTIVE_CAN, &numBytes, data, &ID);

    // If the data packet is crap
    // throw_error_code(ERROR_LEVEL_WARN, ERROR_CANBUS_1_RESPONSE_MALFORMED);
    //out++;

    //sprintf(out, "%x,%x,%x,%x,%x,%x,%x,%x", data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
}

/**
 * @brief Called whenever CANBUS 2 interupt is triggered
 *        * When ever there is data waiting in CAN 2
 */
ISR(INT0_vect)	{
    // Details about the message we're attempting to pull from the CAN bus
    uint8_t data[8];
    uint32_t ID;
    uint8_t numBytes;
    // Get the data from the CAN bus and process it
    CAN_pull_packet(POWER_CAN, &numBytes, data, &ID);

    // If the data packet is crap
    // throw_error_code(ERROR_LEVEL_WARN, ERROR_CANBUS_2_RESPONSE_MALFORMED);

}

/**
 * @brief Called whenever CANBUS 3 interupt is triggered
 *        * When ever there is data waiting in CAN 3
 */
ISR(PCINT0_vect) {
    // Details about the message we're attempting to pull from the CAN bus
    uint8_t data[8];
    uint32_t ID;
    uint8_t numBytes;
    // Get the data from the CAN bus and process it
    CAN_pull_packet(DATA_CAN, &numBytes, data, &ID);

    // If the data packet is crap
    // throw_error_code(ERROR_LEVEL_WARN, ERROR_CANBUS_3_RESPONSE_MALFORMED);



}
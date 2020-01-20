#define F_CPU 16000000
#include "main.h"
#include <string.h>

#include "SDCard/ff.h"
#include "SDCard/SDInterface.h"

uint16_t ignitionStateDebounceCount = 0;
uint8_t ignitionStateLock = 0;
uint8_t ignitionState = 0;


uint8_t shutdownState = 0;

volatile uint8_t testError = 0;

char out[96] = {'\0'}; // CAN 1 SUFF
uint8_t inverterArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};
uint8_t PDMarray[5] = {0, 0, 0, 0, 0};
uint8_t WheelArray[8] = {0, 0, 0, 0, 0, 0, 0, 0};

volatile uint8_t powerCANReady = 0;
volatile uint8_t dataCANReady = 0;
volatile uint8_t tractiveCANReady = 0;

volatile uint32_t numMS = 0;

volatile uint8_t ouft = 0;

FRESULT scan_files(char* path) {
    FRESULT result;
    DIR dir;
    UINT i;
    static FILINFO fno;

    result = f_opendir(&dir, path);
    if (result == FR_OK) {
        for (;;) {
            result = f_readdir(&dir, &fno);
            if (result != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                result = scan_files(path);                    /* Enter the directory */
                if (result != FR_OK) break;
                path[i] = 0;
            } else {
                /* It is a file. */
                uart_puts(path);
                uart_puts(fno.fname);
                uart_putc('\n');
            }
        }
    }
}


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
    uart_puts("Starting..\r\n");

    FATFS fs;
    FRESULT res;
    char buff[256];


    res = f_mount(&fs, "", 1);
    uart_putformatted("mount result: %d\r\n", res);

    FIL logFile;
    /*    res = f_open(&fil, "TEST.TXT", FA_READ | FA_WRITE | FA_OPEN_APPEND);

        uart_putformatted("file open result: %d\r\n", res);
    */
    /*    char line[100];
        uint8_t fileOpen = 0;

        if (res == FR_OK) {
            fileOpen = 1;
            UINT numBytes = 0;
            char* tdata = "I WANT TO WRITE TO FILE\r\n";
            uart_puts("WRITING START\r\n");
            int num = f_printf(&fil, "I WANT TO WRITE TO FILE REEE\r\n");
            uart_putformatted("wrote: %d, e: %d\r\n", num, strlen(tdata));
            uart_puts("WRITING END\r\n");
            f_close(&fil);



        }
    */
    uint8_t powerCAN_data[8];
    uint32_t powerCAN_ID;
    uint8_t powerCAN_numBytes;

    uint8_t dataCAN_data[8];
    uint32_t dataCAN_ID;
    uint8_t dataCAN_numBytes;

    uint8_t tractiveCAN_data[8];
    uint32_t tractiveCAN_ID;
    uint8_t tractiveCAN_numBytes;



    while(1) {
        if (powerCAN_numBytes != 0) {
            memset(powerCAN_data, 0, 8);
            powerCAN_numBytes = 0;
        }

        if (dataCAN_numBytes != 0) {
            memset(dataCAN_data, 0, 8);
            dataCAN_numBytes = 0;
        }

        if (tractiveCAN_numBytes != 0) {
            memset(tractiveCAN_data, 0, 8);
            tractiveCAN_numBytes = 0;
        }

        if (powerCANReady) {
            powerCANReady = 0;

            // Get the data from the CAN bus and process it
            CAN_pull_packet(POWER_CAN, &powerCAN_numBytes, powerCAN_data, &powerCAN_ID);
            uart_puts("pc");
        }

        if (dataCANReady) {
            dataCANReady = 0;

            // Get the data from the CAN bus and process it
            CAN_pull_packet(DATA_CAN, &dataCAN_numBytes, dataCAN_data, &dataCAN_ID);
            uart_puts("dc");
        }

        if (tractiveCANReady) {
            tractiveCANReady = 0;

            // Get the data from the CAN bus and process it
            CAN_pull_packet(TRACTIVE_CAN, &tractiveCAN_numBytes, tractiveCAN_data, &tractiveCAN_ID);
            uart_puts("tc");
        }

        uart_puts("\r\n");

        res = f_open(&logFile, "TEST.TXT", FA_READ | FA_WRITE | FA_OPEN_APPEND);

        if (res == FR_OK) {
            UINT totalLength = 0;
            UINT length;
            if (powerCAN_numBytes > 0) {
                uart_puts("log\r\n");

                //if (tractiveCAN_numBytes > 0 || dataCAN_numBytes > 0 || powerCAN_numBytes > 0) {
                //double current_time = numMS / 1000.0;
                uart_putformatted("t:%d", numMS);
                f_printf(&logFile, "PC");
                res = f_write(&logFile, (void*)&numMS, sizeof(numMS), &length );
                totalLength += length;

                res = f_write(&logFile, (void*)&powerCAN_numBytes, sizeof(powerCAN_numBytes), &length );
                uart_putformatted("s:%d", powerCAN_numBytes);
                totalLength += length;
                if (powerCAN_numBytes > 0) {
                    res = f_write(&logFile, (void*)&powerCAN_ID, sizeof(powerCAN_ID), &length );
                    uart_putformatted("id:%d", powerCAN_ID);
                    totalLength += length;
                    res = f_write(&logFile, (void*)powerCAN_data, powerCAN_numBytes, &length );
                    totalLength += length;
                }

                /*      res = f_write(&logFile, (void*)&dataCAN_numBytes, sizeof(dataCAN_numBytes), &length );
                      totalLength += length;
                      if (dataCAN_numBytes > 0) {
                          res = f_write(&logFile, (void*)&dataCAN_ID, sizeof(dataCAN_ID), &length );
                          totalLength += length;
                          res = f_write(&logFile, (void*)dataCAN_data, dataCAN_numBytes, &length );
                          totalLength += length;
                      }

                      res = f_write(&logFile, (void*)&tractiveCAN_numBytes, sizeof(tractiveCAN_numBytes), &length );
                      totalLength += length;
                      if (tractiveCAN_numBytes > 0) {
                          res = f_write(&logFile, (void*)&tractiveCAN_ID, sizeof(tractiveCAN_ID), &length );
                          totalLength += length;
                          res = f_write(&logFile, (void*)tractiveCAN_data, tractiveCAN_numBytes, &length );
                          totalLength += length;
                      }
                	  */
            }


            uart_putformatted("can len: %d\r\n", totalLength);
            f_close(&logFile);
        }

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
    numMS++;
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
    /* uint8_t data[8];
     uint32_t ID;
     uint8_t numBytes;
     // Get the data from the CAN bus and process it
     CAN_pull_packet(TRACTIVE_CAN, &numBytes, data, &ID);*/
    tractiveCANReady = 1;

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
    /*    uint8_t data[8];
        uint32_t ID;
        uint8_t numBytes;
        // Get the data from the CAN bus and process it
        CAN_pull_packet(POWER_CAN, &numBytes, data, &ID);
    */
    powerCANReady = 1;
    // If the data packet is crap
    // throw_error_code(ERROR_LEVEL_WARN, ERROR_CANBUS_2_RESPONSE_MALFORMED);

}

/**
 * @brief Called whenever CANBUS 3 interrupt is triggered
 *        * When ever there is data waiting in CAN 3
 */
ISR(PCINT0_vect) {
    // Details about the message we're attempting to pull from the CAN bus
    /*uint8_t data[8];
    uint32_t ID;
    uint8_t numBytes;
    // Get the data from the CAN bus and process it
    CAN_pull_packet(DATA_CAN, &numBytes, data, &ID);*/
    dataCANReady = 1;

    // If the data packet is crap
    // throw_error_code(ERROR_LEVEL_WARN, ERROR_CANBUS_3_RESPONSE_MALFORMED);



}
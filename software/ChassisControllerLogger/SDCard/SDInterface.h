/*
 * SDInterface.h
 *
 * Created: 16/01/2020 11:26:22 AM
 *  Author: calvi
 * See http://www.rjhcoding.com/avrc-sd-interface-4.php
 */

#ifndef SDINTERFACE
#define SDINTERFACE

#include "ff.h"

#define SDCARD_PORT_CS PORTK
#define SDCARD_PIN_CS PK3

/* Set SPI clock for initialization (100-400kHz) - fosc/64 ~ 250khz*/
/*								clear SPR0 and SPR1					Set SPR0 and SPR1 */
#define	FCLK_SLOW()		SPCR = (SPCR & ~( (1<<SPR1) | (1<<SPR0) )) | ((1<<SPR1) | (0<<SPR0))
/* Set SPI clock for read/write (20MHz max)  - fosc/16 ~ 1 mhz*/
/*								clear SPR0 and SPR1					Set SPR0 and SPR1 */
#define	FCLK_FAST()		SPCR = (SPCR & ~( (1<<SPR1) | (1<<SPR0) )) | ((0<<SPR1) | (1<<SPR0))


// command definitions
#define CMD0                0
#define CMD0_ARG            0x00000000
#define CMD0_CRC            0x94
#define CMD8                8
#define CMD8_ARG            0x0000001AA
#define CMD8_CRC            0x86
#define CMD9                9
#define CMD9_ARG            0x00000000
#define CMD9_CRC            0x00
#define CMD10               9
#define CMD10_ARG           0x00000000
#define CMD10_CRC           0x00
#define CMD13               13
#define CMD13_ARG           0x00000000
#define CMD13_CRC           0x00
#define CMD17               17
#define CMD17_CRC           0x00
#define CMD24               24
#define CMD24_CRC           0x00
#define CMD55               55
#define CMD55_ARG           0x00000000
#define CMD55_CRC           0x00
#define CMD58               58
#define CMD58_ARG           0x00000000
#define CMD58_CRC           0x00
#define ACMD41              41
#define ACMD41_ARG          0x40000000
#define ACMD41_CRC          0x00

#define SD_IN_IDLE_STATE    0x01
#define SD_READY            0x00
#define SD_R1_NO_ERROR(X)   X < 0x02

#define R3_BYTES            4
#define R7_BYTES            4

#define CMD0_MAX_ATTEMPTS       255
#define CMD55_MAX_ATTEMPTS      255
#define SD_ERROR                1
#define SD_SUCCESS              0
#define SD_MAX_READ_ATTEMPTS    1563
#define SD_READ_START_TOKEN     0xFE
#define SD_INIT_CYCLES          80

#define SD_START_TOKEN          0xFE
#define SD_ERROR_TOKEN          0x00

#define SD_DATA_ACCEPTED        0x05
#define SD_DATA_REJECTED_CRC    0x0B
#define SD_DATA_REJECTED_WRITE  0x0D

#define SD_BLOCK_LEN            512

// SD functions
uint8_t SD_init(void);
void SD_powerUpSeq(void);
void SD_command(uint8_t cmd, uint32_t arg, uint8_t crc);
uint8_t SD_readRes1(void);
void SD_readRes2(uint8_t* res);
void SD_readRes3(uint8_t* res);
void SD_readRes7(uint8_t* res);
void SD_readBytes(uint8_t* res, uint8_t n);
uint8_t SD_goIdleState(void);
void SD_sendIfCond(uint8_t* res);
void SD_sendStatus(uint8_t* res);
void SD_readOCR(uint8_t* res);
uint8_t SD_sendApp(void);
uint8_t SD_sendOpCond(void);
uint8_t SD_readSingleBlock(uint32_t addr, uint8_t* buf, uint8_t* error);
uint8_t SD_writeSingleBlock(uint32_t addr, uint8_t* buf, uint8_t* res);

#endif

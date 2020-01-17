/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

DSTATUS Stat;


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
    /* Supports only single drive */
    if( pdrv != 0)
        return STA_NOINIT;

    /* return current status */
    return( Stat );
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
    /* Supports only single drive */
    if( pdrv != 0)
        return STA_NOINIT;

    /* if initialization succeeds... */
    if( !SD_init() )
    {
        //Clear STA_NOINIT
        Stat &= ~STA_NOINIT;
    }

    /* return current status */
    return( Stat );
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,		/* Physical drive nmuber to identify the drive */
    BYTE* buff,		/* Data buffer to store read data */
    LBA_t sector,	/* Start sector in LBA */
    UINT count		/* Number of sectors to read */
)
{
    /* Supports only single drive and must have a size of 1 sector */
    if( pdrv || !count || (count > 1) )
        return( RES_PARERR );

    /* if we haven't initialized the card yet... */
    if( Stat & STA_NOINIT )
        return( RES_NOTRDY );

    /* Single block read */
    uint8_t token = 0;
    SD_readSingleBlock(sector, buff, &token);
    if (token != 0xFE)
        // if( SD_ReadSector( sector, buff ) )
        return( RES_ERROR );

    /* return successful result: OK */
    return( RES_OK );
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
    BYTE pdrv,			/* Physical drive nmuber to identify the drive */
    const BYTE* buff,	/* Data to be written */
    LBA_t sector,		/* Start sector in LBA */
    UINT count			/* Number of sectors to write */
)
{
    /* Supports only single drive and must have a size of 1 sector */
    if( pdrv || !count || (count > 1) )
        return( RES_PARERR );

    /* if we haven't initialized the card yet... */
    if( Stat & STA_NOINIT )
        return( RES_NOTRDY );

    /* Single block write */
    uint8_t token = 0;
    SD_writeSingleBlock(sector, buff, &token);
    if (token != 0x05)
        // if( SD_WriteSector( sector, buff ) )
        return( RES_ERROR );

    /* return successful result: OK */
    return( RES_OK );
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,		/* Physical drive nmuber (0..) */
    BYTE cmd,		/* Control code */
    void* buff		/* Buffer to send/receive control data */
)
{
    DRESULT res;
    BYTE*  ptr = buff;

    /* Supports only single drive */
    if( pdrv != 0)
        return RES_PARERR;

    /* if we haven't initialized the card yet... */
    if( Stat & STA_NOINIT )
        return RES_NOTRDY;

    res = RES_ERROR;

    switch( cmd )
    {
    /* Flush dirty buffer if present */
    /*   case CTRL_SYNC :
           SD_EnableCS();
           if( SD_WaitForReady() == 0xFF )
               res = RES_OK;
           break;*/

    default:
        res = RES_PARERR;
        break;
    }

    SD_DisableCS();
    SPI_send_byte( 0xFF );
    return res;
}


/****************************************************************************
 *                                                                          *
 * File    : main.c                                                         *
 *                                                                          *
 * Purpose : Console mode (command line) program.                           *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crc32.h"
/****************************************************************************
 *                                                                          *
 * Function: main                                                           *
 *                                                                          *
 * Purpose : Main entry point.                                              *
 *                                                                          *
 * History : Date      Reason                                               *
 *           00/00/00  Created                                              *
 *                                                                          *
 ****************************************************************************/
#define FLASH_START			0x200000
#define FIRMWARE_SIZE 		0x3E000
#define BOOTLOADER_SIZE 	0x4000
#define CC2538_VTABLE_SIZE	0x40	
#define OUTPUT_FIRMWARE_SIZE 65536


int main(int argc, char *argv[])
{
	FILE * f_read;
	FILE * f_write;
	unsigned int * buf;
	unsigned long int size;

	if( argc != 3 ) {
		printf( "Usage: ota_prepare_fw.exe <firmware.bin>\r\n" );
		exit( 0 );
	};

	f_write = fopen( argv[2], "wb" );
	
//read firmware and calculate size minus size of CCA
	f_read = fopen( argv[1], "rb" );
	fseek(f_read, 0, SEEK_END); // seek to end of file
	size = ftell(f_read); // get current file pointer
	fseek(f_read, 0, SEEK_SET); // seek back to beginning of file
	size = size - 0x2C;
	size = OUTPUT_FIRMWARE_SIZE - 4;
	buf = malloc(size + 4); //minus CCA, but add 4 bytes for CRCR
	fread( buf, 1, size, f_read );
/*
	if(( buf[1] < FLASH_START ) || ( buf[1] > FLASH_START + size )) {
		printf( "Reset handler in first firmware is out of range!\r\n" );
		fclose( f_write );
		fclose( f_read );
		exit( 2 );
	};
*/
//write results with CRC as four last bytes   
	buf[(size >> 2 )] = 0 - calc4summ( buf, ( size >> 2 ));
	fwrite( buf, 1, size + 4, f_write );

	fclose( f_read );
	fclose( f_write );
	free( buf );

	printf( "Success!\r\n" );

    return 0;
}


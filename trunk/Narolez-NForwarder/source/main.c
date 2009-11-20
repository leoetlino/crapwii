/*
Dol SD/USB Forwarder [NForwarder]

Copyright (c) 2009 Narolez [narolez[at]googlemail[dot]com]
 
Credits to svpe, Joseph Jordan, SpaceJump, WiiPower, Oggzee and Zektor

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from
the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1.The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software in a
product, an acknowledgment in the product documentation would be
appreciated but is not required.

2.Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3.This notice may not be removed or altered from any source distribution.
*/

/* 
	RELEASE NOTES
	
	v1.7 add defines for CFG USB Loader and Wiiflow	
	v1.6 new button mapping for passing IOS arguments:
	        - press 1 and 2 OR A and B: force ios=249
		    - press 1 OR A;             force ios=222-mload
		    - press 2 OR B:             force ios=223-mload
			- press nothing:            no force, loader default
	v1.5 black screen bugfix, preloader compatiblity		
	v1.4 adjusted countdown for button query - increase performance
	v1.3 bugfix for code dump when run internal dol
	v1,2 change to linkerscript rvl.ld (bugfix for large included dols)
	v1.1 Pass config parameter for IOS (ios=222-mload, ios=223-mload or ios=249)
	     as argument argv[1]
	V1.0 Initial Release
*/

#include <stdio.h>
#include <gccore.h>
#include <fat.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <sdcard/wiisd_io.h>
#include <wiiuse/wpad.h>

#include "dol.h"


static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void init()
{
	// Initialise the video system
	VIDEO_Init();
	
	// initialise the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	
	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);
	
	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);
	
	// Make the display invisible
	VIDEO_SetBlack(TRUE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();
}


void *allocate_memory(u32 size)
{
	return memalign(32, (size+31)&(~31) );
}


#define SD 0
#define USB 1
#define STORAGENOTREADYRETRIES 5
DISC_INTERFACE storage;

u8 mountStorage(int device)
{
	if (device == SD) storage = __io_wiisd;
	else storage = __io_usbstorage;

	int retrycount = 0;

retry:
	storage.startup();

	if (!fatMountSimple("fat", &storage)) 
	{
		fatUnmount("fat");
		storage.shutdown();

		// Retry if USB device is not ready
		if(device == USB && retrycount <= STORAGENOTREADYRETRIES) 
		{
			sleep(1);
			retrycount++;
			goto retry;
		}
		
		return FALSE;
	}
	return TRUE;			
}

#define FILENAMECHARACTERS 200

// define USB Loader
//#define WIIFLOW
#define CFGUL

// Configurable USB Loader
#ifdef CFGUL 
	#define MAXFILES 4
	// add/change filenames here, don't forget to edit MAXFILES!
	char filenames[MAXFILES][FILENAMECHARACTERS] = 
	{ 
		{"fat:/apps/usbloader/boot.dol"},	
		{"fat:/apps/usbloader_cfg/boot.dol"},
		{"fat:/apps/usb_loader/boot.dol"},
		{"fat:/apps/usb-loader/boot.dol"}	
	};
#endif

// Wiiflow
#ifdef WIIFLOW
	#define MAXFILES 2
	// add/change filenames here, don't forget to edit MAXFILES!
	char filenames[MAXFILES][FILENAMECHARACTERS] = 
	{ 
		{"fat:/apps/wiiflow/boot.dol"},
		{"fat:/wiiflow/boot.dol"}
	};
#endif

char filename[FILENAMECHARACTERS];
FILE* dolFile = NULL;


void GetFirstAvailableDol( )
{
	int i;
	for(i = 0; i < MAXFILES; i++)
	{
		sprintf(filename, filenames[i]); 
		dolFile = fopen(filename, "rb");
		if(dolFile != NULL) break;
	}
	
	if(dolFile == NULL)
	{
		fatUnmount("fat");
		storage.shutdown();
	}
	
	return;
}


int main(int argc, char **argv) 
{
	// init stuff
	init();

	// try loading from SD
	if(mountStorage(SD)) GetFirstAvailableDol();

	// try loading from USB if no dol file on SD found
	if(dolFile == NULL && mountStorage(USB)) GetFirstAvailableDol();

	// no dol file on SD or USB found, go to wii system menu!
	if(dolFile == NULL) SYS_ResetSystem(SYS_RETURNTOMENU, 0, 0);

	// set default "not set"
#define SELECTED_PARTITION 12	
	char defaultConfig[64];
	strcpy(defaultConfig, "CFGCNF00000000000000000000000000000000000");
	
	char cfgparam[50];
	
	strcpy(cfgparam, "#CRAPPY-X");
	cfgparam[8] = defaultConfig[SELECTED_PARTITION];
	
	//strcpy(cfgparam, "#RSPE01-4");


/*
	// scan for pressed buttons
	int countdown = 110; // needed because wiimote is not ready on first scan pads
	while(countdown > 0)
	{
		WPAD_ScanPads();
		u32 buttons = WPAD_ButtonsDown(0);
		
		VIDEO_WaitVSync();
				
		// 1 and 2 or A and B pressed: force ios=249
		if(   ((buttons & WPAD_BUTTON_1) && (buttons & WPAD_BUTTON_2))
           || ((buttons & WPAD_BUTTON_A) && (buttons & WPAD_BUTTON_B)))
		{
			sprintf(cfgparam, "ios=249");
			break;
		}

		// 1 or A pressed: force ios=222-mload
		if((buttons & WPAD_BUTTON_1) || (buttons & WPAD_BUTTON_A))
		{
			sprintf(cfgparam, "ios=222-mload");
			break;
		}
		
		// 2 or B pressed: force ios=223-mload
		if((buttons & WPAD_BUTTON_2) || (buttons & WPAD_BUTTON_B))
		{
			sprintf(cfgparam, "ios=223-mload");
			break;
		}
		
		countdown--;
	}
*/

	// build arguments for filename and cfgparam
	// argv[0] = filename | argv[1] = cfgparam (could be: ios=222-mload, ios=223-mload or ios=249)
	struct __argv arg;
	
	bzero(&arg, sizeof(arg));
	arg.argvMagic = ARGV_MAGIC;
	
	arg.length = strlen(filename) + 1 + strlen(cfgparam) + 2;
	arg.commandLine = malloc(arg.length);

	strcpy(arg.commandLine, filename);
	strcpy(&arg.commandLine[strlen(filename) + 1], cfgparam);
	arg.commandLine[strlen(filename)] = '\x00';
	arg.commandLine[arg.length - 1] = '\x00';
	arg.commandLine[arg.length - 2] = '\x00';
	
	arg.argc = 2;
	arg.argv = &arg.commandLine;
	arg.endARGV = arg.argv + 2;

	// dol file on SD or USB found, read it!
	int pos = ftell(dolFile);
	fseek(dolFile, 0, SEEK_END);
	int size = ftell(dolFile);
	fseek(dolFile, pos, SEEK_SET);
	void* myBuffer = allocate_memory(size);
	fread( myBuffer, 1, size, dolFile);

	// close the file and the storage after buffering
	fclose(dolFile);
	fatUnmount("fat");
	storage.shutdown();
	
	// Shutdown the Wiimote
	WPAD_Shutdown();

	// run the buffered dol with arguments
	run_dol(myBuffer, &arg);		
	
	return 0;
}

#include <stdio.h>
#include <ogcsys.h>

#include "apploader.h"
#include "wdvd.h"
#include "patchcode.h"
#include "kenobiwii.h"

extern const unsigned char kenobiwii[];
extern const int kenobiwii_size;

/* Apploader function pointers */
typedef int   (*app_main)(void **dst, int *size, int *offset);
typedef void  (*app_init)(void (*report)(const char *fmt, ...));
typedef void *(*app_final)();
typedef void  (*app_entry)(void (**init)(void (*report)(const char *fmt, ...)), int (**main)(), void *(**final)());

/* Apploader pointers */
static u8 *appldr = (u8 *)0x81200000;


/* Constants */
#define APPLDR_OFFSET	0x2440

/* Variables */
static u32 buffer[0x20] ATTRIBUTE_ALIGN(32);


static void __noprint(const char *fmt, ...)
{
}

/** Anti 002 fix for IOS 249 rev < 12 thanks to WiiPower **/
void Anti_002_fix(void *Address, int Size)
{
        u8 SearchPattern[12] =  { 0x2C, 0x00, 0x00, 0x00, 0x48, 0x00, 0x02, 0x14, 0x3C, 0x60, 0x80, 0x00 };
        u8 PatchData[12] =              { 0x2C, 0x00, 0x00, 0x00, 0x40, 0x82, 0x02, 0x14, 0x3C, 0x60, 0x80, 0x00 };

        void *Addr = Address;
        void *Addr_end = Address+Size;

        while(Addr <= Addr_end-sizeof(SearchPattern))
        {
                if(memcmp(Addr, SearchPattern, sizeof(SearchPattern))==0)
                {
                        memcpy(Addr,PatchData,sizeof(PatchData));
                }
                Addr += 4;
        }
}

bool Remove_002_Protection(void *Address, int Size)
{
	unsigned int SearchPattern[3]	= { 0x2C000000, 0x40820214, 0x3C608000 };
	unsigned int PatchData[3]		= { 0x2C000000, 0x48000214, 0x3C608000 };
	unsigned int *Addr				= (unsigned int*)Address;

	while (Size >= 12)
	{
		if (Addr[0] == SearchPattern[0] && Addr[1] == SearchPattern[1] && Addr[2] == SearchPattern[2])
		{
			*Addr = PatchData[0];
			Addr += 1;
			*Addr = PatchData[1];
			Addr += 1;
			*Addr = PatchData[2];
			return true;
		}

		Addr += 1;
		Size -= 4;
	}

	return false;
} 

s32 Apploader_Run(bool ocarinaSelection, int fix, entry_point *entry)
{
	app_entry appldr_entry;
	app_init  appldr_init;
	app_main  appldr_main;
	app_final appldr_final;

	u32 appldr_len;
	s32 ret;

	/* Read apploader header */
	ret = WDVD_Read(buffer, 0x20, APPLDR_OFFSET);
	if (ret < 0)
		return ret;

	/* Calculate apploader length */
	appldr_len = buffer[5] + buffer[6];

	/* Read apploader code */
	ret = WDVD_Read(appldr, appldr_len, APPLDR_OFFSET + 0x20);
	if (ret < 0)
		return ret;

	/* Set apploader entry function */
	appldr_entry = (app_entry)buffer[4];

	/* Call apploader entry */
	appldr_entry(&appldr_init, &appldr_main, &appldr_final);

	/* Initialize apploader */
	appldr_init(__noprint);
	
	if (ocarinaSelection) //ocarinaChoice
      {
		/*HOOKS STUFF - FISHEARS*/
		memset((void*)0x80001800,0,kenobiwii_size);
		memcpy((void*)0x80001800,kenobiwii,kenobiwii_size);
		DCFlushRange((void*)0x80001800,kenobiwii_size);
		hooktype = 1;
		memcpy((void*)0x80001800, (char*)0x80000000, 6);	// For WiiRD
		/*HOOKS STUFF - FISHEARS*/
	}

	for (;;) {
		void *dst = NULL;
		s32   len = 0, offset = 0;

		/* Run apploader main function */
		ret = appldr_main(&dst, &len, &offset);
		if (!ret)
			break;

		/* Read data from DVD */
		WDVD_Read(dst, len, (u64)(offset << 2));
	
	    if(fix){
			*(u32 *)0x80003140 = *(u32 *)0x80003188;
			DCFlushRange(0x80003140, 4); 
			if (fix & 1) 
			{
				*(u32 *)0x80003140 = *(u32 *)0x80003188;
				DCFlushRange(0x80003140, 4); 
			} 
			
			if (fix & 2) 
			{
				Remove_002_Protection(dst, len);
			} 
			
			if (fix & 4) 
			{
				Anti_002_fix(dst, len);
			}

		}

		if (ocarinaSelection) //(ocarinaChoice)
		{
			dogamehooks(dst,len);
			vidolpatcher(dst,len);
		}
		DCFlushRange(dst, len);
	}

	/* Set entry point from apploader */
	*entry = appldr_final();

	return 0;
}

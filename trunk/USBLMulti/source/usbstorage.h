#ifndef _USBSTORAGE_H_
#define _USBSTORAGE_H_ 

/* Prototypes */
s32  USBStorage_GetCapacity(u32 *);
s32  USBStorage_Init(void);
void USBStorage_Deinit(void);
s32  USBStorage_ReadSectors(u32, u32, void *);
s32  USBStorage_WriteSectors(u32, u32, void *);

#endif

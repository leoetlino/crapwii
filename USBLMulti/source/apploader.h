#ifndef _APPLOADER_H_
#define _APPLOADER_H_

/* Entry point */
typedef void (*entry_point)(void);

/* Prototypes */
s32 Apploader_Run(bool, int, entry_point *); 

#endif

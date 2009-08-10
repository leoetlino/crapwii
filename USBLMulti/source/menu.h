#ifndef _MENU_H_
#define _MENU_H_

#ifdef __CRAPMODE__

#define VERBOSE_LOG 6
#define REGION_OVERRIDE 7
#define OVERRIDEN_REGION 8
#define OCARINA_SELECTION 9
#define FORCE_VIDEO_MODE 10
#define SELECTED_LANGUAGE 11
#define USB_OR_SDCARD 12
#define FIX 15

void Menu_Boot(char *, char *);
#else
/* Prototypes */
void Menu_Device(void);
void Menu_Format(void);
void Menu_Install(void);
void Menu_Remove(void);
void Menu_Loop(void);
void Menu_Boot(char *, char *);
#endif


#endif


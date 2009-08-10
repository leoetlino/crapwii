#include <stdio.h>
#include <stdlib.h>
#include <ogcsys.h>

#include "disc.h"
#include "gui.h"
#include "menu.h"
#include "restart.h"
#include "subsystem.h"
#include "sys.h"
#include "video.h"
#include "multilang.h"

char defaultDiscId[7];
char defaultConfig[32];

int main(int argc, char **argv)
{
	/* SET DEFAULT CONFIG FOR CRAP */
	strcpy(defaultDiscId, "CRAPPY");
	//strcpy(defaultDiscId, "RJ8P64");	//Indiana Jones
	//strcpy(defaultDiscId, "RVUP8P");	//Virtua tennis
	strcpy(defaultConfig, "CFGUSB0000000000");
	
	s32 ret;

	/* Load Custom IOS */
	ret = IOS_ReloadIOS(249);

	/* Initialize system */
	Sys_Init();

	/* Initialize subsystems */
	Subsystem_Init();

	/* Set video mode */
	Video_SetMode();

	/* Initialize console */
	Gui_InitConsole();
	
	#ifndef __CRAPMODE__
	/* Show background */
	Gui_DrawBackground();
	#endif

	/* Check if Custom IOS is loaded */
	if (ret < 0) {
		printf(MSG_GENERIC_ERROR);
		printf(MSG_CIOS_RELOAD_ERROR, ret);

		goto out;
	}

	/* Initialize DIP module */
	ret = Disc_Init();
	if (ret < 0) {
		printf(MSG_GENERIC_ERROR);
		printf(MSG_DIP_MODULE_INIT_ERROR, ret);

		goto out;
	}
	
	#ifdef __CRAPMODE__
	Menu_Boot(defaultDiscId, defaultConfig);	
	#else	
	/* Menu loop */
	Menu_Loop();
	#endif

out:
	/* Restart */
	Restart_Wait();

	return 0;
}

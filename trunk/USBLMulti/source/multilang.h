#ifndef _USB_MULTILANG_

#define _USB_MULTILANG_

#ifdef LOADER_LANGUAGE_EN

//menu.c
#define MSG_SELECT_GAME "[+] Select the game you want to boot:\n\n"
#define MSG_NO_GAMES_FOUND "\t>> No games found!!\n"
#define MSG_FREE_SPACE "[+] Free space: %.2fGB\n"
#define MSG_USED_SPACE "    Used space: %.2fGB\n"
#define MSG_DEVNAME_UNKNOWN "Unknown!"
#define MSG_USB_MASS_STORAGE "USB Mass Storage Device"
#define MSG_SD_SDHC "SD/SDHC Card"
#define MSG_SELECT_DEVICE "[+] Select WBFS device:\n"
#define MSG_DEVICE_SELECT "    Press LEFT/RIGHT to select device.\n"
#define MSG_PRESSA_CONTINUE "    Press A button to continue.\n\n"
#define MSG_MOUNTING_DEVICE_WAIT "[+] Mounting device, please wait...\n"
#define MSG_TIMEOUT "    (%d seconds timeout)\n\n"
#define MSG_ERROR_WBFSINIT "    ERROR! (ret = %d)\n"
#define MSG_NO_PARTITIONS "[+] ERROR: No partitions found! (ret = %d)\n"
#define MSG_SELECT_PARTITION "[+] Selected the partition you want to \n"
#define MSG_FORMAT "    format:\n\n"
#define MSG_PART_DISPLAY1 "Partition #%d: (size = %.2fGB)\n"
#define MSG_PART_DISPLAY2 "Partition #%d: (cannot be formatted)\n"

#define MSG_FORMAT_PROMPT1 "[+] Are you sure you want to format\n"
#define MSG_FORMAT_PROMPT2 "    this partition?\n\n"
#define MSG_FORMAT_PROMPT3 "    Partition #%d\n"
#define MSG_FORMAT_PROMPT4 "    (size = %.2fGB - type: %02X)\n\n"
#define MSG_FORMAT_PROMPT5 "    Press A button to continue.\n"
#define MSG_FORMAT_PROMPT6 "    Press B button to go back.\n\n\n"
#define MSG_FORMAT_PROMPT7 "[+] Formatting, please wait..."
#define MSG_ERROR_FORMAT "\n    ERROR! (ret = %d)\n"
#define MSG_FORMAT_OK " OK!\n"
#define MSG_FORMAT_CONTINUE "    Press any button to continue...\n"


#define MSG_INSTALL_PROMPT1 "[+] Are you sure you want to install a\n"
#define MSG_INSTALL_PROMPT2 "    new Wii game?\n\n" 
#define MSG_INSTALL_PROMPT3 "    Press A button to continue.\n"
#define MSG_INSTALL_PROMPT4 "    Press B button to go back.\n\n\n"

#define MSG_INSERT_DISC "[+] Insert the game DVD disc..."
#define MSG_DISC_ERROR "\n    ERROR! (ret = %d)\n"
#define MSG_DISC_OK " OK!\n"
#define MSG_DISC_OK_2 " OK!\n\n"
#define MSG_OPENING_DISC "[+] Opening DVD disc..."
#define MSG_NOT_VALID_DISC "[+] ERROR: Not a Wii disc!!\n"

#define MSG_ERROR_GAME_ALREADY_INSTALLED "[+] ERROR: Game already installed!!\n"
#define MSG_INSTALLING_GAME "[+] Installing game, please wait...\n\n"
#define MSG_ERROR_INSTALLING_GAME "[+] Installation ERROR! (ret = %d)\n"
#define MSG_INSTALL_CONTINUE "    Press any button to continue...\n"

#define MSG_REMOVE_CONFIRMATION_1 "[+] Are you sure you want to remove this\n"
#define MSG_REMOVE_CONFIRMATION_2 "    game?\n\n"

#define MSG_REMOVE_CONTINUE_1 "    Press A button to continue.\n"
#define MSG_REMOVE_CONTINUE_2 "    Press B button to go back.\n\n\n"

#define MSG_REMOVING_WAIT "[+] Removing game, please wait..."
#define MSG_ERROR_REMOVING "\n    ERROR! (ret = %d)\n"
#define MSG_REMOVING_OK " OK!\n"
#define MSG_REMOVE_CONTINUE "    Press any button to continue...\n"
#define MSG_BOOT_CONFIRM_1 "[+] Are you sure you want to boot this\n"
#define MSG_BOOT_CONFIRM_2 "    game?\n\n"

#define MSG_BOOT_CONTINUE_1 "    Press A button to continue.\n"
#define MSG_BOOT_CONTINUE_2 "    Press B button to go back.\n\n"
#define MSG_GAME_BOOTING "[+] Booting Wii game, please wait...\n"
#define MSG_DISC_OPEN_ERROR "    ERROR: Could not open game! (ret = %d)\n"
#define MSG_GAME_BOOT_ERROR "    Returned! (ret = %d)\n"
#define MSG_BOOT_CONTINUE "    Press any button to continue...\n"

#define MSG_FORMAT_CONFIRM_1 "[+] WARNING:\n\n"
#define MSG_FORMAT_CONFIRM_2 "    No WBFS partition found!\n"
#define MSG_FORMAT_CONFIRM_3 "    You need to format a partition.\n\n"
#define MSG_FORMAT_CONFIRM_4 "    Press A button to format a partition.\n"
#define MSG_FORMAT_CONFIRM_5 "    Press B button to restart.\n\n"

//restart.c

#define MSG_RESTARTING "\n    Restarting Wii..."
#define MSG_RESTART_CONFIRMATION "    Press any button to restart..."

//usb-loader.c

#define MSG_GENERIC_ERROR "[+] ERROR:\n"
#define MSG_CIOS_RELOAD_ERROR "    Custom IOS could not be loaded! (ret = %d)\n"
#define MSG_DIP_MODULE_INIT_ERROR "    Could not initialize DIP module! (ret = %d)\n"

//wbfs.c
#define MSG_WBFS_PROGRESS_INDICATOR "    %.2f%% of %.2fGB (%c) ETA: %d:%02d:%02d\r"
#define MSG_WBFS_RESULT "    %.2fGB copied in %d:%02d:%02d\n"

#endif

#ifdef LOADER_LANGUAGE_TR

//menu.c
#define MSG_SELECT_GAME "[+] Oynamak istedi�iniz oyunu se�in:\n\n"
#define MSG_NO_GAMES_FOUND "\t>> Hi� oyun bulunamad�!!\n"
#define MSG_FREE_SPACE "[+] Bo$  alan : %.2fGB\n"
#define MSG_USED_SPACE "    Dolu alan : %.2fGB\n"
#define MSG_DEVNAME_UNKNOWN "Bilinmiyor!"
#define MSG_USB_MASS_STORAGE "USB Depolama Ayg�t�"
#define MSG_SD_SDHC "SD/SDHC Kart"
#define MSG_SELECT_DEVICE "[+] WBFS Birimini se�in:\n"
#define MSG_DEVICE_SELECT "    SOL/SA�'a t�klayarak birimi se�in.\n"
#define MSG_PRESSA_CONTINUE "    Devam etmek i�in A tu$una bas�n.\n\n"
#define MSG_MOUNTING_DEVICE_WAIT "[+] Birim ba�lan�yor, l�tfen bekleyin...\n"
#define MSG_TIMEOUT "    (en fazla %d saniye bekleme)\n\n"
#define MSG_ERROR_WBFSINIT "    HATA! (ret = %d)\n"
#define MSG_NO_PARTITIONS "[+] HATA: Hi� b�l�m bulunamad�! (d�n�$ kodu = %d)\n"
#define MSG_SELECT_PARTITION "[+] Formatlayaca��n�z b�l�m� \n"
#define MSG_FORMAT "    se�in:\n\n"
#define MSG_PART_DISPLAY1 "B�l�m #%d: (boyut = %.2fGB)\n"
#define MSG_PART_DISPLAY2 "B�l�m #%d: (formatlanamaz)\n"

#define MSG_FORMAT_PROMPT1 "[+] Bu b�l�m� formatlamak istedi�inize\n"
#define MSG_FORMAT_PROMPT2 "    emin misiniz??\n\n"
#define MSG_FORMAT_PROMPT3 "    B�l�m #%d\n"
#define MSG_FORMAT_PROMPT4 "    (boyut = %.2fGB - tip: %02X)\n\n"
#define MSG_FORMAT_PROMPT5 "    Onayl�yorsan�z A tu$una bas�n.\n"
#define MSG_FORMAT_PROMPT6 "    Geri d�nmek i�in B tu$una bas�n.\n\n\n"
#define MSG_FORMAT_PROMPT7 "[+] Formatlan�yor, l�tfen bekleyin.."
#define MSG_ERROR_FORMAT "\n    HATA! (ret = %d)\n"
#define MSG_FORMAT_OK " OK!\n"
#define MSG_FORMAT_CONTINUE "    Devam etmek i�in bir tu$a bas�n.\n"


#define MSG_INSTALL_PROMPT1 "[+] Yeni oyunu y�klemek istedi�inize \n"
#define MSG_INSTALL_PROMPT2 "    emin misiniz?\n\n" 
#define MSG_INSTALL_PROMPT3 "    Onayl�yorsan�z A tu$una bas�n.\n"
#define MSG_INSTALL_PROMPT4 "    Geri d�nmek i�in B tu$una bas�n.\n\n\n"

#define MSG_INSERT_DISC "[+] Oyun DVD'sini s�r�c�ye yerle$tirin..."
#define MSG_DISC_ERROR "\n    HATA! (ret = %d)\n"
#define MSG_DISC_OK " OK!\n"
#define MSG_DISC_OK_2 " OK!\n\n"
#define MSG_OPENING_DISC "[+] Oyun DVD'si a��l�yor"
#define MSG_NOT_VALID_DISC "[+] HATA: Bu bir Wii diski de�il!!\n"

#define MSG_ERROR_GAME_ALREADY_INSTALLED "[+] HATA: Oyun zaten y�klenmi$!!\n"
#define MSG_INSTALLING_GAME "[+] Oyun y�kleniyor, l�tfen bekleyiniz...\n\n"
#define MSG_ERROR_INSTALLING_GAME "[+] Y�kleme HATASI! (ret = %d)\n"
#define MSG_INSTALL_CONTINUE "    Devam etmek i�in bir tu$a bas�n.\n"

#define MSG_REMOVE_CONFIRMATION_1 "[+] Bu oyunu kald�rmak istedi�inizden\n"
#define MSG_REMOVE_CONFIRMATION_2 "    emin misiniz?\n\n"

#define MSG_REMOVE_CONTINUE_1 "    Onayl�yorsan�z A tu$una bas�n.\n"
#define MSG_REMOVE_CONTINUE_2 "    Geri d�nmek i�in B tu$una bas�n.\n\n\n"

#define MSG_REMOVING_WAIT "[+] Oyun kald�r�l�yor, l�tfen bekleyin..."
#define MSG_ERROR_REMOVING "\n    HATA! (d�n�$ kodu = %d)\n"
#define MSG_REMOVING_OK " OK!\n"
#define MSG_REMOVE_CONTINUE "    Devam etmek i�in bir tu$a bas�n.\n"
#define MSG_BOOT_CONFIRM_1 "[+] Bu oyunu �al�$t�rmak istedi�inize\n"
#define MSG_BOOT_CONFIRM_2 "    emin misiniz?\n\n"

#define MSG_BOOT_CONTINUE_1 "    Onayl�yorsan�z A tu$una bas�n.\n"
#define MSG_BOOT_CONTINUE_2 "    Geri d�nmek i�in B tu$una bas�n.\n\n"
#define MSG_GAME_BOOTING "[+] Oyun ba$lat�l�yor, l�tfen bekleyin...\n"
#define MSG_DISC_OPEN_ERROR "    HATA: Oyun ba$lat�lamad�! (d�n�$ = %d)\n"
#define MSG_GAME_BOOT_ERROR "    D�nd�m! (ret = %d)\n"
#define MSG_BOOT_CONTINUE "    Devam etmek i�in bir tu$a bas�n...\n"

#define MSG_FORMAT_CONFIRM_1 "[+] UYARI:\n\n"
#define MSG_FORMAT_CONFIRM_2 "    Hi�bir WBFS b�l�m� bulunamad�!\n"
#define MSG_FORMAT_CONFIRM_3 "    Bir b�l�m formatlamal�s�n�z.\n\n"
#define MSG_FORMAT_CONFIRM_4 "    B�l�m formatlamak i�in A tu$una bas�n.\n"
#define MSG_FORMAT_CONFIRM_5 "    B butonu ile ba$tan ba$lat�n.\n\n"

//restart.c

#define MSG_RESTARTING "\n    Wii yeniden ba$lat�l�yor..."
#define MSG_RESTART_CONFIRMATION "    Yeniden ba$latmak i�in bir tu$a bas�n..."

//usb-loader.c

#define MSG_GENERIC_ERROR "[+] HATA:\n"
#define MSG_CIOS_RELOAD_ERROR "    CIOS y�klenemedi! (d�n�$ = %d)\n"
#define MSG_DIP_MODULE_INIT_ERROR "    DIP mod�l� ba$lat�lamad�! (d�n�$ = %d)\n"

//wbfs.c
#define MSG_WBFS_PROGRESS_INDICATOR "    %.2f%% --- %.2fGB (%c) TBZ: %d:%02d:%02d\r"
#define MSG_WBFS_RESULT "    %.2fGB %d:%02d:%02d saniyede kopyaland�\n"


#endif


#ifdef LOADER_LANGUAGE_DE
#endif

#ifdef LOADER_LANGUAGE_FR
#endif

#endif

//Possibly other languages...
//If your language needs proper font support then you'll need to recompile ogc changing the 
//console_font_8x16.c

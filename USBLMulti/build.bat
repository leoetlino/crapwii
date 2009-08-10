rem ***************************************
rem 
rem 	%1 language
rem 	%2 build/rebuild/clean
rem 
rem ***************************************

PATH=c:\devkitPro\msys\bin;%PATH%

if "%2"=="clean" (

	make clean
	goto end
)

if "%2"=="rebuild" make clean

if "%1" == "EN" (
	make CFLAGS="-Os -Wall $(MACHDEP) $(INCLUDE) -DLOADER_LANGUAGE_EN"
	goto end
)

if "%1" == "TR" or "%1" == "" (
	make CFLAGS="-Os -Wall $(MACHDEP) $(INCLUDE) -DLOADER_LANGUAGE_TR"
	goto end
)


if "%1" == "DE" (
	make CFLAGS="-Os -Wall $(MACHDEP) $(INCLUDE) -DLOADER_LANGUAGE_DE"
	goto end
)

if "%1" == "FR" (
	make CFLAGS="-Os -Wall $(MACHDEP) $(INCLUDE) -DLOADER_LANGUAGE_FR"
	goto end
)
REM BLAH BLAH... ADD OTHER LANGUAGES
goto end


:end

PAUSE

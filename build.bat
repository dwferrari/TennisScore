@echo off
@setlocal EnableDelayedExpansion enableextensions
@cls
@cd %~dp0
@SET PROGRAM_NAME=%~dp0
@for /D %%a in ("%PROGRAM_NAME:~0,-1%.txt") do @SET PROGRAM_NAME=%%~na

@SET EABI=arm-none-eabi

@SET LIB_BIP_PATH="..\libbip"
@SET LIB_BIP="%LIB_BIP_PATH%\libbip.a" 

@SET GCC_OPT=-mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -fno-math-errno -I "%LIB_BIP_PATH%" -c -Os -Wa,-R -Wall -fpie -pie -fpic -mthumb -mlittle-endian  -ffunction-sections -fdata-sections
@SET LD_OPT=-lm -lc -EL -N -Os --cref -pie --gc-sections --no-dynamic-linker

@SET AS=%EABI%-as
@SET LD=%EABI%-ld
@SET OBJCOPY=%EABI%-objcopy
@SET GCC=%EABI%-gcc
@SET NM=%EABI%-nm

if exist label.txt (
set /p LABEL=< label.txt
) else (
SET LABEL = %PROGRAM_NAME%
)

@call :echoColor 0F "====================================" 1
@call :echoColor 0F "Project Information: "
@echo %PROGRAM_NAME%
@call :echoColor 0F "Compiler: "
@echo %COMPILER%
@call :echoColor 0F "====================================" 1
@echo.	

@call :echoColor 0F "Compiling Source Files..." 1
@SET PARTNAME=%PROGRAM_NAME%
@call :echoColor 0B "Target Device: "
@call :echoColor 0E "%PARTNAME%" 1

@SET n=1
@for  %%f in (*.c) do ( 
@	SET FILES_TO_COMPILE=!FILES_TO_COMPILE! %%~nf.o
@call :EchoN "%n%.    Compiling %%~nf.c"
!GCC! !GCC_OPT! -o %%~nf.o %%~nf.c
@if errorlevel 1 goto :error
@call :echoColor 0A "...OK" 1
@SET /a n=n+1)
@SET /a n=n-1
@call :echoColor 0B "Number of Files: "
@call :echoColor 0E "%n%" 1

@call :echoColor 0B "Linking:"
@call :echoColor 07 "    Linking ELF executable..."	1
%LD% -Map %PARTNAME%.map -o %PROGRAM_NAME%.elf %FILES_TO_COMPILE% %LD_OPT% %LIB_BIP%
@if errorlevel 1 goto :error

if exist label.txt (
@call :echoColor 07 "    Adding labels..."	1
%OBJCOPY%  %PROGRAM_NAME%.elf --add-section .elf.label=label.txt
)

@call :EchoN "%PROGRAM_NAME%" > name.txt
@call :echoColor 07 "    Adding elf_name section..."	1
%OBJCOPY%  %PROGRAM_NAME%.elf --add-section .elf.name=name.txt
if exist name.txt del name.txt
@if errorlevel 1 goto :error

if exist asset.res (
@call :echoColor 07 "    Adding resources section..."	1
%OBJCOPY%  %PROGRAM_NAME%.elf --add-section .elf.resources=asset.res
)
if exist settings.bin (
@call :echoColor 07 "    Adding settings section..."	1
%OBJCOPY%  %PROGRAM_NAME%.elf --add-section .elf.settings=settings.bin
)

@call :echoColor 0A "...OK" 1
@call :echoColor 0B "Linking Completed." 1

:done_

@call :echoColor 0A "Process Finished." 1 
pause 
@goto :EOF

:error
@call :echoColor 4e Error! 1
@endlocal & @SET ERROR=ERROR
@pause
@goto :EOF

::===========================================================
:: A function to print text in the specified color without a newline
:EchoN
    
@    <nul set /p strTemp=%~1
@    exit /b 0
::===========================================================
::	Colors for text printing (Windows CMD color codes)
::	3 = Cyan, 8 = Gray
::	1 = Blue, 9 = Light Blue
::	2 = Green, A = Light Green
::	3 = Aqua, B = Light Aqua
::	4 = Red, C = Light Red
::	5 = Purple, D = Light Purple
::	6 = Yellow, E = Light Yellow
::	7 = White, F = Default White
:echoColor [Color] [Text] [\n]
 @ if not defined BS for /F "tokens=1 delims=#" %%i in ('"prompt #$H#& echo on& for %%j in (.) do rem"') do set "BS=%%i"
 @ if not exist foo set /p .=.<nul>foo
 @ set "regex=%~2" !
 @ set "regex=%regex:"="%"
 @ findstr /a:%1 /prc:"\." "%regex%\..\foo" nul
 @ set /p .=%BS%%BS%%BS%%BS%%BS%%BS%%BS%%BS%%BS%<nul
 @ if "%3" neq "" echo.
 @exit /b 0
::===========================================================
====================================================

@echo off
setlocal

:: Set root and build directories
set "SQLITE_ROOT=%~dp0"
set "BUILD_DIR=%SQLITE_ROOT%build-release"
set "INSTALL_DIR=%SQLITE_ROOT%install-release"

:: Ensure Visual Studio environment is set up correctly
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" || (
    echo [ERROR] Failed to set up Visual Studio environment.
    pause
    exit /b 1
)

:: Create build directory if it doesn't exist
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%" || (
    echo [ERROR] Failed to access build directory: %BUILD_DIR%
    pause
    exit /b 1
)

:: Build Lemon Parser Generator
echo [INFO] Building Lemon Parser Generator...
cd /d "%SQLITE_ROOT%\tool" || (
    echo [ERROR] Failed to access SQLite tool directory.
    pause
    exit /b 1
)

cl /nologo /W4 /O2 /Zi /MT /DNDEBUG lemon.c /Felemon.exe || (
    echo [ERROR] Failed to build Lemon.
    pause
    exit /b 1
)

copy /y "%SQLITE_ROOT%\tool\lemon.exe" "%SQLITE_ROOT%\" || (
    echo [ERROR] Failed to copy lemon.exe.
    pause
    exit /b 1
)

:: Ensure we are in the correct directory before generating parse.h and parse.c
cd /d "%SQLITE_ROOT%" || (
    echo [ERROR] Failed to access SQLite root directory: %SQLITE_ROOT%
    pause
    exit /b 1
)

:: Generate parse.h and parse.c using Lemon
echo [INFO] Generating parse.h and parse.c...

set LEMON=  :: Clear any incorrect LEMON environment variable
copy /y "%SQLITE_ROOT%\tool\lempar.c" "%SQLITE_ROOT%\" || (
    echo [ERROR] Failed to copy lempar.c.
    pause
    exit /b 1
)

lemon.exe src\parse.y || (
    echo [ERROR] Failed to generate parse.h and parse.c.
    pause
    exit /b 1
)

:: Ensure parse.h and parse.c exist before copying them
if not exist "%SQLITE_ROOT%\src\parse.h" (
    echo [ERROR] parse.h was not generated in src folder!
    pause
    exit /b 1
)

if not exist "%SQLITE_ROOT%\src\parse.c" (
    echo [ERROR] parse.c was not generated in src folder!
    pause
    exit /b 1
)

:: Copy parse.h and parse.c to root
copy /y "%SQLITE_ROOT%\src\parse.h" "%SQLITE_ROOT%\" || (
    echo [ERROR] Failed to copy parse.h to root.
    pause
    exit /b 1
)

copy /y "%SQLITE_ROOT%\src\parse.c" "%SQLITE_ROOT%\" || (
    echo [ERROR] Failed to copy parse.c to root.
    pause
    exit /b 1
)

:: Generate opcodes.h
echo [INFO] Generating opcodes.h...
type "%SQLITE_ROOT%\parse.h" "%SQLITE_ROOT%\src\vdbe.c" | tclsh tool\mkopcodeh.tcl > opcodes.h || (
    echo [ERROR] Failed to generate opcodes.h.
    pause
    exit /b 1
)

:: Build SQLite with optimizations
echo [INFO] Building SQLite for Release...
nmake /f Makefile.msc USE_AMALGAMATION=1 USE_DLL=1 USE_FULLWARN=1 || (
    echo [ERROR] SQLite build process failed.
    pause
    exit /b 1
)

:: Install SQLite binaries and headers
echo [INFO] Installing SQLite (binaries and libraries)...
if not exist "%INSTALL_DIR%\bin" mkdir "%INSTALL_DIR%\bin"
if not exist "%INSTALL_DIR%\lib" mkdir "%INSTALL_DIR%\lib"
if not exist "%INSTALL_DIR%\include" mkdir "%INSTALL_DIR%\include"

copy /y "%SQLITE_ROOT%\sqlite3.dll" "%INSTALL_DIR%\bin\" || (
    echo [ERROR] Failed to copy sqlite3.dll.
    pause
    exit /b 1
)
copy /y "%SQLITE_ROOT%\sqlite3.lib" "%INSTALL_DIR%\lib\" || (
    echo [ERROR] Failed to copy sqlite3.lib.
    pause
    exit /b 1
)
copy /y "%SQLITE_ROOT%\sqlite3.h" "%INSTALL_DIR%\include\" || (
    echo [ERROR] Failed to copy sqlite3.h.
    pause
    exit /b 1
)

echo [SUCCESS] SQLite built and installed successfully.
pause
exit /b 0

@echo off

:: This batch file builds libwebsockets from source code using MSVC (cl.exe) for Debug.

:: Set the root folder as the location of this script
set "LIBWEBSOCKETS_ROOT=%~dp0"
set "BUILD_DIR=%LIBWEBSOCKETS_ROOT%build-debug"
set "INSTALL_DIR=%LIBWEBSOCKETS_ROOT%install-debug"

:: Ensure Visual Studio environment is set up correctly
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" || (
    echo [ERROR] Failed to set up Visual Studio environment.
    exit /b 1
)

:: Record start time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_START=%%a:%%b:%%c"

:: Create the build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%" || (
    echo [ERROR] Failed to access build directory: %BUILD_DIR%
    pause
    exit /b 1
)

:: Configure libwebsockets with CMake
echo [INFO] Configuring libwebsockets with CMake...
cmake -G "Ninja" ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_C_COMPILER=cl ^
  -DLWS_WITH_STATIC=OFF ^
  -DLWS_WITH_SHARED=ON ^
  -DLWS_WITHOUT_TESTAPPS=ON ^
  -DLWS_WITH_MINIMAL_EXAMPLES=OFF ^
  -DCMAKE_C_FLAGS="/Zi /Od /MDd /EHsc" ^
  -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
  "%LIBWEBSOCKETS_ROOT%" || (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

:: Build libwebsockets with Ninja
echo [INFO] Building libwebsockets...
ninja -j%NUMBER_OF_PROCESSORS% || (
    echo [ERROR] Build process failed.
    pause
    exit /b 1
)

:: Install libwebsockets
echo [INFO] Installing libwebsockets...
ninja install || (
    echo [ERROR] Install process failed.
    pause
    exit /b 1
)

:: Record end time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_END=%%a:%%b:%%c"

:: Success message
echo [SUCCESS] libwebsockets built and installed successfully (Debug).
echo -------------------------------------------------
echo Libraries and DLLs are located in: %INSTALL_DIR%\lib
echo Headers are located in: %INSTALL_DIR%\include
echo Build duration: %TIME_START% - %TIME_END%
echo -------------------------------------------------

pause
exit /b 0

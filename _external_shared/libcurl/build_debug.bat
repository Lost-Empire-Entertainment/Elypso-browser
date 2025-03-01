@echo off

:: This batch file builds libcurl from source using MSVC (cl.exe) for Debug.

:: Set the root folder as the location of this script
set "CURL_ROOT=%~dp0"
set "BUILD_DIR=%CURL_ROOT%build-debug"
set "INSTALL_DIR=%CURL_ROOT%install-debug"

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

:: Configure libcurl with CMake
echo [INFO] Configuring libcurl with CMake...
cmake -G "Ninja" ^
  -DCMAKE_BUILD_TYPE=Debug ^
  -DCMAKE_C_COMPILER=cl ^
  -DBUILD_SHARED_LIBS=ON ^
  -DCURL_USE_OPENSSL=ON ^
  -DCURL_DISABLE_TESTS=ON ^
  -DCMAKE_C_FLAGS="/Od /Zi /EHsc /MDd" ^
  -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
  "%CURL_ROOT%" || (
    echo [ERROR] CMake configuration failed.
	pause
    exit /b 1
)

:: Build libcurl with Ninja
echo [INFO] Building libcurl...
ninja -j%NUMBER_OF_PROCESSORS% || (
    echo [ERROR] Build process failed.
	pause
    exit /b 1
)

:: Install libcurl
echo [INFO] Installing libcurl...
ninja install || (
    echo [ERROR] Install process failed.
	pause
    exit /b 1
)

:: Record end time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_END=%%a:%%b:%%c"

:: Success message
echo [SUCCESS] libcurl built and installed successfully.
echo ---------------------------------------------
echo Dynamic library: %INSTALL_DIR%\bin\libcurl.dll
echo Static library: %INSTALL_DIR%\lib\libcurl.lib
echo Include headers: %INSTALL_DIR%\include
echo Build duration: %TIME_START% - %TIME_END%
echo ---------------------------------------------

pause
exit /b 0

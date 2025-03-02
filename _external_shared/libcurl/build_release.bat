@echo off

:: This batch file builds libcurl from source code using MSVC (cl.exe) for Release.

:: Set the root folder as the location of this script
set "LIBCURL_ROOT=%~dp0"
set "BUILD_DIR=%LIBCURL_ROOT%build-release"
set "INSTALL_DIR=%LIBCURL_ROOT%install-release"

set "LIBCURL_LIB=%LIBCURL_ROOT%\..\LibPSL\release\psl.lib"
set "LIBCURL_HEADERS=%LIBCURL_ROOT%\..\LibPSL"

set "NGHTTP2_LIB=%LIBCURL_ROOT%\..\NGHTTP2\release\nghttp2.lib"
set "NGHTTP2_HEADERS=%LIBCURL_ROOT%\..\NGHTTP2"

set "BROTLI_DEC=%LIBCURL_ROOT%\..\Brotli\release\brotlidec.lib"
set "BROTLI_COMMON=%LIBCURL_ROOT%\..\Brotli\release\brotlicommon.lib"
set "BROTLI_HEADERS=%LIBCURL_ROOT%\..\Brotli"

set "ZSTD_LIB=%LIBCURL_ROOT%\..\Zstd\release\zstd.lib"
set "ZSTD_HEADERS=%LIBCURL_ROOT%\..\Zstd"

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
  -DCMAKE_BUILD_TYPE=Release ^
  -DCMAKE_C_COMPILER=cl ^
  -DBUILD_SHARED_LIBS=ON ^
  -DCMAKE_C_FLAGS="/O2 /GL /DNDEBUG /EHsc /MD" ^
  -DCMAKE_INSTALL_PREFIX="%INSTALL_DIR%" ^
  -DCURL_USE_OPENSSL=ON ^
  -DLIBPSL_LIBRARY="%LIBCURL_LIB%" ^
  -DLIBPSL_INCLUDE_DIR="%LIBCURL_HEADERS%" ^
  -DNGHTTP2_LIBRARY="%NGHTTP2_LIB%" ^
  -DNGHTTP2_INCLUDE_DIR="%NGHTTP2_HEADERS%" ^
  -DBROTLIDEC_LIBRARY="%BROTLI_DEC%" ^
  -DBROTLICOMMON_LIBRARY="%BROTLI_COMMON%" ^
  -DBROTLI_INCLUDE_DIR="%BROTLI_HEADERS%" ^
  -DZSTD_LIBRARY="%ZSTD_LIB%" ^
  -DZSTD_INCLUDE_DIR="%ZSTD_HEADERS%" ^
  "%LIBCURL_ROOT%" || (
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
echo -------------------------------------------------
echo Libraries and DLLs are located in: %INSTALL_DIR%\lib
echo Headers are located in: %INSTALL_DIR%\include
echo Build duration: %TIME_START% - %TIME_END%
echo -------------------------------------------------

pause
exit /b 0

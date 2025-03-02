@echo off
setlocal

:: This batch file builds zstd from source using Meson (MSVC compatible)
:: in Debug mode and disables tests, programs, and contributed binaries.

:: Set the root folder as the location of this script
set "MESON_ROOT=%~dp0"
set "BUILD_DIR=%MESON_ROOT%build-debug"
set "INSTALL_DIR=%MESON_ROOT%install-debug"
:: Source directory where meson.build is located (in build\meson)
set "SOURCE_DIR=%MESON_ROOT%build\meson"

:: Ensure Visual Studio environment is set up correctly (for MSVC users)
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" || (
        echo [ERROR] Failed to set up Visual Studio environment.
        pause
        exit /b 1
    )
)

:: Record start time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_START=%%a:%%b:%%c"

:: First-time setup (only add --prefix if the build directory is new)
if not exist "%BUILD_DIR%" (
    echo [INFO] Running first-time Meson setup...
    meson setup "%BUILD_DIR%" --prefix "%INSTALL_DIR%" "%SOURCE_DIR%" -Dbuildtype=debug -Dbin_tests=false -Dbin_programs=false -Dbin_contrib=false || (
        echo [ERROR] Meson setup failed.
        pause
        exit /b 1
    )
) else (
    echo [INFO] Reconfiguring Meson build...
    meson setup --reconfigure "%BUILD_DIR%" || (
        echo [ERROR] Meson reconfiguration failed.
        pause
        exit /b 1
    )
)

:: Build the project with parallel compilation
echo [INFO] Compiling the project...
meson compile -C "%BUILD_DIR%" -j%NUMBER_OF_PROCESSORS% || (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

:: Ensure the install directory exists
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

:: Install the project
echo [INFO] Installing the project...
meson install -C "%BUILD_DIR%" || (
    echo [ERROR] Install failed.
    pause
    exit /b 1
)

:: Record end time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_END=%%a:%%b:%%c"

:: Success message
echo [SUCCESS] Build and installation completed successfully!
echo ---------------------------------------------------------
echo Installed at: %INSTALL_DIR%
echo Build duration: %TIME_START% - %TIME_END%
echo ---------------------------------------------------------

pause
exit /b 0

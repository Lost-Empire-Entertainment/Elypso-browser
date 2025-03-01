@echo off
setlocal

:: Set the root folder as the location of this script
set "MESON_ROOT=%~dp0"
set "BUILD_DIR=%MESON_ROOT%build-debug"
set "INSTALL_DIR=%MESON_ROOT%install-debug"

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

:: Check if Meson is installed
where meson >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Meson is not installed. Please install it first.
    pause
    exit /b 1
)

:: Check if Ninja is installed
where ninja >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Ninja is not installed. Please install it first.
    pause
    exit /b 1
)

:: First-time setup (only add --prefix if the build directory is new)
if not exist "%BUILD_DIR%" (
    echo [INFO] Running first-time Meson setup for DEBUG...
    meson setup "%BUILD_DIR%" --prefix "%INSTALL_DIR%" --buildtype=debug || (
        echo [ERROR] Meson setup failed for DEBUG.
        pause
        exit /b 1
    )
) else (
    echo [INFO] Reconfiguring Meson build for DEBUG...
    meson setup --reconfigure "%BUILD_DIR%" --buildtype=debug || (
        echo [ERROR] Meson reconfiguration failed for DEBUG.
        pause
        exit /b 1
    )
)

:: Build the project with parallel compilation
echo [INFO] Compiling the DEBUG build...
meson compile -C "%BUILD_DIR%" -j%NUMBER_OF_PROCESSORS% || (
    echo [ERROR] DEBUG build failed.
    pause
    exit /b 1
)

:: Ensure the install directory exists
if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

:: Install the Debug build
echo [INFO] Installing the DEBUG build...
meson install -C "%BUILD_DIR%" || (
    echo [ERROR] DEBUG installation failed.
    pause
    exit /b 1
)

:: Record end time
for /f "tokens=1-4 delims=:.," %%a in ("%TIME%") do set "TIME_END=%%a:%%b:%%c"

:: Success message
echo [SUCCESS] DEBUG build and installation completed successfully!
echo ---------------------------------------------------------
echo Debug installed at: %INSTALL_DIR%
echo Build duration: %TIME_START% - %TIME_END%
echo ---------------------------------------------------------

pause
exit /b 0

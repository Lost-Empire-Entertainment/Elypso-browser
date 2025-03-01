@echo off
setlocal

:: Set root and build directories for DEBUG version
set "OPENSSL_ROOT=%~dp0"
set "BUILD_DIR=%OPENSSL_ROOT%build-debug"
set "INSTALL_DIR=%OPENSSL_ROOT%install-debug"

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

:: Configure OpenSSL for Debug build
echo [INFO] Configuring OpenSSL for DEBUG...
perl "%OPENSSL_ROOT%\Configure" VC-WIN64A --prefix="%INSTALL_DIR%" no-weak-ssl-ciphers no-tests no-docs debug || (
    echo [ERROR] OpenSSL configuration failed for DEBUG.
    pause
    exit /b 1
)

echo [INFO] Building OpenSSL DEBUG version (single-threaded)...
nmake || (
    echo [ERROR] DEBUG Build process failed.
    pause
    exit /b 1
)

echo [INFO] Installing OpenSSL DEBUG (binaries and libraries only)...
nmake install_sw || (
    echo [ERROR] DEBUG Install process failed.
    pause
    exit /b 1
)

echo [SUCCESS] OpenSSL DEBUG version built and installed successfully.
pause
exit /b 0

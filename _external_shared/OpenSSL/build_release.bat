@echo off
setlocal

:: Set root and build directories
set "OPENSSL_ROOT=%~dp0"
set "BUILD_DIR=%OPENSSL_ROOT%build-release"
set "INSTALL_DIR=%OPENSSL_ROOT%install-release"

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

:: Configure OpenSSL with optimizations
echo [INFO] Configuring OpenSSL for Release...
perl "%OPENSSL_ROOT%\Configure" VC-WIN64A --prefix="%INSTALL_DIR%" no-weak-ssl-ciphers no-tests no-docs || (
    echo [ERROR] OpenSSL configuration failed.
    pause
    exit /b 1
)

echo [INFO] Building OpenSSL (single-threaded)...
nmake || (
    echo [ERROR] Build process failed.
    pause
    exit /b 1
)

echo [INFO] Installing OpenSSL (binaries and libraries only)...
nmake install_sw || (
    echo [ERROR] Install process failed.
    pause
    exit /b 1
)

echo [SUCCESS] OpenSSL built and installed successfully.
pause
exit /b 0

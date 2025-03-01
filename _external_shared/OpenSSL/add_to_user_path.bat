@echo off
setlocal

echo Getting current OpenSSL directory...

:: Get the directory where this script is located
set "OPENSSL_DIR=%~dp0"
set "OPENSSL_DIR=%OPENSSL_DIR:~0,-1%"

:: Define the bin directory (adjust if needed)
set "OPENSSL_BIN=%OPENSSL_DIR%\bin"

echo OpenSSL directory detected: "%OPENSSL_BIN%"

:: Get the current user-specific PATH
for /f "tokens=2,* delims= " %%A in ('reg query "HKCU\Environment" /v PATH 2^>nul') do set "current_path=%%B"
if not defined current_path set "current_path="
set "new_path=%current_path%"

echo Checking if OpenSSL is already in PATH...

:: Add OpenSSL directory to PATH if not already present
echo %current_path% | find /i "%OPENSSL_BIN%" >nul || (
    echo Adding OpenSSL to PATH...
    set "new_path=%new_path%;%OPENSSL_BIN%"
)

:: Update the PATH for the current user
setx PATH "%new_path%"
if errorlevel 1 (
    echo [ERROR] Failed to update PATH.
    exit /b 1
)

echo [SUCCESS] OpenSSL has been added to the User PATH.

endlocal
pause
exit /b 0

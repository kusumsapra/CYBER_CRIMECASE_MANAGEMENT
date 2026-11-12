@echo off
setlocal
cd /d "%~dp0.."
if not exist build\cybercrime.exe (
  echo The source is provided, but a fresh MSVC build is required on this computer.
  echo Follow docs\WINDOWS_SETUP.md for the Connector/C++ include and library paths.
  pause
  exit /b 1
)
if "%MYSQL_HOST%"=="" set MYSQL_HOST=localhost
if "%MYSQL_X_PORT%"=="" set MYSQL_X_PORT=33060
if "%MYSQL_USER%"=="" set MYSQL_USER=cybercrime_app
if "%MYSQL_PASSWORD%"=="" (
  echo Set MYSQL_PASSWORD in this terminal before launching.
  pause
  exit /b 1
)
set "PATH=%~dp0..\build;%PATH%"
start "Cyber Crime Terminal" /wait build\cybercrime.exe

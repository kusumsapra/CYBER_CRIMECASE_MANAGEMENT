@echo off
setlocal
cd /d "%~dp0..\web\frontend"
if not exist node_modules call npm install
call npm run dev

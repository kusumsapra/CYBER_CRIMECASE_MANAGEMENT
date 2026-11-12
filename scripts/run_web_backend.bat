@echo off
setlocal
cd /d "%~dp0..\web\backend"
if not exist .env (
  copy .env.example .env
  echo Created web\backend\.env. Edit DB_PASSWORD and JWT_SECRET, then run again.
  pause
  exit /b 1
)
if not exist venv\Scripts\python.exe py -3 -m venv venv
call venv\Scripts\activate.bat
python -m pip install --upgrade pip
pip install -r requirements.txt
python -m uvicorn app.main:app --reload --host 127.0.0.1 --port 8000

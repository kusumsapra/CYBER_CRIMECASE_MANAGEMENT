# Windows setup and run guide

## 1. Prerequisites
- MySQL Server 8.0 (X Protocol enabled on port 33060; SQL port 3306).
- MySQL Workbench (optional, for viewing tables).
- Python 3.10+ and Node.js 18+.
- Visual Studio 2022 Build Tools with **Desktop development with C++** for rebuilding the C/C++ terminal program.
- MySQL Connector/C++ 8.x/9.x or the Connector/C++ package matching the compiler and ABI used by the installed connector. The old project used Connector/C++ X DevAPI; keep its headers, import libraries and DLLs from the same package/version.

## 2. Create the database
1. Open MySQL Workbench and connect to your local MySQL server.
2. Open `database/schema.sql` and execute it.
3. Open `scripts/setup_database.sql`, replace `CHANGE_THIS_PASSWORD` with a strong password, then execute it as a MySQL administrator.
4. Copy `web/backend/.env.example` to `web/backend/.env` (the supplied `.env` is a template) and set `DB_PASSWORD` to the same password. Set `JWT_SECRET` to a unique random string of at least 32 characters.

The schema uses the table/column names used by the existing C++ database layer: `cases`, `users`, `officers`, `evidence`, `investigation_history`, and `reports`. If you already have an older database with tables created, back it up first. `CREATE TABLE IF NOT EXISTS` does not migrate old columns or constraints; compare the existing tables with `database/schema.sql` before altering them.

## 3. Run the web backend
Double-click `scripts/run_web_backend.bat`, or from CMD:

```cmd
cd web\backend
py -3 -m venv venv
venv\Scripts\activate
pip install -r requirements.txt
python -m uvicorn app.main:app --reload --host 127.0.0.1 --port 8000
```

API health: `http://127.0.0.1:8000/api/health`  
Interactive API docs: `http://127.0.0.1:8000/docs`

## 4. Create the first administrator
In a second CMD window:

```cmd
cd web\backend
venv\Scripts\activate
python -m app.init_admin
```

Use the email/password you create on the Sentinel sign-in page. Registration is intentionally not public; create the first administrator from this command. Protect the database and `.env` file.

## 5. Run the React frontend
Double-click `scripts/run_web_frontend.bat`, or:

```cmd
cd web\frontend
npm install
npm run dev
```

Open `http://127.0.0.1:5173`.

## 6. Run the C/C++ terminal application
The existing source is retained under `main.c`, `C_Module`, `Queue`, `PriorityQueue`, `Stack`, `HashTable`, `CPP_Module`, and `Database`. It uses the MySQL X Protocol port (33060), while the FastAPI web backend uses the MySQL SQL port (3306).

Set environment variables in the CMD window before launching:

```cmd
set MYSQL_HOST=localhost
set MYSQL_X_PORT=33060
set MYSQL_USER=cybercrime_app
set MYSQL_PASSWORD=YOUR_DATABASE_PASSWORD
```

The source now reads these variables instead of embedding `root` / `1234`. Rebuild the terminal program with MSVC x64 and the matching Connector/C++ headers/import libraries. Link the canonical `Database/bridge.cpp` only; do not compile both `bridge.cpp` and `c_bridge.cpp`, because they expose overlapping C symbols. Include the C modules, C++ domain classes, `Database/database.cpp`, and `Database/bridge.cpp` in the build. Put the matching Connector/C++ DLLs beside the executable or on `PATH`.

The ZIP intentionally omits stale `.obj` files and old executables: they were built against a particular local connector/compiler installation and cannot be guaranteed to match another Windows machine. The terminal application source is preserved, but a fresh build is required after changing credentials or connector paths.

## 7. Useful checks
```sql
USE cyber_crime_db;
SHOW TABLES;
SELECT * FROM cases;
SELECT * FROM evidence;
SELECT * FROM officers;
SELECT * FROM investigation_history;
SELECT * FROM reports;
```

## Security and operational notes
- Never commit `.env`, real database passwords, JWT secrets, database dumps containing personal data, or compiled connector DLLs.
- The web app uses parameterized SQL and JWT authentication. The current prototype has one administrator-level access policy; role-based authorization, password reset, audit logging, rate limiting, HTTPS, backups, and evidence chain-of-custody controls are required before real-world deployment.
- The terminal application and web app share MySQL as persistent storage. The in-memory DSA structures are terminal-process data structures, not the authoritative database.
- Do not expose MySQL directly to a browser or public network. Only the API should access the database in a deployed web architecture.

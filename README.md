# Cyber Crime Case Management System

A combined academic project demonstrating **C data structures**, **C++ object-oriented programming**, **MySQL persistence**, and a **React + FastAPI web application**.

## Project modules

| Layer | Technology | Responsibility |
|---|---|---|
| Terminal application | C | Linked list, FIFO queue, priority queue, stack, hash table, console workflow |
| Domain model | C++ | Case, Evidence, Investigation, Officer and User classes |
| Database layer | MySQL | Persistent cases, evidence, officers, investigation history and reports |
| Language bridge | C ABI + C++ | Allows the C menu to call C++ database functions |
| Web API | FastAPI + Python | Authenticated REST endpoints and database access |
| Web UI | React + Vite | Sentinel dashboard and record management interface |

## Main features
- Case create, list, search, update and delete (API and terminal where supported).
- Evidence register, case association and status updates.
- Officer directory and availability management.
- Investigation activity log and report records.
- Dashboard counts and recent cases.
- JWT login for the web dashboard.
- MySQL shared persistence between terminal and web API.

## Start here
Read [`docs/WINDOWS_SETUP.md`](docs/WINDOWS_SETUP.md) for the complete Windows installation, database setup, administrator creation and run commands.

- SQL schema: `database/schema.sql`
- Backend: `web/backend/`
- Frontend: `web/frontend/`
- Existing C/C++ terminal source: root folders `C_Module/`, `Queue/`, `PriorityQueue/`, `Stack/`, `HashTable/`, `CPP_Module/`, `Database/`
- Windows launch scripts: `scripts/`

## Important
This is an academic prototype, not a production law-enforcement system. Use synthetic test data. Add formal authorization, audit trails, secure password lifecycle, evidence integrity/chain-of-custody, backups, HTTPS and security testing before any real deployment.

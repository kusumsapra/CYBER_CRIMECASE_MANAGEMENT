from fastapi import FastAPI, Depends, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from mysql.connector import IntegrityError, Error
import secrets, string
from .db import get_connection, fetch_all, execute
from .schemas import LoginIn, CaseIn, CaseUpdate, EvidenceIn, OfficerIn, InvestigationIn, ReportIn
from .security import hash_password, verify_password, create_token, current_user

app = FastAPI(title="Cyber Crime Case Management API", version="1.0.0",
              description="Authenticated REST API for the Cyber Crime Case Management System")
app.add_middleware(CORSMiddleware, allow_origins=["http://localhost:5173", "http://127.0.0.1:5173"],
                   allow_credentials=True, allow_methods=["*"], allow_headers=["*"])

@app.get("/api/health")
def health():
    try:
        rows = fetch_all("SELECT 1 AS ok")
        return {"status": "ok", "database": "connected" if rows else "unknown"}
    except Error as exc:
        raise HTTPException(503, f"Database unavailable: {exc.msg}")

@app.post("/api/auth/login")
def login(data: LoginIn):
    rows = fetch_all("SELECT user_id,name,email,password,role FROM users WHERE email=%s LIMIT 1", (data.email.lower(),))
    if not rows or not verify_password(data.password, rows[0]["password"]):
        raise HTTPException(401, "Incorrect email or password")
    u = rows[0]
    return {"access_token": create_token(u["user_id"], u["email"], u["role"]), "token_type": "bearer",
            "user": {"user_id": u["user_id"], "name": u["name"], "email": u["email"], "role": u["role"]}}

@app.get("/api/auth/me")
def me(user=Depends(current_user)):
    rows = fetch_all("SELECT user_id,name,email,role FROM users WHERE user_id=%s", (user["user_id"],))
    if not rows: raise HTTPException(404, "User not found")
    return rows[0]

@app.get("/api/dashboard")
def dashboard(user=Depends(current_user)):
    queries = {
        "cases": "SELECT COUNT(*) n FROM cases",
        "open_cases": "SELECT COUNT(*) n FROM cases WHERE LOWER(status) NOT IN ('closed','resolved')",
        "evidence": "SELECT COUNT(*) n FROM evidence",
        "officers": "SELECT COUNT(*) n FROM officers",
        "investigations": "SELECT COUNT(*) n FROM investigation_history",
        "reports": "SELECT COUNT(*) n FROM reports",
    }
    result = {}
    for key, sql in queries.items():
        try: result[key] = fetch_all(sql)[0]["n"]
        except Error: result[key] = 0
    result["recent_cases"] = fetch_all("SELECT c.case_id,c.victim_name,c.crime_type,c.priority,c.status,c.officer_id FROM cases c ORDER BY c.case_id DESC LIMIT 5")
    return result

@app.get("/api/cases")
def list_cases(user=Depends(current_user)):
    return fetch_all("SELECT case_id,victim_name,crime_type,priority,status,officer_id FROM cases ORDER BY case_id DESC")

@app.post("/api/cases", status_code=201)
def add_case(data: CaseIn, user=Depends(current_user)):
    try:
        execute("INSERT INTO cases(case_id,victim_name,crime_type,priority,status,officer_id) VALUES(%s,%s,%s,%s,%s,%s)",
                (data.case_id,data.victim_name,data.crime_type,data.priority,data.status,data.officer_id))
        return {"message":"Case created", "case_id":data.case_id}
    except IntegrityError: raise HTTPException(409, "Case ID already exists or officer is invalid")
    except Error as exc: raise HTTPException(400, exc.msg)

@app.get("/api/cases/{case_id}")
def get_case(case_id: int, user=Depends(current_user)):
    rows=fetch_all("SELECT case_id,victim_name,crime_type,priority,status,officer_id FROM cases WHERE case_id=%s",(case_id,))
    if not rows: raise HTTPException(404,"Case not found")
    return rows[0]

@app.patch("/api/cases/{case_id}")
def update_case(case_id: int, data: CaseUpdate, user=Depends(current_user)):
    fields=data.model_dump(exclude_unset=True)
    if not fields: raise HTTPException(400,"No fields supplied")
    allowed={"victim_name","crime_type","priority","status","officer_id"}
    if set(fields)-allowed: raise HTTPException(400,"Invalid field")
    sql="UPDATE cases SET "+", ".join(f"{k}=%s" for k in fields)+" WHERE case_id=%s"
    _,count=execute(sql,tuple(fields.values())+(case_id,))
    if not count: raise HTTPException(404,"Case not found")
    return {"message":"Case updated"}

@app.delete("/api/cases/{case_id}")
def delete_case(case_id: int, user=Depends(current_user)):
    _,count=execute("DELETE FROM cases WHERE case_id=%s",(case_id,))
    if not count: raise HTTPException(404,"Case not found")
    return {"message":"Case deleted"}

@app.get("/api/evidence")
def list_evidence(case_id: int|None=None, user=Depends(current_user)):
    if case_id is None: return fetch_all("SELECT evidence_id,case_id,evidence_type,description,status FROM evidence ORDER BY evidence_id DESC")
    return fetch_all("SELECT evidence_id,case_id,evidence_type,description,status FROM evidence WHERE case_id=%s ORDER BY evidence_id DESC",(case_id,))

@app.post("/api/evidence",status_code=201)
def add_evidence(data: EvidenceIn,user=Depends(current_user)):
    try:
        execute("INSERT INTO evidence(evidence_id,case_id,evidence_type,description,status) VALUES(%s,%s,%s,%s,%s)",(data.evidence_id,data.case_id,data.evidence_type,data.description,data.status))
        return {"message":"Evidence added"}
    except IntegrityError: raise HTTPException(409,"Evidence ID exists or case does not exist")
    except Error as exc: raise HTTPException(400,exc.msg)

@app.patch("/api/evidence/{evidence_id}/status")
def update_evidence(evidence_id:int,status:str,user=Depends(current_user)):
    _,count=execute("UPDATE evidence SET status=%s WHERE evidence_id=%s",(status,evidence_id))
    if not count: raise HTTPException(404,"Evidence not found")
    return {"message":"Evidence status updated"}

@app.delete("/api/evidence/{evidence_id}")
def delete_evidence(evidence_id:int,user=Depends(current_user)):
    _,count=execute("DELETE FROM evidence WHERE evidence_id=%s",(evidence_id,))
    if not count: raise HTTPException(404,"Evidence not found")
    return {"message":"Evidence deleted"}

@app.get("/api/officers")
def list_officers(user=Depends(current_user)):
    return fetch_all("SELECT o.officer_id,o.user_id,u.name,u.email,o.specialization,o.availability FROM officers o LEFT JOIN users u ON u.user_id=o.user_id ORDER BY o.officer_id DESC")

@app.post("/api/officers",status_code=201)
def add_officer(data:OfficerIn,user=Depends(current_user)):
    conn=get_connection()
    try:
        cur=conn.cursor()
        temporary_password = ''.join(secrets.choice(string.ascii_letters + string.digits + "!@#") for _ in range(16))
        cur.execute("INSERT INTO users(name,email,password,role) VALUES(%s,%s,%s,'Officer')",(data.name,data.email.lower(),hash_password(temporary_password)))
        uid=cur.lastrowid
        cur.execute("INSERT INTO officers(user_id,specialization,availability) VALUES(%s,%s,%s)",(uid,data.specialization,data.availability))
        conn.commit()
        return {"message":"Officer created. Share this one-time temporary password securely and arrange a password reset.","temporary_password":temporary_password,"user_id":uid,"officer_id":cur.lastrowid}
    except IntegrityError: conn.rollback(); raise HTTPException(409,"Email already exists")
    except Error as exc: conn.rollback(); raise HTTPException(400,exc.msg)
    finally: cur.close(); conn.close()

@app.patch("/api/officers/{officer_id}/availability")
def update_officer(officer_id:int,availability:str,user=Depends(current_user)):
    _,count=execute("UPDATE officers SET availability=%s WHERE officer_id=%s",(availability,officer_id))
    if not count: raise HTTPException(404,"Officer not found")
    return {"message":"Availability updated"}

@app.delete("/api/officers/{officer_id}")
def delete_officer(officer_id:int,user=Depends(current_user)):
    _,count=execute("DELETE FROM officers WHERE officer_id=%s",(officer_id,))
    if not count: raise HTTPException(404,"Officer not found")
    return {"message":"Officer deleted"}

@app.get("/api/investigations")
def list_investigations(case_id:int|None=None,user=Depends(current_user)):
    if case_id is None: return fetch_all("SELECT investigation_id,case_id,officer_id,activity,investigation_date,remarks FROM investigation_history ORDER BY investigation_id DESC")
    return fetch_all("SELECT investigation_id,case_id,officer_id,activity,investigation_date,remarks FROM investigation_history WHERE case_id=%s ORDER BY investigation_id DESC",(case_id,))

@app.post("/api/investigations",status_code=201)
def add_investigation(data:InvestigationIn,user=Depends(current_user)):
    try:
        new_id,_=execute("INSERT INTO investigation_history(case_id,officer_id,activity,investigation_date,remarks) VALUES(%s,NULL,%s,CURDATE(),'')",(data.case_id,data.activity))
        return {"message":"Investigation activity recorded","investigation_id":new_id}
    except Error as exc: raise HTTPException(400,exc.msg)

@app.delete("/api/investigations/{investigation_id}")
def delete_investigation(investigation_id:int,user=Depends(current_user)):
    _,count=execute("DELETE FROM investigation_history WHERE investigation_id=%s",(investigation_id,))
    if not count: raise HTTPException(404,"Activity not found")
    return {"message":"Activity deleted"}

@app.get("/api/reports")
def list_reports(case_id:int|None=None,user=Depends(current_user)):
    if case_id is None: return fetch_all("SELECT report_id,case_id,report_type,report_description AS description FROM reports ORDER BY report_id DESC")
    return fetch_all("SELECT report_id,case_id,report_type,report_description AS description FROM reports WHERE case_id=%s ORDER BY report_id DESC",(case_id,))

@app.post("/api/reports",status_code=201)
def add_report(data:ReportIn,user=Depends(current_user)):
    try:
        report_id,_=execute("INSERT INTO reports(case_id,report_type,report_description) VALUES(%s,%s,%s)",(data.case_id,data.report_type,data.description))
        return {"message":"Report created","report_id":report_id}
    except Error as exc: raise HTTPException(400,exc.msg)

@app.delete("/api/reports/{report_id}")
def delete_report(report_id:int,user=Depends(current_user)):
    _,count=execute("DELETE FROM reports WHERE report_id=%s",(report_id,))
    if not count: raise HTTPException(404,"Report not found")
    return {"message":"Report deleted"}

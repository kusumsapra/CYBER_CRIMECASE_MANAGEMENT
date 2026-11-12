from pydantic import BaseModel, Field, EmailStr
from typing import Optional

class LoginIn(BaseModel):
    email: EmailStr
    password: str = Field(min_length=1, max_length=128)

class RegisterIn(BaseModel):
    name: str = Field(min_length=2, max_length=100)
    email: EmailStr
    password: str = Field(min_length=8, max_length=128)

class CaseIn(BaseModel):
    case_id: int = Field(gt=0)
    victim_name: str = Field(min_length=1, max_length=100)
    crime_type: str = Field(min_length=1, max_length=100)
    priority: int = Field(ge=1, le=5)
    status: str = Field(default="Open", max_length=40)
    officer_id: Optional[int] = None

class CaseUpdate(BaseModel):
    victim_name: Optional[str] = Field(default=None, max_length=100)
    crime_type: Optional[str] = Field(default=None, max_length=100)
    priority: Optional[int] = Field(default=None, ge=1, le=5)
    status: Optional[str] = Field(default=None, max_length=40)
    officer_id: Optional[int] = None

class EvidenceIn(BaseModel):
    evidence_id: int = Field(gt=0)
    case_id: int = Field(gt=0)
    evidence_type: str = Field(min_length=1, max_length=100)
    description: str = Field(min_length=1, max_length=1000)
    status: str = Field(default="Collected", max_length=40)

class OfficerIn(BaseModel):
    name: str = Field(min_length=2, max_length=100)
    email: EmailStr
    specialization: str = Field(min_length=1, max_length=100)
    availability: str = Field(default="Available", max_length=40)

class InvestigationIn(BaseModel):
    case_id: int = Field(gt=0)
    activity: str = Field(min_length=1, max_length=2000)

class ReportIn(BaseModel):
    case_id: int = Field(gt=0)
    report_type: str = Field(min_length=1, max_length=100)
    description: str = Field(min_length=1, max_length=5000)

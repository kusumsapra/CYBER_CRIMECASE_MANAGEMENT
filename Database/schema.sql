CREATE DATABASE IF NOT EXISTS cyber_crime_db CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;
USE cyber_crime_db;

CREATE TABLE IF NOT EXISTS users (
  user_id INT AUTO_INCREMENT PRIMARY KEY,
  name VARCHAR(100) NOT NULL,
  email VARCHAR(150) NOT NULL UNIQUE,
  password VARCHAR(255) NOT NULL,
  role VARCHAR(30) NOT NULL DEFAULT 'Officer',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS officers (
  officer_id INT AUTO_INCREMENT PRIMARY KEY,
  user_id INT NULL,
  specialization VARCHAR(100) NOT NULL,
  availability VARCHAR(40) NOT NULL DEFAULT 'Available',
  CONSTRAINT fk_officer_user FOREIGN KEY(user_id) REFERENCES users(user_id) ON DELETE SET NULL
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS cases (
  case_id INT PRIMARY KEY,
  victim_name VARCHAR(100) NOT NULL,
  crime_type VARCHAR(100) NOT NULL,
  priority INT NOT NULL CHECK(priority BETWEEN 1 AND 5),
  status VARCHAR(40) NOT NULL DEFAULT 'Open',
  officer_id INT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_case_officer FOREIGN KEY(officer_id) REFERENCES officers(officer_id) ON DELETE SET NULL,
  INDEX idx_cases_status(status), INDEX idx_cases_priority(priority)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS evidence (
  evidence_id INT PRIMARY KEY,
  case_id INT NOT NULL,
  evidence_type VARCHAR(100) NOT NULL,
  description TEXT NOT NULL,
  status VARCHAR(40) NOT NULL DEFAULT 'Collected',
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_evidence_case FOREIGN KEY(case_id) REFERENCES cases(case_id) ON DELETE CASCADE,
  INDEX idx_evidence_case(case_id)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS investigation_history (
  investigation_id INT AUTO_INCREMENT PRIMARY KEY,
  case_id INT NOT NULL,
  officer_id INT NULL,
  activity TEXT NOT NULL,
  investigation_date DATE NULL,
  remarks TEXT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_investigation_case FOREIGN KEY(case_id) REFERENCES cases(case_id) ON DELETE CASCADE,
  CONSTRAINT fk_investigation_officer FOREIGN KEY(officer_id) REFERENCES officers(officer_id) ON DELETE SET NULL,
  INDEX idx_investigation_case(case_id)
) ENGINE=InnoDB;

CREATE TABLE IF NOT EXISTS reports (
  report_id INT AUTO_INCREMENT PRIMARY KEY,
  case_id INT NOT NULL,
  report_type VARCHAR(100) NOT NULL,
  report_description TEXT NOT NULL,
  created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
  CONSTRAINT fk_report_case FOREIGN KEY(case_id) REFERENCES cases(case_id) ON DELETE CASCADE,
  INDEX idx_reports_case(case_id)
) ENGINE=InnoDB;

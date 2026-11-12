-- Run database/schema.sql first as a MySQL administrator, then create the app user.
CREATE USER IF NOT EXISTS 'cybercrime_app'@'localhost' IDENTIFIED BY 'CHANGE_THIS_PASSWORD';
GRANT SELECT, INSERT, UPDATE, DELETE ON cyber_crime_db.* TO 'cybercrime_app'@'localhost';
FLUSH PRIVILEGES;

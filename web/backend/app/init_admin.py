"""Create the first administrator interactively. Run from web/backend."""
import getpass
from app.db import execute
from app.security import hash_password
name=input('Admin name: ').strip()
email=input('Admin email: ').strip().lower()
pw=getpass.getpass('Password (8+ characters): ')
if len(name)<2 or len(pw)<8 or '@' not in email:
    raise SystemExit('Invalid input. Name, email and an 8+ character password are required.')
execute("INSERT INTO users(name,email,password,role) VALUES(%s,%s,%s,'Admin')",(name,email,hash_password(pw)))
print('Administrator created. You can now sign in on the web app.')

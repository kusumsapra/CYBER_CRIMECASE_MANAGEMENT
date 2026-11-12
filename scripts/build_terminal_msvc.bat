@echo off
setlocal
if "%MYSQL_CONNECTOR_ROOT%"=="" (
  echo Set MYSQL_CONNECTOR_ROOT to your extracted MySQL Connector/C++ folder.
  echo Example: set MYSQL_CONNECTOR_ROOT=C:\path\to\mysql-connector-c++
  exit /b 1
)
if not exist "%MYSQL_CONNECTOR_ROOT%\include\mysqlx\xdevapi.h" (
  echo Could not find include\mysqlx\xdevapi.h under MYSQL_CONNECTOR_ROOT.
  exit /b 1
)
if not exist build mkdir build
call "%ProgramFiles(x86)%\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if errorlevel 1 exit /b 1
set "INC=/I%MYSQL_CONNECTOR_ROOT%\include"
set "LIBPATH=/LIBPATH:%MYSQL_CONNECTOR_ROOT%\lib64\vs14"
cl /nologo /TC /c main.c /Fo:build\main.obj
if errorlevel 1 exit /b 1
cl /nologo /TC /c C_Module\case.c /Fo:build\case_c.obj
if errorlevel 1 exit /b 1
cl /nologo /TC /c C_Module\linked_list.c /Fo:build\linked_list.obj
if errorlevel 1 exit /b 1
cl /nologo /TC /c Queue\queue.c /Fo:build\queue.obj
if errorlevel 1 exit /b 1
cl /nologo /TC /c PriorityQueue\priority_queue.c /Fo:build\priority_queue.obj
if errorlevel 1 exit /b 1
cl /nologo /TC /c Stack\stack.c /Fo:build\stack.obj
if errorlevel 1 exit /b 1
cl /nologo /TC /c HashTable\hash_table.c /Fo:build\hash_table.obj
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MD %INC% /c CPP_Module\Case.cpp /Fo:build\Case.obj
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MD %INC% /c CPP_Module\Evidence.cpp /Fo:build\Evidence.obj
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MD %INC% /c CPP_Module\Investigation.cpp /Fo:build\Investigation.obj
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MD %INC% /c CPP_Module\Officer.cpp /Fo:build\Officer.obj
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MD %INC% /c CPP_Module\User.cpp /Fo:build\User.obj
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MD %INC% /c Database\database.cpp /Fo:build\database.obj
if errorlevel 1 exit /b 1
cl /nologo /std:c++17 /EHsc /MD %INC% /c Database\bridge.cpp /Fo:build\bridge.obj
if errorlevel 1 exit /b 1
link /nologo /OUT:build\cybercrime.exe build\main.obj build\case_c.obj build\linked_list.obj build\queue.obj build\priority_queue.obj build\stack.obj build\hash_table.obj build\Case.obj build\Evidence.obj build\Investigation.obj build\Officer.obj build\User.obj build\database.obj build\bridge.obj %LIBPATH% mysqlcppconnx.lib mysqlcppconn.lib ws2_32.lib
if errorlevel 1 exit /b 1
echo Build completed: build\cybercrime.exe

#include <iostream>
#include <cstdlib>
#include <mysqlx/xdevapi.h>

using namespace std;
using namespace mysqlx;

int main()
{
    try
    {
        cout << "Connecting to MySQL..." << endl;

        const char* host = std::getenv("MYSQL_HOST");
        const char* portText = std::getenv("MYSQL_X_PORT");
        const char* user = std::getenv("MYSQL_USER");
        const char* password = std::getenv("MYSQL_PASSWORD");
        Session session(host ? host : "localhost", std::atoi(portText ? portText : "33060"),
                        user ? user : "cybercrime_app", password ? password : "");

        cout << "Connected to MySQL successfully!" << endl;

        Schema db = session.getSchema("cyber_crime_db");

        cout << "Connected to database: "
             << db.getName() << endl;

        Table cases = db.getTable("cases");

        cout << "Cases table found successfully!" << endl;

        session.close();

        cout << "\nDatabase connection test successful!" << endl;
    }
    catch (const mysqlx::Error &error)
    {
        cout << "\nMySQL Error: "
             << error.what() << endl;
    }
    catch (const exception &error)
    {
        cout << "\nError: "
             << error.what() << endl;
    }

    return 0;
}
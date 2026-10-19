#include <iostream>
#include <mysqlx/xdevapi.h>

using namespace std;
using namespace mysqlx;

int main()
{
    try
    {
        cout << "Connecting to MySQL..." << endl;

        Session session(
            "localhost",
            33060,
            "root",
            "1234"
        );

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
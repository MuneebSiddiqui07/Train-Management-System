#include "Declarations.h" 

int main() {
    LoginManager loginManager; // Instance to manage user accounts
    TrainManager trainManager;  // Instance to manage trains
    int choice;

    // Load existing trains from file
    trainManager.loadFromFile("trains.csv");

    do {
        cout << "Welcome to the Train Management System!" << endl;
        cout << "1. Sign Up" << endl;
        cout << "2. Log In" << endl;
        cout << "3. Exit" << endl;
        cout << "Select an option: ";
        cin >> choice;

        switch (choice) {
            case 1: { // Sign Up
                int userType;
                cout << "Select User Type:\n1. Admin\n2. Regular User\nChoose: ";
                cin >> userType;

                if (userType == 1) {
                    Admin* newAdmin = new Admin();
                    newAdmin->signup();
                    loginManager.addAdmin(newAdmin);
                } else if (userType == 2) {
                    RegularUser * newUser  = new RegularUser ();
                    newUser ->signup();
                    loginManager.addUser (newUser );
                } else {
                    cout << "Invalid user type!" << endl;
                }
                break;
            }
            case 2: { // Log In
                cout << "Select User Type:\n1. Admin\n2. Regular User\nChoose: ";
                int usertype;
                cin >> usertype;

                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                if (usertype == 1) { // Admin Login
                    Admin admin; // Create an instance of Admin
                    bool val;
                    val = admin.login(username, password); // Call the login method for Admin
                    while (val != true) {
                        cout << "Enter username: ";
                        cin >> username;
                        cout << "Enter password: ";
                        cin >> password;
                        val = admin.login(username, password); // Call the login method for Admin
                    }
                    admin.displayMenu(); // Show admin menu
                } else if (usertype == 2) { // Regular User Login
                    RegularUser  regularUser ; // Create an instance of RegularUser   
                    bool val;
                    val = regularUser .login(username, password); // Call the login method for RegularUser    
                    while (val != true) {
                        cout << "Enter username: ";
                        cin >> username;
                        cout << "Enter password: ";
                        cin >> password;
                        val = regularUser .login(username, password); // Call the login method for RegularUser 
                    }
                    regularUser .displayMenu(); // Show regular user menu
                } else {
                    cout << "Invalid user type!" << endl;
                }
                break;
            }
            case 3: // Exit
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid option! Please try again." << endl;
                break;
        }
    } while (choice != 3);

    return 0;
}
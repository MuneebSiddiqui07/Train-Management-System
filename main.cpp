#include "Declarations.h"
//this is main file
// this is muneeb
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
                cin>>usertype;
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;

                User* user = loginManager.authenticate(username, password);
                if (user) {
                    cout << "Login successful!" << endl;

                    // User Menu
                    if (Admin* admin = dynamic_cast<Admin*>(user)) {
                        int adminChoice;
                        do {
                            cout << "\nAdmin Menu:\n1. View Train Schedule\n2. Add Train\n3. Update Train Schedule\n4. Remove Train\n5. View Passengers\n6. Log Out\n Choose:";
                            cin >> adminChoice;

                            switch (adminChoice) {
                                case 1:
                                    admin->viewTrainSchedule();
                                    break;
                                case 2:
                                    admin->addTrain();
                                    break;
                                case 3:
                                    admin->updateTrainSchedule();
                                    break;
                                case 4:
                                    admin->removeTrain();
                                    break;
                                case 5:
                                    admin->viewpassenger();
                                    break;
                                case 6:
                                    cout << "Logging out..." << endl;
                                    break;
                                default:
                                    cout << "Invalid option!" << endl;
                                    break;
                            }
                        } while (adminChoice != 6);
                    } else if (RegularUser * regularUser  = dynamic_cast<RegularUser *>(user)) {
                        int userChoice;
                        do {
                            cout << "\nUser  Menu:\n1. View Train Schedule\n2. Book Ticket\n3. Cancel Ticket\n4. Logout\nChoose: ";
                            cin >> userChoice;

                            switch (userChoice) {
                                case 1:
                                    regularUser ->viewTrainSchedule();
                                    break;
                                case 2:
                                    regularUser ->bookTicket();
                                    break;
                                case 3:
                                    regularUser ->cancelTicket();
                                    break;
                                case 4:
                                    cout << "Logging out..." << endl;
                                    break;
                                default:
                                    cout << "Invalid option!" << endl;
                                    break;
                            }
                        } while (userChoice != 4);
                    }
                } else {
                    cout << "Invalid username or password!" << endl;
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

    // Save train data before exiting
    // trainManager.saveToFile("trains.csv");
    
    return 0;
}
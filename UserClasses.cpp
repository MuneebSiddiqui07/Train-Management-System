#include "Declarations.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // for std::remove_if
#include <map>
using namespace std;


// ==================================================== Helper Functions ============================================================= //


// // Function to trim whitespace from both ends of a string
// string trim(const string& str) {
//     size_t first = str.find_first_not_of(' ');
//     if (first == string::npos) return ""; // no content
//     size_t last = str.find_last_not_of(' ');
//     return str.substr(first, (last - first + 1));
// }

bool validateCredentials(const string& fileName, const string& username, const string& password) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return false;
    }

    string line, fileUsername, filePassword;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, fileUsername, ','); // Read username until comma
        getline(ss, filePassword); // Read password until end of line
        // Trim whitespace from both username and password
        // fileUsername = trim(fileUsername);
        // filePassword = trim(filePassword);

        if (fileUsername == username && filePassword == password) {
            return true; // Credentials matched
        }
    }

    return false; // No match found
}

bool isUsernameTaken(const string& fileName, const string& username) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return false;
    }

    string line, fileUsername;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, fileUsername, ',');
        if (fileUsername == username) {
            return true; // Username already exists
        }
    }

    return false; // Username not found
}

void saveCredentials(const string& fileName, const string& username, const string& password) {
    ofstream file(fileName, ios::app);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return;
    }
    file << username << "," << password << endl;
    file.close();
}
map<string, int> trainOccupancy;
void saveOccupancyData() {
    ofstream occupancyFile("occupancy.csv");
    if (!occupancyFile.is_open()) {
        cerr << "Error: Could not open occupancy file for writing." << endl;
        return;
    }

    for (const auto& entry : trainOccupancy) {
        occupancyFile << entry.first << "," << entry.second << endl; // trainID, occupancy
    }

    occupancyFile.close();
}

void loadOccupancyData() {
    ifstream occupancyFile("occupancy.csv");
    if (!occupancyFile.is_open()) {
        cerr << "Error: Could not open occupancy file for reading." << endl;
        return;
    }

    string line;
    while (getline(occupancyFile, line)) {
        stringstream ss(line);
        string trainID;
        int occupancy;

        getline(ss, trainID, ',');
        ss >> occupancy;

        trainOccupancy[trainID] = occupancy; 
    }

    occupancyFile.close();
}


// ==================================================== User Class ============================================================= //

User::User(){
    username = ""; 
    password = ""; 
}

User::User(const string& username, const string& password){
    this->username = username;
    this->password = password; 
    }

User::~User() {}

void User::login() {
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;
}

string User::getUsername() const {
    return username;
}

string User::getPassword() const{
    return password;
}

void User::viewTrainSchedule() const {

}


// ==================================================== Admin Class ============================================================= //

Admin::Admin() : User() {}

Admin::Admin(const string& username, const string& password) : User(username, password) {}

void Admin::signup() {
    cout << "[Admin Signup]" << endl;

    cout << "Enter a username: ";
    cin >> username;

    while (isUsernameTaken("admins.csv", username)) {
        cout << "Username already taken. Please choose a different username: ";
        cin >> username;
    }

    while (isUsernameTaken("users.csv", username)) {
        cout << "Username already taken. Please choose a different username: ";
        cin >> username;
    }

    cout << "Enter a password: ";
    cin >> password;

    saveCredentials("admins.csv", username, password);
    cout << "Admin account created successfully!" << endl;
}

void Admin::login() {
    cout << "[Admin Login]" << endl;
    User::login();

    if (validateCredentials("admins.csv", username, password) == true) {
        cout << "Admin login successful." << endl;
    } else {
        cout << "Invalid credentials. Please try again." << endl;
        username = password = ""; // Resetting credentials
    }
}

void Admin::displayMenu() {
    int choice = -1;

    while (choice != 9) {  // Admin will continue until they select "Logout" (option 9)
        cout << "\nAdmin Menu:" << endl;
        cout << "1. View Train Schedule" << endl;
        cout << "2. Add Train Schedule" << endl;
        cout << "3. Add a Train"  << endl;
        cout << "4. Update Train Details"<<endl;
        cout << "5. View Train Details"<<endl;
        cout << "6. Update Train Schedule" << endl;
        cout << "7. Remove Train" << endl;
        cout <<  "8. Remove Train Schedule"<<endl;
        cout << "9. Logout" << endl;
        cout << "Select an Option: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                // View train schedule
                User::viewTrainSchedule();
                break;
            }
            case 2: {
                // Add a new train schedule
                addTrainschedule();
                break;
            }
            case 3: {
                //add new train 
                addTrain();
                break;
            }
            case 4: {
                // update a train
                updateTrainDetails();
                break;
            }
            case 5: {
                viewTrain();
                break;
            }
            case 6:{
                updateTrainSchedule();
                break;
            }
            case 7:{
                removeTrain();
                break;
            }
            case 8:{
                removeTrainSchedule();
                break;
            }
            case 9:{
                cout<<"Logging Out."<<endl;
            }
            default: {
                // Handle invalid choices
                cout << "Invalid option. Please try again." << endl;
                break;
            }
        }
    }
}
void Admin::viewTrainSchedule() const {
    ifstream trainFile("schedules.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    string line;
    cout << "Train Schedule:" << endl;

    // Read each line from the train file
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileTrainID, fileTrainName, fileSource, fileDestination, fileDepartureTime, fileArrivalTime;

        // Extract each field from the line
        getline(ss, fileTrainID, ',');        // Get trainID
        getline(ss, fileTrainName, ',');      // Get train name
        getline(ss, fileSource, ',');         // Get source
        getline(ss, fileDestination, ',');    // Get destination
        getline(ss, fileDepartureTime, ',');  // Get departure time
        getline(ss, fileArrivalTime);          // Get arrival time

        cout<<"Train ID: "<<fileTrainID<<endl;
        cout << "Train Name: " << fileTrainName << endl;
        cout << "Source: " << fileSource << endl;
        cout << "Destination: " << fileDestination << endl;
        cout << "Departure Time: " << fileDepartureTime << endl;
        cout << "Arrival Time: " << fileArrivalTime << endl;
        cout << "---------------------------------------" << endl;
    }

    trainFile.close();
}

void Admin::viewpassenger() const {
    ifstream PassengerFile("passengers.csv");
    if (!PassengerFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    string line;
    cout << "Passengers Travelling:" << endl;

    // Read each line from the train file
    while (getline(PassengerFile, line)) {
        stringstream ss(line);
        string fileTicketID, fileName, fileTrainID,filetrainName, fileCNIC;

        // Extract each field from the line
        getline(ss, fileTicketID, ',');        // Skip ticketID
        getline(ss, fileName, ',');      // Get Passenger name
        getline(ss, fileTrainID, ',');         // Get TRAIN ID
        getline(ss,filetrainName,',');  //Get Train Name
        getline(ss, fileCNIC, ',');    // Get CNIC
    

        // Print all details except trainID
        cout << "Passenger Name: " << fileName << endl;
        cout <<" Ticket ID: " << fileTicketID << endl;
        cout << "CNIC Number " << fileCNIC<< endl;
        cout<<"Train Name "<<filetrainName<<endl;
        cout << "Train ID " << fileTrainID << endl;
        cout << "---------------------------------------" << endl;
    }

    PassengerFile.close();
}


void Admin::addTrainschedule() {
    cout << "Adding a new train schedule..." << endl;

    string trainID, trainName, source, destination, departure_Time, arrival_Time;
    ifstream trainFile("schedules.csv");
        if (!trainFile.is_open()) {
            cerr << "Error: Could not open train schedule file." << endl;
            return;
        }
    cout << "Enter Train ID: ";
    cin >> trainID;
    string line;
    cout << "Enter Train Name: ";
    cin.ignore(); // Clear the newline character from the input buffer
    getline(cin, trainName);
    cout << "Enter Source: ";
    getline(cin, source);
    cout << "Enter Destination: ";
    getline(cin, destination);
    cout << "Enter Departure Time: ";
    getline(cin, departure_Time);
    cout << "Enter Arrival Time: ";
    getline(cin, arrival_Time);

    // Create a new Train object
    Schedules newschedule(trainID, trainName, source, destination, departure_Time, arrival_Time);

    // Append the new train details to the CSV file
    ofstream ScheduleFileOut("schedules.csv", ios::app);
    newschedule.saveToFile(ScheduleFileOut);
    ScheduleFileOut.close();

    cout << "Train schedule added successfully!" << endl;
}



void Admin::updateTrainSchedule() {
    cout << "Updating train schedule..." << endl;

    string trainID;
    cout << "Enter Train ID to update: ";
    cin >> trainID;

    // Open the original file for reading
    ifstream trainFile("schedules.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    // Create a temporary file for updated train information
    ofstream tempFile("schedules_temp.csv");
    if (!tempFile.is_open()) {
        cerr << "Error: Could not create a temporary file." << endl;
        trainFile.close();
        return;
    }

    string line;
    bool trainFound = false;

    // Read the original file line by line
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileTrainID, fileTrainName, fileSource, fileDestination, fileDepartureTime, fileArrivalTime;

        // Parse the line
        getline(ss, fileTrainID, ',');
        getline(ss, fileTrainName, ',');
        getline(ss, fileSource, ',');
        getline(ss, fileDestination, ',');
        getline(ss, fileDepartureTime, ',');
        getline(ss, fileArrivalTime);

        if (fileTrainID == trainID) {
            trainFound = true;

            // Display current train details
            cout << "\nCurrent Train Details:" << endl;
            cout << "Train ID: " << fileTrainID << endl;
            cout << "Train Name: " << fileTrainName << endl;
            cout << "Source: " << fileSource << endl;
            cout << "Destination: " << fileDestination << endl;
            cout << "Departure Time: " << fileDepartureTime << endl;
            cout << "Arrival Time: " << fileArrivalTime << endl;

            int choice = -1;
            while (choice != 0) {
                // Display update menu
                cout << "\nWhat would you like to update?" << endl;
                cout << "1. Train Name" << endl;
                cout << "2. Source" << endl;
                cout << "3. Destination" << endl;
                cout << "4. Departure Time" << endl;
                cout << "5. Arrival Time" << endl;
                cout << "0. Exit Update Menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                // Update selected field
                switch (choice) {
                    case 1: {
                        cout << "Enter new Train Name: ";
                        cin.ignore();
                        getline(cin, fileTrainName);
                        break;
                    }
                    case 2: {
                        cout << "Enter new Source: ";
                        cin.ignore();
                        getline(cin, fileSource);
                        break;
                    }
                    case 3: {
                        cout << "Enter new Destination: ";
                        cin.ignore();
                        getline(cin, fileDestination);
                        break;
                    }
                    case 4: {
                        cout << "Enter new Departure Time: ";
                        cin.ignore();
                        getline(cin, fileDepartureTime);
                        break;
                    }
                    case 5: {
                        cout << "Enter new Arrival Time: ";
                        cin.ignore();
                        getline(cin, fileArrivalTime);
                        break;
                    }
                    case 0: {
                        cout << "Exiting Update Menu..." << endl;
                        break;
                    }
                    default: {
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                    }
                }
            }

            // Write updated train details to the temporary file
            tempFile << fileTrainID << "," << fileTrainName << "," << fileSource << "," << fileDestination << ","
                     << fileDepartureTime << "," << fileArrivalTime << endl;
        } else {
            // Write unchanged lines to the temporary file
            tempFile << line << endl;
        }
    }

    trainFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (trainFound) {
        remove("schedules.csv");
        rename("schedules_temp.csv", "schedules.csv");
        cout << "Train schedule updated successfully!" << endl;
    } else {
        remove("schedules_temp.csv");
        cout << "Error: Train with ID " << trainID << " not found!" << endl;
    }
}


void Admin::removeTrainSchedule() {
    cout << "Removing a train..." << endl;

    string trainID;
    cout << "Enter Train ID to remove: ";
    cin >> trainID;

    // Open the trains file for reading
    ifstream trainFile("schedules.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    // Create a temporary file to store updated train data
    ofstream tempFile("schedules_temp.csv");
    if (!tempFile.is_open()) {
        cerr << "Error: Could not create a temporary file." << endl;
        trainFile.close();
        return;
    }

    string line;
    bool trainFound = false;

    // Read the file line by line
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileTrainID;

        // Parse the line to extract the train ID
        getline(ss, fileTrainID, ',');  // First element is trainID

        if (fileTrainID == trainID) {
            trainFound = true;
            // Skip the train to be removed (don't write it to temp file)
        } else {
            // Write all other lines to the temporary file
            tempFile << line << endl;
        }
    }

    trainFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (trainFound) {
        remove("schedules.csv");
        rename("schedules_temp.csv", "schedules.csv");
        cout << "Train with ID " << trainID << " removed successfully!" << endl;
    } else {
        remove("schedules_temp.csv");
        cout << "Error: Train with ID " << trainID << " not found!" << endl;
    }
}

void Admin::viewTrain()  {
    ifstream trainFile("trains.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    string line;
    cout << "Trains:" << endl;

    // Read each line from the train file
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileTrainID, fileTrainName, fileCapacity;

        // Extract each field from the line
        getline(ss, fileTrainID, ',');        // GET trainID
        getline(ss, fileTrainName, ',');      // Get train name
        getline(ss, fileCapacity, ',');         // Get source

        // Print all details 
        cout <<"Train ID: " <<fileTrainID<<endl;
        cout << "Train Name: " << fileTrainName << endl;
        cout << "Seating Capacity: " << fileCapacity<< endl;
        cout << "---------------------------------------" << endl;
    }

    trainFile.close();
}




void Admin::addTrain() {
    cout << "Adding a new train..." << endl;

    string trainID, trainName, capacity;

    // Loop until a unique Train ID is entered
    while (true) {
        cout << "Enter Train ID: ";
        cin >> trainID;

        ifstream trainFile("trains.csv");
        if (!trainFile.is_open()) {
            cerr << "Error: Could not open train schedule file." << endl;
            return;
        }

        string line;
        bool idExists = false;

        // Check if the trainID already exists in the file
        while (getline(trainFile, line)) {
            stringstream ss(line);
            string fileTrainID;

            // Parse the line to get the train ID
            getline(ss, fileTrainID, ',');
            if (trainID == fileTrainID) {
                idExists = true;
                break; // Exit the loop if the ID is found
            }
        }
        trainFile.close(); // Close the file after reading

        if (idExists) {
            cout << "Train with this ID already exists, Please try again!" << endl;
        } else {
            break; // Exit the loop if the ID is unique
        }
    }

    // After a unique trainID is obtained, proceed to get other details
    cout << "Enter Train Name: ";
    cin.ignore(); // Clear the newline character from the input buffer
    getline(cin, trainName);
    cout << "Enter Seating Capacity: ";
    getline(cin, capacity);
    // Create a new Train object
    Train newTrain(trainID, trainName, capacity);

    // Append the new train details to the CSV file
    ofstream trainFileOut("trains.csv", ios::app);
    newTrain.saveToFile(trainFileOut);
    trainFileOut.close();

    cout << "Train added successfully!" << endl;
}

void Admin::updateTrainDetails() {
    cout << "Updating train schedule..." << endl;

    string trainID;
    cout << "Enter Train ID to update: ";
    cin >> trainID;

    // Open the original file for reading
    ifstream trainFile("trains.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    // Create a temporary file for updated train information
    ofstream tempFile("trains_temp.csv");
    if (!tempFile.is_open()) {
        cerr << "Error: Could not create a temporary file." << endl;
        trainFile.close();
        return;
    }

    string line;
    bool trainFound = false;

    // Read the original file line by line
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileTrainID, fileTrainName, fileCapacity;

        // Parse the line
        getline(ss, fileTrainID, ',');
        getline(ss, fileTrainName, ',');
        getline(ss, fileCapacity, ',');

        if (fileTrainID == trainID) {
            trainFound = true;

            // Display current train details
            cout << "\nCurrent Train Details:" << endl;
            cout << "Train ID: " << fileTrainID << endl;
            cout << "Train Name: " << fileTrainName << endl;
            cout << "Seating Capacity: "<<fileCapacity<<endl;

            int choice = -1;
            while (choice != 0) {
                // Display update menu
                cout << "\nWhat would you like to update?" << endl;
                cout << "1. Train Name" << endl;
                cout << "2. Seating Capacity" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                // Update selected field
                switch (choice) {
                    case 1: {
                        cout << "Enter new Train Name: ";
                        cin.ignore();
                        getline(cin, fileTrainName);
                        break;
                    }
                    case 2: {
                        cout << "Enter new Seating Capacity: ";
                        cin.ignore();
                        getline(cin, fileCapacity);
                        break;
                    default: {
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                    }
                }
            }

            // Write updated train details to the temporary file
            tempFile << fileTrainID << "," << fileTrainName << "," << fileCapacity << endl;
        } 
        }   
    }

    trainFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (trainFound) {
        remove("trains.csv");
        rename("trains_temp.csv", "trains.csv");
        cout << "Train schedule updated successfully!" << endl;
    } else {
        remove("trains_temp.csv");
        cout << "Error: Train with ID " << trainID << " not found!" << endl;
    }
}



void Admin::removeTrain() {
    cout << "Removing a train..." << endl;

    string trainID;
    cout << "Enter Train ID to remove: ";
    cin >> trainID;

    // Open the trains file for reading
    ifstream trainFile("trains.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    // Create a temporary file to store updated train data
    ofstream tempFile("trains_temp.csv");
    if (!tempFile.is_open()) {
        cerr << "Error: Could not create a temporary file." << endl;
        trainFile.close();
        return;
    }

    string line;
    bool trainFound = false;

    // Read the file line by line
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileTrainID;

        // Parse the line to extract the train ID
        getline(ss, fileTrainID, ',');  // First element is trainID

        if (fileTrainID == trainID) {
            trainFound = true;
            // Skip the train to be removed (don't write it to temp file)
        } else {
            // Write all other lines to the temporary file
            tempFile << line << endl;
        }
    }

    trainFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (trainFound) {
        remove("trains.csv");
        rename("trains_temp.csv", "trains.csv");
        cout << "Train with ID " << trainID << " removed successfully!" << endl;
    } else {
        remove("trains_temp.csv");
        cout << "Error: Train with ID " << trainID << " not found!" << endl;
    }
}

// ==================================================== RegularUser Class ============================================================= //

RegularUser::RegularUser() : User() {}

RegularUser::RegularUser(const string& username, const string& password) : User(username, password) {}

void RegularUser::signup() {
    cout << "[User Signup]" << endl;

    cout << "Enter a username: ";
    cin >> username;

    while (isUsernameTaken("users.csv", username)) {
        cout << "Username already taken. Please choose a different username: ";
        cin >> username;
    }
    
    while (isUsernameTaken("admins.csv", username)) {
        cout << "Username already taken. Please choose a different username: ";
        cin >> username;
    }

    cout << "Enter a password: ";
    cin >> password;

    saveCredentials("users.csv", username, password);
    cout << "User account created successfully!" << endl;
}

void RegularUser::login() {
    cout << "[User Login]" << endl;
    User::login();

    if (validateCredentials("users.csv", username, password)) {
        cout << "User login successful." << endl;
    } else {
        cout << "Invalid credentials. Please try again." << endl;
        username = password = ""; // Resetting credentials
    }
}

void RegularUser::displayMenu(){
    int choice = -1;
    while (choice != 5) {  // 5 is the option for Logout
        cout << "User Menu:" << endl;
        cout << "1. View Train Schedule" << endl;
        cout << "2. View Train Details" << endl;
        cout << "3. Book Ticket" << endl;
        cout << "4. Cancel Ticket" << endl;
        cout << "5. Logout" << endl;
        cout << "Select an Option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewTrainSchedule();
                break;
            case 2:
                // viewTrainDetails();
                break;
            case 3:
                bookTicket();
                break;
            case 4:
                cancelTicket();
                break;
            case 5:
                cout << "Logging out..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }
}


void RegularUser::bookTicket() {
    loadOccupancyData();
    cout << "Booking tickets..." << endl;

    string trainID;
    cout << "Enter Train ID: ";
    cin >> trainID;

    // Check if the train exists
    ifstream trainFile("trains.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }
    int occupancy,capint;
    bool trainFound = false;
    string line,fileCapacity,filetrainID,filetrainName;
    while (getline(trainFile, line)) {
        if (line.find(trainID) != string::npos) {
            trainFound = true;
            stringstream ss(line);
            getline(ss,filetrainID,',');
            getline(ss,filetrainName,',');
            getline(ss,fileCapacity,',');
            break;
        }
    }
    trainFile.close();

    if (!trainFound) {
        cout << "Error: Train with ID " << trainID << " not found!" << endl;
        return;
    }

    ifstream scheduleFile("schedules.csv");
    if (!scheduleFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }
    
    bool scheduleFound = false;
    while (getline(scheduleFile, line)) {
        if (line.find(trainID) != string::npos) {
            scheduleFound  = true;
            break;
        }
    }
    scheduleFile.close();

    if (!scheduleFound) {
        cout << "Error: Train with ID " << trainID << " doesn't have any available schedule." << endl;
        return;
    }



    // Start booking tickets for multiple people
    double totalPrice = 0.0;
    int numTickets;
    
    cout << "How many tickets do you want to book? ";
    cin >> numTickets;
    int currentOccupancy = trainOccupancy[trainID];
    capint=stoi(fileCapacity);
    if (currentOccupancy + numTickets > capint){
        cout<<"Train has no more Occupancy Try booking another Train or Fly better Fly Emirates"<<endl;
        return;
    }
    trainOccupancy[trainID] += numTickets;
    saveOccupancyData();
    // Open the ticket file for appending
    ofstream ticketFile("tickets.csv", ios::app);
    if (!ticketFile.is_open()) {
        cerr << "Error: Could not open ticket file for saving." << endl;
        return;
    }

    cin.ignore(); // Clear the input buffer
    for (int i = 1; i <= numTickets; ++i) {
        cout << "Enter details for Passenger " << i << ":" << endl;

        string passengerName, cnic;
        int age;

        cout << "Name: ";
        getline(cin, passengerName);

        cout << "CNIC: ";
        getline(cin, cnic);

        cout << "Age: ";
        cin >> age;
        cin.ignore(); // Clear the input buffer after age

        // Determine ticket price
        double ticketPrice = (age <= 15) ? 2000.0 : 5000.0;
        totalPrice += ticketPrice;

        // Generate a unique ticket ID
        string ticketID = username + "_" + to_string(time(nullptr)) + "_" + to_string(i);

        // Write ticket details directly to the file
        ticketFile << ticketID << "," << username << "," << trainID << "," << passengerName << "," << cnic << "," << age << "," << ticketPrice << endl;

        // Display ticket details to the user
        cout << "Ticket ID: " << ticketID << " - Price: PKR " << ticketPrice << endl;
        
        ofstream PassengerFile("passengers.csv", ios::app);
        if (!PassengerFile.is_open()) {
            cerr << "Error: Could not open Passenger file for saving." << endl;
            return;
        }
        string trainName;
        Passenger newPassenger(ticketID, username, trainID,trainName, cnic);
        newPassenger.saveToFile(PassengerFile);
        PassengerFile.close();
    }

    ticketFile.close();

    // Display total price
    cout << "All tickets booked successfully!" << endl;
    cout << "Total Price for " << numTickets << " ticket(s): PKR " << totalPrice << endl;
}


void RegularUser::viewTrainSchedule() const {
    ifstream trainFile("trains.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    string line;
    cout << "Train Schedule:" << endl;

    // Read each line from the train file
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileTrainID, fileTrainName, fileSource, fileDestination, fileDepartureTime, fileArrivalTime;

        // Extract each field from the line
        getline(ss, fileTrainID, ',');        // Skip trainID
        getline(ss, fileTrainName, ',');      // Get train name
        getline(ss, fileSource, ',');         // Get source
        getline(ss, fileDestination, ',');    // Get destination
        getline(ss, fileDepartureTime, ',');  // Get departure time
        getline(ss, fileArrivalTime);          // Get arrival time

        // Print all details except trainID
        cout << "Train Name: " << fileTrainName << endl;
        cout << "Source: " << fileSource << endl;
        cout << "Destination: " << fileDestination << endl;
        cout << "Departure Time: " << fileDepartureTime << endl;
        cout << "Arrival Time: " << fileArrivalTime << endl;
        cout << "---------------------------------------" << endl;
    }

    trainFile.close();
}


void RegularUser::cancelTicket() {
    cout << "Cancelling a ticket..." << endl;

    string ticketID;
    cout << "Enter Ticket ID: ";
    cin >> ticketID;

    // Open the tickets file for reading
    ifstream ticketFile("tickets.csv");
    if (!ticketFile.is_open()) {
        cerr << "Error: Could not open ticket file." << endl;
        return;
    }

    // Create a temporary file to store updated data
    ofstream tempFile("tickets_temp.csv");
    if (!tempFile.is_open()) {
        cerr << "Error: Could not create a temporary file." << endl;
        ticketFile.close();
        return;
    }

    string line;
    bool ticketFound = false;

    // Read the file line by line
    while (getline(ticketFile, line)) {
        stringstream ss(line);
        string fileTicketID, fileUsername;
        getline(ss, fileTicketID, ',');  // First element is ticketID
        getline(ss, fileUsername, ','); // Second element is username

        // Check if the ticketID and username match
        if (fileTicketID == ticketID && fileUsername == username) {
            // Skip the line containing the ticket to cancel
            ticketFound = true;
        } else {
            // Write all other lines to the temporary file
            tempFile << line << endl;
        }
    }

    ticketFile.close();
    tempFile.close();

    ifstream PassengerFile("passengers.csv");
    if (!PassengerFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    // Create a temporary file to store updated train data
    ofstream newtempFile("passenger_temp.csv");
    if (!newtempFile.is_open()) {
        cerr << "Error: Could not create a temporary file." << endl;
        PassengerFile.close();
        return;
    }

    line="";
    bool PassengerFound = false;

    // Read the file line by line
    while (getline(PassengerFile, line)) {
        stringstream ss(line);
        string fileticketID;

        // Parse the line to extract the ticket ID
        getline(ss, fileticketID, ',');  // First element is ticketID

        if (fileticketID == ticketID) {
            PassengerFound = true;
            // Skip the passenger to be removed (don't write it to temp file)
        } else {
            // Write all other lines to the temporary file
            newtempFile << line << endl;
        }
    }

    PassengerFile.close();
    newtempFile.close();


    if (PassengerFound) {
        remove("passengers.csv");
        rename("passenger_temp.csv", "passengers.csv");
        cout << "Passenger with "<<ticketID<< " removed successfully!" << endl;
    } else {
        remove("trains_temp.csv");
        cout << "Error: Train with ID " << ticketID << " not found!" << endl;
    }


    // Replace the original file with the temporary file
    remove("tickets.csv");
    rename("tickets_temp.csv", "tickets.csv");

    if (ticketFound) {
        cout << "Ticket with ID " << ticketID << " cancelled successfully." << endl;
    } else {
        cout << "Error: Ticket with ID " << ticketID << " not found or does not belong to you." << endl;
    }
}




// ==================================================== LoginManager Class ============================================================= //

// Resizing functions for dynamic arrays
void LoginManager::resizeAdmins() {
    adminCapacity *= 2;
    Admin** newAdmins = new Admin*[adminCapacity];  // Allocate an array of Admin pointers
    for (int i = 0; i < adminCount; ++i)
        newAdmins[i] = admins[i];
    delete[] admins;
    admins = newAdmins;
}

void LoginManager::resizeUsers() {
    userCapacity *= 2;
    RegularUser** newUsers = new RegularUser*[userCapacity];  // Allocate an array of RegularUser pointers
    for (int i = 0; i < userCount; ++i)
        newUsers[i] = users[i];
    delete[] users;
    users = newUsers;
}

// Constructor
LoginManager::LoginManager() : adminCount(0), userCount(0), adminCapacity(5), userCapacity(5) {
    admins = new Admin*[adminCapacity];  // Allocate memory for admin pointers
    users = new RegularUser*[userCapacity];  // Allocate memory for user pointers
}

// Destructor
LoginManager::~LoginManager() {
    delete[] admins;  // Deallocate array of admin pointers
    delete[] users;  // Deallocate array of user pointers
}

// Add Admin to the admins array
void LoginManager::addAdmin(Admin* admin) {
    if (adminCount == adminCapacity) {
        resizeAdmins();  // Resize if the array is full
    }
    admins[adminCount++] = admin;
}

// Add User to the users array
void LoginManager::addUser(RegularUser* user) {
    if (userCount == userCapacity) {
        resizeUsers();  // Resize if the array is full
    }
    users[userCount++] = user;
}

// Authenticate user or admin
User* LoginManager::authenticate(const std::string& username, const std::string& password) {
    for (int i = 0; i < adminCount; ++i) {
        if (admins[i]->getUsername() == username && admins[i]->getPassword() == password)
            return admins[i];
    }

    for (int i = 0; i < userCount; ++i) {
        if (users[i]->getUsername() == username && users[i]->getPassword() == password)
            return users[i];
    }

    return nullptr;  // No match found
}
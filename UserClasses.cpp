#include "Declarations.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm> // for std::remove_if
#include <map>
using namespace std;


// ==================================================== Helper Functions ============================================================= //

bool validateCredentials(const string& fileName, const string& username, const string& password)  {
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return false;
    }

    string line, fileUsername, filePassword;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, fileUsername, ','); // Read username until comma
        getline(ss, filePassword,','); // Read password until end of line
        
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
        occupancyFile << entry.first << "," << entry.second << endl; // ScheduleID_TrainID, Occupancy
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
        string scheduleTrainID;
        int occupancy;

        getline(ss, scheduleTrainID, ','); // ScheduleID_TrainID
        ss >> occupancy;

        trainOccupancy[scheduleTrainID] = occupancy;
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

bool User::login(const string& username, const string& password) {
    return true;
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

bool Admin::login(const string& username, const string& password) {
    bool islogged=false;
    cout << "[Admin Login]" << endl;
    this->username = username; // Store the username
    this->password = password; // Store the password

    if (validateCredentials("admins.csv", username, password)) {
        cout << "Admin login successful." << endl; // Assuming you have a member variable to track login status
        islogged=true;
        return islogged;
    } else {
        cout << "Invalid credentials. Please try again." << endl;
        this->username = this->password = ""; // Resetting credentials
        return islogged;
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
                viewTrainSchedule();
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
                break;
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
        string fileScheduleID, fileTrainID, fileTrainName, fileSource, fileDestination, fileDepartureTime, fileArrivalTime;

        // Extract each field from the line
        getline(ss, fileScheduleID, ',');     // Get ScheduleID  
        getline(ss, fileTrainID, ',');        // Get trainID
        getline(ss, fileTrainName, ',');      // Get train name
        getline(ss, fileSource, ',');         // Get source
        getline(ss, fileDestination, ',');    // Get destination
        getline(ss, fileDepartureTime, ',');  // Get departure time
        getline(ss, fileArrivalTime);          // Get arrival time

        cout << "Schedule ID: " << fileScheduleID << endl;
        cout << "Train ID: "<< fileTrainID<<endl;
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
        getline(ss, fileTicketID, ',');        // Get ticketID
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

    string trainID, trainName, ScheduleID, source, destination, departure_Time, arrival_Time;
    string line;

    // Open the trains.csv file to validate the train ID and Train Name
    ifstream trainsFile("trains.csv");
    if (!trainsFile.is_open()) {
        cerr << "Error: Could not open trains file to validate Train ID." << endl;
        return;
    }

    // Open the schedules.csv file to validate the ScheduleID
    ifstream schedulesFile("schedules.csv");
    if (!schedulesFile.is_open()) {
        cerr << "Error: Could not open schedules file to validate Schedule ID." << endl;
        return;
    }


    cout << "Enter Train ID: ";
    cin >> trainID;

    bool trainExists = false;
    string correctTrainName; // To store the correct train name if Train ID is found

    // Check if the Train ID and Train Name exist in the trains.csv file
    while (getline(trainsFile, line)) {
        stringstream ss(line);
        string fileTrainID, fileTrainName;

        getline(ss, fileTrainID, ',');  // Assume Train ID is the first field
        getline(ss, fileTrainName, ','); // Assume Train Name is the second field

        if (fileTrainID == trainID) {
            trainExists = true;
            correctTrainName = fileTrainName;
            break;
        }
    }
    trainsFile.close();

    if (!trainExists) {
        cerr << "Error: Train ID " << trainID << " does not exist!" << endl;
        return;
    }


    cout << "Enter Train Name: ";
    cin.ignore(); // Clear the newline character from the input buffer
    getline(cin, trainName);

    if (trainName != correctTrainName) {
        cerr << "Error: Train Name does not match the Train ID!" << endl;
        return;
    }

    cout << "Enter Schedule ID: ";
    cin >> ScheduleID;

    // Check if the ScheduleID already exists in schedules.csv
    bool scheduleExists = false;

    while (getline(schedulesFile, line)) {
        stringstream ss(line);
        string fileScheduleID;

        getline(ss, fileScheduleID, ','); // Assume ScheduleID is the first field

        if (fileScheduleID == ScheduleID) {
            scheduleExists = true;
            break;
        }
    }
    schedulesFile.close();

    if (scheduleExists) {
        cerr << "Error: Schedule ID " << ScheduleID << " already exists!" << endl;
        return;
    }
    cin.ignore(); // Clear any leftover newline character from the input buffer
    cout << "Enter Source: ";
    getline(cin, source);
    cout << "Enter Destination: ";
    getline(cin, destination);
    cout << "Enter Departure Time: ";
    getline(cin, departure_Time);
    cout << "Enter Arrival Time: ";
    getline(cin, arrival_Time);

    // Create a new Train Schedule object
    Schedules newschedule(trainID, trainName, ScheduleID, source, destination, departure_Time, arrival_Time);

    // Append the new train schedule to the schedules.csv file
    ofstream scheduleFileOut("schedules.csv", ios::app);
    if (!scheduleFileOut.is_open()) {
        cerr << "Error: Could not open schedules.csv for writing." << endl;
        return;
    }

    newschedule.saveToFile(scheduleFileOut);
    scheduleFileOut.close();

    cout << "Train schedule added successfully!" << endl;
}


void Admin::updateTrainSchedule() {
    cout << "Updating train schedule..." << endl;

    string scheduleID;
    cout << "Enter Schedule ID to update: ";
    cin >> scheduleID;

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
    bool scheduleFound = false;

    // Read the original file line by line
    while (getline(trainFile, line)) {
        stringstream ss(line);
        string fileScheduleID, fileTrainID, fileTrainName, fileSource, fileDestination, fileDepartureTime, fileArrivalTime;

        // Parse the line
        getline(ss, fileScheduleID, ',');   // First field is ScheduleID
        getline(ss, fileTrainID, ',');     // Second field is TrainID
        getline(ss, fileTrainName, ',');   // Third field is TrainName
        getline(ss, fileSource, ',');      // Fourth field is Source
        getline(ss, fileDestination, ','); // Fifth field is Destination
        getline(ss, fileDepartureTime, ','); // Sixth field is Departure Time
        getline(ss, fileArrivalTime);      // Seventh field is Arrival Time

        if (fileScheduleID == scheduleID) {
            scheduleFound = true;

            // Display current schedule details
            cout << "\nCurrent Schedule Details:" << endl;
            cout << "Schedule ID: " << fileScheduleID << endl;
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
                // cout << "1. Train Name" << endl;
                cout << "1. Source" << endl;
                cout << "2. Destination" << endl;
                cout << "3. Departure Time" << endl;
                cout << "4. Arrival Time" << endl;
                cout << "0. Exit Update Menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                // Update selected field
                switch (choice) {
                    case 1: {
                        cout << "Enter new Source: ";
                        cin.ignore();
                        getline(cin, fileSource);
                        break;
                    }
                    case 2: {
                        cout << "Enter new Destination: ";
                        cin.ignore();
                        getline(cin, fileDestination);
                        break;
                    }
                    case 3: {
                        cout << "Enter new Departure Time: ";
                        cin.ignore();
                        getline(cin, fileDepartureTime);
                        break;
                    }
                    case 4: {
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

            // Write updated schedule details to the temporary file
            tempFile << fileScheduleID << "," << fileTrainID << "," << fileTrainName << "," << fileSource << "," 
                     << fileDestination << "," << fileDepartureTime << "," << fileArrivalTime << endl;
        } else {
            // Write unchanged lines to the temporary file
            tempFile << line << endl;
        }
    }

    trainFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (scheduleFound) {
        remove("schedules.csv");
        rename("schedules_temp.csv", "schedules.csv");
        cout << "Train schedule updated successfully!" << endl;
    } else {
        remove("schedules_temp.csv");
        cout << "Error: Schedule with ID " << scheduleID << " not found!" << endl;
    }
}

void Admin::removeTrainSchedule() {
    cout << "Removing a train schedule..." << endl;

    string scheduleID;
    cout << "Enter Schedule ID to remove: ";
    cin >> scheduleID;

    // Open the schedules file for reading
    ifstream scheduleFile("schedules.csv");
    if (!scheduleFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    // Create a temporary file to store updated schedule data
    ofstream tempFile("schedules_temp.csv");
    if (!tempFile.is_open()) {
        cerr << "Error: Could not create a temporary file." << endl;
        scheduleFile.close();
        return;
    }

    string line;
    bool scheduleFound = false;

    // Read the file line by line
    while (getline(scheduleFile, line)) {
        stringstream ss(line);
        string fileScheduleID, fileTrainID, fileTrainName, fileSource, fileDestination, fileDepartureTime, fileArrivalTime;

        // Parse the line to extract schedule details
        getline(ss, fileScheduleID, ',');      // First element is ScheduleID
        getline(ss, fileTrainID, ',');         // Second element is TrainID
        getline(ss, fileTrainName, ',');       // Third element is TrainName
        getline(ss, fileSource, ',');          // Fourth element is Source
        getline(ss, fileDestination, ',');     // Fifth element is Destination
        getline(ss, fileDepartureTime, ',');   // Sixth element is Departure Time
        getline(ss, fileArrivalTime);          // Seventh element is Arrival Time

        if (fileScheduleID == scheduleID) {
            scheduleFound = true;
            // Skip the schedule to be removed (don't write it to temp file)
        } else {
            // Write all other lines to the temporary file
            tempFile << line << endl;
        }
    }

    scheduleFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (scheduleFound) {
        remove("schedules.csv");
        rename("schedules_temp.csv", "schedules.csv");
        cout << "Schedule with ID " << scheduleID << " removed successfully!" << endl;
    } else {
        remove("schedules_temp.csv");
        cout << "Error: Schedule with ID " << scheduleID << " not found!" << endl;
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
        getline(ss, fileCapacity, ',');         // Get Capacity

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
            cout << "Seating Capacity: " << fileCapacity << endl;

            int choice = -1;
            while (choice != 0) {
                // Display update menu
                cout << "\nWhat would you like to update?" << endl;
                cout << "1. Train Name" << endl;
                cout << "2. Seating Capacity" << endl;
                cout << "0. Exit to Update Menu" << endl;
                cout << "Enter your choice: ";
                cin >> choice;

                // Update selected field
                switch (choice) {
                    case 1: {
                        cout << "Enter new Train Name: ";
                        cin.ignore(); // Clear the input buffer
                        getline(cin, fileTrainName);
                        break;
                    }
                    case 2: {
                        cout << "Enter new Seating Capacity: ";
                        cin.ignore(); // Clear the input buffer
                        getline(cin, fileCapacity);
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

            // Only write updated train details to the temporary file if changes were made
            tempFile << fileTrainID << "," << fileTrainName << "," << fileCapacity << endl;
        } else {
            // If the train ID does not match, write the original line to the temp file
            tempFile << line << endl;
        }
    }

    trainFile.close();
    tempFile.close();

    // Replace the original file with the temporary file
    if (trainFound) {
        remove("trains.csv");
        rename("trains_temp.csv", "trains.csv");
        cout << "Train details updated successfully!" << endl;
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

    ifstream scheduleFile("schedules.csv");
        if (!scheduleFile.is_open()) {
            cerr << "Error: Could not open train schedule file." << endl;
            return;
        }

    // Create a temporary file to store updated train data
    ofstream temp2File("schedules_temp.csv");
        if (!temp2File.is_open()) {
            cerr << "Error: Could not create a temporary file." << endl;
            scheduleFile.close();
            return;
        }

    line="";
    bool schedFound = false;

    // Read the file line by line
    while (getline(scheduleFile, line)) {
        stringstream ss(line);
        string fileScheduleID,fileTrainID;

        
        // Parse the line to extract the train ID
        getline(ss, fileScheduleID, ','); 
        getline(ss, fileTrainID, ',');  

        if (fileTrainID == trainID) {
            schedFound = true;
            // Skip the train to be removed (don't write it to temp file)
        } else {
            // Write all other lines to the temporary file
            temp2File << line << endl;
        }
    }

    scheduleFile.close();
    temp2File.close();

        // Replace the original file with the temporary file
    if (schedFound) {
        remove("schedules.csv");
        rename("schedules_temp.csv", "schedules.csv");
        cout << "Schedules associated with the train ID " << trainID << " removed successfully!" << endl;
    } else {
        remove("schedules_temp.csv");
        // cout << "Error: Schedule with ID " << trainID << " not found!" << endl;
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

bool RegularUser::login(const string& username, const string& password) {
    bool islogged=false;
    cout << "[User  Login]" << endl;
    this->username = username; // Store the username
    this->password = password; // Store the password

    if (validateCredentials("users.csv", username, password)) {
        cout << "User  login successful." << endl; // Assuming you have a member variable to track login status
        islogged=true;
        return islogged;
    } else {
        cout << "Invalid credentials. Please try again." << endl;
        this->username = this->password = ""; // Resetting credentials
        return islogged;
    }
}

void RegularUser::displayMenu(){
    int choice = -1;
    while (choice != 4) {  // 5 is the option for Logout
        cout << "User Menu:" << endl;
        cout << "1. View Train Schedule" << endl;
        cout << "2. Book Ticket" << endl;
        cout << "3. Cancel Ticket" << endl;
        cout << "4. Logout" << endl;
        cout << "Select an Option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                viewTrainSchedule();
                break;
            case 2:
                bookTicket();
                break;
            case 3:
                cancelTicket();
                break;
            case 4:
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
    RegularUser::viewTrainSchedule();
    cout << "Booking tickets..." << endl;

    string trainID, scheduleID;
    cout << "Enter Train ID: ";
    cin >> trainID;

    cout << "Enter Schedule ID: ";
    cin >> scheduleID;

    // Check if the schedule exists in schedules.csv
    ifstream scheduleFile("schedules.csv");
    if (!scheduleFile.is_open()) {
        cerr << "Error: Could not open schedules file." << endl;
        return;
    }

    bool scheduleFound = false;
    string line, fileTrainID, fileScheduleID;
    while (getline(scheduleFile, line)) {
        stringstream ss(line);
        getline(ss, fileScheduleID, ',');  // Get schedule ID
        getline(ss, fileTrainID, ',');     // Get train ID

        if (fileTrainID == trainID && fileScheduleID == scheduleID) {
            scheduleFound = true;
            break;
        }
    }
    scheduleFile.close();

    if (!scheduleFound) {
        cerr << "Error: Schedule with ID " << scheduleID << " for Train ID " << trainID << " not found!" << endl;
        return;
    }

    // Load train details and check capacity
    ifstream trainFile("trains.csv");
    if (!trainFile.is_open()) {
        cerr << "Error: Could not open trains file for reading." << endl;
        return;
    }

    bool trainFound = false;
    string fileCapacity, filetrainName;
    while (getline(trainFile, line)) {
        stringstream ss(line);
        getline(ss, fileTrainID, ',');  // Get train ID
        getline(ss, filetrainName, ',');
        getline(ss, fileCapacity, ','); // Get train capacity

        if (fileTrainID == trainID) {
            trainFound = true;
            break;
        }
    }
    trainFile.close();

    if (!trainFound) {
        cerr << "Error: Train with ID " << trainID << " not found!" << endl;
        return;
    }

    int capacity = stoi(fileCapacity); // Train capacity
    string scheduleTrainKey = scheduleID + "_" + trainID;
    int currentOccupancy = trainOccupancy[scheduleTrainKey]; // Get current occupancy from occupancy.csv

    // Start booking tickets
    int numTickets;
    cout << "How many tickets do you want to book? ";
    cin >> numTickets;

    if (currentOccupancy + numTickets > capacity) {
        cerr << "Error: Insufficient occupancy for this schedule. Please choose another schedule or fly better fly Emirates :)" << endl;
        return;
    }

    // Update the occupancy in trainOccupancy map
    trainOccupancy[scheduleTrainKey] += numTickets;
    saveOccupancyData();  // Save the updated occupancy data

    // Open the ticket file for appending
    ofstream ticketFile("tickets.csv", ios::app);
    if (!ticketFile.is_open()) {
        cerr << "Error: Could not open ticket file for saving." << endl;
        return;
    }

    ofstream passengerFile("passengers.csv", ios::app);
    if (!passengerFile.is_open()) {
        cerr << "Error: Could not open passengers file for saving." << endl;
        ticketFile.close();
        return;
    }

    double totalPrice = 0.0;
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

        // Save ticket details
        ticketFile << ticketID << "," << username << "," << scheduleID << "," << trainID << "," << passengerName << "," << cnic << "," << age << "," << ticketPrice << endl;

        // Save passenger details
        passengerFile << ticketID << "," << username << "," << scheduleID << "," << trainID << "," << fileTrainID << "," << passengerName << "," << cnic << "," << age << endl;

        cout << "Ticket ID: " << ticketID << " - Price: PKR " << ticketPrice << endl;
    }

    ticketFile.close();
    passengerFile.close();

    // Display total price
    cout << "All tickets booked successfully!" << endl;
    cout << "Total Price for " << numTickets << " ticket(s): PKR " << totalPrice << endl;
}


void RegularUser::viewTrainSchedule() const {
    ifstream scheduleFile("schedules.csv");
    if (!scheduleFile.is_open()) {
        cerr << "Error: Could not open train schedule file." << endl;
        return;
    }

    string line;
    cout << "Train Schedule:" << endl;

    // Read each line from the schedule file
    while (getline(scheduleFile, line)) {
        stringstream ss(line);
        string scheduleID, trainID, trainName, source, destination, departureTime, arrivalTime;

        // Extract fields from the line
        getline(ss, scheduleID, ',');        // Get Schedule ID
        getline(ss, trainID, ',');          // Get Train ID
        getline(ss, trainName, ',');        // Get Train Name
        getline(ss, source, ',');           // Get Source
        getline(ss, destination, ',');      // Get Destination
        getline(ss, departureTime, ',');    // Get Departure Time
        getline(ss, arrivalTime);           // Get Arrival Time

        // Display the schedule information
        cout << "Schedule ID: " << scheduleID << endl;
        cout << "Train Name: " << trainName << endl;
        cout << "Source: " << source << endl;
        cout << "Destination: " << destination << endl;
        cout << "Departure Time: " << departureTime << endl;
        cout << "Arrival Time: " << arrivalTime << endl;
        cout << "---------------------------------------" << endl;
    }

    scheduleFile.close();
}


void RegularUser::cancelTicket() {
    cout << "Cancelling a ticket..." << endl;

    string ticketID;
    cout << "Enter Ticket ID: ";
    cin >> ticketID;
    loadOccupancyData(); // Load current occupancy data

    // Open the tickets file for reading
    ifstream ticketFile("tickets.csv");
    if (!ticketFile.is_open()) {
        cerr << "Error: Could not open tickets file." << endl;
        return;
    }

    // Create a temporary file to store updated ticket data
    ofstream tempFile("tickets_temp.csv");
    if (!tempFile.is_open()) {
        cerr << "Error: Could not create a temporary file for tickets." << endl;
        ticketFile.close();
        return;
    }

    string line;
    bool ticketFound = false;
    string trainID, scheduleID;
    int ticketsToCancel = 0;

    // Read the file line by line
    while (getline(ticketFile, line)) {
        stringstream ss(line);
        string fileTicketID, fileUsername, fileScheduleID, fileTrainID;

        getline(ss, fileTicketID, ',');  // First element is TicketID
        getline(ss, fileUsername, ','); // Second element is Username
        getline(ss, fileScheduleID, ','); // Third element is ScheduleID
        getline(ss, fileTrainID, ','); // Fourth element is TrainID

        if (fileTicketID == ticketID && fileUsername == username) {
            // Ticket to cancel found
            ticketFound = true;
            scheduleID = fileScheduleID;
            trainID = fileTrainID;
            ticketsToCancel++; // Increment the count of tickets to cancel
        } else {
            // Write all other lines to the temporary file
            tempFile << line << endl;
        }
    }

    ticketFile.close();
    tempFile.close();

    if (ticketFound) {
        // Update the occupancy for the schedule
        string scheduleTrainKey = scheduleID + "_" + trainID;
        if (trainOccupancy.find(scheduleTrainKey) != trainOccupancy.end()) {
            trainOccupancy[scheduleTrainKey] -= ticketsToCancel; // Decrement occupancy
        }
        saveOccupancyData(); // Save updated occupancy data

        // Replace the original tickets file with the updated file
        remove("tickets.csv");
        rename("tickets_temp.csv", "tickets.csv");

        cout << "Ticket with ID " << ticketID << " cancelled successfully." << endl;
    } else {
        // If ticket was not found, clean up the temporary file
        remove("tickets_temp.csv");
        cout << "Error: Ticket with ID " << ticketID << " not found or does not belong to you." << endl;
        return;
    }

    // Handle passenger file updates
    ifstream passengerFile("passengers.csv");
    if (!passengerFile.is_open()) {
        cerr << "Error: Could not open passengers file." << endl;
        return;
    }

    ofstream passengerTempFile("passengers_temp.csv");
    if (!passengerTempFile.is_open()) {
        cerr << "Error: Could not create a temporary file for passengers." << endl;
        passengerFile.close();
        return;
    }

    bool passengerFound = false;

    while (getline(passengerFile, line)) {
        stringstream ss(line);
        string fileTicketID;

        getline(ss, fileTicketID, ','); // First element is TicketID

        if (fileTicketID == ticketID) {
            // Passenger entry to remove
            passengerFound = true;
        } else {
            // Write all other lines to the temporary file
            passengerTempFile << line << endl;
        }
    }

    passengerFile.close();
    passengerTempFile.close();

    if (passengerFound) {
        remove("passengers.csv");
        rename("passengers_temp.csv", "passengers.csv");
        cout << "Passenger with ticket ID " << ticketID << " removed successfully!" << endl;
    } else {
        remove("passengers_temp.csv");
        cout << "Error: Passenger with ticket ID " << ticketID << " not found!" << endl;
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
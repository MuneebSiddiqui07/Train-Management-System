#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>  
#include <sstream>
using namespace std;

// Base Class: User
class User {
protected:
    string username;
    string password;

public:
    User();
    User(const string& username, const string& password);
    virtual ~User(); // Virtual destructor for polymorphism

    // Pure virtual functions; 
    virtual bool login(const string& username, const string& password);
    void viewTrainSchedule() const;


    // Getters
    string getUsername() const;
    string getPassword() const;
};

// Derived Class: Admin
class Admin : public User {
public:
    Admin();
    Admin(const string& username, const string& password);

    void signup(); 
    bool login(const string& username, const string& password);
    void displayMenu();

    // Admin-specific functions
    void viewTrainSchedule() const;
    void addTrain();
    void viewTrain();
    void updateTrainSchedule();
    void addTrainschedule();
    void updateTrainDetails();
    void viewpassenger() const;
    void removeTrain();
    void removeTrainSchedule();
};

// Derived Class: RegularUser
class RegularUser : public User {
public:
    RegularUser();
    RegularUser(const string& username, const string& password);

    void signup() ; // New signup functionality
    bool login(const string& username, const string& password);
    void displayMenu();

    // RegularUser-specific functions
    void viewTrainSchedule() const;
    void bookTicket();
    void cancelTicket();
};

// Train Class
class Train {
protected:
    string trainID;
    string trainName;
    string capacity;

public:
    Train();
    Train(const string& id, const string& name, const string& capacity);

    // Copy constructor
    Train(const Train& other);
    // Destructor
    ~Train();

    // Getters and Setters
    string getTrainID() const;
    void setTrainID(const string& id);

    string getTrainName() const;
    void setTrainName(const string& name);

    void displaySchedule() const;

    // Serialization to save train data
    void saveToFile(ofstream& outFile) const;
    void loadFromFile(ifstream& inFile);
};

class Schedules: public Train {
private:
    string ScheduleID;
    string source;
    string destination;
    string depart_time;
    string arrival_time;

public:
    Schedules();
    Schedules(const string& id, const string& name,const string& schedid, const string& src, const string& dest, const string& d_time, const string& a_time);
    // Copy constructor
    Schedules(const Schedules& other);
    


    // Destructor
    ~Schedules();

    // Getters and Setters
    string getTrainID() const;
    void setTrainID(const string& id);

    string getTrainName() const;
    void setTrainName(const string& name);

    void displaySchedule() const;

    // Serialization to save train data
    void saveToFile(ofstream& outFile) const;
    void loadFromFile(ifstream& inFile);
};

class Passenger {
private:
    string ticketId;            // Unique identifier for ticket
    string name;                // passenger name
    string trainId;             // Id of the train booked
    string CNIC;                // cnic for passenger unique identifier 
    string trainName;           // name of the train 

public:
    // Constructors
    Passenger();  // Default constructor
    Passenger(const string& ticketId);  // Parameterized constructor (with ID only)

    Passenger(const string&  TicketId,const string&  Name,const string&  TrainID,const string& TrainName,const string&  NIC);
    // Copy constructor
    Passenger(const Passenger& other);

    // Getters and Setters
    string getticketID() const;
    void setticketID(const string& ticketId);

    string getname() const;
    void setname(const string& name);

    string gettrainId() const;
    void settrainId(const string& trainId);

    string getCNIC() const;
    void setCNIC(const string& CNIC);

    string getTrainName() const;
    void setTrainName(const string& trainName);

    void saveToFile(ofstream& outFile) const;
    void loadFromFile(ifstream& inFile);
};

// TrainManager Class (Aggregation)
class TrainManager {
private:
    Train* trains;      // Dynamic array of Train objects
    int trainCount;     // Current number of trains
    int trainCapacity;  // Maximum capacity of the train array

    void resizeTrains(); // Helper function to resize the train array

public:
    TrainManager();
    ~TrainManager(); // Destructor to clean up dynamic memory

    // Add, update, and remove trains
    void addTrain(const Train& train);
    void removeTrain(const string& trainID);
    Train* findTrain(const string& trainID);

    // Display all trains
    void displayAllTrains() const;

    // Serialization
    void saveToFile(const string& fileName) const;
    void loadFromFile(const string& fileName);
};

// LoginManager Class (Containment)
class LoginManager {
private:
    RegularUser** users; // Pointer to array of RegularUser pointers
    Admin** admins;      // Pointer to array of Admin pointers
    int adminCount;
    int userCount;
    int adminCapacity;
    int userCapacity;

    // Resizing functions for dynamic arrays
    void resizeAdmins();
    void resizeUsers();

public:
    // Constructor and Destructor
    LoginManager();
    ~LoginManager();

    // Add Admin or User to respective arrays
    void addAdmin(Admin* admin);
    void addUser(RegularUser* user);

    // Authenticate user or admin
    User* authenticate(const std::string& username, const std::string& password);
};

#endif

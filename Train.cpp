#include "Declarations.h"

// ================================================= TRAIN CLASS DEFINITIONS ====================================================== //

// Default constructor
Train::Train() {
    trainID = "";
    trainName = "";
    capacity=0;
}

// Parameterized constructor
Train::Train(const string& id, const string& name, const int& capa) {
    trainID = id;
    trainName = name;
    capacity=capa;
}

// Copy constructor
Train::Train(const Train& other) {
    trainID = other.trainID;
    trainName = other.trainName;
    capacity=other.capacity;
}

// Destructor
Train::~Train() {
    
}

// Getter for trainID
string Train::getTrainID() const {
    return trainID;
}

// Setter for trainID
void Train::setTrainID(const string& id) {
    trainID = id;
}

// Getter for trainName
string Train::getTrainName() const {
    return trainName;
}

// Setter for trainName
void Train::setTrainName(const string& name) {
    trainName = name;
}


// Display the schedule of the train
void Train::displaySchedule() const {
    cout << "Schedule for Train " << trainName << " (" << trainID << "):\n";
    // Displaying schedule stops (for now, this is just a placeholder since actual schedule data isn't in this version)
    cout << "Seating Capacity: " << to_string(capacity) << "\n";
}

// Save the train data to a file
void Train::saveToFile(ofstream& outFile) const {
    outFile << trainID << "," << trainName << "," << to_string(capacity) << endl;
}

// Load the train data from a file
string capacitystr;
void Train::loadFromFile(ifstream& inFile) {
    getline(inFile, trainID, ',');
    getline(inFile, trainName, ',');
    getline(inFile, capacitystr,',');
    capacity=stoi(capacitystr);

}

Schedules::Schedules() {
    trainID = "";
    trainName = "";
    source = "";
    destination = "";
    depart_time = "";
    arrival_time = "";
}

// Parameterized constructor
Schedules::Schedules(const string& id, const string& name, const string& src, const string& dest, const string& d_time, const string& a_time) {
    trainID = id;
    trainName = name;
    source = src;
    destination = dest;
    depart_time = d_time;
    arrival_time = a_time;
}

// Copy constructor
Schedules::Schedules(const Schedules& other) {
    trainID = other.trainID;
    trainName = other.trainName;
    source = other.source;
    destination = other.destination;
    depart_time = other.depart_time;
    arrival_time = other.arrival_time;
}

// Destructor
Schedules::~Schedules() {
    // No dynamic memory allocation in this version, so no cleanup needed here
}

// Getter for trainID
string Schedules::getTrainID() const {
    return trainID;
}

// Setter for trainID
void Schedules::setTrainID(const string& id) {
    trainID = id;
}

// Getter for trainName
string Schedules::getTrainName() const {
    return trainName;
}

// Setter for trainName
void Schedules::setTrainName(const string& name) {
    trainName = name;
}


// Display the schedule of the train
void Schedules::displaySchedule() const {
    cout << "Schedule for Train " << trainName << " (" << trainID << "):\n";
    // Displaying schedule stops (for now, this is just a placeholder since actual schedule data isn't in this version)
    cout << "Source: " << source << "\n";
    cout << "Destination: " << destination << "\n";
    cout << "Departure: " << depart_time << "\n";
    cout << "Arrival: " << arrival_time << "\n";
}

// Save the train data to a file
void Schedules::saveToFile(ofstream& outFile) const {
    outFile << trainID << "," << trainName << "," << source << "," << destination << "," << depart_time << "," << arrival_time << endl;
}

// Load the train data from a file
void Schedules::loadFromFile(ifstream& inFile) {
    getline(inFile, trainID, ',');
    getline(inFile, trainName, ',');
    getline(inFile, source, ',');
    getline(inFile, destination, ',');
    getline(inFile, depart_time, ',');
    getline(inFile, arrival_time, ',');

}
// =================================================== PASSENGER CLASS DEFINITIONS ======================================================//

// Bogie Class Definitions

// Default constructor
Passenger::Passenger() {
    ticketId = "";
    name="";
    trainId="";
    CNIC="";    
}

// Parameterized constructor
Passenger::Passenger(const string&  TicketId) {
    ticketId = TicketId;
    name="";
    trainId="";
    CNIC="";    
}

Passenger::Passenger(const string&  TicketId,const string&  Name,const string&  TrainID,const string& TrainName,const string&  NIC) {
    ticketId = TicketId;
    name=Name;
    trainId=TrainID;
    CNIC=NIC;   
    trainName=TrainName; 
}

// Copy constructor
Passenger::Passenger(const Passenger& other) 
{
    ticketId = other.ticketId;
    name=other.name;
    trainId=other.trainId;
    CNIC=other.CNIC;
    trainName=other.trainName;
}

// Getter and Setter for passnegerID
string Passenger::getticketID() const { 
    return ticketId; 
}

void Passenger::setticketID(const string& ticketid) { 
    ticketId = ticketid; 
}

// Getter and Setter for name
string Passenger::getname() const { 
    return name; 
}

void Passenger::setname(const string& Name) { 
    name = Name; 
}
// Getter and Setter for train id
string Passenger::gettrainId() const { 
    return trainId; 
}

void Passenger::settrainId(const string& Trainid) { 
    trainId = Trainid; 
}

string Passenger::getCNIC() const { 
    return CNIC; 
}

void Passenger::setCNIC(const string& NIC) { 
    CNIC = NIC; 
}

string Passenger::getTrainName()const{
    return trainName;
}
void Passenger::setTrainName(const string& TrainName){
    trainName=TrainName;
}
//SAVE Passenger details in file
void Passenger::saveToFile(ofstream& outFile) const {
    outFile << ticketId << "," << name << "," << trainId << ","<<trainName<< ","<< CNIC <<endl;
}


// =============================================== TRAIN MANAGER CLASS DEFINITIONS =================================================//

// TrainManager Helper Function
void TrainManager::resizeTrains() {
    trainCapacity *= 2;
    Train* newTrains = new Train[trainCapacity];
    for (int i = 0; i < trainCount; ++i)
        newTrains[i] = trains[i];
    delete[] trains;
    trains = newTrains;
}

TrainManager::TrainManager(){
    trainCount = 0; 
    trainCapacity= 5; 
    trains = new Train[trainCapacity];
}

TrainManager:: ~TrainManager() { delete[] trains; }

void TrainManager::addTrain(const Train& train) {
    if (trainCount == trainCapacity)
        resizeTrains();
    trains[trainCount++] = train;
}

void TrainManager::removeTrain(const string& trainID) {
    for (int i = 0; i < trainCount; ++i) {
        if (trains[i].getTrainID() == trainID) {
            trains[i] = trains[--trainCount];
            return;
        }
    }
    cout << "Train not found.\n";
}


void TrainManager::saveToFile(const string& fileName) const {
    ofstream outFile(fileName);
    outFile << trainCount << endl;
    for (int i = 0; i < trainCount; ++i)
        trains[i].saveToFile(outFile);
}

void TrainManager::loadFromFile(const string& fileName) {
    ifstream inFile(fileName);
    inFile >> trainCount;
    inFile.ignore();

    delete[] trains;
    trainCapacity = trainCount + 5;
    trains = new Train[trainCapacity];

    for (int i = 0; i < trainCount; ++i)
        trains[i].loadFromFile(inFile);
}



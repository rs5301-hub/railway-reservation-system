/*
 * ╔══════════════════════════════════════════════════════════════╗
 * ║         RAILWAY RESERVATION SYSTEM                          ║
 * ║         C++ Port of Python Pandas Project                   ║
 * ╚══════════════════════════════════════════════════════════════╝
 *
 *  CSV Files used (auto-created if missing):
 *    Train_details.csv     — train database
 *    Passenger_details.csv — passenger/booking database
 *
 *  Compile:  g++ -std=c++17 -o railway railway.cpp
 *  Run:      ./railway
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <map>
#include <limits>
#include <cmath>

using namespace std;

// ══════════════════════════════════════════════════════════
//  STRUCTS
// ══════════════════════════════════════════════════════════

struct Train {
    int    trainNum;
    string trainName;
    string source;
    string destination;
    string departure;   // e.g. "06:00"
    string arrival;     // e.g. "14:00"
    int    quantity;    // total seats
    double cost;
};

struct Passenger {
    string name;
    string age;
    string gender;
    string phone;
    string dateOfJourney;
    int    trainNumber;
    string pnr;         // CNF / RAC / WL
};

// ══════════════════════════════════════════════════════════
//  GLOBALS
// ══════════════════════════════════════════════════════════

vector<Train>     trains;
vector<Passenger> passengers;

const string TRAIN_FILE     = "Train_details.csv";
const string PASSENGER_FILE = "Passenger_details.csv";

// ══════════════════════════════════════════════════════════
//  UTILITY
// ══════════════════════════════════════════════════════════

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine(char ch = '-', int len = 70) {
    cout << string(len, ch) << "\n";
}

void printHeader(const string& title) {
    printLine('=');
    int pad = (70 - (int)title.size()) / 2;
    cout << string(max(pad, 0), ' ') << title << "\n";
    printLine('=');
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Trim whitespace from both ends
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    if (start == string::npos) return "";
    return s.substr(start, end - start + 1);
}

// Safe integer input
int inputInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return val;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid input. Please enter a number.\n";
    }
}

// Safe string input (full line)
string inputLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return trim(s);
}

// ══════════════════════════════════════════════════════════
//  CSV PARSING HELPERS
// ══════════════════════════════════════════════════════════

// Split a CSV line respecting quoted fields
vector<string> parseCSVLine(const string& line) {
    vector<string> fields;
    string field;
    bool inQuotes = false;
    for (size_t i = 0; i < line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(trim(field));
    return fields;
}

// ══════════════════════════════════════════════════════════
//  CSV READ / WRITE
// ══════════════════════════════════════════════════════════

void loadTrains() {
    trains.clear();
    ifstream file(TRAIN_FILE);
    if (!file.is_open()) return;

    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        if (trim(line).empty()) continue;
        auto f = parseCSVLine(line);
        if (f.size() < 8) continue;
        Train t;
        try {
            t.trainNum   = stoi(f[0]);
            t.trainName  = f[1];
            t.source     = f[2];
            t.destination= f[3];
            t.departure  = f[4];
            t.arrival    = f[5];
            t.quantity   = stoi(f[6]);
            t.cost       = stod(f[7]);
            trains.push_back(t);
        } catch (...) {}
    }
}

void loadPassengers() {
    passengers.clear();
    ifstream file(PASSENGER_FILE);
    if (!file.is_open()) return;

    string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        if (trim(line).empty()) continue;
        auto f = parseCSVLine(line);
        if (f.size() < 7) continue;
        Passenger p;
        try {
            p.name          = f[0];
            p.age           = f[1];
            p.gender        = f[2];
            p.phone         = f[3];
            p.dateOfJourney = f[4];
            p.trainNumber   = stoi(f[5]);
            p.pnr           = f[6];
            passengers.push_back(p);
        } catch (...) {}
    }
}

void savePassengers() {
    ofstream file(PASSENGER_FILE);
    file << "Name of Passenger,Age,Gender,Phone Number,Date of Journey,Train Number,PNR\n";
    for (const auto& p : passengers) {
        file << p.name << ","
             << p.age  << ","
             << p.gender << ","
             << p.phone  << ","
             << p.dateOfJourney << ","
             << p.trainNumber   << ","
             << p.pnr  << "\n";
    }
}

void saveTrains() {
    ofstream file(TRAIN_FILE);
    file << "Train Num,Train Name,Source,Destination,Departure,Arrival,Quantity,Cost\n";
    for (const auto& t : trains) {
        file << t.trainNum   << ","
             << t.trainName  << ","
             << t.source     << ","
             << t.destination<< ","
             << t.departure  << ","
             << t.arrival    << ","
             << t.quantity   << ","
             << fixed << setprecision(2) << t.cost << "\n";
    }
}

// Create default CSVs if they don't exist
void createDefaultCSVs() {
    {
        ifstream f(TRAIN_FILE);
        if (!f.is_open()) {
            ofstream out(TRAIN_FILE);
            out << "Train Num,Train Name,Source,Destination,Departure,Arrival,Quantity,Cost\n";
            out << "12345,Chennai Express,Chennai,Mumbai,06:00,22:00,200,850.00\n";
            out << "22301,Duronto Express,Chennai,Delhi,07:30,07:30,180,1200.00\n";
            out << "16057,Saptagiri Express,Chennai,Tirupati,06:10,09:40,250,320.00\n";
            out << "12163,Chennai Dadar Express,Chennai,Dadar,14:00,18:30,160,950.00\n";
            out << "22625,Double Decker Express,Chennai,Bangalore,06:05,11:15,300,450.00\n";
            out << "17311,Vasco Express,Chennai,Vasco,22:00,06:30,140,1100.00\n";
            out << "12605,Pallavan Express,Chennai,Coimbatore,07:00,12:45,220,380.00\n";
            out << "11041,Mumbai Express,Chennai,Mumbai,11:45,11:45,190,870.00\n";
        }
    }
    {
        ifstream f(PASSENGER_FILE);
        if (!f.is_open()) {
            ofstream out(PASSENGER_FILE);
            out << "Name of Passenger,Age,Gender,Phone Number,Date of Journey,Train Number,PNR\n";
        }
    }
}

// ══════════════════════════════════════════════════════════
//  DISPLAY HELPERS
// ══════════════════════════════════════════════════════════

void printTrainTable(const vector<Train>& list) {
    if (list.empty()) {
        cout << "  No trains found.\n";
        return;
    }
    printLine('-');
    cout << left
         << setw(8)  << "Num"
         << setw(26) << "Train Name"
         << setw(14) << "Destination"
         << setw(11) << "Depart"
         << setw(11) << "Arrive"
         << setw(8)  << "Seats"
         << setw(10) << "Cost(Rs)"
         << "\n";
    printLine('-');
    for (const auto& t : list) {
        cout << left
             << setw(8)  << t.trainNum
             << setw(26) << t.trainName
             << setw(14) << t.destination
             << setw(11) << t.departure
             << setw(11) << t.arrival
             << setw(8)  << t.quantity
             << setw(10) << fixed << setprecision(2) << t.cost
             << "\n";
    }
    printLine('-');
}

void printPassengerTable(const vector<Passenger>& list) {
    if (list.empty()) {
        cout << "  No passengers found.\n";
        return;
    }
    printLine('-');
    cout << left
         << setw(20) << "Name"
         << setw(6)  << "Age"
         << setw(8)  << "Gender"
         << setw(14) << "Phone"
         << setw(14) << "Date"
         << setw(10) << "Train No"
         << setw(6)  << "PNR"
         << "\n";
    printLine('-');
    for (const auto& p : list) {
        cout << left
             << setw(20) << p.name
             << setw(6)  << p.age
             << setw(8)  << p.gender
             << setw(14) << p.phone
             << setw(14) << p.dateOfJourney
             << setw(10) << p.trainNumber
             << setw(6)  << p.pnr
             << "\n";
    }
    printLine('-');
}

// ══════════════════════════════════════════════════════════
//  ASCII BAR CHART (replaces matplotlib)
// ══════════════════════════════════════════════════════════

void plotPopularity() {
    printHeader("TRAIN POPULARITY  (Passenger Count)");

    // Count passengers per train
    map<int, int> counts;
    for (const auto& p : passengers) counts[p.trainNumber]++;

    if (counts.empty()) {
        cout << "\n  No booking data available yet.\n";
        return;
    }

    // Find max for scaling
    int maxCount = 0;
    for (const auto& kv : counts) maxCount = max(maxCount, kv.second);

    const int BAR_WIDTH = 40;
    cout << "\n";
    for (const auto& kv : counts) {
        // Find train name
        string name = "Train " + to_string(kv.first);
        for (const auto& t : trains)
            if (t.trainNum == kv.first) { name = t.trainName; break; }

        int barLen = (maxCount > 0) ? (kv.second * BAR_WIDTH / maxCount) : 0;
        cout << "  " << left << setw(26) << name
             << " |" << string(barLen, '#')
             << " " << kv.second << "\n";
    }
    cout << "\n  " << string(28, ' ') << "|"
         << string(BAR_WIDTH, '-') << "\n";
    cout << "  " << string(29, ' ')
         << "0" << string(BAR_WIDTH / 2 - 1, ' ')
         << maxCount / 2 << string(BAR_WIDTH / 2 - 2, ' ')
         << maxCount << "\n";
}

void plotCost() {
    printHeader("TICKET COST PER TRAIN  (Rs)");

    if (trains.empty()) {
        cout << "\n  No train data available.\n";
        return;
    }

    // Find max cost for scaling
    double maxCost = 0;
    for (const auto& t : trains) maxCost = max(maxCost, t.cost);

    const int BAR_WIDTH = 40;
    cout << "\n";
    for (const auto& t : trains) {
        int barLen = (maxCost > 0) ? (int)(t.cost * BAR_WIDTH / maxCost) : 0;
        cout << "  " << left << setw(26) << t.trainName
             << " |" << string(barLen, '*')
             << " Rs " << fixed << setprecision(0) << t.cost << "\n";
    }
    cout << "\n  " << string(28, ' ') << "|"
         << string(BAR_WIDTH, '-') << "\n";
    cout << "  " << string(29, ' ')
         << "0" << string(BAR_WIDTH / 2 - 1, ' ')
         << (int)(maxCost / 2) << string(BAR_WIDTH / 2 - 4, ' ')
         << (int)maxCost << " Rs\n";
}

// ══════════════════════════════════════════════════════════
//  MENU FUNCTIONS  (1:1 with Python functions)
// ══════════════════════════════════════════════════════════

// 1. Details of all trains
void trainDetails() {
    clearScreen();
    printHeader("ALL TRAIN DETAILS");
    cout << "\n";
    printTrainTable(trains);
}

// 2. Train names and numbers only
void trainNamesAndNumbers() {
    clearScreen();
    printHeader("TRAIN NAMES AND NUMBERS");
    cout << "\n";
    printLine('-');
    cout << left << setw(10) << "Train Num" << "Train Name\n";
    printLine('-');
    for (const auto& t : trains)
        cout << left << setw(10) << t.trainNum << t.trainName << "\n";
    printLine('-');
}

// 3. Ticket reservation
void ticketReservation() {
    clearScreen();
    printHeader("TICKET RESERVATION");

    Passenger p;
    p.name          = inputLine("\n  Enter passenger name       : ");
    p.age           = inputLine("  Enter passenger age        : ");
    p.gender        = inputLine("  Enter passenger gender     : ");
    p.phone         = inputLine("  Enter phone number         : ");
    p.dateOfJourney = inputLine("  Enter date of journey      \n  (dd/mm/yyyy)               : ");

    cout << "\n  All available trains:\n\n";
    printTrainTable(trains);

    string dest = inputLine("\n  Enter your destination      : ");

    // Filter trains by destination
    vector<Train> available;
    for (const auto& t : trains)
        if (t.destination == dest) available.push_back(t);

    if (available.empty()) {
        cout << "\n  No trains found for destination: " << dest << "\n";
        cout << "  Please check the spelling (case-sensitive).\n";
        return;
    }

    cout << "\n  Available trains to " << dest << ":\n\n";
    printTrainTable(available);

    // Let passenger choose
    while (true) {
        int chosenNum = inputInt("\n  Enter desired train number  : ");

        // Check if in available list
        Train* chosen = nullptr;
        for (auto& t : available)
            if (t.trainNum == chosenNum) { chosen = &t; break; }

        if (!chosen) {
            cout << "  Train not found in available list. Please check the number.\n";
            continue;
        }

        cout << "\n  You have chosen:\n\n";
        printTrainTable({*chosen});

        // Count existing passengers on this train
        int numPassengers = 0;
        for (const auto& pass : passengers)
            if (pass.trainNumber == chosenNum) numPassengers++;

        int qty = chosen->quantity;
        cout << "  Total seats on train : " << qty << "\n";
        cout << "  Seats already booked : " << numPassengers << "\n";

        // PNR logic (mirrors Python)
        if (numPassengers <= 0.8 * qty)
            p.pnr = "CNF";
        else if (numPassengers <= qty)
            p.pnr = "RAC";
        else
            p.pnr = "WL";

        p.trainNumber = chosenNum;

        passengers.push_back(p);
        savePassengers();

        cout << "\n  ✔  Ticket reservation successful!\n";
        cout << "  Your current PNR status : " << p.pnr << "\n";

        if (p.pnr == "WL")
            cout << "  Note: You are on the Waiting List. "
                    "A seat will be confirmed if others cancel.\n";
        else if (p.pnr == "RAC")
            cout << "  Note: RAC – you share a berth. "
                    "May be upgraded to CNF if a seat opens.\n";
        break;
    }
}

// 4. PNR Status check
void pnrStatus() {
    clearScreen();
    printHeader("PNR STATUS");

    string name = inputLine("\n  Enter passenger name   : ");
    int trainNo  = inputInt("  Enter train number     : ");

    bool found = false;
    for (const auto& p : passengers) {
        if (p.name == name && p.trainNumber == trainNo) {
            cout << "\n  PNR status for " << name
                 << " on train " << trainNo
                 << "  :  " << p.pnr << "\n";
            found = true;
        }
    }
    if (!found)
        cout << "\n  No record found. Please check your name and train number.\n";
}

// 5. Full ticket details
void ticketDetails() {
    clearScreen();
    printHeader("TICKET DETAILS");

    string name = inputLine("\n  Enter passenger name   : ");
    int trainNo  = inputInt("  Enter train number     : ");

    vector<Passenger> results;
    for (const auto& p : passengers)
        if (p.name == name && p.trainNumber == trainNo)
            results.push_back(p);

    if (results.empty())
        cout << "\n  No record found. Please check your entered values.\n";
    else {
        cout << "\n";
        printPassengerTable(results);
    }
}

// 6. Add column to passengers (display only – mirrors Python behaviour)
void addColToPassengers() {
    clearScreen();
    printHeader("ADD COLUMN TO PASSENGER DATA (Preview)");

    cout << "\n  Current Passenger DataFrame:\n\n";
    printPassengerTable(passengers);

    if (passengers.empty()) {
        cout << "  No passengers to add column for.\n";
        return;
    }

    string colName = inputLine("\n  Enter name of column to add : ");
    cout << "  Enter values one by one:\n\n";

    vector<string> values;
    for (int i = 0; i < (int)passengers.size(); i++) {
        string val = inputLine("  Row " + to_string(i + 1) +
                               " (" + passengers[i].name + ") : ");
        values.push_back(val);
    }

    // Display preview with the extra column
    printLine('-');
    cout << left
         << setw(20) << "Name"
         << setw(10) << "Train No"
         << setw(6)  << "PNR"
         << setw(20) << colName
         << "\n";
    printLine('-');
    for (int i = 0; i < (int)passengers.size(); i++) {
        cout << left
             << setw(20) << passengers[i].name
             << setw(10) << passengers[i].trainNumber
             << setw(6)  << passengers[i].pnr
             << setw(20) << values[i]
             << "\n";
    }
    printLine('-');
    cout << "\n  (Preview only – original data unchanged, as per original design)\n";
}

// 7. Add column to trains (display only)
void addColToTrains() {
    clearScreen();
    printHeader("ADD COLUMN TO TRAIN DATA (Preview)");

    cout << "\n  Current Train DataFrame:\n\n";
    printTrainTable(trains);

    if (trains.empty()) {
        cout << "  No trains to add column for.\n";
        return;
    }

    string colName = inputLine("\n  Enter name of column to add : ");
    cout << "  Enter values one by one:\n\n";

    vector<string> values;
    for (int i = 0; i < (int)trains.size(); i++) {
        string val = inputLine("  Row " + to_string(i + 1) +
                               " (" + trains[i].trainName + ") : ");
        values.push_back(val);
    }

    // Display preview with extra column
    printLine('-');
    cout << left
         << setw(8)  << "Num"
         << setw(26) << "Train Name"
         << setw(20) << colName
         << "\n";
    printLine('-');
    for (int i = 0; i < (int)trains.size(); i++) {
        cout << left
             << setw(8)  << trains[i].trainNum
             << setw(26) << trains[i].trainName
             << setw(20) << values[i]
             << "\n";
    }
    printLine('-');
    cout << "\n  (Preview only – original data unchanged, as per original design)\n";
}

// 8. Delete a column from trains (display only)
void deleteColFromTrains() {
    clearScreen();
    printHeader("DELETE COLUMN FROM TRAIN DATA (Preview)");

    cout << "\n  Available columns:\n";
    cout << "    Train Num | Train Name | Source | Destination |\n"
         << "    Departure | Arrival    | Quantity | Cost\n\n";

    printTrainTable(trains);

    string colName = inputLine("\n  Enter column name to delete : ");

    // Map column name to display
    vector<string> validCols = {
        "Train Num","Train Name","Source","Destination",
        "Departure","Arrival","Quantity","Cost"
    };

    bool found = false;
    for (const auto& c : validCols)
        if (c == colName) { found = true; break; }

    if (!found) {
        cout << "\n  Column \"" << colName
             << "\" does not exist in Train DataFrame!\n";
        return;
    }

    // Print table without that column
    cout << "\n  Preview after deleting \"" << colName << "\":\n\n";
    printLine('-');
    for (const auto& c : validCols)
        if (c != colName) cout << left << setw(14) << c;
    cout << "\n";
    printLine('-');

    for (const auto& t : trains) {
        if (colName != "Train Num")    cout << left << setw(14) << t.trainNum;
        if (colName != "Train Name")   cout << left << setw(14) << t.trainName;
        if (colName != "Source")       cout << left << setw(14) << t.source;
        if (colName != "Destination")  cout << left << setw(14) << t.destination;
        if (colName != "Departure")    cout << left << setw(14) << t.departure;
        if (colName != "Arrival")      cout << left << setw(14) << t.arrival;
        if (colName != "Quantity")     cout << left << setw(14) << t.quantity;
        if (colName != "Cost")         cout << left << setw(14) << fixed << setprecision(2) << t.cost;
        cout << "\n";
    }
    printLine('-');
    cout << "\n  (Preview only – original data unchanged, as per original design)\n";
}

// 9. Ticket cancellation
void ticketCancellation() {
    clearScreen();
    printHeader("TICKET CANCELLATION");

    string name = inputLine("\n  Enter passenger name   : ");
    int trainNo  = inputInt("  Enter train number     : ");

    auto it = passengers.begin();
    bool found = false;
    while (it != passengers.end()) {
        if (it->name == name && it->trainNumber == trainNo) {
            it    = passengers.erase(it);
            found = true;
        } else {
            ++it;
        }
    }

    if (!found) {
        cout << "\n  No matching record found. Please check your entered values.\n";
    } else {
        savePassengers();
        cout << "\n  ✔  Ticket cancelled successfully!\n\n";
        cout << "  Updated passenger list:\n\n";
        printPassengerTable(passengers);
    }
}

// 10. Popularity chart
void visualizePopularity() {
    clearScreen();
    plotPopularity();
}

// 11. Cost chart
void visualizeCost() {
    clearScreen();
    plotCost();
}

// ══════════════════════════════════════════════════════════
//  MAIN MENU
// ══════════════════════════════════════════════════════════

void printMenu() {
    printHeader("RAILWAY RESERVATION SYSTEM");
    cout << "\n";
    cout << "   1.  Details of all trains\n";
    cout << "   2.  Train names and numbers\n";
    cout << "   3.  Ticket Reservation  (Buy a ticket)\n";
    cout << "   4.  PNR Status\n";
    cout << "   5.  Ticket Details\n";
    cout << "   6.  Add a column to Passenger data\n";
    cout << "   7.  Add a column to Train data\n";
    cout << "   8.  Delete a column from Train data\n";
    cout << "   9.  Ticket Cancellation  (Delete booking)\n";
    cout << "  10.  Visualization - Popularity of trains\n";
    cout << "  11.  Visualization - Cost per train\n";
    cout << "  12.  EXIT\n\n";
    printLine('-');
}

int main() {
    // Bootstrap CSV files if they don't exist yet
    createDefaultCSVs();

    // Load data
    loadTrains();
    loadPassengers();

    cout << "  Loaded " << trains.size()
         << " trains and " << passengers.size()
         << " passenger records.\n";
    pause();

    while (true) {
        clearScreen();
        printMenu();

        int choice = inputInt("  Please enter your choice: ");

        switch (choice) {
            case 1:  trainDetails();         pause(); break;
            case 2:  trainNamesAndNumbers(); pause(); break;
            case 3:  ticketReservation();    pause(); break;
            case 4:  pnrStatus();            pause(); break;
            case 5:  ticketDetails();        pause(); break;
            case 6:  addColToPassengers();   pause(); break;
            case 7:  addColToTrains();       pause(); break;
            case 8:  deleteColFromTrains();  pause(); break;
            case 9:  ticketCancellation();   pause(); break;
            case 10: visualizePopularity();  pause(); break;
            case 11: visualizeCost();        pause(); break;
            case 12:
                clearScreen();
                printHeader("GOODBYE");
                cout << "\n  Thank you for using the Railway Reservation System.\n\n";
                return 0;
            default:
                cout << "\n  Invalid choice. Please enter a number between 1 and 12.\n";
                pause();
        }
    }
}

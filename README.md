# Railway Reservation System

A **C++17-based Railway Reservation System** that simulates essential railway booking operations through an interactive, menu-driven console application. The system enables users to explore train information, reserve tickets, retrieve booking details, check reservation status, and manage cancellations.

The application uses **CSV-based file handling for persistent data storage**, allowing train and passenger records to be maintained across multiple executions. It also incorporates a reservation-status mechanism based on train occupancy and provides console-based visualizations to represent train popularity and ticket costs.

## Key Features

### 1. Train Information Management
- Display complete train information, including train number, name, source, destination, departure time, arrival time, seat capacity, and ticket cost.
- Retrieve train names and corresponding train numbers.
- Filter available trains based on the passenger's selected destination.

### 2. Ticket Reservation
- Collect passenger details, including name, age, gender, phone number, and journey date.
- Display trains operating to the selected destination.
- Allow passengers to select their preferred train.
- Calculate reservation status based on existing bookings and available seat capacity.
- Automatically save new booking records to the passenger CSV file.

### 3. PNR Status Management
- Retrieve reservation status using the passenger's name and train number.
- Support three reservation categories:
  - **CNF (Confirmed):** Booking occupancy is up to 80% of train capacity.
  - **RAC (Reservation Against Cancellation):** Occupancy is above 80% and up to 100%.
  - **WL (Waiting List):** Existing bookings exceed the available train capacity.

### 4. Ticket Management
- Retrieve complete ticket information using passenger details and train number.
- Cancel existing reservations.
- Update passenger records after successful cancellation.
- Persist changes in the CSV database.

### 5. Data Management
- Add custom columns to passenger and train datasets through an interactive preview.
- Preview train records after selecting a column for deletion.
- Preserve original datasets during preview operations.
- Automatically create the required CSV files when they are unavailable.

### 6. Data Visualization
- Generate a console-based bar chart representing train popularity using passenger booking counts.
- Visualize ticket costs across different trains using scaled bar charts.

## Technical Implementation

| Component | Implementation |
|---|---|
| Programming Language | C++ |
| Language Standard | C++17 |
| Data Structures | Structures, Vectors, Maps |
| File Handling | CSV reading and writing |
| Standard Library | STL |
| Input Handling | Console-based interactive input |
| Data Persistence | CSV files |
| Visualization | Console-based bar charts |

### Core C++ Concepts Demonstrated
- User-defined structures (`struct`)
- Functions and modular programming
- Vectors for dynamic data storage
- Maps for frequency counting
- File streams (`ifstream`, `ofstream`)
- String manipulation and CSV parsing
- Iterators and STL algorithms
- Conditional statements and loops
- Input validation and exception handling

## Data Storage

The application uses two CSV files to maintain its records:

**Train_details.csv**
- Stores train numbers, names, source, destination, departure and arrival times, seat capacity, and ticket costs.
- Automatically initialized with sample train records if the file does not exist.

**Passenger_details.csv**
- Stores passenger information, journey details, train numbers, and reservation status.
- Updated whenever a ticket is booked or cancelled.
- Automatically created if unavailable.

Passenger booking records are loaded when the application starts and saved back to the CSV file whenever booking-related changes occur.

## Project Structure

```text
Railway-Reservation-System/
│
├── railway.cpp
├── Train_details.csv
├── .gitignore
└── README.md
```

`Passenger_details.csv` is generated automatically during execution and excluded from version control.

## Installation and Execution

### Prerequisites
- C++ compiler supporting C++17
- G++ compiler
- macOS, Linux, or another compatible command-line environment

The application initializes the required CSV files automatically if they do not already exist.

## Limitations

- Reservation status is calculated using overall train occupancy rather than date-specific availability.
- Passenger records are identified using passenger name and train number rather than a unique booking identifier.
- Custom column addition and deletion operations are preview-based and do not modify the original datasets.
- The application uses a console interface rather than a graphical user interface.
- The reservation logic is a simplified simulation and does not implement a complete railway booking or payment infrastructure.

## Future Enhancements

- Introduce unique PNR generation for individual bookings.
- Implement date-wise seat availability and reservation tracking.
- Add automatic waiting-list and RAC status updates following cancellations.
- Implement more advanced passenger search and record management.
- Introduce a graphical user interface.
- Integrate a database management system for more scalable data storage.



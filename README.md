# Kigali Smart Parking Management System

An in-memory C++ parking management solution for Kigali City. The system uses appropriate data structures and OOP design to manage slot configuration, vehicle entry/exit, fee calculation, and operational reports without a database.

## Default Parking Rates

| Vehicle Type | Hourly Rate |
|--------------|-------------|
| Motorcycle   | 500 RWF     |
| Car          | 1,000 RWF   |
| Truck        | 2,000 RWF   |

Fees are calculated only at vehicle exit. Partial hours are billed as full hours (e.g. 15 minutes = 1 hour, 1 hour 20 minutes = 2 hours).

## Requirements

- C++17 compiler (`g++` or `clang++`)

## Compile and Run

### Windows (MinGW / g++)

```bash
g++ -std=c++17 -o parking_system main.cpp ParkingSystem.cpp
parking_system.exe
```

### Linux / macOS

```bash
g++ -std=c++17 -o parking_system main.cpp ParkingSystem.cpp
./parking_system
```

## Menu Options

| Option | Description |
|--------|-------------|
| 1 | Configure a new parking slot (unique ID, vehicle type, zone) |
| 2 | View all configured parking slots |
| 3 | Register vehicle entry and auto-allocate a matching slot |
| 4 | Register vehicle exit, release slot, calculate and display fee |
| 5 | Update hourly parking rate for a vehicle type |
| 6 | View currently available slots |
| 7 | View all currently parked vehicles |
| 8 | View completed parking transaction history |
| 9 | View daily revenue (by date or all records) |
| 10 | View current active parking rates |
| 11 | Load sample test slots |
| 0 | Exit the program |

## Data Structure Choices

| Structure | Purpose | Justification |
|-----------|---------|---------------|
| `unordered_map<string, ParkingSlot>` | Slot storage | O(1) average lookup, insert, and update by unique Slot ID |
| `unordered_map<string, ParkedVehicle>` | Active vehicles | O(1) duplicate-entry detection by plate number |
| `vector<ParkingTransaction>` | Completed history | Efficient append on exit and sequential traversal for reports |
| `unordered_map<VehicleType, int>` | Active rates | O(1) rate lookup at exit; updates do not alter stored history |

## OOP Design

- **Encapsulation**: `ParkingSlot`, `ParkedVehicle`, and `ParkingTransaction` hide internal state behind getters.
- **Abstraction**: `Displayable` interface defines a common `display()` contract for reports.
- **Inheritance**: `ParkingSlot`, `ParkedVehicle`, and `ParkingTransaction` implement `Displayable`.
- **Polymorphism**: `FeeCalculator` / `StandardFeeCalculator` allow fee logic to be extended without changing exit handling.

## Suggested Test Flow

See `test_input.txt` for step-by-step manual test scenarios covering all required features.

## Project Files

```
main.cpp              - Menu-driven console interface
ParkingSystem.h/cpp   - Core system logic and data structures
ParkingSlot.h         - Parking slot model
ParkedVehicle.h       - Active parked vehicle model
ParkingTransaction.h  - Completed transaction record
FeeCalculator.h       - Fee calculation strategy
Displayable.h         - Abstract display interface
enums.h               - Vehicle type and slot status helpers
test_input.txt        - Manual test scenarios
docs/                 - System flow docs, file reference, and Mermaid diagrams
```

## Documentation

Full technical documentation is in the [`docs/`](docs/) folder:

- [System Flow](docs/system-flow.md) — process flows and task mapping
- [File Reference](docs/file-reference.md) — purpose of every project file
- [Diagrams](docs/diagrams/) — architecture, class, entry/exit, and data structure diagrams

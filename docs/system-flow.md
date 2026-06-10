# System Flow Documentation

This document describes how the Kigali Smart Parking Management System works from startup to exit, including how each assessment task maps to the implementation.

---

## 1. High-Level System Flow

The system follows a three-layer architecture:

1. **Presentation Layer** (`main.cpp`) — menu-driven console interface
2. **Business Logic Layer** (`ParkingSystem`) — rules, validation, and orchestration
3. **Data Layer** (in-memory maps and vectors) — slots, active vehicles, rates, and history

```mermaid
flowchart TB
    subgraph Presentation["Presentation Layer"]
        UI[main.cpp<br/>Menu & Input Handlers]
    end

    subgraph Business["Business Logic Layer"]
        PS[ParkingSystem]
        FC[FeeCalculator]
    end

    subgraph Data["In-Memory Data Layer"]
        SM[(slots<br/>unordered_map)]
        AV[(activeVehicles<br/>unordered_map)]
        TH[(transactionHistory<br/>vector)]
        HR[(hourlyRates<br/>unordered_map)]
    end

    UI -->|user actions| PS
    PS --> FC
    PS --> SM
    PS --> AV
    PS --> TH
    PS --> HR
    PS -->|reports & messages| UI
```

---

## 2. Application Startup Flow

```mermaid
flowchart TD
    A[Program Starts] --> B[Create ParkingSystem object]
    B --> C[Initialize default hourly rates<br/>Motorcycle: 500 | Car: 1000 | Truck: 2000]
    C --> D[Create StandardFeeCalculator]
    D --> E[Initialize empty data stores]
    E --> F[Display welcome message]
    F --> G[Show main menu]
    G --> H{User selects option}
    H -->|0| I[Exit program]
    H -->|1-11| J[Execute selected operation]
    J --> G
```

On startup, no slots or vehicles exist until the operator configures slots manually (option 1) or loads sample data (option 11).

---

## 3. Menu Navigation Flow

```mermaid
flowchart LR
    MENU[Main Menu]

    MENU --> M1[1. Add Slot]
    MENU --> M2[2. View All Slots]
    MENU --> M3[3. Vehicle Entry]
    MENU --> M4[4. Vehicle Exit]
    MENU --> M5[5. Update Rates]
    MENU --> M6[6. Available Slots]
    MENU --> M7[7. Parked Vehicles]
    MENU --> M8[8. History]
    MENU --> M9[9. Daily Revenue]
    MENU --> M10[10. Current Rates]
    MENU --> M11[11. Sample Data]
    MENU --> M0[0. Exit]

    M1 --> PS1[ParkingSystem::addSlot]
    M3 --> PS3[ParkingSystem::registerEntry]
    M4 --> PS4[ParkingSystem::registerExit]
    M5 --> PS5[ParkingSystem::updateHourlyRate]
    M2 --> PS2[ParkingSystem::displayAllSlots]
    M6 --> PS6[ParkingSystem::displayAvailableSlots]
    M7 --> PS7[ParkingSystem::displayParkedVehicles]
    M8 --> PS8[ParkingSystem::displayTransactionHistory]
    M9 --> PS9[ParkingSystem::displayDailyRevenue]
    M10 --> PS10[ParkingSystem::displayCurrentRates]
    M11 --> PS11[ParkingSystem::loadSampleData]
```

The menu never calls model classes directly. All operations go through `ParkingSystem`, which keeps business rules centralized.

---

## 4. Task 1 — Parking Slot Configuration Flow

**Goal:** Add uniquely identified slots with vehicle type, zone, and status.

```mermaid
flowchart TD
    A[User selects Option 1] --> B[Enter Slot ID, Vehicle Type, Zone]
    B --> C{Slot ID empty?}
    C -->|Yes| D[Show error]
    C -->|No| E{Zone empty?}
    E -->|Yes| D
    E -->|No| F{Slot ID already exists<br/>in slots map?}
    F -->|Yes| G[Show duplicate error]
    F -->|No| H[Create ParkingSlot object<br/>Status = Available]
    H --> I[Insert into slots unordered_map<br/>Key = Slot ID]
    I --> J[Confirm success]
```

**Data structure:** `unordered_map<string, ParkingSlot> slots`

| Operation | Complexity | How |
|-----------|------------|-----|
| Insert slot | O(1) avg | `slots.emplace(slotId, slot)` |
| Lookup slot | O(1) avg | `slots.find(slotId)` |
| Traverse all | O(n) | iterate map for reports |

---

## 5. Task 2 — Vehicle Entry Flow

**Goal:** Register a vehicle, prevent duplicate parking, and allocate a matching available slot.

```mermaid
flowchart TD
    A[User selects Option 3] --> B[Enter plate number & vehicle type]
    B --> C{Plate empty?}
    C -->|Yes| ERR[Show error]
    C -->|No| D{Plate exists in<br/>activeVehicles?}
    D -->|Yes| E[Error: already parked]
    D -->|No| F[findAvailableSlot vehicleType]
    F --> G{Matching available slot found?}
    G -->|No| H[Error: no suitable slot]
    G -->|Yes| I[Mark slot as Occupied]
    I --> J[Record entry time = now]
    J --> K[Create ParkedVehicle]
    K --> L[Insert into activeVehicles<br/>Key = plate number]
    L --> M[Display slot & entry time]
```

**Slot search logic (`findAvailableSlot`):**

```mermaid
flowchart LR
    A[Iterate slots map] --> B{Slot available?}
    B -->|No| A
    B -->|Yes| C{Slot type matches<br/>vehicle type?}
    C -->|No| A
    C -->|Yes| D[Return first matching Slot ID]
    A -->|end of map| E[Return empty string]
```

**Data structure:** `unordered_map<string, ParkedVehicle> activeVehicles`

Duplicate detection is O(1) because the plate number is the map key.

---

## 6. Task 3 — Fee Calculation and Rate Update Flow

### 6.1 Fee Calculation (at exit only)

Fees are **never** calculated at entry. They are computed when the vehicle exits.

```mermaid
flowchart TD
    A[Vehicle Exit triggered] --> B[Get entry time from ParkedVehicle]
    B --> C[Get exit time = now]
    C --> D[durationMinutes = exit - entry]
    D --> E[billedHours = ceil durationMinutes / 60<br/>Minimum 1 hour if duration <= 0]
    E --> F[Get current rate from hourlyRates map]
    F --> G[totalFee = billedHours × hourlyRate]
    G --> H[Store hourlyRateApplied in transaction]
```

**Billing examples:**

| Actual Duration | Billed Hours | Car Fee (1000 RWF/hr) |
|-----------------|--------------|------------------------|
| 0 min           | 1            | 1,000 RWF              |
| 15 min          | 1            | 1,000 RWF              |
| 60 min          | 1            | 1,000 RWF              |
| 80 min (1h 20m) | 2            | 2,000 RWF              |

### 6.2 Rate Update Flow

```mermaid
flowchart TD
    A[User selects Option 5] --> B[Choose vehicle type & new rate]
    B --> C{Rate > 0?}
    C -->|No| D[Show error]
    C -->|Yes| E[Update hourlyRates map]
    E --> F[Future exits use new rate]
    F --> G[Past transactions unchanged<br/>hourlyRateApplied is stored per record]
```

**Why history is unaffected:** Each `ParkingTransaction` stores its own `hourlyRateApplied` and `totalFee` at exit time. Updating `hourlyRates` only affects future exits.

---

## 7. Task 4 — Vehicle Exit Flow

**Goal:** Release slot, calculate fee, update records, and archive the transaction.

```mermaid
flowchart TD
    A[User selects Option 4] --> B[Enter plate number]
    B --> C{Plate empty?}
    C -->|Yes| ERR[Show error]
    C -->|No| D{Plate in activeVehicles?}
    D -->|No| E[Error: not parked]
    D -->|Yes| F[Read ParkedVehicle record]
    F --> G[Calculate duration & fee via FeeCalculator]
    G --> H[Set allocated slot status = Available]
    H --> I[Build ParkingTransaction with<br/>rate, fee, times, duration]
    I --> J[Append to transactionHistory vector]
    J --> K[Remove from activeVehicles]
    K --> L[Display exit summary & total fee]
```

**Record lifecycle:**

```mermaid
stateDiagram-v2
    [*] --> Available: Slot configured
    Available --> Occupied: Vehicle entry
    Occupied --> Available: Vehicle exit

    [*] --> Active: Entry registered
    Active --> Archived: Exit completed
    Archived --> [*]: Stored in transactionHistory
```

---

## 8. Reporting Flows

### 8.1 Available Slots (Option 6)

Traverse `slots` map → filter where `status == Available` → call `display()` on each.

### 8.2 Parked Vehicles (Option 7)

Traverse `activeVehicles` map → display each `ParkedVehicle`.

### 8.3 Vehicle History (Option 8)

Sequential traversal of `transactionHistory` vector → display each completed `ParkingTransaction`.

### 8.4 Daily Revenue (Option 9)

```mermaid
flowchart TD
    A[User enters date or blank] --> B[Traverse transactionHistory]
    B --> C{Date filter empty OR<br/>exit date matches?}
    C -->|Yes| D[Add totalFee to running total]
    C -->|No| B
    D --> B
    B -->|done| E[Display transactions + total revenue]
```

---

## 9. End-to-End Parking Session

A complete parking session from arrival to payment:

```mermaid
sequenceDiagram
    actor Operator
    participant UI as main.cpp
    participant PS as ParkingSystem
    participant Slots as slots map
    participant Active as activeVehicles map
    participant FC as FeeCalculator
    participant History as transactionHistory

    Operator->>UI: Option 3 - Register Entry
    UI->>PS: registerEntry(plate, type)
    PS->>Active: Check duplicate (O(1))
    PS->>Slots: findAvailableSlot(type)
    PS->>Slots: Set status = Occupied
    PS->>Active: Insert ParkedVehicle
    PS-->>UI: Entry confirmation
    UI-->>Operator: Show slot & time

    Note over Operator,History: Vehicle remains parked...

    Operator->>UI: Option 4 - Register Exit
    UI->>PS: registerExit(plate)
    PS->>Active: Lookup vehicle (O(1))
    PS->>FC: calculateBilledHours & calculateFee
    PS->>Slots: Set status = Available
    PS->>History: push_back(transaction)
    PS->>Active: erase(plate)
    PS-->>UI: Exit summary + fee
    UI-->>Operator: Display payment details
```

---

## 10. Error Handling Summary

| Scenario | Handling |
|----------|----------|
| Empty slot ID / zone / plate | Validation error, operation cancelled |
| Duplicate slot ID | Rejected before insert |
| Vehicle already parked | Rejected via `activeVehicles` lookup |
| No matching available slot | Graceful message, no state change |
| Exit unknown plate | Error, no records modified |
| Invalid vehicle type input | Re-prompt in `readVehicleType()` |
| Non-numeric menu choice | Re-prompt in `readInt()` |
| Rate update ≤ 0 | Rejected with error message |

---

## 11. Task-to-Component Mapping

| Assessment Task | Primary Files | Key Methods |
|-----------------|---------------|-------------|
| Task 1: Slot Configuration | `ParkingSlot.h`, `ParkingSystem.cpp` | `addSlot()`, `displayAllSlots()` |
| Task 2: Vehicle Entry | `ParkedVehicle.h`, `ParkingSystem.cpp` | `registerEntry()`, `findAvailableSlot()` |
| Task 3: Fee Calculation | `FeeCalculator.h`, `ParkingSystem.cpp` | `registerExit()`, `updateHourlyRate()` |
| Task 4: Vehicle Exit | `ParkingTransaction.h`, `ParkingSystem.cpp` | `registerExit()` |
| Reports | `ParkingSystem.cpp`, `Displayable.h` | `displayAvailableSlots()`, `displayParkedVehicles()`, `displayTransactionHistory()`, `displayDailyRevenue()` |
| User Interface | `main.cpp` | `printMenu()`, handler functions |
| Shared Types | `enums.h` | `VehicleType`, `SlotStatus`, parsers |

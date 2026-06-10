#include "ParkingSystem.h"

#include <algorithm>
#include <chrono>
#include <iostream>

ParkingSystem::ParkingSystem() : feeCalculator(std::make_unique<StandardFeeCalculator>()) {
    hourlyRates[VehicleType::Motorcycle] = 500;
    hourlyRates[VehicleType::Car] = 1000;
    hourlyRates[VehicleType::Truck] = 2000;
}

bool ParkingSystem::addSlot(const std::string& slotId, VehicleType type,
                            const std::string& zone) {
    if (slotId.empty()) {
        std::cout << "Error: Slot ID cannot be empty.\n";
        return false;
    }
    if (zone.empty()) {
        std::cout << "Error: Zone cannot be empty.\n";
        return false;
    }
    if (slots.find(slotId) != slots.end()) {
        std::cout << "Error: Slot ID '" << slotId << "' already exists.\n";
        return false;
    }

    slots.emplace(slotId, ParkingSlot(slotId, type, zone, SlotStatus::Available));
    std::cout << "Parking slot '" << slotId << "' configured successfully.\n";
    return true;
}

std::string ParkingSystem::findAvailableSlot(VehicleType vehicleType) const {
    for (const auto& pair : slots) {
        const ParkingSlot& slot = pair.second;
        if (slot.isAvailable() && slot.supports(vehicleType)) {
            return slot.getSlotId();
        }
    }
    return "";
}

bool ParkingSystem::registerEntry(const std::string& plateNumber, VehicleType vehicleType) {
    if (plateNumber.empty()) {
        std::cout << "Error: Plate number cannot be empty.\n";
        return false;
    }
    if (activeVehicles.find(plateNumber) != activeVehicles.end()) {
        std::cout << "Error: Vehicle '" << plateNumber
                  << "' is already parked. A vehicle cannot be parked twice.\n";
        return false;
    }

    const std::string allocatedSlotId = findAvailableSlot(vehicleType);
    if (allocatedSlotId.empty()) {
        std::cout << "Error: No available parking slot for "
                  << vehicleTypeToString(vehicleType) << ".\n";
        return false;
    }

    auto slotIt = slots.find(allocatedSlotId);
    if (slotIt == slots.end()) {
        std::cout << "Error: Internal slot lookup failed.\n";
        return false;
    }

    const TimePoint entryTime = std::chrono::system_clock::now();
    slotIt->second.setStatus(SlotStatus::Occupied);
    activeVehicles.emplace(plateNumber,
                             ParkedVehicle(plateNumber, vehicleType, entryTime, allocatedSlotId));

    std::cout << "Vehicle '" << plateNumber << "' entered successfully.\n";
    std::cout << "Allocated Slot: " << allocatedSlotId << '\n';
    std::cout << "Entry Time: " << ParkedVehicle::formatTime(entryTime) << '\n';
    return true;
}

long long ParkingSystem::minutesBetween(TimePoint start, TimePoint end) const {
    return std::chrono::duration_cast<std::chrono::minutes>(end - start).count();
}

bool ParkingSystem::registerExit(const std::string& plateNumber, int& outFee,
                                 ParkingTransaction& outTransaction) {
    if (plateNumber.empty()) {
        std::cout << "Error: Plate number cannot be empty.\n";
        return false;
    }

    auto vehicleIt = activeVehicles.find(plateNumber);
    if (vehicleIt == activeVehicles.end()) {
        std::cout << "Error: Vehicle '" << plateNumber << "' is not currently parked.\n";
        return false;
    }

    const ParkedVehicle& parkedVehicle = vehicleIt->second;
    const TimePoint exitTime = std::chrono::system_clock::now();
    const long long durationMinutes =
        minutesBetween(parkedVehicle.getEntryTime(), exitTime);

    const int hourlyRate = getHourlyRate(parkedVehicle.getVehicleType());
    const int billedHours = feeCalculator->calculateBilledHours(durationMinutes);
    const int totalFee = feeCalculator->calculateFee(durationMinutes, hourlyRate);

    auto slotIt = slots.find(parkedVehicle.getAllocatedSlotId());
    if (slotIt != slots.end()) {
        slotIt->second.setStatus(SlotStatus::Available);
    }

    outTransaction = ParkingTransaction(
        parkedVehicle.getPlateNumber(),
        parkedVehicle.getVehicleType(),
        parkedVehicle.getAllocatedSlotId(),
        parkedVehicle.getEntryTime(),
        exitTime,
        durationMinutes,
        billedHours,
        hourlyRate,
        totalFee);

    transactionHistory.push_back(outTransaction);
    activeVehicles.erase(vehicleIt);
    outFee = totalFee;

    std::cout << "\n--- Vehicle Exit Summary ---\n";
    std::cout << "Plate Number : " << outTransaction.getPlateNumber() << '\n';
    std::cout << "Vehicle Type : " << vehicleTypeToString(outTransaction.getVehicleType()) << '\n';
    std::cout << "Slot Released: " << outTransaction.getSlotId() << '\n';
    std::cout << "Entry Time   : " << ParkedVehicle::formatTime(outTransaction.getEntryTime()) << '\n';
    std::cout << "Exit Time    : " << ParkedVehicle::formatTime(outTransaction.getExitTime()) << '\n';
    std::cout << "Duration     : " << durationMinutes << " minute(s)\n";
    std::cout << "Billed Hours : " << billedHours << " hour(s)\n";
    std::cout << "Rate Applied : " << hourlyRate << " RWF/hour\n";
    std::cout << "Total Fee    : " << totalFee << " RWF\n";
    std::cout << "----------------------------\n";
    return true;
}

bool ParkingSystem::updateHourlyRate(VehicleType type, int newRate) {
    if (newRate <= 0) {
        std::cout << "Error: Hourly rate must be a positive value.\n";
        return false;
    }

    const int oldRate = hourlyRates[type];
    hourlyRates[type] = newRate;
    std::cout << vehicleTypeToString(type) << " rate updated from "
              << oldRate << " to " << newRate << " RWF/hour.\n";
    std::cout << "Note: Completed transactions keep their original applied rates.\n";
    return true;
}

void ParkingSystem::displayAllSlots() const {
    if (slots.empty()) {
        std::cout << "No parking slots configured.\n";
        return;
    }

    std::cout << "\n--- All Parking Slots ---\n";
    for (const auto& pair : slots) {
        pair.second.display();
    }
}

void ParkingSystem::displayAvailableSlots() const {
    std::cout << "\n--- Available Parking Slots ---\n";
    bool found = false;
    for (const auto& pair : slots) {
        if (pair.second.isAvailable()) {
            pair.second.display();
            found = true;
        }
    }
    if (!found) {
        std::cout << "No available slots at the moment.\n";
    }
}

void ParkingSystem::displayParkedVehicles() const {
    if (activeVehicles.empty()) {
        std::cout << "No vehicles currently parked.\n";
        return;
    }

    std::cout << "\n--- Currently Parked Vehicles ---\n";
    for (const auto& pair : activeVehicles) {
        pair.second.display();
    }
}

void ParkingSystem::displayTransactionHistory() const {
    if (transactionHistory.empty()) {
        std::cout << "No completed parking transactions yet.\n";
        return;
    }

    std::cout << "\n--- Parking Transaction History ---\n";
    for (const auto& transaction : transactionHistory) {
        transaction.display();
    }
}

void ParkingSystem::displayDailyRevenue(const std::string& date) const {
    int totalRevenue = 0;
    int transactionCount = 0;

    std::cout << "\n--- Daily Revenue Report";
    if (!date.empty()) {
        std::cout << " (" << date << ")";
    }
    std::cout << " ---\n";

    for (const auto& transaction : transactionHistory) {
        if (date.empty() || transaction.getExitDate() == date) {
            transaction.display();
            totalRevenue += transaction.getTotalFee();
            ++transactionCount;
        }
    }

    if (transactionCount == 0) {
        std::cout << "No transactions found";
        if (!date.empty()) {
            std::cout << " for date " << date;
        }
        std::cout << ".\n";
        return;
    }

    std::cout << "Transactions: " << transactionCount << '\n';
    std::cout << "Total Revenue: " << totalRevenue << " RWF\n";
}

void ParkingSystem::displayCurrentRates() const {
    std::cout << "\n--- Current Active Parking Rates ---\n";
    std::cout << "Motorcycle: " << getHourlyRate(VehicleType::Motorcycle) << " RWF/hour\n";
    std::cout << "Car       : " << getHourlyRate(VehicleType::Car) << " RWF/hour\n";
    std::cout << "Truck     : " << getHourlyRate(VehicleType::Truck) << " RWF/hour\n";
}

void ParkingSystem::loadSampleData() {
    slots.clear();
    activeVehicles.clear();
    transactionHistory.clear();

    addSlot("M-01", VehicleType::Motorcycle, "Zone A");
    addSlot("M-02", VehicleType::Motorcycle, "Zone A");
    addSlot("C-01", VehicleType::Car, "Zone B");
    addSlot("C-02", VehicleType::Car, "Zone B");
    addSlot("C-03", VehicleType::Car, "Zone C");
    addSlot("T-01", VehicleType::Truck, "Zone D");
    addSlot("T-02", VehicleType::Truck, "Zone D");

    std::cout << "Sample parking slots loaded successfully.\n";
}

int ParkingSystem::getHourlyRate(VehicleType type) const {
    const auto it = hourlyRates.find(type);
    if (it != hourlyRates.end()) {
        return it->second;
    }
    return 0;
}

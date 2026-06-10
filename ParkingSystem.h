#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "FeeCalculator.h"
#include "ParkedVehicle.h"
#include "ParkingSlot.h"
#include "ParkingTransaction.h"
#include "enums.h"

class ParkingSystem {
private:
    // Slot ID -> ParkingSlot (O(1) lookup, update, delete)
    std::unordered_map<std::string, ParkingSlot> slots;

    // Plate number -> active parked vehicle (O(1) duplicate detection)
    std::unordered_map<std::string, ParkedVehicle> activeVehicles;

    // Completed parking records (append-only history traversal)
    std::vector<ParkingTransaction> transactionHistory;

    // Current active hourly rates by vehicle type
    std::unordered_map<VehicleType, int> hourlyRates;

    std::unique_ptr<FeeCalculator> feeCalculator;

    std::string findAvailableSlot(VehicleType vehicleType) const;
    long long minutesBetween(TimePoint start, TimePoint end) const;

public:
    ParkingSystem();

    bool addSlot(const std::string& slotId, VehicleType type, const std::string& zone);
    bool registerEntry(const std::string& plateNumber, VehicleType vehicleType);
    bool registerExit(const std::string& plateNumber, int& outFee, ParkingTransaction& outTransaction);
    bool updateHourlyRate(VehicleType type, int newRate);

    void displayAllSlots() const;
    void displayAvailableSlots() const;
    void displayParkedVehicles() const;
    void displayTransactionHistory() const;
    void displayDailyRevenue(const std::string& date) const;
    void displayCurrentRates() const;

    void loadSampleData();

    int getHourlyRate(VehicleType type) const;
    size_t getSlotCount() const { return slots.size(); }
    size_t getActiveVehicleCount() const { return activeVehicles.size(); }
    size_t getTransactionCount() const { return transactionHistory.size(); }
};

#endif

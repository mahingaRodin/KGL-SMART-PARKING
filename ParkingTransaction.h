#ifndef PARKING_TRANSACTION_H
#define PARKING_TRANSACTION_H

#include <chrono>
#include <iostream>
#include <string>

#include "Displayable.h"
#include "ParkedVehicle.h"
#include "enums.h"

class ParkingTransaction : public Displayable {
private:
    std::string plateNumber;
    VehicleType vehicleType;
    std::string slotId;
    TimePoint entryTime;
    TimePoint exitTime;
    long long durationMinutes;
    int billedHours;
    int hourlyRateApplied;
    int totalFee;

public:
    ParkingTransaction() = default;

    ParkingTransaction(const std::string& plate, VehicleType type, const std::string& slot,
                       TimePoint entry, TimePoint exit, long long minutes, int hours,
                       int rate, int fee)
        : plateNumber(plate), vehicleType(type), slotId(slot), entryTime(entry),
          exitTime(exit), durationMinutes(minutes), billedHours(hours),
          hourlyRateApplied(rate), totalFee(fee) {}

    const std::string& getPlateNumber() const { return plateNumber; }
    VehicleType getVehicleType() const { return vehicleType; }
    const std::string& getSlotId() const { return slotId; }
    TimePoint getEntryTime() const { return entryTime; }
    TimePoint getExitTime() const { return exitTime; }
    long long getDurationMinutes() const { return durationMinutes; }
    int getBilledHours() const { return billedHours; }
    int getHourlyRateApplied() const { return hourlyRateApplied; }
    int getTotalFee() const { return totalFee; }

    std::string getExitDate() const {
        return ParkedVehicle::formatTime(exitTime).substr(0, 10);
    }

    void display() const override {
        std::cout << "Plate: " << plateNumber
                  << " | Type: " << vehicleTypeToString(vehicleType)
                  << " | Slot: " << slotId
                  << " | Entry: " << ParkedVehicle::formatTime(entryTime)
                  << " | Exit: " << ParkedVehicle::formatTime(exitTime)
                  << " | Duration: " << durationMinutes << " min"
                  << " | Billed: " << billedHours << " hr(s)"
                  << " | Rate: " << hourlyRateApplied << " RWF/hr"
                  << " | Fee: " << totalFee << " RWF\n";
    }
};

#endif

#ifndef PARKED_VEHICLE_H
#define PARKED_VEHICLE_H

#include <chrono>
#include <iostream>
#include <string>

#include "Displayable.h"
#include "enums.h"

using TimePoint = std::chrono::system_clock::time_point;

class ParkedVehicle : public Displayable {
private:
    std::string plateNumber;
    VehicleType vehicleType;
    TimePoint entryTime;
    std::string allocatedSlotId;

public:
    ParkedVehicle() = default;

    ParkedVehicle(const std::string& plate, VehicleType type, TimePoint entry,
                  const std::string& slotId)
        : plateNumber(plate), vehicleType(type), entryTime(entry), allocatedSlotId(slotId) {}

    const std::string& getPlateNumber() const { return plateNumber; }
    VehicleType getVehicleType() const { return vehicleType; }
    TimePoint getEntryTime() const { return entryTime; }
    const std::string& getAllocatedSlotId() const { return allocatedSlotId; }

    void display() const override {
        std::cout << "Plate: " << plateNumber
                  << " | Type: " << vehicleTypeToString(vehicleType)
                  << " | Slot: " << allocatedSlotId
                  << " | Entry: " << formatTime(entryTime) << '\n';
    }

    static std::string formatTime(TimePoint timePoint) {
        const std::time_t rawTime = std::chrono::system_clock::to_time_t(timePoint);
        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &rawTime);
#else
        localtime_r(&rawTime, &localTime);
#endif
        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
        return buffer;
    }
};

#endif

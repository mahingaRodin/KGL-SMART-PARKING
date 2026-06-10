#ifndef ENUMS_H
#define ENUMS_H

#include <string>

enum class VehicleType { Motorcycle, Car, Truck };
enum class SlotStatus { Available, Occupied };

inline std::string vehicleTypeToString(VehicleType type) {
    switch (type) {
        case VehicleType::Motorcycle: return "Motorcycle";
        case VehicleType::Car:        return "Car";
        case VehicleType::Truck:      return "Truck";
        default:                      return "Unknown";
    }
}

inline std::string slotStatusToString(SlotStatus status) {
    switch (status) {
        case SlotStatus::Available: return "Available";
        case SlotStatus::Occupied:  return "Occupied";
        default:                      return "Unknown";
    }
}

inline bool parseVehicleType(const std::string& input, VehicleType& out) {
    if (input == "1" || input == "Motorcycle" || input == "motorcycle") {
        out = VehicleType::Motorcycle;
        return true;
    }
    if (input == "2" || input == "Car" || input == "car") {
        out = VehicleType::Car;
        return true;
    }
    if (input == "3" || input == "Truck" || input == "truck") {
        out = VehicleType::Truck;
        return true;
    }
    return false;
}

#endif

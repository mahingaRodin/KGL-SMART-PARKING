#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <iostream>
#include <string>

#include "Displayable.h"
#include "enums.h"

class ParkingSlot : public Displayable {
private:
    std::string slotId;
    VehicleType supportedType;
    std::string zone;
    SlotStatus status;

public:
    ParkingSlot() = default;

    ParkingSlot(const std::string& id, VehicleType type, const std::string& zoneName,
                SlotStatus slotStatus = SlotStatus::Available)
        : slotId(id), supportedType(type), zone(zoneName), status(slotStatus) {}

    const std::string& getSlotId() const { return slotId; }
    VehicleType getSupportedType() const { return supportedType; }
    const std::string& getZone() const { return zone; }
    SlotStatus getStatus() const { return status; }

    void setStatus(SlotStatus newStatus) { status = newStatus; }

    bool isAvailable() const { return status == SlotStatus::Available; }

    bool supports(VehicleType vehicleType) const { return supportedType == vehicleType; }

    void display() const override {
        std::cout << "Slot ID: " << slotId
                  << " | Type: " << vehicleTypeToString(supportedType)
                  << " | Zone: " << zone
                  << " | Status: " << slotStatusToString(status) << '\n';
    }
};

#endif

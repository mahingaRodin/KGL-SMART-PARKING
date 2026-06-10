#include <iostream>
#include <limits>
#include <string>

#include "ParkingSystem.h"
#include "enums.h"

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readInt(const std::string& prompt) {
    int value = 0;
    std::cout << prompt;
    while (!(std::cin >> value)) {
        std::cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return value;
}

std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

VehicleType readVehicleType() {
    while (true) {
        std::cout << "Select vehicle type:\n";
        std::cout << "  1. Motorcycle\n";
        std::cout << "  2. Car\n";
        std::cout << "  3. Truck\n";
        const std::string choice = readLine("Enter choice (1-3 or name): ");

        VehicleType type{};
        if (parseVehicleType(choice, type)) {
            return type;
        }
        std::cout << "Invalid vehicle type. Try again.\n";
    }
}

void printMenu() {
    std::cout << "\n========================================\n";
    std::cout << "   KIGALI SMART PARKING MANAGEMENT\n";
    std::cout << "========================================\n";
    std::cout << " 1. Configure Parking Slot (Add)\n";
    std::cout << " 2. View All Parking Slots\n";
    std::cout << " 3. Register Vehicle Entry\n";
    std::cout << " 4. Register Vehicle Exit\n";
    std::cout << " 5. Update Parking Rates\n";
    std::cout << " 6. View Available Slots\n";
    std::cout << " 7. View Parked Vehicles\n";
    std::cout << " 8. View Vehicle History\n";
    std::cout << " 9. View Daily Revenue\n";
    std::cout << "10. View Current Rates\n";
    std::cout << "11. Load Sample Test Data\n";
    std::cout << " 0. Exit\n";
    std::cout << "========================================\n";
}

void handleAddSlot(ParkingSystem& system) {
    const std::string slotId = readLine("Enter unique Slot ID: ");
    const VehicleType type = readVehicleType();
    const std::string zone = readLine("Enter zone/location: ");
    system.addSlot(slotId, type, zone);
}

void handleVehicleEntry(ParkingSystem& system) {
    const std::string plate = readLine("Enter vehicle plate number: ");
    const VehicleType type = readVehicleType();
    system.registerEntry(plate, type);
}

void handleVehicleExit(ParkingSystem& system) {
    const std::string plate = readLine("Enter vehicle plate number: ");
    int fee = 0;
    ParkingTransaction transaction;
    system.registerExit(plate, fee, transaction);
}

void handleUpdateRates(ParkingSystem& system) {
    const VehicleType type = readVehicleType();
    const int newRate = readInt("Enter new hourly rate (RWF): ");
    system.updateHourlyRate(type, newRate);
}

void handleDailyRevenue(ParkingSystem& system) {
    const std::string date = readLine("Enter date (YYYY-MM-DD) or press Enter for all: ");
    system.displayDailyRevenue(date);
}

int main() {
    ParkingSystem system;

    std::cout << "Welcome to Kigali Smart Parking Management System\n";
    std::cout << "In-memory DSA solution with real-time slot allocation.\n";

    while (true) {
        printMenu();
        const int choice = readInt("Select an option: ");

        switch (choice) {
            case 1:
                handleAddSlot(system);
                break;
            case 2:
                system.displayAllSlots();
                break;
            case 3:
                handleVehicleEntry(system);
                break;
            case 4:
                handleVehicleExit(system);
                break;
            case 5:
                handleUpdateRates(system);
                break;
            case 6:
                system.displayAvailableSlots();
                break;
            case 7:
                system.displayParkedVehicles();
                break;
            case 8:
                system.displayTransactionHistory();
                break;
            case 9:
                handleDailyRevenue(system);
                break;
            case 10:
                system.displayCurrentRates();
                break;
            case 11:
                system.loadSampleData();
                break;
            case 0:
                std::cout << "Exiting system. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid option. Please choose a number from the menu.\n";
                break;
        }
    }
}

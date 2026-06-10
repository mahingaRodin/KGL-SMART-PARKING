#ifndef FEE_CALCULATOR_H
#define FEE_CALCULATOR_H

#include <cmath>

// Polymorphism: fee calculation strategy can be swapped without changing exit logic.
class FeeCalculator {
public:
    virtual ~FeeCalculator() = default;

    virtual int calculateBilledHours(long long durationMinutes) const {
        if (durationMinutes <= 0) {
            return 1;
        }
        return static_cast<int>(std::ceil(durationMinutes / 60.0));
    }

    virtual int calculateFee(long long durationMinutes, int hourlyRate) const {
        return calculateBilledHours(durationMinutes) * hourlyRate;
    }
};

class StandardFeeCalculator : public FeeCalculator {
public:
    int calculateBilledHours(long long durationMinutes) const override {
        return FeeCalculator::calculateBilledHours(durationMinutes);
    }

    int calculateFee(long long durationMinutes, int hourlyRate) const override {
        return FeeCalculator::calculateFee(durationMinutes, hourlyRate);
    }
};

#endif

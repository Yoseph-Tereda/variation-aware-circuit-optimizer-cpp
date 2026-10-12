#ifndef OBJECTIVE_H
#define OBJECTIVE_H
#include <vector>

class CircuitObjective {
    public:
        double gainError(
            double gain,
            double targetGain
            ) const;
        double meanGainError(
            const std::vector<double>& gains,
            double targetGain) const;
};


#endif
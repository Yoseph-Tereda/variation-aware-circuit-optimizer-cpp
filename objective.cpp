#include "objective.h"
#include <cmath>
#include <numeric>
#include <stdexcept>

double CircuitObjective::gainError(
            double gain,
            double targetGain
            ) const {
    if (targetGain < 0) {
        throw std::invalid_argument("Target gain can not be negative");
    }
    double difference = std::abs(gain) - targetGain;
    double error = difference * difference;

    return error;
}

double CircuitObjective::meanGainError(
            const std::vector<double>& gains,
            double targetGain
            ) const {
    if (gains.empty()) {
        throw std::invalid_argument(
            "Gains cannot be empty"
        );
    }
    if (targetGain < 0) {
        throw std::invalid_argument(
            "Target gain cannot be negative"
        );
    }
    std::vector<double> errors;

    for (double gain : gains) {
        double error = gainError(
            gain,
            targetGain
        );

        errors.push_back(error);
    }
    double mean_error = std::accumulate(
        errors.begin(),
        errors.end(), 0.0) / errors.size();
    return mean_error;
}

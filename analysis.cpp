#include "analysis.h"
#include "circuit.h"
#include <stdexcept>
#include <vector>
#include <numeric>
#include <cmath>

void MonteCarloAnalyzer::validateGains(
    const std::vector<double>& gains) const {
    if (gains.empty()) {
        throw std::invalid_argument(
            "Gains cannot be empty"
        );
    }
}
double MonteCarloAnalyzer::meanAmplifierGain(
    const std::vector<double>& gains) const {
    validateGains(gains);
    double mean_gain =
        std::accumulate(
        gains.begin(),
        gains.end(),
        0.0
        )
    / gains.size();
    return mean_gain;
}
double MonteCarloAnalyzer::stdAmplifierGain(
    const std::vector<double>& gains) const {
    validateGains(gains);
    double mean_gain = meanAmplifierGain(gains);
    double sum_squared_difference = 0.0;
    for (double gain: gains) {
        double difference = gain - mean_gain;
        sum_squared_difference += difference * difference;
    }
    double variance = sum_squared_difference / gains.size();
    double std_gain = std::sqrt(variance);
    return std_gain;
}

double MonteCarloAnalyzer::gainYield(
       const std::vector<double>& gains,
        double minimumGain) const {
    validateGains(gains);

    if (minimumGain < 0) {
        throw std::invalid_argument("minimum gain can not be negative");
    }

    int passed = 0;

    for (double gain: gains) {
        if (std::abs(gain) >= minimumGain) {
            passed++;
        }
    }

    double yield_percent =
        (static_cast<double>(passed) / gains.size()) * 100.0;
    return yield_percent;
}

std::vector<double> MonteCarloAnalyzer::simulateGains(
    const std::vector<AnalogCircuit>& samples,
    const CircuitSimulator& simulator,
    double length,
    double drainCurrent,
    double processParameter
) const {
    std::vector<double> gains;

    for (const AnalogCircuit& circuit: samples) {
        double gain = simulator.amplifierGain(
            circuit,
            length,
            drainCurrent,
            processParameter
        );

        gains.push_back(gain);
    }

    return gains;
}

std::vector<double> MonteCarloAnalyzer::summarizeGains(
    const std::vector<double>& gains,
    double minimumGain
) const {
    double mean_gain = meanAmplifierGain(gains);
    double std_gain = stdAmplifierGain(gains);
    double yield_percent = gainYield(gains, minimumGain);

    return {
        mean_gain,
        std_gain,
        yield_percent
    };
}

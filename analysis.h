#ifndef ANALYSIS_H
#define ANALYSIS_H
#include <random>
#include <vector>

#include "circuit.h"
#include "simulator.h"
class MonteCarloAnalyzer {

public:
    void validateGains(
        const std::vector<double>& gains
    ) const;
    double meanAmplifierGain(
         const std::vector<double>& gains) const;
    double stdAmplifierGain(
        const std::vector<double>& gains) const;
    double gainYield(
       const std::vector<double>& gains,
        double minimumGain) const;
    std::vector<double> simulateGains(
        const std::vector<AnalogCircuit>& samples,
        const CircuitSimulator& simulator,
        double length,
        double drainCurrent,
        double processParameter
        ) const;
    std::vector<double> summarizeGains(
        const std::vector<double>& gains,
        double minimumGain) const;

};


#endif
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "circuit.h"
#include <vector>

class CircuitSimulator {
    public:
    std:: vector<double> transistorTransconductances(
        const AnalogCircuit& circuit,
        double length,
        double drainCurrent,
        double processParameter
    ) const;
    double amplifierGain(
        const AnalogCircuit& circuit,
        double length,
        double drainCurrent,
        double processParameter) const;
};

#endif
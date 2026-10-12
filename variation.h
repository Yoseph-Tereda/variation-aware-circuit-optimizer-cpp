#ifndef VARIATION_H
#define VARIATION_H
#include <random>
#include <vector>
#include "circuit.h"

class CircuitVariation {

private:
    unsigned int seed;
    std::mt19937 rng;

public:
    CircuitVariation(unsigned int seed);
    void resetRng();
    double varyWidth(double width, double delta) const;
    std::vector<double> varyTransistorWidths(
        const AnalogCircuit& circuit,
        const std::vector<double>& deltas
        ) const;
    std::vector<double> generateWidthVariation(
        double standardDeviation
        );
    AnalogCircuit generateVariedCircuit(
        const AnalogCircuit& circuit,
        double widthStandardDeviation);
    std::vector<AnalogCircuit> generateMonteCarloSamples(
        const AnalogCircuit& circuit,
        double widthStandardDeviation,
        int numberOfSamples);
};

#endif




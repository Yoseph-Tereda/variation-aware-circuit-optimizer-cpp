#ifndef SCIPY_OPTIMIZER_HPP
#define SCIPY_OPTIMIZER_HPP
#include "optimizer.h"


class ScipyCircuitOptimizer {
private:
    const CircuitOptimizer& optimizer;

public:
    ScipyCircuitOptimizer(
        const CircuitOptimizer& optimizer
    );
    double optimize_w1(
        double initial_w1,
        const AnalogCircuit& circuit,
        double targetGain,
        double widthStandardDeviation,
        int numberOfSamples,
        double length,
        double drainCurrent,
        double processParameter,
        double h
    ) const;
};





#endif

#ifndef OPTIMIZER_H
#define OPTIMIZER_H

#include "simulator.h"
#include "variation.h"
#include "objective.h"

class CircuitOptimizer {
private:
    const CircuitSimulator& simulator;
    CircuitVariation& variation;
    const CircuitObjective& objective;

public:
    CircuitOptimizer(
        const CircuitSimulator& simulator,
        CircuitVariation& variation,
        const CircuitObjective& objective
    );
    double evaluate_w1(
        double w1,
        const AnalogCircuit& circuit,
        double targeGain,
        double widthStandardDeviation,
        int numberOfSamples,
        double length,
        double drainCurrent,
        double processParameter) const;
    double w1_gradient(
        double w1,
        const AnalogCircuit& circuit,
        double targetGain,
        double widthStandardDeviation,
        int numberOfSamples,
        double h,
        double length,
        double drainCurrent,
        double processParameter) const;
    double gradient_descent_w1(
        double initial_w1,
        const AnalogCircuit& circuit,
        double targetGain,
        double widthStandardDeviation,
        int numberOfSamples,
        double h,
        double learningRate,
        int maxIterations,
        double tolerance,
        double length,
        double drainCurrent,
        double processParameter) const;
};
#endif

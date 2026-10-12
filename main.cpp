#include <iostream>
#include <vector>
#include <iomanip>

#include "circuit.h"
#include "simulator.h"
#include "variation.h"
#include "analysis.h"
#include "objective.h"
#include "optimizer.h"


int main() {

    double length = 1.0;
    double drainCurrent = 0.001;
    double processParameter = 0.0002;
    double initialW1  = 10.0;
    double h = 0.01;
    double learningRate = 10.0;
    int maxIterations = 1000;
    double tolerance = 1e-6;
    double targetGain = 2.10;
    double minimumGain = 2.05;
    double widthStandardDeviation = 0.03;
    unsigned int optimizationSeed = 42;
    unsigned int validationSeed = 123;
    int optimizationSamples = 1000;
    int validationSamplesCount = 1000;

    AnalogCircuit circuit = AnalogCircuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );

    MonteCarloAnalyzer analyzer;
    CircuitSimulator simulator;
    CircuitVariation variation(optimizationSeed);
    CircuitVariation validationVariation(validationSeed);
    CircuitObjective objective;

    CircuitOptimizer optimizer(
        simulator,
        variation,
        objective
    );


    double optimizedW1 = optimizer.gradient_descent_w1(
        initialW1,
        circuit,
        targetGain,
        widthStandardDeviation,
        optimizationSamples,
        h,
        learningRate,
        maxIterations,
        tolerance,
        length,
        drainCurrent,
        processParameter
    );

    AnalogCircuit initialValidationCircuit = AnalogCircuit(
        initialW1,
        circuit.getW2(),
        circuit.getW3(),
        circuit.getW4(),
        circuit.getResistance(),
        circuit.getCapacitance()
    );

    AnalogCircuit validationCircuit = AnalogCircuit(
        optimizedW1,
        circuit.getW2(),
        circuit.getW3(),
        circuit.getW4(),
        circuit.getResistance(),
        circuit.getCapacitance()
    );

    validationVariation.resetRng();

    std::vector<AnalogCircuit> initialValidationSamples =
        validationVariation.generateMonteCarloSamples(
        initialValidationCircuit,
        widthStandardDeviation,
        validationSamplesCount
        );

    validationVariation.resetRng();

    std::vector<AnalogCircuit> validationSamples =
        validationVariation.generateMonteCarloSamples(
            validationCircuit,
            widthStandardDeviation,
            validationSamplesCount
        );

    std::vector<double> initialValidationGains =
        analyzer.simulateGains(
            initialValidationSamples,
            simulator,
            length,
            drainCurrent,
            processParameter
        );

    std::vector<double> validationGains =
        analyzer.simulateGains(
            validationSamples,
            simulator,
            length,
            drainCurrent,
            processParameter
        );

    double initialValidationObjective =
        objective.meanGainError(
            initialValidationGains,
            targetGain
        );

    double validationObjective =
        objective.meanGainError(
            validationGains,
            targetGain
        );

    std::vector<double> initialValidationSummary =
        analyzer.summarizeGains(
            initialValidationGains,
            minimumGain
        );

    std::vector<double> validationSummary =
        analyzer.summarizeGains(
            validationGains,
            minimumGain
        );

    std::cout << "Initial W1: " << initialW1 << "\n";

    std::cout << "Initial validation mena gain: "
              << initialValidationSummary[0] << "\n";

    std::cout <<"Initial validation standard deviation: "
              << initialValidationSummary[1] << "\n";

    std::cout << "Initial validation yields (%): "
              << std::fixed << std::setprecision(2)
              << initialValidationSummary[2]<< "\n";

    std::cout << std::defaultfloat << std::setprecision(10);

    std::cout << "Initial validation mean squared gain error: "
              << initialValidationObjective << "\n";

    std::cout << "Optimized W1: : "
              << optimizedW1 << "\n";

    std::cout << "Validation mean gain: "
              << validationSummary[0] << "\n";

    std::cout << "Validation mean gain: "
              << validationSummary[1] << "\n";

    std::cout << "Validation yeld (%): "
              << std::fixed << std::setprecision(2)
              << validationSummary[2] << "\n";

    std::cout << std::defaultfloat << std::setprecision(10);

    std::cout << "Validation mean squared gain error: "
              << validationObjective << "\n";

    return 0;
}
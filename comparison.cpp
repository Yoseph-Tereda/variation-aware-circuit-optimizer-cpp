#include <iostream>
#include <chrono>
#include <iomanip>

#include "circuit.h"
#include "simulator.h"
#include "variation.h"
#include "analysis.h"
#include "objective.h"
#include "optimizer.h"
#include "scipy_optimizer.h"

int main () {
    double length = 1;
    double drainCurrent = 0.001;
    double processParameter = 0.0002;

    double initial_w1 = 10;
    double h = 0.01;
    double learningRate = 10;
    int maxIterations = 1000;
    double tolerance = 1e-6;

    double targetGain = 2.10;
    double minimumGain = 2.05;
    double widthStandardDeviation = 0.03;

    unsigned int optimizationSeed = 42;
    unsigned int validationSeed = 123;
    double optimizationSamples = 1000;
    double validationSamplesCount = 1000;

    AnalogCircuit circuit = AnalogCircuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6);
    MonteCarloAnalyzer analyzer = MonteCarloAnalyzer();
    CircuitSimulator simulator = CircuitSimulator();
    CircuitVariation variation = CircuitVariation(optimizationSeed);
    CircuitVariation validationVariation = CircuitVariation(validationSeed);
    CircuitObjective objective = CircuitObjective();
    CircuitOptimizer optimizer = CircuitOptimizer(
        simulator,
     variation,
        objective
    );
    ScipyCircuitOptimizer scipyOptimizer = ScipyCircuitOptimizer(
        optimizer
    );
    auto gradientDescentStart=
        std::chrono::high_resolution_clock::now();

   double gradientDescentW1 = optimizer.gradient_descent_w1(
       initial_w1,
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

    auto gradientDescentEnd =
        std::chrono::high_resolution_clock::now();

    double gradientDescentTime =
        std::chrono::duration<double>(
            gradientDescentEnd-gradientDescentStart
            ).count();

    auto lbfgsbStart=
           std::chrono::high_resolution_clock::now();

    double lbfgsbW1 = scipyOptimizer.optimize_w1(
        initial_w1,
        circuit,
        targetGain,
        widthStandardDeviation,
        optimizationSamples,
        length,
        drainCurrent,
        processParameter,
        h
        );

    auto lbfgsbEnd =
        std::chrono::high_resolution_clock::now();

    double lbfgsbTime =
        std::chrono::duration<double>(
            lbfgsbEnd - lbfgsbStart
            ).count();

    AnalogCircuit initialCircuit(
        initial_w1,
        circuit.getW2(),
        circuit.getW3(),
        circuit.getW4(),
        circuit.getResistance(),
        circuit.getCapacitance()
    );

    AnalogCircuit gradientDescentCircuit(
        gradientDescentW1,
        circuit.getW2(),
        circuit.getW3(),
        circuit.getW4(),
        circuit.getResistance(),
        circuit.getCapacitance()
    );

    AnalogCircuit lbfgsbCircuit(
        lbfgsbW1,
        circuit.getW2(),
        circuit.getW3(),
        circuit.getW4(),
        circuit.getResistance(),
        circuit.getCapacitance()
    );

    validationVariation.resetRng();
    std::vector<AnalogCircuit> initialSamples =
        validationVariation.generateMonteCarloSamples(
            initialCircuit,
            widthStandardDeviation,
            validationSamplesCount
        );

    validationVariation.resetRng();
    std::vector<AnalogCircuit> gradientDescentSamples =
        validationVariation.generateMonteCarloSamples(
            gradientDescentCircuit,
            widthStandardDeviation,
            validationSamplesCount
        );

    validationVariation.resetRng();
    std::vector<AnalogCircuit> lbfgsbSamples =
        validationVariation.generateMonteCarloSamples(
            lbfgsbCircuit,
            widthStandardDeviation,
            validationSamplesCount
        );

    std::vector<double> initialGains = analyzer.simulateGains(
        initialSamples,
        simulator,
        length,
        drainCurrent,
        processParameter
    );

    std::vector<double> gradientDescentGains = analyzer.simulateGains(
       gradientDescentSamples,
       simulator,
       length,
       drainCurrent,
       processParameter
   );

    std::vector<double> lbfgsbGains = analyzer.simulateGains(
       lbfgsbSamples,
       simulator,
       length,
       drainCurrent,
       processParameter
   );

    double initialObjective = objective.meanGainError(
        initialGains,
        targetGain
    );

    double gradientDescentObjective = objective.meanGainError(
        gradientDescentGains,
        targetGain
    );

    double lbfgsbObjective = objective.meanGainError(
        lbfgsbGains,
        targetGain
    );

    std::vector<double> initialSummary = analyzer.summarizeGains(
        initialGains,
        minimumGain
        );

    std::vector<double> gradientSummary = analyzer.summarizeGains(
       gradientDescentGains,
       minimumGain
       );

    std::vector<double> lbfgsbSummary = analyzer.summarizeGains(
       lbfgsbGains,
       minimumGain
       );

    double initialMean = initialSummary[0];
    double initialStd = initialSummary[1];
    double initialYield = initialSummary[2];

    double gradientDescentMean = gradientSummary[0];
    double gradientDescentStd = gradientSummary[1];
    double gradientDescentYield = gradientSummary[2];

    double lbfgsbMean = lbfgsbSummary[0];
    double lbfgsbStd = lbfgsbSummary[1];
    double lbfgsbYield = lbfgsbSummary[2];

    std::cout << "\nInitial\n";
    std::cout << "w1: " << initial_w1 << '\n';
    std::cout << "mean gain: " << initialMean << '\n';
    std::cout << "Standard deviation: " << initialStd << '\n';
    std::cout << "Yield: "
              << std::fixed << std::setprecision(10)
              << initialYield
              << std::defaultfloat << '\n';
    std::cout << "Mean squared gain error: "
              << initialObjective << '\n';


    std::cout << "\nGradient descent\n";
    std::cout << "w1: " << gradientDescentW1 << '\n';
    std::cout << "mean gain: " << gradientDescentMean << '\n';
    std::cout << "Standard deviation: " << gradientDescentStd << '\n';
    std::cout << "Yield: "
              << std::fixed << std::setprecision(10)
              << gradientDescentYield
              << std::defaultfloat << '\n';
    std::cout << "Mean squared gain error: "
              << gradientDescentObjective << '\n';
    std::cout << "Runtime (seconds): "
              << gradientDescentTime << '\n';


    std::cout << "\nL-BFGS-B\n";
    std::cout << "w1: " << lbfgsbW1 << '\n';
    std::cout << "mean gain: " << lbfgsbMean << '\n';
    std::cout << "Standard deviation: " << lbfgsbStd << '\n';
    std::cout << "Yield: "
              << std::fixed << std::setprecision(10)
              << lbfgsbYield
              << std::defaultfloat << '\n';
    std::cout << "Mean squared gain error: "
              << lbfgsbObjective << '\n';
    std::cout << "Runtime (seconds): "
              << lbfgsbTime << '\n';
}
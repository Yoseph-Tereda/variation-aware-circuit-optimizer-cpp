#include "optimizer.h"
#include <cmath>
#include <stdexcept>
#include <vector>

CircuitOptimizer::CircuitOptimizer(
    const CircuitSimulator& simulator,
    CircuitVariation& variation,
    const CircuitObjective& objective
)
    : simulator(simulator),
      variation(variation),
      objective(objective)
{
}

double CircuitOptimizer::evaluate_w1(
       double w1,
       const AnalogCircuit& circuit,
       double targetGain,
       double widthStandardDeviation,
       int numberOfSamples,
       double length,
       double drainCurrent,
       double processParameter
) const {

    if (numberOfSamples <= 0) {
        throw std::invalid_argument(
            "Number of samples must be greater than 0."
        );
    }

    AnalogCircuit candidateCircuit = AnalogCircuit(
        w1,
    circuit.getW2(),
    circuit.getW3(),
    circuit.getW4(),
        circuit.getResistance(),
        circuit.getCapacitance()
    );

    variation.resetRng();

    std:: vector<AnalogCircuit> samples =
        variation.generateMonteCarloSamples(
            candidateCircuit,
            widthStandardDeviation,
            numberOfSamples
    );

    std::vector<double> gains;

    for (const AnalogCircuit& sample : samples) {

        double gain = simulator.amplifierGain(
            sample,
            length,
            drainCurrent,
            processParameter
        );

        gains.push_back(gain);
    }
    double objective_value =
        objective.meanGainError(
             gains,
             targetGain
        );

    return objective_value;

}

double CircuitOptimizer:: w1_gradient(
       double w1,
       const AnalogCircuit& circuit,
       double targetGain,
       double widthStandardDeviation,
       int numberOfSamples,
       double h,
       double length,
       double drainCurrent,
       double processParameter
) const {
    if (h <= 0) {
        throw std::invalid_argument("h must be greater than 0.");
    }

    if (w1 - h <= 0) {
        throw std::invalid_argument("w1 - h  must be greater than 0.");
    }

    double jMinus = evaluate_w1(
        w1- h,
        circuit,
        targetGain,
        widthStandardDeviation,
        numberOfSamples,
        length,
        drainCurrent,
        processParameter
    );
    double jPlus = evaluate_w1(
        w1+ h,
        circuit,
        targetGain,
        widthStandardDeviation,
        numberOfSamples,
        length,
        drainCurrent,
        processParameter
    );

    double gradient = (jPlus - jMinus) / (2 * h);

    return gradient;

}

double CircuitOptimizer::gradient_descent_w1(
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
    double processParameter
) const {

    if (initial_w1 <= 0) {
        throw std::invalid_argument(
            "Initial w1 must be greater than 0."
        );
    }

    if (learningRate <= 0) {
        throw std::invalid_argument(
            "Learning rate must be greater than 0."
        );
    }

    if (maxIterations <= 0) {
        throw std::invalid_argument(
            "Maximum iterations must be greater than 0."
        );
    }

    if (tolerance <= 0) {
        throw std::invalid_argument(
            "Tolerance must be greater than 0."
        );
    }

    double w1 = initial_w1;

    for (int iteration = 0;
        iteration < maxIterations;
        iteration++) {

        double gradient = w1_gradient(
            w1,
            circuit,
            targetGain,
            widthStandardDeviation,
            numberOfSamples,
            h,
            length,
            drainCurrent,
            processParameter
        );

        if (std::abs(gradient) <= tolerance) {
            break;
        }

        w1 = w1 - learningRate * gradient;
    }
    return w1;
}







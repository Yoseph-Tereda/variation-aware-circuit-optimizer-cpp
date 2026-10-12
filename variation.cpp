#include "variation.h"
#include "circuit.h"
#include <stdexcept>
#include <vector>

CircuitVariation::CircuitVariation(
    unsigned int seed
    )
     :seed(seed),
      rng(seed)
{
}

void CircuitVariation::resetRng()  {
      rng.seed(seed);
}
double CircuitVariation::varyWidth(
    double width,
    double delta
    ) const {
    double w_varied = width * (1 + delta);
    return w_varied;
}

std::vector<double> CircuitVariation::varyTransistorWidths(
        const AnalogCircuit& circuit,
        const std::vector<double>& deltas
        ) const {
        std::vector<double> widths = {
            circuit.getW1(),
            circuit.getW2(),
            circuit.getW3(),
            circuit.getW4()
        };
    if (widths.size() != deltas.size()) {
        throw std::invalid_argument(
            "Exactly four width variations are required"
        );
    }
    std::vector<double> varied_widths;
    for (std::size_t i = 0; i < widths.size(); ++i) {
        double result = varyWidth(
            widths[i],
            deltas[i]
        );
        varied_widths.push_back(result);
    }
    return varied_widths;

}
std::vector<double> CircuitVariation::generateWidthVariation(
    double standardDeviation)  {
    if (standardDeviation < 0.0) {
        throw std::invalid_argument("Standard deviation cannot be negative");
    }

    std::normal_distribution<double> distribution(
        0.0,
        standardDeviation
    );

    std::vector<double> deltas;

    for (int i = 0; i < 4; ++i) {
        deltas.push_back(distribution(rng));
    }

    return deltas;
}

AnalogCircuit CircuitVariation::generateVariedCircuit(
    const AnalogCircuit& circuit,
    double widthStandardDeviation) {
    std::vector<double> deltas = generateWidthVariation(widthStandardDeviation);
    std::vector<double> varied_widths = varyTransistorWidths(circuit, deltas);
    return AnalogCircuit(
        varied_widths[0],
        varied_widths[1],
        varied_widths[2],
        varied_widths[3],
        circuit.getResistance(),
        circuit.getCapacitance());
}
std::vector<AnalogCircuit> CircuitVariation:: generateMonteCarloSamples(
        const AnalogCircuit& circuit,
        double widthStandardDeviation,
        int numberOfSamples) {
    std::vector<AnalogCircuit> samples;
    for (int i = 0; i < numberOfSamples; ++i) {
        AnalogCircuit sample = generateVariedCircuit(
            circuit,
            widthStandardDeviation
        );
        samples.push_back(sample);
    }

    return samples;
}

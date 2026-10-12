#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "variation.h"
#include "../variation.h"

TEST(CircuitVariationTest, ResetRngReproducesVariation) {
    CircuitVariation variation(42);
    std::vector<double> value1 = variation.generateWidthVariation(0.03);
    variation.resetRng();
    std::vector <double> value2 = variation.generateWidthVariation(0.03);
    for (std::size_t i = 0; i < value1.size(); ++i) {
        EXPECT_DOUBLE_EQ(
            value1[i],
            value2[i]
        );
    }
}

TEST(CircuitVariationTest, varyWidth) {
    CircuitVariation variation(42);
    double variedWidth = variation.varyWidth(10, 0.03);
    EXPECT_DOUBLE_EQ(
        variedWidth,
        10.3
    );
}

TEST(CircuitVariationTest, ZeroVariationKeepsWidthUnchanged) {
    CircuitVariation variation(42);
    double variedWidth = variation.varyWidth(10, 0);
    EXPECT_DOUBLE_EQ(
        variedWidth,
        10
    );
}

TEST(CircuitVariationTest, GenerateWidthVariationsReturnsFourValues) {
    CircuitVariation variation(42);
    std::vector<double> variedWidth =
        variation.generateWidthVariation(0.03);
    EXPECT_EQ(
        variedWidth.size(),
        4
    );
}

TEST(CircuitVariationTest, GenerateWidthVariationsRejectsNegativeStd) {
    CircuitVariation variation(42);
    EXPECT_THROW(
        variation.generateWidthVariation(-0.03),
        std::invalid_argument
    );
}

TEST(CircuitVariationTest, VaryTransistorWidths) {
    CircuitVariation variation(42);
    AnalogCircuit circuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );
    std::vector<double> deltas = {
        0.10,
        -0.10,
        0.05,
        0.0
    };
    std::vector<double> widths =
        variation.varyTransistorWidths(
            circuit,
            deltas
        );
    EXPECT_DOUBLE_EQ(
        widths[0], 11);
    EXPECT_DOUBLE_EQ(
        widths[1], 10.8);
    EXPECT_DOUBLE_EQ(
        widths[2], 8.4);
    EXPECT_DOUBLE_EQ(
        widths[3], 10);
}

TEST(CircuitVariationTest, GenerateMonteCarloSamplesReturnsRequestedCount) {
    CircuitVariation variation(42);
    AnalogCircuit circuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );
    std::vector<AnalogCircuit> samples =
        variation.generateMonteCarloSamples(
            circuit,
            0.03,
            10
        );

        EXPECT_EQ(
            samples.size(),
            10
        );
}



TEST(CircuitVariationTest, VariedCircuitKeepsResistanceAndCapacitance) {
    CircuitVariation variation(42);

    AnalogCircuit circuit(
    10,
    12,
    8,
    10,
    1000,
    1e-6
);
    AnalogCircuit veriedCircuits =
        variation.generateVariedCircuit(
            circuit,
            0.03
        );
    EXPECT_DOUBLE_EQ(
        veriedCircuits.getResistance(),
        circuit.getResistance()
    );
    EXPECT_DOUBLE_EQ(
        veriedCircuits.getCapacitance(),
        circuit.getCapacitance()
    );
}

TEST(CircuitVariationTest, ZeroStdProducesNominalCircuit) {
    CircuitVariation variation(42);
    AnalogCircuit circuit(
    10,
    12,
    8,
    10,
    1000,
    1e-6
);
    AnalogCircuit veriedCircuits =
        variation.generateVariedCircuit(
            circuit,
            0
        );
    EXPECT_DOUBLE_EQ(
        veriedCircuits.getW1(),
        circuit.getW1()
    );
    EXPECT_DOUBLE_EQ(
        veriedCircuits.getW2(),
        circuit.getW2()
    );

    EXPECT_DOUBLE_EQ(
        veriedCircuits.getW3(),
        circuit.getW3()
    );
    EXPECT_DOUBLE_EQ(
        veriedCircuits.getW4(),
        circuit.getW4()
    );

}
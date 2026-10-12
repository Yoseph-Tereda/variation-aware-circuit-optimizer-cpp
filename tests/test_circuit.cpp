#include <gtest/gtest.h>
#include <cmath>
#include <stdexcept>

#include "circuit.h"

TEST(AnalogCircuitTest, ZeroTransistorWidthThrows) {
    EXPECT_THROW(
        AnalogCircuit(
            0,
            12,
            8,
            10,
            1000,
            1e-6
        ),
        std::invalid_argument
    );
}

TEST(AnalogCircuitTest, TotalWidth) {
    AnalogCircuit circuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );

    EXPECT_DOUBLE_EQ(
        circuit.totalWidth(),
        40.0
    );
}

TEST(AnalogCircuitTest, transconductance) {
    AnalogCircuit circuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );
    EXPECT_DOUBLE_EQ(
        circuit.transconductance(
            10,
            1,
            0.001,
            0.0002
        ),
        std::sqrt(
            2 * 0.0002 * (10.0 / 1.0) *0.001
        )
    );
}

TEST(AnalogCircuitTest, voltageGain) {
    AnalogCircuit circuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );
    EXPECT_DOUBLE_EQ(
        circuit.voltageGain(0.002),
        -2
        );
}
TEST(AnalogCircuitTest, bandWidth) {
    AnalogCircuit circuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );
    EXPECT_DOUBLE_EQ(
        circuit.bandWidth(),
        1.0 / (2 * std::acos(-1.0) * 1000 * 1e-6)
        );
}

TEST(AnalogCircuitTest, power) {
    AnalogCircuit circuit(
        10,
        12,
        8,
        10,
        1000,
        1e-6
    );
    EXPECT_DOUBLE_EQ(
        circuit.power(1.8,
            0.001
        ),
        0.0018
    );
}

TEST(AnalogCircuitTest, ZeroResistanceThrows) {
    EXPECT_THROW(
        AnalogCircuit(
            10,
            12,
            8,
            10,
            0,
            1e-6
        ),
        std::invalid_argument
    );
}

TEST(AnalogCircuitTest, ZeroCapacitanceThrows) {
    EXPECT_THROW(
        AnalogCircuit(
            10,
            12,
            8,
            10,
            1000,
            0
        ),
        std::invalid_argument
    );
}
#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "simulator.h"


TEST(CircuitSimulatorTest, TransistorTransconductancesReturnFourValue) {
        CircuitSimulator simulator;

         AnalogCircuit circuit (
            10,
            12,
            8,
            10,
            1000,
            1e-6
        );

        std::vector<double> gmValues =
            simulator.transistorTransconductances(
            circuit,
            1,
            0.001,
            0.0002
            );

        EXPECT_EQ(
            gmValues.size(),
            4
        );
}

TEST(CircuitSimulatorTest, FirstTransistorTransconductancesReturn) {
    CircuitSimulator simulator;

    AnalogCircuit circuit (
       10,
       12,
       8,
       10,
       1000,
       1e-6
   );

    std::vector<double> gmValues =
        simulator.transistorTransconductances(
        circuit,
        1,
        0.001,
        0.0002
        );

    EXPECT_DOUBLE_EQ(
        gmValues[0],
        std::sqrt(
            2 * 0.0002 * (10.0 / 1.0) * 0.001
        )
    );
}

TEST(CircuitSimulatorTest, amplifierGain) {
    CircuitSimulator simulator;

    AnalogCircuit circuit (
       10,
       12,
       8,
       10,
       1000,
       1e-6
   );



    double actualGain =
        simulator.amplifierGain(
        circuit,
        1,
        0.001,
        0.0002
        );
    double gm1 = std::sqrt(2 * 0.0002 * (10.0 / 1.0) * 0.001);
    double gm2 = std::sqrt(2 * 0.0002 * (12.0 / 1.0) * 0.001);

    EXPECT_DOUBLE_EQ(
        actualGain,
        (-(gm1 + gm2) / 2.0) * 1000

    );
}


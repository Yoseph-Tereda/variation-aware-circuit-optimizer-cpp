#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "objective.h"

TEST(CircuitObjectiveTest, GainError) {
    CircuitObjective objective;
    double gain = -2.0;
    double targetGain = 2.1;
    double actualError = objective.gainError(gain, targetGain);
    double expectedError = std::pow(std::abs(gain) - targetGain, 2);
    EXPECT_DOUBLE_EQ(
       actualError,
       expectedError);
}

TEST(CircuitObjectiveTest, GainErrorUsesGainMagnitude) {
    CircuitObjective objective;
    double negativeGainError = objective.gainError(-2.0, 2.1);
    double postiveGainError = objective.gainError(2.0, 2.1);
    EXPECT_DOUBLE_EQ(
        negativeGainError,
        postiveGainError
    );
}

TEST(CircuitObjectiveTest, MeanGainError) {
    CircuitObjective objective;
    std::vector<double> gains = {
        -2.0,
        -2.2,
        -2.1
    };
    double targetGain = 2.1;
    double actualError = objective.meanGainError(gains, targetGain);
    double expectedError = 0.02 / 3.0;
    EXPECT_NEAR(
        actualError,
        expectedError,
        1e-12
    );
}

TEST(CircuitObjectiveTest, GainErrorIsZeroAtTarget) {
    CircuitObjective objective;
    double gain = -2.1;
    double targetGain = 2.1;
    double actualError = objective.gainError(gain, targetGain);
    EXPECT_DOUBLE_EQ(
        actualError,
        0.0
    );
}
TEST(CircuitObjectiveTest, MeanGainErrorRejectsEmptyGains) {
    CircuitObjective objective;
    std::vector<double> gains = {};
    double targetGain = 2.1;
    EXPECT_THROW(
        objective.meanGainError(gains, targetGain),
        std::invalid_argument
    );
}



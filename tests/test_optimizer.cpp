#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "optimizer.h"

TEST(CircuitOptimizerTest, W1GradientRejectsZeroH) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    EXPECT_THROW(
        optimizer.w1_gradient(
            10,
            circuit,
            2.10,
            0.03,
            10,
            0,
            1,
            0.001,
            0.0002
        ),
        std::invalid_argument
    );

}

TEST(CircuitOptimizerTest, W1GradientRejectsNonpositiveW1MinusH) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    EXPECT_THROW(
        optimizer.w1_gradient(
            10,
            circuit,
            2.10,
            0.03,
            10,
            10,
            1,
            0.001,
            0.0002
        ),
        std::invalid_argument
    );

}

TEST(CircuitOptimizerTest, EvaluateW1RejectsZeroSamples) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;

    CircuitOptimizer optimizer(simulator, variation, objective);

    EXPECT_THROW(
        optimizer.evaluate_w1(
            10,
            circuit,
            2.10,
            0.03,
            0,
            1,
            0.001,
            0.0002
        ),
        std::invalid_argument
    );

}

TEST(CircuitOptimizerTest, GradientDescentRejectsZeroLearningRate) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    EXPECT_THROW(
        optimizer.gradient_descent_w1(
            10,
            circuit,
            2.10,
            0.03,
            10,
            0.01,
            0,
            100,
            1e-6,
            1,
            0.001,
            0.0002
        ),
        std::invalid_argument
    );

}

TEST(CircuitOptimizerTest, EvaluateW1IsDeterministic) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);


       double value1 = optimizer.evaluate_w1(
            10,
            circuit,
            2.10,
            0.03,
            100,
            1,
            0.001,
            0.0002
        );
        double value2 = optimizer.evaluate_w1(
            10,
            circuit,
            2.10,
            0.03,
            100,
            1,
            0.001,
            0.0002);
       EXPECT_DOUBLE_EQ(
           value1,
           value2
        );
}

TEST(CircuitOptimizerTest, GradientDescentImprovesObjective) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    double initialObjective = optimizer.evaluate_w1(
             10,
             circuit,
             2.10,
             0.03,
             100,
             1,
             0.001,
             0.0002
     );

    double optimizedW1 = optimizer.gradient_descent_w1(
            10,
            circuit,
            2.10,
            0.03,
            100,
            0.01,
            10,
            100,
            1e-6,
            1,
            0.001,
            0.0002
    );
    double optimizedObjective = optimizer.evaluate_w1(
            optimizedW1,
            circuit,
            2.10,
            0.03,
            100,
            1,
            0.001,
            0.0002
    );
    EXPECT_LT(
        optimizedObjective,
        initialObjective
     );
}

TEST(CircuitOptimizerTest, W1GradientMatchesCenteredDifference) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    double jPlus = optimizer.evaluate_w1(
             10 + 0.01,
             circuit,
             2.10,
             0.03,
             100,
             1,
             0.001,
             0.0002
     );

    double jMinus = optimizer.evaluate_w1(
                 10 - 0.01,
                 circuit,
                 2.10,
                 0.03,
                 100,
                 1,
                 0.001,
                 0.0002
         );
    double expectedGradient = (jPlus - jMinus) / (2.0 * 0.01);

    double actualGradient = optimizer.w1_gradient(
               10,
               circuit,
               2.10,
               0.03,
               100,
               0.01,
               1,
               0.001,
               0.0002
     );
    EXPECT_DOUBLE_EQ(
        actualGradient,
        expectedGradient
     );
}


TEST(CircuitOptimizerTest, GradientDescentRejectsZeroInitialW1) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    EXPECT_THROW(
    optimizer.gradient_descent_w1(
            0,
            circuit,
            2.10,
            0.03,
            100,
            0.01,
            10,
            100,
            1e-6,
            1,
            0.001,
            0.0002
    ),
    std::invalid_argument
    );
}

TEST(CircuitOptimizerTest, GradientDescentRejectsZeroMaxIterations) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    EXPECT_THROW(
    optimizer.gradient_descent_w1(
            10,
            circuit,
            2.10,
            0.03,
            100,
            0.01,
            10,
            0,
            1e-6,
            1,
            0.001,
            0.0002
    ),
    std::invalid_argument
    );
}

TEST(CircuitOptimizerTest, GradientDescentRejectsZeroTolerance) {
    AnalogCircuit circuit (
           10,
           12,
           8,
           10,
           1000,
           1e-6
       );
    CircuitSimulator simulator;
    CircuitVariation variation (42);
    CircuitObjective objective;
    CircuitOptimizer optimizer(simulator, variation, objective);

    EXPECT_THROW(
    optimizer.gradient_descent_w1(
            10,
            circuit,
            2.10,
            0.03,
            100,
            0.01,
            10,
            100,
            0,
            1,
            0.001,
            0.0002
    ),
    std::invalid_argument
    );
}
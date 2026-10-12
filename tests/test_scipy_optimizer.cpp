#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "scipy_optimizer.h"
#include "analysis.h"

TEST(ScipyCircuitOptimizerTest, OptimizationImprovesObjective) {
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
    CircuitOptimizer optimizer(
        simulator,
        variation,
        objective
    );

    ScipyCircuitOptimizer scipyOptimizer(optimizer);
    double initialW1 = 10.0;
    double initialObjective = optimizer.evaluate_w1(
        initialW1,
        circuit,
        2.10,
        0.03,
        100,
        1.0,
        0.001,
        0.0002
        );

    double optimizedw1 = scipyOptimizer.optimize_w1(
        initialW1,
        circuit,
        2.10,
        0.03,
        100,
        1.0,
        0.001,
        0.0002,
        0.01
    );
    double optimized_objective = optimizer.evaluate_w1(
        optimizedw1,
        circuit,
        2.10,
        0.03,
        100,
        1.0,
        0.001,
        0.0002
    );

    EXPECT_LT(
        optimized_objective,
        initialObjective

    );

}

TEST(ScipyCircuitOptimizerTest, OptimizedW1RespectsLowerBound) {
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
    CircuitOptimizer optimizer(
        simulator,
        variation,
        objective
    );

    ScipyCircuitOptimizer scipyOptimizer(optimizer);
    double initialW1 = 10.0;

    double optimizedw1 = scipyOptimizer.optimize_w1(
        initialW1,
        circuit,
        2.10,
        0.03,
        100,
        1.0,
        0.001,
        0.0002,
        0.01
    );

    EXPECT_GE(
        optimizedw1,
        0.001
    );
}

TEST(ScipyCircuitOptimizerTest, OptimizeW1IsFinite) {
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

    CircuitOptimizer optimizer(
        simulator,
        variation,
        objective
    );

    ScipyCircuitOptimizer scipyOptimizer(optimizer);

    double optimizedW1 = scipyOptimizer.optimize_w1(
        10.0,
        circuit,
        2.10,
        0.03,
        100,
        1.0,
        0.001,
        0.0002,
        0.01
    );
    EXPECT_TRUE(std::isfinite(optimizedW1));
}


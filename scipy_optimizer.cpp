#include "scipy_optimizer.h"

#include <Eigen/Core>
#include <LBFGSB.h>
#include <limits>

ScipyCircuitOptimizer::ScipyCircuitOptimizer(
    const CircuitOptimizer& optimizer
)
    : optimizer(optimizer)
{

}

double ScipyCircuitOptimizer:: optimize_w1(
       double initial_w1,
       const AnalogCircuit& circuit,
       double targetGain,
       double widthStandardDeviation,
       int numberOfSamples,
       double length,
       double drainCurrent,
       double processParameter,
       double h
) const {

    auto objective_value =
        [&](const Eigen::VectorXd& x,
            Eigen::VectorXd& grad){

        double w1 = x[0];

        double objectiveValue = optimizer.evaluate_w1(
            w1,
            circuit,
            targetGain,
            widthStandardDeviation,
            numberOfSamples,
            length,
            drainCurrent,
            processParameter
        );

        grad[0] = optimizer.w1_gradient(
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

        return objectiveValue;
    };
    LBFGSpp::LBFGSBParam<double> param;
    LBFGSpp::LBFGSBSolver<double> solver(param);

    Eigen::VectorXd x(1);
    x[0] = initial_w1;

    Eigen::VectorXd lowerBound(1);
    lowerBound[0] = 0.001;

    Eigen::VectorXd upperBound(1);
    upperBound[0] =
        std::numeric_limits<double>::infinity();

    double objectiveValue;

    solver.minimize(
        objective_value,
        x,
        objectiveValue,
        lowerBound,
        upperBound
    );

    return x[0];
}





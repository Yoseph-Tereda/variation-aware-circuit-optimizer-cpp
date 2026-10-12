#include <gtest/gtest.h>
#include <vector>
#include <cmath>
#include <stdexcept>

#include "analysis.h"
#include "../simulator.h"


TEST(MonteCarloAnalyzerTest, MeanAmplifierGain) {
    MonteCarloAnalyzer analyzer;
    std::vector<double> gains = {
        -2.0,
        -2.2,
        -1.8
    };
    double actualMean = analyzer.meanAmplifierGain(gains);
    EXPECT_DOUBLE_EQ(
        actualMean,
        -2.0
    );
}

TEST(MonteCarloAnalyzerTest, StdAmplifierGain) {
    MonteCarloAnalyzer analyzer;
    std::vector<double> gains = {
        -1.0,
        -2.0,
        -3.0
    };
    double actualStd = analyzer.stdAmplifierGain(gains);
    EXPECT_DOUBLE_EQ(
        actualStd,
        std::sqrt(2.0 / 3.0)
    );
}

TEST(MonteCarloAnalyzerTest, GainYield) {
    MonteCarloAnalyzer analyzer;
    std::vector<double> gains = {
        -2.10,
        -2.04,
        -2.20,
        -1.90
    };
    double minimumGain = 2.05;
    double actualYeld = analyzer.gainYield(gains, minimumGain);
    EXPECT_DOUBLE_EQ(
        actualYeld,
        50.0);
}

TEST(MonteCarloAnalyzerTest, MeanGainRejectsEmptyList) {
    MonteCarloAnalyzer analyzer;
    std::vector<double> gains = {};
    EXPECT_THROW(
        analyzer.meanAmplifierGain(gains),
        std::invalid_argument
    );
}

TEST(MonteCarloAnalyzerTest, GainYeldRejectsNegativeMinimumGain) {
    MonteCarloAnalyzer analyzer;
    std::vector<double> gains = {
        -2.0,
        -2.1,
    };
    double minimumGain = -1.0;
    EXPECT_THROW(
        analyzer.gainYield(gains, minimumGain),
        std::invalid_argument
    );
}

TEST(MonteCarloAnalyzerTest, summarizeGain) {
    MonteCarloAnalyzer analyzer;
    std::vector<double> gains = {
        -1.0,
        -2.0,
        -3.0
    };
    double minimumGain = 2.0;
    double expectedYield = (2.0 / 3.0) * 100.0;
    std ::vector<double> msy =
        analyzer.summarizeGains(gains, minimumGain);
    EXPECT_DOUBLE_EQ(
        msy[0],
        -2.0
    );
    EXPECT_DOUBLE_EQ(
        msy[1],
        std::sqrt(2.0 / 3.0)
    );
    EXPECT_DOUBLE_EQ(
        msy[2],
        expectedYield
    );
}

TEST(MonteCarloAnalyzerTest, SimulateGainsReturnsOneGainPerSample) {
    MonteCarloAnalyzer analyzer;
    CircuitSimulator simulator;
    std::vector<AnalogCircuit> samples = {
        AnalogCircuit(
            10,
            12,
            8,
            10,
            1000,
            1e-6),
        AnalogCircuit(
            11,
            12,
            8,
            10,
            1000,
            1e-6
            )
    };
    std::vector<double> gains =
        analyzer.simulateGains(
            samples,
            simulator,
            1,
            0.001,
            0.0002
        );
    EXPECT_DOUBLE_EQ(
        gains.size(),
        samples.size()
    );
}
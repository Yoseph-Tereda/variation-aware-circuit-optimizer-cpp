#ifndef CIRCUIT_H
#define CIRCUIT_H

class AnalogCircuit {
private:
    double w1;
    double w2;
    double w3;
    double w4;
    double resistance;
    double capacitance;

public:
    AnalogCircuit(
        double w1,
        double w2,
        double w3,
        double w4,
        double resistance,
        double capacitance
    );
    double totalWidth() const;
    double transconductance(
        double width,
        double length,
        double drainCurrent,
        double processParameter) const;
    double voltageGain(
        double gm
        ) const;
    double bandWidth() const;
    double power(
        double supplyVoltage,
        double drainCurrent) const;
    double getW1() const;
    double getW2() const;
    double getW3() const;
    double getW4() const;
    double getResistance() const;
    double getCapacitance() const;

};

#endif
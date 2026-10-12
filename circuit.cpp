#include "circuit.h"

#include <stdexcept>
#include <cmath>
#include <numbers>
AnalogCircuit::AnalogCircuit(
    double w1,
    double w2,
    double w3,
    double w4,
    double resistance,
    double capacitance
)
   : w1(w1),
     w2(w2),
     w3(w3),
     w4(w4),
     resistance(resistance),
     capacitance(capacitance)
{
if (w1 <= 0 || w2 <= 0 || w3 <= 0 || w4 <= 0) {
    throw std::invalid_argument(
        "Transistor width must be greater than zero"
    );
}
if (resistance <= 0) {
    throw std::invalid_argument(
        "Resistance must be greater than zero"
    );
}
if (capacitance <= 0) {
    throw std::invalid_argument(
        "Capacitance must be greater than zero"
    );
}

}
double AnalogCircuit::totalWidth() const {
    return w1 + w2 + w3 + w4;
}
double AnalogCircuit::transconductance(
    double width,
    double length,
    double drainCurrent ,
    double processParameter
    ) const {
    double gm = std::sqrt(2 * processParameter *(width / length) * drainCurrent) ;
    return gm;
}
double AnalogCircuit::voltageGain(double gm) const {
    double A_v = - gm * resistance;
    return A_v;
}
double AnalogCircuit::bandWidth() const {
    double f_BW = 1/(2 * std::numbers::pi * resistance * capacitance);
    return f_BW;
}
double AnalogCircuit::power(
    double supplyVoltage,
    double drainCurrent) const {
    double p = supplyVoltage * drainCurrent;
    return p;
}

double AnalogCircuit::getW1() const {
    return w1;
}
double AnalogCircuit::getW2() const {
    return w2;
}
double AnalogCircuit::getW3() const {
    return w3;
}
double AnalogCircuit::getW4() const {
    return w4;
}
double AnalogCircuit::getResistance() const {
    return resistance;
}
double AnalogCircuit::getCapacitance() const {
    return capacitance;
}
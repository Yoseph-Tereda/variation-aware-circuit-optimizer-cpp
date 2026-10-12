#include "simulator.h"

std::vector<double> CircuitSimulator::transistorTransconductances(
const AnalogCircuit& circuit,
   double length,
   double drainCurrent,
   double processParameter
    ) const {
    std::vector<double> widths ={circuit.getW1(), circuit.getW2(), circuit.getW3(), circuit.getW4()};
    std::vector<double> gm_values;
    for (double w : widths) {
        double gm = circuit.transconductance(
            w,
            length,
            drainCurrent,
            processParameter);

        gm_values.push_back(gm);
    }
    return gm_values;
}

double CircuitSimulator::amplifierGain(
    const AnalogCircuit &circuit,
    double length,
    double drainCurrent,
    double processParameter
    ) const {
    std::vector<double> gm_values =
        transistorTransconductances(
            circuit,
            length,
            drainCurrent,
            processParameter);
    double gm_eff = (gm_values[0] + gm_values[1]) / 2;
    double A_v = circuit.voltageGain(gm_eff);
    return A_v;

}



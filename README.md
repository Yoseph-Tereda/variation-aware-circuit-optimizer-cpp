# Variation-Aware Circuit Optimizer (C++)

This project implements variation-aware analog circuit optimization in C++20. It uses Monte Carlo simulation to evaluate amplifier performance under transistor-width variation and to optimize the width of the first transistor.

The design variable is the width of the first transistor, \(W_1\). The remaining widths, \(W_2\), \(W_3\), and \(W_4\), stay fixed. Monte Carlo simulation is used to evaluate how changes in transistor widths affect amplifier gain and the proportion of samples that meet a minimum-gain requirement.

The repository implements two optimization approaches: finite-difference gradient descent and an L-BFGS-based optimizer using LBFGS++ and Eigen. I also report a result from SCSD, an unpublished method I am developing, for comparison only. **SCSD is not implemented in this repository.**

## Mathematical model

For a transistor with width \(W\), the transconductance is

\[
g_m = \sqrt{2\mu\left(\frac{W}{L}\right)I_D},
\]

where \(\mu\) is the process parameter, \(L\) is transistor length, and \(I_D\) is drain current.

The voltage gain and bandwidth are modeled as

\[
A_v = -g_m R, \qquad f_{\mathrm{BW}} = \frac{1}{2\pi RC},
\]

where \(R\) and \(C\) are the circuit resistance and capacitance. The amplifier simulation uses the effective transconductance calculated from the first two transistors.

## Modeling process variation

Each transistor width is perturbed according to

\[
W_i^{(s)} = W_i(1+\delta_i^{(s)}),
\]

where \(\delta_i^{(s)}\) is a normally distributed random perturbation. The experiments use a width-variation standard deviation of \(0.03\) and 1,000 Monte Carlo samples per objective evaluation.

The random-number generator is reset during finite-difference evaluations so that neighboring candidates are compared using the same sequence of perturbations. Final validation uses a separate seed, with the same variation sequence applied to the initial and optimized designs.

## Objective function

The target gain magnitude is \(A_{\mathrm{target}}=2.10\). For \(N\) Monte Carlo samples, the objective is

\[
J(W_1) = \frac{1}{N}\sum_{s=1}^{N}
\left(\left|A_v^{(s)}(W_1)\right|-A_{\mathrm{target}}\right)^2.
\]

Only \(W_1\) is optimized; the other transistor widths are held constant. Validation also reports mean gain, standard deviation, and gain yield against a minimum gain magnitude of \(2.05\).

## Optimization methods

### Finite-difference gradient descent

The derivative is approximated by a centered difference:

\[
J'(W_1) \approx \frac{J(W_1+h)-J(W_1-h)}{2h}.
\]

The update is

\[
W_1^{(k+1)}=W_1^{(k)}-\alpha J'(W_1^{(k)}).
\]

The main experiment uses \(h=0.01\), \(\alpha=10\), and an initial width of \(W_1=10\). The iteration limit is 1,000, with a tolerance of \(10^{-6}\).

### L-BFGS-based optimization

The second implementation uses LBFGS++ with Eigen to optimize the same Monte Carlo objective. It provides a separate numerical optimization approach for comparison with gradient descent. The optimizer uses LBFGS++ and Eigen.

## Numerical results

The following results come from the C++ main program, using 1,000 validation samples and a validation seed of 123.

| Design | \(W_1\) | Mean gain | Standard deviation | Yield (%) | Validation MSE |
|---|---:|---:|---:|---:|---:|
| Initial | 10.000000 | -2.095590 | 0.022586 | 97.50 | 0.000529593 |
| Gradient descent | 10.090906 | -2.100123 | 0.022635 | 99.00 | 0.000512344 |

Gradient descent reduces validation mean squared gain error by approximately **3.26%** and increases yield from **97.50% to 99.00%** (a gain of 1.50 percentage points). These are results from one reproducible seeded run, not a statistical estimate across multiple independent runs.

### SCSD (external experimental comparison)

I also evaluated **SCSD**, an unpublished optimization method I am developing, on the same circuit optimization problem. Its results are included as a separate experimental reference; SCSD is not implemented in this repository.

| Metric | SCSD |
|---|---:|
| Optimized width, \(W_1\) | 10.097758 |
| Validation yield | 97.90% |
| Validation mean squared gain error | 0.000514155 |

These figures come from a separate experiment and should not be treated as a direct, sample-for-sample comparison with the C++ results above. The SCSD algorithm and source code are not included here.

## Project structure

| File | Purpose |
|---|---|
| `circuit.h`, `circuit.cpp` | Circuit parameters, transconductance, gain, and bandwidth |
| `simulator.h`, `simulator.cpp` | Transistor transconductances and amplifier simulation |
| `variation.h`, `variation.cpp` | Random width perturbations and Monte Carlo samples |
| `analysis.h`, `analysis.cpp` | Gain statistics, yield, and simulation over samples |
| `objective.h`, `objective.cpp` | Mean squared gain-error objective |
| `optimizer.h`, `optimizer.cpp` | Finite-difference gradient and gradient descent |
| `scipy_optimizer.h`, `scipy_optimizer.cpp` | C++ L-BFGS-based optimization interface |
| `comparison.cpp` | Comparison of the implemented optimizers |
| `main.cpp` | End-to-end optimization and independent validation |
| `tests/` | GoogleTest test cases |
| `CMakeLists.txt` | CMake build configuration |

The `scipy_optimizer` files contain the C++ optimization interface based on LBFGS++ and Eigen.

## Build and run

The project uses **C++20**, **CMake**, **Eigen**, **LBFGS++**, and **GoogleTest**. From the project root, configure and build it with:

```bash
cmake -S . -B cmake-build-debug
cmake --build cmake-build-debug
```

Run the main example:

```bash
./cmake-build-debug/variation_aware_circuit_optimizer_cpp
```

The exact executable path may vary with the CMake generator and target configuration.

## Testing

Run the full test suite with:

```bash
ctest --test-dir cmake-build-debug --output-on-failure
```

The current suite contains **45 GoogleTest cases** covering circuit calculations, simulation, random variation, Monte Carlo analysis, objective evaluation, gradient descent, the L-BFGS-based optimizer, and input validation.

On the latest local run, **all 45 tests passed** with no failures.

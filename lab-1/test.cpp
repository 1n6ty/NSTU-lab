/**
 * @file    test.cpp
 * @version 1.0
 * 
 * @brief   Automated testing for Pirson-VII, Mix and Empirical distributions.
 */

#include "pirson/alt_pirson.h"
#include "mixd/mixd.h"
#include "empirical/empirical.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <cstdlib>

/** Simple assert-like utility */
void checkClose(double val, double expected, double tol, const std::string &label) {
    double diff = std::fabs(val - expected);
    std::cout << label << ": value = " << val << ", expected ≈ " << expected;
    if (diff < tol) std::cout << " Close (diff = ";
    else std::cout << " Too match (diff = ";
    std::cout << diff << ")\n";
}

// 3.1 Tests for Pirson-VII distribution
void testPirsonVII() {
    std::cout << "\n=== 3.1 Pirson-VII Distribution Tests ===\n";
    Alt_Pirson_Status ps;

    // 3.1.1
    double nu = 5.0;
    auto p1 = std::unique_ptr<Alt_Pirson_p>(new_Alt_Pirson_p(new_Pirson_p(nu), 0.0, 1.0));
    double f0 = pirson_compute_density(0.0, p1.get(), &ps);
    double m = pirson_compute_mat_expectation(p1.get(), &ps);
    double d = pirson_compute_dispersion(p1.get(), &ps);
    double g1 = pirson_compute_skewness(p1.get(), &ps);
    double g2 = pirson_compute_excess(p1.get(), &ps);
    std::cout << "[3.1.1] Pirson-VII distribution:\n";
    checkClose(f0, 1.164, 0.01, "Density");
    checkClose(m, 0, 0.01, "Mean");
    checkClose(d, 0.1428, 0.01, "Dispersion");
    checkClose(g1, 0, 0.01, "Skewness");
    checkClose(g2, 1.2, 0.01, "Excess");

    // 3.1.2
    auto p2 = std::unique_ptr<Alt_Pirson_p>(new_Alt_Pirson_p(new_Pirson_p(nu), 0.0, 2.0));
    double f0s = pirson_compute_density(0.0, p2.get(), &ps);
    m = pirson_compute_mat_expectation(p1.get(), &ps);
    d = pirson_compute_dispersion(p1.get(), &ps);
    g1 = pirson_compute_skewness(p1.get(), &ps);
    g2 = pirson_compute_excess(p1.get(), &ps);
    std::cout << "[3.1.2] Scaled Pirson-VII distribution:\n";
    checkClose(f0s, f0 / 2.0, 0.02, "Density");
    checkClose(m, 0, 0.01, "Mean");
    checkClose(d, 0.1428, 0.01, "Dispersion");
    checkClose(g1, 0, 0.01, "Skewness");
    checkClose(g2, 1.2, 0.01, "Excess");

    // 3.1.3
    auto p3 = std::unique_ptr<Alt_Pirson_p>(new_Alt_Pirson_p(new_Pirson_p(nu), 1.5, 2.0));
    double fshift = pirson_compute_density(1.5, p3.get(), &ps);
    m = pirson_compute_mat_expectation(p1.get(), &ps);
    d = pirson_compute_dispersion(p1.get(), &ps);
    g1 = pirson_compute_skewness(p1.get(), &ps);
    g2 = pirson_compute_excess(p1.get(), &ps);
    std::cout << "[3.1.3] Shifted Pirson-VII distribution:\n";
    checkClose(fshift, f0 / 2.0, 0.02, "Density");
    checkClose(m, 0, 0.01, "Mean");
    checkClose(d, 0.1428, 0.01, "Dispersion");
    checkClose(g1, 0, 0.01, "Skewness");
    checkClose(g2, 1.2, 0.01, "Excess");
}

// 3.2 Tests for the mix distribution
void testMix() {
    std::cout << "\n=== 3.2 Mix Distribution Tests ===\n";
    Mix_Status ms;

    // --- 3.2.1 Standard mix test
    Alt_Pirson_p p1 = *new_Alt_Pirson_p(new_Pirson_p(5), 1.0, 1.0);
    Alt_Pirson_p p2 = *new_Alt_Pirson_p(new_Pirson_p(5), 2.0, 1.0);
    double coefs[2] = {0.75, 0.25};
    auto mix = std::unique_ptr<Mix_p>(new_Mix_p(&p1, coefs, 2));
    mix->params[1] = p2;

    std::cout << "[3.2.1] Standard mix distribution:\n";
    double mean = mix_compute_mat_expectation(mix.get(), &ms);
    double disp = mix_compute_dispersion(mix.get(), &ms);
    double f = mix_compute_density(0.0, mix.get(), &ms);
    double g1 = mix_compute_skewness(mix.get(), &ms);
    double g2 = mix_compute_excess(mix.get(), &ms);
    checkClose(f, 0.027, 0.02, "Density");
    checkClose(mean, 1.25, 0.01, "Expected mean");
    checkClose(disp, 0.33, 0.01, "Expected dispersion");
    checkClose(g1, 0.494, 0.01, "Skewness");
    checkClose(g2, 0.009, 0.01, "Excess");

    // --- 3.2.2 Shifted mix test
    p1 = *new_Alt_Pirson_p(new_Pirson_p(5), 0.0, 1.0);
    p2 = *new_Alt_Pirson_p(new_Pirson_p(5), 2.0, 1.0);
    coefs[1] = 0.75;
    coefs[0] = 0.25;
    mix = std::unique_ptr<Mix_p>(new_Mix_p(&p1, coefs, 2));
    mix->params[1] = p2;

    std::cout << "[3.2.2] Shifted mix distribution:\n";
    mean = mix_compute_mat_expectation(mix.get(), &ms);
    disp = mix_compute_dispersion(mix.get(), &ms);
    f = mix_compute_density(1.5, mix.get(), &ms);
    g1 = mix_compute_skewness(mix.get(), &ms);
    g2 = mix_compute_excess(mix.get(), &ms);
    checkClose(f, 0.28, 0.02, "Density");
    checkClose(mean, 1.5, 0.1, "Expected mean");
    checkClose(disp, 0.75, 0.2, "Expected dispersion");
    checkClose(g1, -0.88, 0.01, "Skewness");
    checkClose(g2, -0.43, 0.01, "Excess");

    // --- 3.2.3 Scale mix test
    p1 = *new_Alt_Pirson_p(new_Pirson_p(5), 0.0, 1.0);
    p2 = *new_Alt_Pirson_p(new_Pirson_p(5), 0.0, 3.0);
    double coefs2[2] = {0.5, 0.5};
    auto mix2 = std::unique_ptr<Mix_p>(new_Mix_p(&p1, coefs2, 2));
    mix2->params[1] = p2;

    std::cout << "[3.2.3] Scaled mix distribution:\n";
    mean = mix_compute_mat_expectation(mix2.get(), &ms);
    disp = mix_compute_dispersion(mix2.get(), &ms);
    f = mix_compute_density(0.0, mix2.get(), &ms);
    g1 = mix_compute_skewness(mix2.get(), &ms);
    g2 = mix_compute_excess(mix2.get(), &ms);
    checkClose(f, 0.77, 0.02, "Density");
    checkClose(mean, 0, 0.1, "Expected mean");
    checkClose(disp, 0.71, 0.2, "Expected dispersion");
    checkClose(g1, 0, 0.01, "Skewness");
    checkClose(g2, 3.88, 0.01, "Excess");

    // --- 3.2.4 Unequal v parameters
    p1 = *new_Alt_Pirson_p(new_Pirson_p(4), 0.0, 1.0);
    p2 = *new_Alt_Pirson_p(new_Pirson_p(10), 0.0, 1.0);
    auto mix3 = std::unique_ptr<Mix_p>(new_Mix_p(&p1, coefs2, 2));
    mix3->params[1] = p2;

    std::cout << "[3.2.4] Unequal shape mix distribution:\n";
    mean = mix_compute_mat_expectation(mix3.get(), &ms);
    disp = mix_compute_dispersion(mix3.get(), &ms);
    f = mix_compute_density(0.0, mix3.get(), &ms);
    g1 = mix_compute_skewness(mix3.get(), &ms);
    g2 = mix_compute_excess(mix3.get(), &ms);
    checkClose(f, 1.36, 0.02, "Density");
    checkClose(mean, 0, 0.1, "Expected mean");
    checkClose(disp, 0.12, 0.2, "Expected dispersion");
    checkClose(g1, 0, 0.01, "Skewness");
    checkClose(g2, 3.32, 0.01, "Excess");
}

// 3.3 Tests for the empirical distribution
void testEmpirical() {
    std::cout << "\n=== 3.3 Empirical Distribution Tests ===\n";
    Empirical_Status es;

    std::vector<double> sample;
    for(size_t i = 0; i < 100; i++) sample.push_back(rand() * 10. / RAND_MAX);
    auto emp = std::unique_ptr<Empirical_p>(new_Empirical_p(sample.data(), sample.size()));

    // --- 3.3.1 Compute empirical characteristics
    double M = empirical_compute_mat_expectation(emp.get(), &es);
    double D = empirical_compute_dispersion(emp.get(), &es);
    double g1 = empirical_compute_skewness(emp.get(), &es);
    double g2 = empirical_compute_excess(emp.get(), &es);

    std::cout << "[3.3.1] Empirical characteristics:\n";
    std::cout << "Mean = " << M << ", Var = " << D
              << ", Skewness = " << g1 << ", Excess = " << g2 << "\n";

    // --- 3.3.2 Generate new same sized sample
    std::vector<double> gen;
    for (size_t i = 0; i < sample.size(); ++i)
        gen.push_back(empirical_generate_x(emp.get(), &es));

    auto emp2 = std::unique_ptr<Empirical_p>(new_Empirical_p(gen.data(), gen.size()));
    double M2 = empirical_compute_mat_expectation(emp2.get(), &es);
    double D2 = empirical_compute_dispersion(emp2.get(), &es);
    double g12 = empirical_compute_skewness(emp2.get(), &es);
    double g22 = empirical_compute_excess(emp2.get(), &es);

    std::cout << "[3.3.2] Generated-sample comparison:\n";
    checkClose(M2, M, 0.2, "Expected mean");
    checkClose(D2, D, 0.2, "Expected dispersion");
    checkClose(g12, g1, 0.2, "Expected skewness");
    checkClose(g22, g2, 0.2, "Expected excess");
}

int main() {
    testPirsonVII();
    testMix();
    testEmpirical();
    std::cout << "\nAll tests complete.\n";
    return 0;
}

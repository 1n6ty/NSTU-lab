/**
 * @file    test.cpp
 * @version 1.0
 * 
 * @brief   Automated testing for Pirson-VII, Mix, and Empirical distributions.
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
    if (diff < tol) std::cout << " Close\n";
    else std::cout << " Too match (diff = " << diff << ")\n";
}

// 3.1 Tests for Pirson-VII distribution
void testPirsonVII() {
    std::cout << "\n=== 3.1 Pirson-VII Distribution Tests ===\n";
    Alt_Pirson_Status ps;

    // 3.1.1
    double nu = 5.0;
    auto p1 = std::unique_ptr<Alt_Pirson_p>(new_Alt_Pirson_p(new_Pirson_p(nu), 0.0, 1.0));
    double f0 = pirson_compute_density(0.0, p1.get(), &ps);
    std::cout << "[3.1.1] Pirson-VII distribution:\n";
    checkClose(f0, 1.164, 0.01, "Density");

    // 3.1.2
    auto p2 = std::unique_ptr<Alt_Pirson_p>(new_Alt_Pirson_p(new_Pirson_p(nu), 0.0, 2.0));
    double f0s = pirson_compute_density(0.0, p2.get(), &ps);
    std::cout << "[3.1.2] Scaled Pirson-VII distribution:\n";
    checkClose(f0s, f0 / 2.0, 0.02, "Density");

    // 3.1.3
    auto p3 = std::unique_ptr<Alt_Pirson_p>(new_Alt_Pirson_p(new_Pirson_p(nu), 1.5, 2.0));
    double fshift = pirson_compute_density(1.5, p3.get(), &ps);
    std::cout << "[3.1.3] Shifted Pirson-VII distribution:\n";
    checkClose(fshift, f0 / 2.0, 0.02, "Density");

    Empirical_Status es;
    std::vector<double> gen;
    for (size_t i = 0; i < 10000; ++i)
        gen.push_back(pirson_generate_x(p3.get(), &ps));

    auto emp = std::unique_ptr<Empirical_p>(new_Empirical_p(gen.data(), gen.size()));
    double M = empirical_compute_mat_expectation(emp.get(), &es);
    double D = empirical_compute_dispersion(emp.get(), &es);

    std::cout << "[3.1.4] Generated-sample comparison #1:\n";
    checkClose(M, 1.5, 0.2, "Expected mean");
    checkClose(D, 0.5, 0.2, "Expected dispersion");

    gen.clear();
    for (size_t i = 0; i < 100000; ++i)
        gen.push_back(pirson_generate_x(p3.get(), &ps));

    auto emp1 = std::unique_ptr<Empirical_p>(new_Empirical_p(gen.data(), gen.size()));
    double M1 = empirical_compute_mat_expectation(emp1.get(), &es);
    double D1 = empirical_compute_dispersion(emp1.get(), &es);

    std::cout << "[3.1.4] Generated-sample comparison #2:\n";
    checkClose(M1, 1.5, 0.2, "Expected mean");
    checkClose(D1, 0.5, 0.2, "Expected dispersion");

    gen.clear();
    for (size_t i = 0; i < 10000; ++i)
        gen.push_back(pirson_generate_x(p3.get(), &ps));

    auto emp2 = std::unique_ptr<Empirical_p>(new_Empirical_p(gen.data(), gen.size()));
    double dens_e = empirical_compute_density(0, emp2.get(), &es);
    double dens = pirson_compute_density(0, p3.get(), &ps);

    std::cout << "[3.1.4] Generated-sample comparison #3:\n";
    checkClose(dens_e, dens, 0.2, "Expected density");
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
    checkClose(mean, 1.25, 0.01, "Expected mean");
    checkClose(disp, 0.33, 0.01, "Expected dispersion");

    // --- 3.2.2 Shifted mix test
    p1 = *new_Alt_Pirson_p(new_Pirson_p(5), 0.0, 1.0);
    p2 = *new_Alt_Pirson_p(new_Pirson_p(5), 2.0, 1.0);
    coefs[0] = 0.75;
    coefs[1] = 0.25;
    mix = std::unique_ptr<Mix_p>(new_Mix_p(&p1, coefs, 2));
    mix->params[1] = p2;

    std::cout << "[3.2.2] Shifted mix distribution:\n";
    mean = mix_compute_mat_expectation(mix.get(), &ms);
    disp = mix_compute_dispersion(mix.get(), &ms);
    checkClose(mean, 0.5, 0.1, "Expected mean");
    checkClose(disp, 0.75, 0.2, "Expected dispersion");

    // --- 3.2.3 Scale mix test
    p1 = *new_Alt_Pirson_p(new_Pirson_p(5), 0.0, 1.0);
    p2 = *new_Alt_Pirson_p(new_Pirson_p(5), 0.0, 3.0);
    double coefs2[2] = {0.5, 0.5};
    auto mix2 = std::unique_ptr<Mix_p>(new_Mix_p(&p1, coefs2, 2));
    mix2->params[1] = p2;

    std::cout << "[3.2.3] Scaled mix distribution:\n";
    mean = mix_compute_mat_expectation(mix2.get(), &ms);
    disp = mix_compute_dispersion(mix2.get(), &ms);
    checkClose(mean, 0, 0.1, "Expected mean");
    checkClose(disp, 0.71, 0.2, "Expected dispersion");

    // --- 3.2.4 Unequal ν parameters
    p1 = *new_Alt_Pirson_p(new_Pirson_p(4), 0.0, 1.0);
    p2 = *new_Alt_Pirson_p(new_Pirson_p(10), 0.0, 1.0);
    auto mix3 = std::unique_ptr<Mix_p>(new_Mix_p(&p1, coefs2, 2));
    mix3->params[1] = p2;

    std::cout << "[3.2.4] Unequal shape mix distribution:\n";
    mean = mix_compute_mat_expectation(mix3.get(), &ms);
    disp = mix_compute_dispersion(mix3.get(), &ms);
    checkClose(mean, 0, 0.1, "Expected mean");
    checkClose(disp, 0.12, 0.2, "Expected dispersion");
    
    Empirical_Status es;
    std::vector<double> gen;
    for (size_t i = 0; i < 10000; ++i)
        gen.push_back(mix_generate_x(mix3.get(), &ms));

    auto emp = std::unique_ptr<Empirical_p>(new_Empirical_p(gen.data(), gen.size()));
    double M = empirical_compute_mat_expectation(emp.get(), &es);
    double D = empirical_compute_dispersion(emp.get(), &es);

    std::cout << "[3.2.5] Generated-sample comparison #1:\n";
    checkClose(M, 0, 0.2, "Expected mean");
    checkClose(D, 0.12, 0.2, "Expected dispersion");

    gen.clear();
    for (size_t i = 0; i < 100000; ++i)
        gen.push_back(mix_generate_x(mix3.get(), &ms));

    auto emp1 = std::unique_ptr<Empirical_p>(new_Empirical_p(gen.data(), gen.size()));
    double M1 = empirical_compute_mat_expectation(emp1.get(), &es);
    double D1 = empirical_compute_dispersion(emp1.get(), &es);

    std::cout << "[3.2.5] Generated-sample comparison #2:\n";
    checkClose(M1, 0, 0.2, "Expected mean");
    checkClose(D1, 0.12, 0.2, "Expected dispersion");

    gen.clear();
    for (size_t i = 0; i < 10000; ++i)
        gen.push_back(mix_generate_x(mix3.get(), &ms));

    auto emp2 = std::unique_ptr<Empirical_p>(new_Empirical_p(gen.data(), gen.size()));
    double dens_e = empirical_compute_density(0, emp2.get(), &es);
    double dens = mix_compute_density(0, mix3.get(), &ms);

    std::cout << "[3.2.5] Generated-sample comparison #3:\n";
    checkClose(dens_e, dens, 0.2, "Expected density");
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

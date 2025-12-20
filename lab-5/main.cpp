/**
 * @file    main.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Main program for testing Pearson-VII, Mix, Empirical distribution classes and Estimate function.
 */

#include "pearson/pearson.h"
#include "mix/mix.h"
#include "empirical/empirical.h"
#include "estimate/estimate.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <string>
#include <climits>
#include <sstream>
#include <iomanip>

/** Utility: safe double input */
double inputDouble(const std::string &prompt = "Enter a number: ") {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) return value;
        std::cout << "Error, please input a valid double!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

/** Utility: safe integer input */
int inputInt(const std::string &prompt = "Enter an integer: ", int min = 1, int max = INT_MAX) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min && value <= max) return value;
        std::cout << "Error, please input an integer between " << min << " and " << max << "!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

/** Write an array of doubles to a file */
void writeToFile(const std::string &filename, const std::vector<double> &data) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file) {
        std::cerr << "Error opening " << filename << " for writing!\n";
        return;
    }
    for (size_t i = 0; i < data.size(); ++i) {
        if (i > 0) file << ' ';
        file << std::setprecision(10) << data[i];
    }
    file << '\n';
    file.close();
    std::cout << "File '" << filename << "' written successfully.\n";
}

/** Base Pearson-VII workflow */
void runPearson() {
    std::cout << "\n=== Base Pearson-VII Distribution ===" << std::endl;
    try {
        PearsonVII dist;
        while (true) {
            int mode = inputInt("Choose mode (0-exit, 1-set parameters, 2-load from file, 3-compute characteristics, 4-generate x, 5-compute density, 6-save to file): ", 0, 6);
            if (mode == 0) break;

            switch (mode) {
                case 1: {
                    double shift = inputDouble("Shift parameter: ");
                    double scale = inputDouble("Scale parameter: ");
                    double shape = inputDouble("Shape parameter: ");
                    dist = PearsonVII(shift, scale, shape);
                    std::cout << "Parameters set successfully.\n";
                    break;
                }

                case 2: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    dist = PearsonVII(filename);
                    std::cout << "Distribution loaded from file. Parameters: "
                              << "shift=" << dist.getShift()
                              << ", scale=" << dist.getScale()
                              << ", shape=" << dist.getShape() << std::endl;
                    break;
                }

                case 3: {
                    std::cout << std::fixed << std::setprecision(6);
                    try {
                        std::cout << "Mathematical expectation: " << dist.computeExpectation() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Variance: " << dist.computeVariance() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Skewness: " << dist.computeSkewness() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Kurtosis: " << dist.computeKurtosis() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    }
                    break;
                }

                case 4: {
                    int count = inputInt("Number of x to generate: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        results.push_back(dist.generateRandom());
                    }
                    writeToFile("base_pearson_output.txt", results);
                    break;
                }

                case 5: {
                    int count = inputInt("Number of density points: ");
                    double start = inputDouble("Start x: ");
                    double end = inputDouble("End x: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        double x = start + (end - start) * static_cast<double>(i) / (count - 1);
                        results.push_back(dist.computeDensity(x));
                    }
                    writeToFile("base_pearson_density.txt", results);
                    break;
                }

                case 6: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    dist.saveToFile(filename);
                    std::cout << "Distribution saved to file.\n";
                    break;
                }
            }
        }
    } catch (const PearsonException& e) {
        std::cerr << "Pearson-VII Error: " << e.what() << std::endl;
    }
}

/** Mix workflow */
void runMix() {
    std::cout << "\n=== Mix Distribution ===" << std::endl;
    try {
        Mix<PearsonVII, PearsonVII> mix(PearsonVII(), PearsonVII(), 0.5);
        while (true) {
            int mode = inputInt("Choose mode (0-exit, 1-set components, 2-load from file, 3-compute characteristics, 4-generate x, 5-compute density, 6-save to file): ", 0, 6);
            if (mode == 0) break;

            switch (mode) {
                case 1: {
                    std::cout << "\n--- First component ---\n";
                    double shift1 = inputDouble("Shift: ");
                    double scale1 = inputDouble("Scale: ");
                    double shape1 = inputDouble("Shape: ");
                    PearsonVII d1(shift1, scale1, shape1);

                    std::cout << "\n--- Second component ---\n";
                    double shift2 = inputDouble("Shift: ");
                    double scale2 = inputDouble("Scale: ");
                    double shape2 = inputDouble("Shape: ");
                    PearsonVII d2(shift2, scale2, shape2);

                    double p = inputDouble("Mix coefficient p (probability of second component): ");
                    mix = Mix<PearsonVII, PearsonVII>(d1, d2, p);
                    std::cout << "Mix distribution updated.\n";
                    break;
                }

                case 2: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    mix = Mix<PearsonVII, PearsonVII>(filename);
                    std::cout << "Mix distribution loaded from file.\n";
                    break;
                }

                case 3: {
                    std::cout << std::fixed << std::setprecision(6);
                    try {
                        std::cout << "Mathematical expectation: " << mix.computeExpectation() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Variance: " << mix.computeVariance() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Skewness: " << mix.computeSkewness() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Kurtosis: " << mix.computeKurtosis() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    }
                    break;
                }

                case 4: {
                    int count = inputInt("Number of x to generate: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        results.push_back(mix.generateRandom());
                    }
                    writeToFile("mix_output.txt", results);
                    break;
                }

                case 5: {
                    int count = inputInt("Number of density points: ");
                    double start = inputDouble("Start x: ");
                    double end = inputDouble("End x: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        double x = start + (end - start) * static_cast<double>(i) / (count - 1);
                        results.push_back(mix.computeDensity(x));
                    }
                    writeToFile("mix_density.txt", results);
                    break;
                }

                case 6: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    mix.saveToFile(filename);
                    std::cout << "Mix distribution saved to file.\n";
                    break;
                }
            }
        }
    } catch (const MixException& e) {
        std::cerr << "Mix Error: " << e.what() << std::endl;
    } catch (const PearsonException& e) {
        std::cerr << "Pearson Error in Mix: " << e.what() << std::endl;
    }
}

/** Mix-Empirical workflow */
void runMixEmpirical() {
    std::cout << "\n=== Mix + Empirical Distribution ===" << std::endl;
    try {
        double arr[2] = {0, 1};
        Mix<PearsonVII, Empirical> mix(PearsonVII(), Empirical(arr, 2), 0.5);
        while (true) {
            int mode = inputInt("Choose mode (0-exit, 1-set components, 2-load from file, 3-compute characteristics, 4-generate x, 5-compute density, 6-save to file): ", 0, 6);
            if (mode == 0) break;

            switch (mode) {
                case 1: {
                    std::cout << "\n--- First component ---\n";
                    double shift1 = inputDouble("Shift: ");
                    double scale1 = inputDouble("Scale: ");
                    double shape1 = inputDouble("Shape: ");
                    PearsonVII d1(shift1, scale1, shape1);

                    std::cout << "\n--- Second component ---\n";
                    std::cout << "Enter number of samples: ";
                    size_t n = static_cast<size_t>(inputInt());
                    std::vector<double> data(n);
                    std::cout << "Enter " << n << " numbers:\n";
                    for (size_t i = 0; i < n; ++i) {
                        data[i] = inputDouble("x[" + std::to_string(i) + "] = ");
                    }
                    double p = inputDouble("Mix coefficient p (probability of second component): ");
                    mix = Mix<PearsonVII, Empirical>(d1, Empirical(data.data(), n), p);
                    std::cout << "Mix distribution updated.\n";
                    break;
                }

                case 2: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    mix = Mix<PearsonVII, Empirical>(filename);
                    std::cout << "Mix distribution loaded from file.\n";
                    break;
                }

                case 3: {
                    std::cout << std::fixed << std::setprecision(6);
                    try {
                        std::cout << "Mathematical expectation: " << mix.computeExpectation() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Variance: " << mix.computeVariance() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Skewness: " << mix.computeSkewness() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Kurtosis: " << mix.computeKurtosis() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    }
                    break;
                }

                case 4: {
                    int count = inputInt("Number of x to generate: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        results.push_back(mix.generateRandom());
                    }
                    writeToFile("mix_output.txt", results);
                    break;
                }

                case 5: {
                    int count = inputInt("Number of density points: ");
                    double start = inputDouble("Start x: ");
                    double end = inputDouble("End x: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        double x = start + (end - start) * static_cast<double>(i) / (count - 1);
                        results.push_back(mix.computeDensity(x));
                    }
                    writeToFile("mix_density.txt", results);
                    break;
                }

                case 6: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    mix.saveToFile(filename);
                    std::cout << "Mix distribution saved to file.\n";
                    break;
                }
            }
        }
    } catch (const MixException& e) {
        std::cerr << "Mix Error: " << e.what() << std::endl;
    } catch (const PearsonException& e) {
        std::cerr << "Pearson Error in Mix: " << e.what() << std::endl;
    }
}


/** Mix-Empirical workflow */
void runMixMix() {
    std::cout << "\n=== Mix + Mix Distribution ===" << std::endl;
    try {
        Mix<PearsonVII, Mix<PearsonVII, PearsonVII>> mix(PearsonVII(), Mix<PearsonVII, PearsonVII>(PearsonVII(), PearsonVII(), 0.5), 0.5);
        while (true) {
            int mode = inputInt("Choose mode (0-exit, 1-set components, 2-load from file, 3-compute characteristics, 4-generate x, 5-compute density, 6-save to file): ", 0, 6);
            if (mode == 0) break;

            switch (mode) {
                case 1: {
                    std::cout << "\n--- First component ---\n";
                    double shift = inputDouble("Shift: ");
                    double scale = inputDouble("Scale: ");
                    double shape = inputDouble("Shape: ");
                    PearsonVII d1(shift, scale, shape);

                    double p1 = inputDouble("Mix coefficient p (probability of second component): ");

                    std::cout << "\n--- Inner First component ---\n";
                    shift = inputDouble("Shift: ");
                    scale = inputDouble("Scale: ");
                    shape = inputDouble("Shape: ");
                    PearsonVII d2(shift, scale, shape);

                    std::cout << "\n--- Inner Second component ---\n";
                    shift = inputDouble("Shift: ");
                    scale = inputDouble("Scale: ");
                    shape = inputDouble("Shape: ");
                    PearsonVII d3(shift, scale, shape);

                    double p2 = inputDouble("Mix coefficient p (probability of second component): ");

                    mix = Mix(d1, Mix<PearsonVII, PearsonVII>(d2, d3, p2), p1);
                    break;
                }

                case 2: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    mix = Mix<PearsonVII, Mix<PearsonVII, PearsonVII>>(filename);
                    std::cout << "Mix distribution loaded from file.\n";
                    break;
                }

                case 3: {
                    std::cout << std::fixed << std::setprecision(6);
                    try {
                        std::cout << "Mathematical expectation: " << mix.computeExpectation() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Variance: " << mix.computeVariance() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Skewness: " << mix.computeSkewness() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    }

                    try {
                        std::cout << "Kurtosis: " << mix.computeKurtosis() << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    } catch (const MixException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    }
                    break;
                }

                case 4: {
                    int count = inputInt("Number of x to generate: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        results.push_back(mix.generateRandom());
                    }
                    writeToFile("mix_output.txt", results);
                    break;
                }

                case 5: {
                    int count = inputInt("Number of density points: ");
                    double start = inputDouble("Start x: ");
                    double end = inputDouble("End x: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        double x = start + (end - start) * static_cast<double>(i) / (count - 1);
                        results.push_back(mix.computeDensity(x));
                    }
                    writeToFile("mix_density.txt", results);
                    break;
                }

                case 6: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    mix.saveToFile(filename);
                    std::cout << "Mix distribution saved to file.\n";
                    break;
                }
            }
        }
    } catch (const MixException& e) {
        std::cerr << "Mix Error: " << e.what() << std::endl;
    } catch (const PearsonException& e) {
        std::cerr << "Pearson Error in Mix: " << e.what() << std::endl;
    }
}

/** Empirical workflow */
void runEmpirical() {
    std::cout << "\n=== Empirical Distribution ===" << std::endl;
    try {
        double arr[2] = {0, 1};
        Empirical emp(arr, 2), def(arr, 2);
        while (true) {
            int mode = inputInt("Choose mode (0-exit, 1-from data, 2-from Pearson, 3-from Mix, 4-load from file, 5-characteristics, 6-generate x, 7-compute density, 8-save samples): ", 0, 8);
            if (mode == 0) break;

            switch (mode) {
                case 1: {
                    std::cout << "Enter number of samples: ";
                    size_t n = static_cast<size_t>(inputInt());
                    std::vector<double> data(n);
                    std::cout << "Enter " << n << " numbers:\n";
                    for (size_t i = 0; i < n; ++i) {
                        data[i] = inputDouble("x[" + std::to_string(i) + "] = ");
                    }
                    emp = Empirical(data.data(), n);
                    std::cout << "Empirical distribution created from data.\n";
                    break;
                }

                case 2: {
                    double shift = inputDouble("Pearson shift: ");
                    double scale = inputDouble("Pearson scale: ");
                    double shape = inputDouble("Pearson shape: ");
                    size_t n = static_cast<size_t>(inputInt("Sample size: "));
                    PearsonVII dist(shift, scale, shape);
                    emp = Empirical(dist, n);
                    std::cout << "Empirical distribution created from Pearson-VII.\n";
                    break;
                }

                case 3: {
                    std::cout << "--- First Pearson component ---\n";
                    double s1 = inputDouble("Shift: ");
                    double sc1 = inputDouble("Scale: ");
                    double sh1 = inputDouble("Shape: ");
                    std::cout << "--- Second Pearson component ---\n";
                    double s2 = inputDouble("Shift: ");
                    double sc2 = inputDouble("Scale: ");
                    double sh2 = inputDouble("Shape: ");
                    double p = inputDouble("Mix coefficient: ");
                    size_t n = static_cast<size_t>(inputInt("Sample size: "));

                    PearsonVII d1(s1, sc1, sh1);
                    PearsonVII d2(s2, sc2, sh2);
                    Mix mix(d1, d2, p);
                    emp = Empirical(mix, n);
                    std::cout << "Empirical distribution created from Mix.\n";
                    break;
                }

                case 4: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    emp = Empirical(filename);
                    std::cout << "Empirical distribution loaded from file.\n";
                    break;
                }

                case 5: {
                    std::cout << std::fixed << std::setprecision(6);
                    try {
                        std::cout << "Mathematical expectation: " << emp.computeExpectation() << std::endl;
                        std::cout << "Variance: " << emp.computeVariance() << std::endl;
                        std::cout << "Skewness: " << emp.computeSkewness() << std::endl;
                        std::cout << "Kurtosis: " << emp.computeKurtosis() << std::endl;
                    } catch (const EmpiricalException& e) {
                        std::cout << "Error computing characteristics: " << e.what() << std::endl;
                    }
                    break;
                }

                case 6: {
                    int count = inputInt("Number of x to generate: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        results.push_back(emp.generateRandom());
                    }
                    writeToFile("empirical_output.txt", results);
                    break;
                }

                case 7: {
                    int count = inputInt("Number of density points: ");
                    double start = inputDouble("Start x: ");
                    double end = inputDouble("End x: ");
                    std::vector<double> results;
                    results.reserve(count);
                    for (int i = 0; i < count; ++i) {
                        double x = start + (end - start) * static_cast<double>(i) / (count - 1);
                        results.push_back(emp.computeDensity(x));
                    }
                    writeToFile("empirical_density.txt", results);
                    break;
                }

                case 8: {
                    std::string filename;
                    std::cout << "Enter filename: ";
                    std::cin >> filename;
                    emp.saveToFile(filename);
                    std::cout << "Empirical samples saved to file.\n";
                    break;
                }

                case 9: {
                    emp = Empirical(def);
                    break;
                }
            }
        }
    } catch (const EmpiricalException& e) {
        std::cerr << "Empirical Error: " << e.what() << std::endl;
    } catch (const PearsonException& e) {
        std::cerr << "Pearson Error: " << e.what() << std::endl;
    } catch (const MixException& e) {
        std::cerr << "Mix Error: " << e.what() << std::endl;
    }
}

void testEmpiricalCopyConstructor() {
    std::cout << "\n=== Testing Empirical Copy Constructor & Assignment ===" << std::endl;

    try {
        double rawData[] = {1.0, 2.0, 3.0, 4.0, 5.0, 10.0};
        Empirical original(rawData, 6);

        std::cout << "Original expectation: " << original.computeExpectation() << std::endl;
        std::cout << "Original min: " << original.getMin() << ", max: " << original.getMax() << std::endl;

        Empirical copy1(original);
        Empirical copy2 = original;

        double origExp = original.computeExpectation();
        double copy1Exp = copy1.computeExpectation();
        double copy2Exp = copy2.computeExpectation();

        if (std::abs(origExp - copy1Exp) > 1e-12 || std::abs(origExp - copy2Exp) > 1e-12) {
            std::cerr << "FAIL: Copies do not match original in expectation!" << std::endl;
            return;
        }

        const double* origSample = original.getSample();
        const double* copy1Sample = copy1.getSample();
        const double* copy2Sample = copy2.getSample();

        if (origSample == copy1Sample || origSample == copy2Sample || copy1Sample == copy2Sample) {
            std::cerr << "FAIL: Shallow copy detected! Pointers are equal." << std::endl;
            return;
        }

        std::cout << "PASS: Deep copy works correctly. Objects are independent." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception during copy test: " << e.what() << std::endl;
    }
}


void testEstimate() {
    std::cout << "=== Lab 5: Aggregation by Reference & Robust Estimation ===\n";

    int n = 500; 
    
    double true_loc = inputDouble("Enter True Location: ");
    double true_scale = inputDouble("Enter True Scale: ");
    double true_shape = 100.0; 
    
    PearsonVII idealDist;
    try {
        idealDist = PearsonVII(true_loc, true_scale, true_shape);
    } catch (const std::exception& e) {
        std::cerr << "Exception during testing Estimate: " << e.what() << std::endl;
    }
    
    // 2. Генерация чистой выборки [cite: 9]
    std::cout << "Generating ideal sample (n=" << n << ")...\n";
    Empirical dataObj(idealDist, static_cast<size_t>(n));

    dataObj.saveToFile("normal_data");

    double param_c = inputDouble("Enter parameter 'c' for loss function (e.g., 2.0): ");
    
    // Создаем объект оценки, связываем с данными (dataObj)
    
    Estimate estimator(dataObj, true_scale, param_c, 100);

    // Вывод результатов на чистых данных [cite: 10]
    std::cout << "\n--- Clean Data Results ---\n";
    std::cout << "True Location: " << true_loc << "\n";
    std::cout << "Arithmetic Mean: " << dataObj.computeExpectation() << "\n";
    std::cout << "Robust Estimate: " << estimator.getMu() << "\n";

    // 4. Засорение данных (Атака) [cite: 8, 11]
    double contamination_fraction = inputDouble("Enter contamination fraction p (0.1 - 0.4): ");
    double contamination_shift = inputDouble("Enter shift for contaminating distribution (e.g., 5.0): ");
    
    // Засоряющее распределение (сдвинутое)
    PearsonVII poisonDist(contamination_shift, true_scale, true_shape);
    
    Empirical(poisonDist, n).saveToFile("poisoning_data");

    int n_poison = static_cast<int>(n * contamination_fraction);
    std::cout << "\nPoisoning " << n_poison << " observations...\n";

    // "Отравление" данных: заменяем первые n_poison элементов
    double *data = const_cast<double *>(dataObj.getSample());
    for (int i = 0; i < n_poison; ++i) {
        double bad_value = poisonDist.generateRandom();
        data[i] = bad_value;
    }

    Empirical(data, n).saveToFile("poisoned_data");

    // 5. Уведомление наблюдателей 
    std::cout << "Data changed. Calling notify()...\n";
    dataObj.notify();

    // 6. Сравнение результатов [cite: 12]
    double mean_poisoned = dataObj.computeExpectation();
    double robust_poisoned = estimator.getMu();

    std::cout << "\n--- Contaminated Data Results ---\n";
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Arithmetic Mean: " << mean_poisoned 
              << " (Error: " << std::abs(mean_poisoned - true_loc) << ")\n";
    std::cout << "Robust Estimate: " << robust_poisoned 
              << " (Error: " << std::abs(robust_poisoned - true_loc) << ")\n";

    if (std::abs(robust_poisoned - true_loc) < std::abs(mean_poisoned - true_loc)) {
        std::cout << "Conclusion: Robust estimate is better.\n";
    } else {
        std::cout << "Conclusion: Arithmetic mean is better (check parameters).\n";
    }

    std::cout << "\nSample weights analysis (first 100 points):\n";
    std::cout << "Val\t\tWeight\n";
    for (int i = 0; i < 100; ++i) {
        double val = data[i];
        double w = estimator.weight((val - robust_poisoned) / true_scale);
        std::cout << val << "\t" << w << "\n";
    }
}


int main() {
    std::cout << "Pearson-VII Distribution Classes Test Program" << std::endl;
    std::cout << "=============================================" << std::endl;

    while (true) {
        int mode = inputInt("\nMain menu: (0-exit, 1-Base Pearson, 2-Mix<P, P>, 3-Mix<P, E>, 4-Mix<P, Mix<P, P>>, 5-Empirical, 6-Copy Test, 7-Estimate Test): ", 0, 7);
        if (mode == 0) break;

        switch (mode) {
            case 1: runPearson(); break;
            case 2: runMix(); break;
            case 3: runMixEmpirical(); break;
            case 4: runMixMix(); break;
            case 5: runEmpirical(); break;
            case 6: testEmpiricalCopyConstructor(); break;
            case 7: testEstimate(); break;
        }
    }

    std::cout << "Program finished. Goodbye!" << std::endl;
    return 0;
}
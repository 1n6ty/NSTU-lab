/**
 * @file    main.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Main program for testing Pearson-VII distribution classes.
 *          Input/output operations are performed outside class functions.
 */

#include "pearson/pearson.h"
#include "mixd/mixd.h"
#include "empirical/empirical.h"

#include <iostream>
#include <fstream>
#include <limits>
#include <memory>
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
    for (double x : data) file << x << ' ';
    file.close();
    std::cout << "File '" << filename << "' written successfully.\n";
}

/** Base Pearson-VII workflow */
void runPearson() {
    double buf;
    std::cout << "\n=== Base Pearson-VII Distribution ===" << std::endl;
    
    try {
        PearsonVII dist;
        
        while (true) {
            int mode = inputInt("Choose mode (0-exit, 1-set parameters, 2-load from file, 3-compute characteristics, 4-generate x, 5-compute density, 6-save to file): ", 0, 6);
            if (mode == 0) break;

            switch (mode) {
                case 1: {
                    // Установка параметров
                    double shift = inputDouble("Shift parameter: ");
                    double scale = inputDouble("Scale parameter: ");
                    double shape = inputDouble("Shape parameter: ");
                    
                    dist = PearsonVII(shift, scale, shape);
                    std::cout << "Parameters set successfully.\n";
                    break;
                }
                
                case 2: {
                    // Загрузка из файла
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
                    // Вычисление характеристик
                    try {
                        buf = dist.computeExpectation();

                        std::cout << std::fixed << std::setprecision(6);
                        std::cout << "Mathematical expectation: " << buf << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Mathematical expectation: " << e.what() << std::endl;
                    }
                    
                    try {
                        buf = dist.computeVariance();

                        std::cout << "Variance: " << buf << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Variance: " << e.what() << std::endl;
                    }
                    
                    try {
                        buf = dist.computeSkewness();

                        std::cout << "Skewness: " << buf << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Skewness: " << e.what() << std::endl;
                    }
                    
                    try {
                        buf = dist.computeKurtosis();
                        
                        std::cout << "Kurtosis: " << buf << std::endl;
                    } catch (const PearsonException& e) {
                        std::cout << "Kurtosis: " << e.what() << std::endl;
                    }
                    break;
                }
                
                case 4: {
                    // Генерация случайных величин
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
                    // Вычисление плотности
                    int count = inputInt("Number of density points: ");
                    double start = inputDouble("Start x: ");
                    double end = inputDouble("End x: ");
                    
                    std::vector<double> results;
                    results.reserve(count);
                    
                    for (int i = 0; i < count; ++i) {
                        double x = start + (end - start) * i / (count - 1);
                        try {
                            double density = dist.computeDensity(x);
                            results.push_back(density);
                        } catch (const PearsonException& e) {
                            std::cout << "Error computing density at x=" << x << ": " << e.what() << std::endl;
                            results.push_back(0.0);
                        }
                    }
                    
                    writeToFile("base_pearson_density.txt", results);
                    break;
                }
                
                case 6: {
                    // Сохранение в файл
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
        // Создаем смесь из двух распределений
        std::vector<PearsonVII> distributions = {
            PearsonVII(),
            PearsonVII()
        };
        std::vector<double> coefficients = {0.5, 0.5};
        
        Mix_p* mix = new_Mix_p(distributions, coefficients);
        if (!mix) {
            std::cerr << "Failed to create mix distribution" << std::endl;
            return;
        }
        
        Mix_Status ms;
        
        while (true) {
            int mode = inputInt("Mix: Choose mode (0-exit, 1-parameters, 2-generate x, 3-generate density): ", 0, 3);
            if (mode == 0) break;

            // Ввод параметров для каждого распределения в смеси
            for (size_t i = 0; i < mix->distributions.size(); ++i) {
                std::cout << "\nMix: Input parameters for distribution " << (i + 1) << '\n';
                double shift = inputDouble("Shift parameter: ");
                double scale = inputDouble("Scale parameter: ");
                double shape = inputDouble("Shape parameter: ");
                double coef = inputDouble("Partial coefficient: ");
                
                mix->distributions[i] = PearsonVII(shift, scale, shape);
                mix->coefficients[i] = coef;
            }

            if (mode == 1) {
                double val = mix_compute_mat_expectation(mix, &ms);
                std::cout << "Mat. expectation: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn't exist") << '\n';
                
                val = mix_compute_dispersion(mix, &ms);
                std::cout << "Dispersion: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn't exist") << '\n';
                
                val = mix_compute_skewness(mix, &ms);
                std::cout << "Skewness: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn't exist") << '\n';
                
                val = mix_compute_excess(mix, &ms);
                std::cout << "Excess: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn't exist") << '\n';
            } 
            else if (mode == 2) {
                int count = inputInt("Number of x to generate: ");
                std::vector<double> results;
                results.reserve(count);
                for (int i = 0; i < count; ++i) {
                    double val = mix_generate_x(mix, &ms);
                    if (ms != MIX_SUCCESS) break;
                    results.push_back(val);
                }
                writeToFile("mix_output.txt", results);
            } 
            else if (mode == 3) {
                int count = inputInt("Number of density points: ");
                double start = inputDouble("Start x: ");
                double end = inputDouble("End x: ");
                std::vector<double> results;
                results.reserve(count);
                for (int i = 0; i < count; ++i) {
                    double x = start + (end - start) * i / count;
                    double val = mix_compute_density(x, mix, &ms);
                    if (ms != MIX_SUCCESS) break;
                    results.push_back(val);
                }
                writeToFile("mix_density.txt", results);
            }
        }
        
        del_Mix_p(mix);
        
    } catch (const std::exception& e) {
        std::cerr << "Mix Error: " << e.what() << std::endl;
    }
}

/** Empirical workflow */
void runEmpirical() {
    Empirical_Status es;

    while (true) {
        int mode = inputInt("Empirical: Choose mode (0-exit, 1-parameters, 2-generate x, 3-generate density): ", 0, 3);
        if (mode == 0) break;

        int n = inputInt("Number of empirical x values: ");
        std::vector<double> x;
        x.reserve(n);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::string line;
        std::cout << "Enter " << n << " values separated by spaces:\n> ";
        std::getline(std::cin, line);
        std::istringstream iss(line);

        double val;
        while (iss >> val) x.push_back(val);

        if (x.size() != static_cast<size_t>(n)) {
            std::cerr << "Error: expected " << n << " values, got " << x.size() << ".\n";
            continue;
        }

        auto emp = std::unique_ptr<Empirical_p>(new_Empirical_p(x.data(), n));

        if (mode == 1) {
            double res = empirical_compute_mat_expectation(emp.get(), &es);
            std::cout << "Mat. expectation: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(res) : "Doesn’t exist") << '\n';
            res = empirical_compute_dispersion(emp.get(), &es);
            std::cout << "Dispersion: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(res) : "Doesn’t exist") << '\n';
            res = empirical_compute_skewness(emp.get(), &es);
            std::cout << "Skewness: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(res) : "Doesn’t exist") << '\n';
            res = empirical_compute_excess(emp.get(), &es);
            std::cout << "Excess: " << ((es == EMPIRICAL_SUCCESS) ? std::to_string(res) : "Doesn’t exist") << '\n';
        } 
        else if (mode == 2) {
            int count = inputInt("Number of x to generate: ");
            std::vector<double> results;
            results.reserve(count);
            for (int i = 0; i < count; ++i) {
                double r = empirical_generate_x(emp.get(), &es);
                if (es != EMPIRICAL_SUCCESS) break;
                results.push_back(r);
            }
            writeToFile("output.txt", results);
        } 
        else if (mode == 3) {
            int count = inputInt("Number of density points: ");
            double start = inputDouble("Start x: ");
            double end = inputDouble("End x: ");
            std::vector<double> results;
            results.reserve(count);
            for (int i = 0; i < count; ++i) {
                double xi = start + (end - start) * i / count;
                double r = empirical_compute_density(xi, emp.get(), &es);
                if (es != EMPIRICAL_SUCCESS) break;
                results.push_back(r);
            }
            writeToFile("output.txt", results);
        }
    }
}

int main() {
    std::cout << "Pearson-VII Distribution Classes Test Program" << std::endl;
    std::cout << "=============================================" << std::endl;
    
    while (true) {
        int mode = inputInt("\nMain menu: (0-exit, 1-Base Pearson, 2-Mix, 3-Empirical): ", 0, 4);
        if (mode == 0) break;
        
        switch (mode) {
            case 1: runPearson(); break;
            case 2: runMix(); break;
            case 3: runEmpirical(); break;
        }
    }
    
    std::cout << "Program finished. Goodbye!" << std::endl;
    return 0;
}
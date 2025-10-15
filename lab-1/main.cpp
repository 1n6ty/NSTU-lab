#include "pirson/alt_pirson.h"
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

/** Pirson-VII workflow */
void runPirsonVII() {
    Alt_Pirson_Status ps;
    auto p = std::unique_ptr<Alt_Pirson_p>(new_Alt_Pirson_p(new_Pirson_p(1), 1, 1));

    while (true) {
        int mode = inputInt("Pirson-VII: Choose mode (0-exit, 1-parameters, 2-generate x, 3-generate density): ", 0, 3);
        if (mode == 0) break;

        p->dp->v = inputDouble("Shape coefficient: ");
        p->u = inputDouble("Shear coefficient: ");
        p->a = inputDouble("Scale coefficient: ");

        if (mode == 1) {
            double val = pirson_compute_mat_expectation(p.get(), &ps);
            std::cout << "Mat. expectation: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
            val = pirson_compute_dispersion(p.get(), &ps);
            std::cout << "Dispersion: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
            val = pirson_compute_skewness(p.get(), &ps);
            std::cout << "Skewness: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
            val = pirson_compute_excess(p.get(), &ps);
            std::cout << "Excess: " << ((ps == ALT_PIRSON_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
        } 
        else if (mode == 2) {
            int count = inputInt("Number of x to generate: ");
            std::vector<double> results;
            results.reserve(count);
            for (int i = 0; i < count; ++i) {
                double val = pirson_generate_x(p.get(), &ps);
                if (ps != ALT_PIRSON_SUCCESS) break;
                results.push_back(val);
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
                double x = start + (end - start) * i / count;
                double val = pirson_compute_density(x, p.get(), &ps);
                if (ps != ALT_PIRSON_SUCCESS) break;
                results.push_back(val);
            }
            writeToFile("output.txt", results);
        }
    }
}

/** Mix workflow */
void runMix() {
    Mix_Status ms;
    double coefs[2] = {0.5, 0.5};
    Alt_Pirson_p params[] = {
        *new_Alt_Pirson_p(new_Pirson_p(1), 1, 1),
        *new_Alt_Pirson_p(new_Pirson_p(1), 1, 1)
    };
    auto mix = std::unique_ptr<Mix_p>(new_Mix_p(params, coefs, 2));

    while (true) {
        int mode = inputInt("Mix: Choose mode (0-exit, 1-parameters, 2-generate x, 3-generate density): ", 0, 3);
        if (mode == 0) break;

        for (int i = 0; i < 2; ++i) {
            std::cout << "\nMix: Input parameters for distribution " << (i + 1) << '\n';
            mix->params[i].dp->v = inputDouble("Shape coefficient: ");
            mix->params[i].u = inputDouble("Shear coefficient: ");
            mix->params[i].a = inputDouble("Scale coefficient: ");
            mix->coefs[i] = inputDouble("Partial coefficient: ");
        }

        if (mode == 1) {
            double val = mix_compute_mat_expectation(mix.get(), &ms);
            std::cout << "Mat. expectation: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
            val = mix_compute_dispersion(mix.get(), &ms);
            std::cout << "Dispersion: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
            val = mix_compute_skewness(mix.get(), &ms);
            std::cout << "Skewness: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
            val = mix_compute_excess(mix.get(), &ms);
            std::cout << "Excess: " << ((ms == MIX_SUCCESS) ? std::to_string(val) : "Doesn’t exist") << '\n';
        } 
        else if (mode == 2) {
            int count = inputInt("Number of x to generate: ");
            std::vector<double> results;
            results.reserve(count);
            for (int i = 0; i < count; ++i) {
                double val = mix_generate_x(mix.get(), &ms);
                if (ms != MIX_SUCCESS) break;
                results.push_back(val);
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
                double x = start + (end - start) * i / count;
                double val = mix_compute_density(x, mix.get(), &ms);
                if (ms != MIX_SUCCESS) break;
                results.push_back(val);
            }
            writeToFile("output.txt", results);
        }
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

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // clear leftover newline
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

/** Entry point */
int main() {
    while (true) {
        int mode = inputInt("\nMain menu: (0-exit, 1-Pirson-VII, 2-Mix, 3-Empirical): ", 0, 3);
        if (mode == 0) break;
        switch (mode) {
            case 1: runPirsonVII(); break;
            case 2: runMix(); break;
            case 3: runEmpirical(); break;
        }
    }
    return 0;
}

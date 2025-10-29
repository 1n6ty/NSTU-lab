/**
 * @file    pirson.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Implementation of PearsonVII class.
 */

#include "pirson.h"
#include <fstream>
#include <cmath>
#include <random>
#include <sstream>

// Вспомогательная функция для вычисления бета-функции
double betaFunction(double a, double b) {
    return std::tgamma(a) * std::tgamma(b) / std::tgamma(a + b);
}

PearsonVII::PearsonVII(double location, double scale, double shape) 
    : location_(location), scale_(scale), shape_(shape) {
    updateNormalizationConstant();
    are_parameters_valid_ = validateParameters();
}

PearsonVII::PearsonVII(const std::string& filename) {
    loadFromFile(filename);
}

void PearsonVII::updateNormalizationConstant() {
    if (shape_ > 0.5 && scale_ > 0) {
        normalization_constant_ = 1.0 / (std::abs(scale_) * 
                                betaFunction(shape_ - 0.5, 0.5) * 
                                std::pow(1.0, shape_ - 0.5));
    }
}

bool PearsonVII::validateParameters() const {
    return (shape_ > 0.5 && scale_ > 0);
}

bool PearsonVII::isValid() const {
    return are_parameters_valid_;
}

// Set-функции
void PearsonVII::setLocation(double location) {
    location_ = location;
    are_parameters_valid_ = validateParameters();
}

void PearsonVII::setScale(double scale) {
    if (scale <= 0) {
        throw PearsonException("Scale parameter must be positive");
    }
    scale_ = scale;
    updateNormalizationConstant();
    are_parameters_valid_ = validateParameters();
}

void PearsonVII::setShape(double shape) {
    if (shape <= 0.5) {
        throw PearsonException("Shape parameter must be greater than 0.5");
    }
    shape_ = shape;
    updateNormalizationConstant();
    are_parameters_valid_ = validateParameters();
}

void PearsonVII::setParameters(double location, double scale, double shape) {
    if (scale <= 0) {
        throw PearsonException("Scale parameter must be positive");
    }
    if (shape <= 0.5) {
        throw PearsonException("Shape parameter must be greater than 0.5");
    }
    
    location_ = location;
    scale_ = scale;
    shape_ = shape;
    updateNormalizationConstant();
    are_parameters_valid_ = true;
}

// Get-функции
double PearsonVII::getLocation() const { return location_; }
double PearsonVII::getScale() const { return scale_; }
double PearsonVII::getShape() const { return shape_; }

// Вычисление плотности
double PearsonVII::computeDensity(double x) const {
    if (!are_parameters_valid_) {
        throw PearsonException("Invalid parameters for density computation");
    }
    
    double z = (x - location_) / scale_;
    return normalization_constant_ * std::pow(1.0 + z * z, -shape_);
}

// Математическое ожидание
double PearsonVII::computeExpectation() const {
    if (!are_parameters_valid_) {
        throw PearsonException("Invalid parameters for expectation computation");
    }
    if (shape_ <= 1.0) {
        throw PearsonException("Expectation does not exist for shape <= 1");
    }
    return location_;
}

// Дисперсия
double PearsonVII::computeVariance() const {
    if (!are_parameters_valid_) {
        throw PearsonException("Invalid parameters for variance computation");
    }
    if (shape_ <= 1.5) {
        throw PearsonException("Variance does not exist for shape <= 1.5");
    }
    return scale_ * scale_ / (2.0 * shape_ - 3.0);
}

// Коэффициент асимметрии
double PearsonVII::computeSkewness() const {
    if (!are_parameters_valid_) {
        throw PearsonException("Invalid parameters for skewness computation");
    }
    if (shape_ <= 1.5) {
        throw PearsonException("Skewness does not exist for shape <= 1.5");
    }
    return 0.0; // Распределение симметрично
}

// Коэффициент эксцесса
double PearsonVII::computeKurtosis() const {
    if (!are_parameters_valid_) {
        throw PearsonException("Invalid parameters for kurtosis computation");
    }
    if (shape_ <= 2.0) {
        throw PearsonException("Kurtosis does not exist for shape <= 2");
    }
    return 6.0 / (2.0 * shape_ - 5.0);
}

// Генерация случайной величины
double PearsonVII::generateRandom() const {
    if (!are_parameters_valid_) {
        throw PearsonException("Invalid parameters for random generation");
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    
    // Генерация через обратное преобразование
    double u = dis(gen);
    double t = std::pow(u, -1.0 / (shape_ - 0.5)) - 1.0;
    
    // Используем константу PI вместо M_PI
    const double PI = 3.14159265358979323846;
    double r = std::sqrt(t) * std::cos(2.0 * PI * dis(gen));
    
    return location_ + scale_ * r;
}

// Сохранение в файл
void PearsonVII::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw PearsonException("Cannot open file for writing: " + filename);
    }
    
    file << location_ << " " << scale_ << " " << shape_ << std::endl;
    file.close();
}

// Загрузка из файла
void PearsonVII::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw PearsonException("Cannot open file for reading: " + filename);
    }
    
    double loc, scale, shape;
    if (!(file >> loc >> scale >> shape)) {
        throw PearsonException("Invalid file format");
    }
    
    setParameters(loc, scale, shape);
    file.close();
}
/**
 * @file    alt_pirson.cpp
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Implementation of TransformedPearson class.
 */

#include "alt_pirson.h"
#include <fstream>
#include <cmath>
#include <sstream>

TransformedPearson::TransformedPearson(double location, double scale, double shape)
    : location_(location), scale_(scale), shape_(shape) {
    updateBaseDistribution();
    is_parameters_valid_ = validateParameters();
}

TransformedPearson::TransformedPearson(const std::string& filename) {
    loadFromFile(filename);
}

// Конструктор копирования
TransformedPearson::TransformedPearson(const TransformedPearson& other)
    : location_(other.location_), scale_(other.scale_), shape_(other.shape_),
      is_parameters_valid_(other.is_parameters_valid_) {
    updateBaseDistribution();
}

// Оператор присваивания
TransformedPearson& TransformedPearson::operator=(const TransformedPearson& other) {
    if (this != &other) {
        location_ = other.location_;
        scale_ = other.scale_;
        shape_ = other.shape_;
        is_parameters_valid_ = other.is_parameters_valid_;
        updateBaseDistribution();
    }
    return *this;
}

void TransformedPearson::updateBaseDistribution() {
    base_distribution_ = std::make_unique<PearsonVII>(0.0, 1.0, shape_);
}

bool TransformedPearson::validateParameters() const {
    return (shape_ > 0.5 && scale_ > 0);
}

bool TransformedPearson::isValid() const {
    return is_parameters_valid_ && base_distribution_->isValid();
}

// Set-функции
void TransformedPearson::setLocation(double location) {
    location_ = location;
    is_parameters_valid_ = validateParameters();
}

void TransformedPearson::setScale(double scale) {
    if (scale <= 0) {
        throw TransformedPearsonException("Scale parameter must be positive");
    }
    scale_ = scale;
    is_parameters_valid_ = validateParameters();
}

void TransformedPearson::setShape(double shape) {
    if (shape <= 0.5) {
        throw TransformedPearsonException("Shape parameter must be greater than 0.5");
    }
    shape_ = shape;
    updateBaseDistribution();
    is_parameters_valid_ = validateParameters();
}

void TransformedPearson::setParameters(double location, double scale, double shape) {
    if (scale <= 0) {
        throw TransformedPearsonException("Scale parameter must be positive");
    }
    if (shape <= 0.5) {
        throw TransformedPearsonException("Shape parameter must be greater than 0.5");
    }
    
    location_ = location;
    scale_ = scale;
    shape_ = shape;
    updateBaseDistribution();
    is_parameters_valid_ = true;
}

// Get-функции
double TransformedPearson::getLocation() const { return location_; }
double TransformedPearson::getScale() const { return scale_; }
double TransformedPearson::getShape() const { return shape_; }

// Преобразования между базовым и преобразованным распределениями
double TransformedPearson::transformToBase(double x) const {
    return (x - location_) / scale_;
}

double TransformedPearson::transformFromBase(double y) const {
    return location_ + scale_ * y;
}

// Вычисление плотности
double TransformedPearson::computeDensity(double x) const {
    if (!is_parameters_valid_) {
        throw TransformedPearsonException("Invalid parameters for density computation");
    }
    
    try {
        double base_x = transformToBase(x);
        double base_density = base_distribution_->computeDensity(base_x);
        return base_density / scale_;
    } catch (const PearsonException& e) {
        throw TransformedPearsonException(std::string("Base distribution error: ") + e.what());
    }
}

// Математическое ожидание
double TransformedPearson::computeExpectation() const {
    if (!is_parameters_valid_) {
        throw TransformedPearsonException("Invalid parameters for expectation computation");
    }
    
    try {
        double base_expectation = base_distribution_->computeExpectation();
        return transformFromBase(base_expectation);
    } catch (const PearsonException& e) {
        throw TransformedPearsonException(std::string("Base distribution error: ") + e.what());
    }
}

// Дисперсия
double TransformedPearson::computeVariance() const {
    if (!is_parameters_valid_) {
        throw TransformedPearsonException("Invalid parameters for variance computation");
    }
    
    try {
        double base_variance = base_distribution_->computeVariance();
        return scale_ * scale_ * base_variance;
    } catch (const PearsonException& e) {
        throw TransformedPearsonException(std::string("Base distribution error: ") + e.what());
    }
}

// Коэффициент асимметрии
double TransformedPearson::computeSkewness() const {
    if (!is_parameters_valid_) {
        throw TransformedPearsonException("Invalid parameters for skewness computation");
    }
    
    try {
        // Коэффициент асимметрии инвариантен относительно линейного преобразования
        return base_distribution_->computeSkewness();
    } catch (const PearsonException& e) {
        throw TransformedPearsonException(std::string("Base distribution error: ") + e.what());
    }
}

// Коэффициент эксцесса
double TransformedPearson::computeKurtosis() const {
    if (!is_parameters_valid_) {
        throw TransformedPearsonException("Invalid parameters for kurtosis computation");
    }
    
    try {
        // Коэффициент эксцесса инвариантен относительно линейного преобразования
        return base_distribution_->computeKurtosis();
    } catch (const PearsonException& e) {
        throw TransformedPearsonException(std::string("Base distribution error: ") + e.what());
    }
}

// Генерация случайной величины
double TransformedPearson::generateRandom() const {
    if (!is_parameters_valid_) {
        throw TransformedPearsonException("Invalid parameters for random generation");
    }
    
    try {
        double base_random = base_distribution_->generateRandom();
        return transformFromBase(base_random);
    } catch (const PearsonException& e) {
        throw TransformedPearsonException(std::string("Base distribution error: ") + e.what());
    }
}

// Сохранение в файл
void TransformedPearson::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw TransformedPearsonException("Cannot open file for writing: " + filename);
    }
    
    file << location_ << " " << scale_ << " " << shape_ << std::endl;
    file.close();
}

// Загрузка из файла
void TransformedPearson::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw TransformedPearsonException("Cannot open file for reading: " + filename);
    }
    
    double loc, scale, shape;
    if (!(file >> loc >> scale >> shape)) {
        throw TransformedPearsonException("Invalid file format");
    }
    
    setParameters(loc, scale, shape);
    file.close();
}
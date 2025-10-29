/**
 * @file    pirson.h
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Header file for Pearson-VII distribution class.
 */

#ifndef PEARSON_H
#define PEARSON_H

#include <string>
#include <stdexcept>

class PearsonVII {
private:
    // Обязательные атрибуты
    double location_;    // параметр сдвига
    double scale_;       // параметр масштаба  
    double shape_;       // параметр формы
    
    // Вспомогательные атрибуты для эффективности вычислений
    double normalization_constant_;
    bool are_parameters_valid_;
    
    // Вспомогательные методы для вычислений
    void updateNormalizationConstant();
    bool validateParameters() const;

public:
    // Конструкторы
    PearsonVII(double location = 0.0, double scale = 1.0, double shape = 2.0);
    PearsonVII(const std::string& filename); // инициализация из файла
    
    // Set-функции
    void setLocation(double location);
    void setScale(double scale);
    void setShape(double shape);
    void setParameters(double location, double scale, double shape);
    
    // Get-функции
    double getLocation() const;
    double getScale() const; 
    double getShape() const;
    
    // Основной функционал
    double computeDensity(double x) const;
    double computeExpectation() const;
    double computeVariance() const;
    double computeSkewness() const;
    double computeKurtosis() const;
    double generateRandom() const;
    
    // Функции персистентности
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    
    // Валидация
    bool isValid() const;
};

// Исключения для класса
class PearsonException : public std::runtime_error {
public:
    explicit PearsonException(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif
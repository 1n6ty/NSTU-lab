/**
 * @file    alt_pirson.h
 * @version 2.0
 *
 * @section DESCRIPTION
 *
 *          Header file for transformed Pearson-VII distribution class.
 */

#ifndef ALT_PEARSON_H
#define ALT_PEARSON_H

#include "pirson.h"
#include <string>
#include <stdexcept>
#include <memory>

class TransformedPearson {
private:
    // Обязательные атрибуты
    double location_;    // параметр сдвига
    double scale_;       // параметр масштаба
    double shape_;       // параметр формы
    
    // Вспомогательные атрибуты
    std::unique_ptr<PearsonVII> base_distribution_;
    bool is_parameters_valid_;
    
    // Вспомогательные методы
    void updateBaseDistribution();
    bool validateParameters() const;

public:
    // Конструкторы
    TransformedPearson(double location = 0.0, double scale = 1.0, double shape = 2.0);
    TransformedPearson(const std::string& filename);
    
    // Запрет копирования (из-за unique_ptr)
    TransformedPearson(const TransformedPearson& other);
    TransformedPearson& operator=(const TransformedPearson& other);
    
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
    
    // Специфичные методы для преобразованного распределения
    double transformToBase(double x) const;
    double transformFromBase(double y) const;
};

// Исключения для класса
class TransformedPearsonException : public std::runtime_error {
public:
    explicit TransformedPearsonException(const std::string& message) 
        : std::runtime_error(message) {}
};

#endif
using System;
using System.Globalization;

namespace PNumberCalculator.Models
{
    public class TPNumber : ICloneable
    {
        private double _value;          // значение числа в десятичном виде
        private int _base;              // основание системы счисления (2-16)
        private int _precision; 

        private const string DIGITS = "0123456789ABCDEF";
        private const int DEFAULT_PRECISION = 10;
        private const int DEFAULT_BASE = 10;

        public int Base
        {
            get => _base;
            set
            {
                if (value < 2 || value > 16)
                    throw new ArgumentException("Основание должно быть в диапазоне 2..16");
                _base = value;
            }
        }

        public int Precision
        {
            get => _precision;
            set
            {
                if (value < 0)
                    throw new ArgumentException("Точность не может быть отрицательной");
                _precision = value;
            }
        }

        public double Value => _value;

        public TPNumber()
        {
            _value = 0;
            _base = DEFAULT_BASE;
            _precision = DEFAULT_PRECISION;
        }

        // Конструктор из числа и основания
        public TPNumber(double value, int baseNum, int precision = DEFAULT_PRECISION)
        {
            Base = baseNum;
            Precision = precision;
            _value = value;
        }

        // Конструктор из строки
        public TPNumber(string strValue, int baseNum, int precision = DEFAULT_PRECISION)
        {
            Base = baseNum;
            Precision = precision;
            _value = ParseString(strValue, baseNum);
        }

        // Парсинг строки в заданной системе счисления
        private double ParseString(string str, int baseNum)
        {
            if (string.IsNullOrEmpty(str))
                return 0;

            str = str.Trim().ToUpper();
            bool isNegative = str.StartsWith("-");
            if (isNegative)
                str = str.Substring(1);

            // Находим разделитель дробной части
            char decimalSeparator = GetDecimalSeparator();
            int separatorIndex = str.IndexOf(decimalSeparator);

            string intPart = separatorIndex >= 0 ? str.Substring(0, separatorIndex) : str;
            string fracPart = separatorIndex >= 0 ? str.Substring(separatorIndex + 1) : "";

            double result = 0;

            // Целая часть
            for (int i = 0; i < intPart.Length; i++)
            {
                char c = intPart[intPart.Length - 1 - i];
                int digit = DIGITS.IndexOf(c);
                if (digit < 0 || digit >= baseNum)
                    throw new ArgumentException($"Недопустимый символ '{c}' для основания {baseNum}");
                result += digit * Math.Pow(baseNum, i);
            }

            // Дробная часть
            for (int i = 0; i < fracPart.Length; i++)
            {
                char c = fracPart[i];
                int digit = DIGITS.IndexOf(c);
                if (digit < 0 || digit >= baseNum)
                    throw new ArgumentException($"Недопустимый символ '{c}' для основания {baseNum}");
                result += digit * Math.Pow(baseNum, -(i + 1));
            }

            return isNegative ? -result : result;
        }

        private char GetDecimalSeparator()
        {
            return ',';
        }

        // Преобразование в строку
        public override string ToString()
        {
            return ConvertToString(_value, _base, _precision);
        }

        private string ConvertToString(double value, int baseNum, int precision)
        {
            if (value == 0)
                return "0";

            bool isNegative = value < 0;
            double absValue = Math.Abs(value);

            long intPart = (long)Math.Floor(absValue);
            double fracPart = absValue - intPart;

            string result = "";

            // Целая часть
            if (intPart == 0)
            {
                result = "0";
            }
            else
            {
                while (intPart > 0)
                {
                    int remainder = (int)(intPart % baseNum);
                    result = DIGITS[remainder] + result;
                    intPart /= baseNum;
                }
            }

            // Дробная часть
            if (precision > 0 && fracPart > 0)
            {
                result += ",";
                for (int i = 0; i < precision && fracPart > 0; i++)
                {
                    fracPart *= baseNum;
                    int digit = (int)Math.Floor(fracPart);
                    result += DIGITS[digit];
                    fracPart -= digit;
                }
            }

            return isNegative ? "-" + result : result;
        }

        public TPNumber Add(TPNumber other)
        {
            CheckCompatibility(other);
            return new TPNumber(_value + other._value, _base, _precision);
        }

        public TPNumber Subtract(TPNumber other)
        {
            CheckCompatibility(other);
            return new TPNumber(_value - other._value, _base, _precision);
        }

        public TPNumber Multiply(TPNumber other)
        {
            CheckCompatibility(other);
            return new TPNumber(_value * other._value, _base, _precision);
        }

        public TPNumber Divide(TPNumber other)
        {
            CheckCompatibility(other);
            if (Math.Abs(other._value) < 1e-10)
                throw new DivideByZeroException("Деление на ноль");
            return new TPNumber(_value / other._value, _base, _precision);
        }

        public TPNumber Square()
        {
            return new TPNumber(_value * _value, _base, _precision);
        }

        public TPNumber Inverse()
        {
            if (Math.Abs(_value) < 1e-10)
                throw new DivideByZeroException("Обратное значение от нуля не существует");
            return new TPNumber(1.0 / _value, _base, _precision);
        }

        public TPNumber Copy()
        {
            return new TPNumber(_value, _base, _precision);
        }

        private void CheckCompatibility(TPNumber other)
        {
            if (other._base != _base)
                throw new ArgumentException($"Основания не совпадают: {_base} и {other._base}");
        }

        public object Clone()
        {
            return Copy();
        }

        public void ChangeBase(int newBase)
        {
            Base = newBase;
        }
    }
}
using System;
using System.Globalization;

namespace UniversalCalculator.Models
{
    public class TPNumber : TANumber
    {
        private double _value;
        private int _base;
        private int _precision;
        private const string DIGITS = "0123456789ABCDEF";
        
        public int Base => _base;
        public int Precision => _precision;

        public TPNumber() : this(0, 10, 10) { }

        public TPNumber(double value, int baseNum, int precision = 10)
        {
            _base = (baseNum >= 2 && baseNum <= 16) ? baseNum : throw new ArgumentException("Основание 2..16");
            _precision = precision;
            _value = value;
        }

        public TPNumber(double value) : this(value, 10, 10) { }

        public TPNumber(string str, int baseNum, int precision = 10)
        {
            _base = (baseNum >= 2 && baseNum <= 16) ? baseNum : throw new ArgumentException("Основание 2..16");
            _precision = precision;
            _value = ParseString(str);
        }

        private double ParseString(string str)
        {
            if (string.IsNullOrWhiteSpace(str)) return 0;
            str = str.Trim().ToUpper();
            bool neg = str.StartsWith("-");
            if (neg) str = str.Substring(1);
            
            var parts = str.Split(',');
            double result = 0;
            
            // Целая часть
            for (int i = 0; i < parts[0].Length; i++)
            {
                int digit = DIGITS.IndexOf(parts[0][parts[0].Length - 1 - i]);
                if (digit < 0 || digit >= _base) throw new FormatException("Неверный символ");
                result += digit * Math.Pow(_base, i);
            }
            
            // Дробная часть
            if (parts.Length > 1)
            {
                for (int i = 0; i < parts[1].Length; i++)
                {
                    int digit = DIGITS.IndexOf(parts[1][i]);
                    if (digit < 0 || digit >= _base) throw new FormatException("Неверный символ");
                    result += digit * Math.Pow(_base, -(i + 1));
                }
            }
            
            return neg ? -result : result;
        }

        public override string ToString()
        {
            if (Math.Abs(_value) < 1e-12) return "0";
            
            bool neg = _value < 0;
            double abs = Math.Abs(_value);
            long intPart = (long)abs;
            double frac = abs - intPart;
            
            string res = "";
            
            // Целая часть
            if (intPart == 0)
            {
                res = "0";
            }
            else
            {
                while (intPart > 0)
                {
                    res = DIGITS[(int)(intPart % _base)] + res;
                    intPart /= _base;
                }
            }
            
            // Дробная часть
            if (frac > 0 && _precision > 0)
            {
                res += ",";
                for (int i = 0; i < _precision && Math.Abs(frac) > 1e-12; i++)
                {
                    frac *= _base;
                    int d = (int)frac;
                    res += DIGITS[d];
                    frac -= d;
                }
            }
            
            return neg ? "-" + res : res;
        }

        public override void FromString(string str)
        {
            _value = ParseString(str);
        }

        public double GetDecimalValue()
        {
            return _value;
        }

        public override TANumber Add(TANumber other)
        {
            var o = (TPNumber)other;
            // Приводим к одному основанию, если нужно
            if (_base != o._base)
            {
                throw new ArgumentException("Основания должны совпадать");
            }
            return new TPNumber(_value + o._value, _base, _precision);
        }
        
        public override TANumber Subtract(TANumber other)
        {
            var o = (TPNumber)other;
            if (_base != o._base)
            {
                throw new ArgumentException("Основания должны совпадать");
            }
            return new TPNumber(_value - o._value, _base, _precision);
        }
        
        public override TANumber Multiply(TANumber other)
        {
            var o = (TPNumber)other;
            if (_base != o._base)
            {
                throw new ArgumentException("Основания должны совпадать");
            }
            return new TPNumber(_value * o._value, _base, _precision);
        }
        
        public override TANumber Divide(TANumber other)
        {
            var o = (TPNumber)other;
            if (_base != o._base)
            {
                throw new ArgumentException("Основания должны совпадать");
            }
            if (Math.Abs(o._value) < 1e-12) throw new DivideByZeroException();
            return new TPNumber(_value / o._value, _base, _precision);
        }
        
        public override TANumber Square()
        {
            return new TPNumber(_value * _value, _base, _precision);
        }
        
        public override TANumber Inverse()
        {
            if (Math.Abs(_value) < 1e-12) throw new DivideByZeroException();
            return new TPNumber(1.0 / _value, _base, _precision);
        }
        
        public override bool IsZero()
        {
            return Math.Abs(_value) < 1e-12;
        }
        
        public override TANumber Copy()
        {
            return new TPNumber(_value, _base, _precision);
        }
        
        public void ChangeBase(int newBase)
        {
            if (newBase >= 2 && newBase <= 16)
            {
                _base = newBase;
            }
        }
    }
}
using System;
using System.Text;

namespace PNumberCalculator.Models
{
    public class Editor
    {
        private StringBuilder _currentNumber;
        private int _currentBase;
        private const string DIGITS = "0123456789ABCDEF";
        private const char DECIMAL_SEPARATOR = ',';

        public string CurrentNumber => _currentNumber.ToString();
        public int CurrentBase => _currentBase;

        public Editor(int baseNum = 10)
        {
            _currentBase = baseNum;
            _currentNumber = new StringBuilder("0");
        }

        public void SetBase(int baseNum)
        {
            if (baseNum < 2 || baseNum > 16)
                throw new ArgumentException("Основание должно быть в диапазоне 2..16");

            if (_currentBase != baseNum && !IsZero())
            {
                try
                {
                    TPNumber temp = new TPNumber(_currentNumber.ToString(), _currentBase);
                    temp.ChangeBase(baseNum);
                    _currentNumber.Clear();
                    _currentNumber.Append(temp.ToString());
                }
                catch
                {
                    Clear();
                }
            }
            _currentBase = baseNum;
        }

        public bool IsZero()
        {
            string str = _currentNumber.ToString().TrimStart('-');
            if (str == "0") return true;
            
            foreach (char c in str)
            {
                if (c != '0' && c != DECIMAL_SEPARATOR)
                    return false;
            }
            return true;
        }

        public string AddDigit(int digit)
        {
            if (digit < 0 || digit >= _currentBase)
                return _currentNumber.ToString();

            char digitChar = DIGITS[digit];

            if (_currentNumber.ToString() == "0")
            {
                _currentNumber.Clear();
                _currentNumber.Append(digitChar);
            }
            else if (_currentNumber.ToString() == "-0")
            {
                _currentNumber.Clear();
                _currentNumber.Append("-" + digitChar);
            }
            else
            {
                _currentNumber.Append(digitChar);
            }

            return _currentNumber.ToString();
        }

        public string AddZero()
        {
            return AddDigit(0);
        }

        public string AddSign()
        {
            string str = _currentNumber.ToString();
            if (str.StartsWith("-"))
                _currentNumber.Remove(0, 1);
            else if (str != "0")
                _currentNumber.Insert(0, '-');

            return _currentNumber.ToString();
        }

        public string AddSeparator()
        {
            string str = _currentNumber.ToString();
            string checkStr = str.StartsWith("-") ? str.Substring(1) : str;
            
            if (!checkStr.Contains(DECIMAL_SEPARATOR.ToString()))
            {
                _currentNumber.Append(DECIMAL_SEPARATOR);
            }

            return _currentNumber.ToString();
        }

        public string Backspace()
        {
            string str = _currentNumber.ToString();
            if (str.Length > 1)
            {
                _currentNumber.Remove(_currentNumber.Length - 1, 1);
            }
            else
            {
                _currentNumber.Clear();
                _currentNumber.Append("0");
            }

            if (_currentNumber.ToString() == "-")
            {
                _currentNumber.Clear();
                _currentNumber.Append("0");
            }

            return _currentNumber.ToString();
        }

        public string Clear()
        {
            _currentNumber.Clear();
            _currentNumber.Append("0");
            return _currentNumber.ToString();
        }

        public void SetNumber(string number)
        {
            _currentNumber.Clear();
            _currentNumber.Append(number);
        }

        public bool IsValidChar(char c)
        {
            c = char.ToUpper(c);

            if (c == DECIMAL_SEPARATOR)
                return !_currentNumber.ToString().Contains(DECIMAL_SEPARATOR);

            if (c == '-')
                return _currentNumber.ToString() == "0";

            int digit = DIGITS.IndexOf(c);
            return digit >= 0 && digit < _currentBase;
        }

        public string ProcessChar(char c)
        {
            c = char.ToUpper(c);

            if (c == DECIMAL_SEPARATOR)
                return AddSeparator();
            else if (c == '-')
                return AddSign();
            else
            {
                int digit = DIGITS.IndexOf(c);
                if (digit >= 0 && digit < _currentBase)
                    return AddDigit(digit);
            }

            return _currentNumber.ToString();
        }
    }
}
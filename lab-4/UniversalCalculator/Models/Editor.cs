using System.Text;

namespace UniversalCalculator.Models
{
    public class Editor
    {
        private StringBuilder _current;
        private CalculatorMode _mode;
        private int _base;
        private const string DIGITS = "0123456789ABCDEF";

        public string CurrentNumber => _current.ToString();
        public int CurrentBase => _base;
        public CalculatorMode Mode => _mode;

        public Editor(CalculatorMode mode = CalculatorMode.PNumber, int baseNum = 10)
        {
            _mode = mode;
            _base = (baseNum >= 2 && baseNum <= 16) ? baseNum : 10;
            _current = new StringBuilder("0");
        }

        public void SetMode(CalculatorMode mode, int baseNum = 10)
        {
            _mode = mode;
            if (mode == CalculatorMode.PNumber)
            {
                _base = (baseNum >= 2 && baseNum <= 16) ? baseNum : 10;
            }
            Clear();
        }

        public void SetBase(int b)
        {
            if (b >= 2 && b <= 16 && _mode == CalculatorMode.PNumber)
            {
                _base = b;
            }
        }

        public bool IsZero()
        {
            string s = _current.ToString().TrimStart('-');
            foreach (char c in s)
                if (c != '0' && c != ',' && c != '/' && c != 'i') return false;
            return true;
        }

        public void AddDigit(int digit)
        {
            if (_mode == CalculatorMode.PNumber && digit >= _base) return;
            if (digit < 0 || digit > 15) return;

            char ch = DIGITS[digit];
            string str = _current.ToString();
            
            // Для дробей: если уже есть /, то добавляем цифру к знаменателю
            if (_mode == CalculatorMode.Fraction && str.Contains("/"))
            {
                int slashPos = str.IndexOf('/');
                _current.Insert(slashPos + 1, ch);
                return;
            }
            
            // Для комплексных чисел: если уже есть i, то добавляем цифру к коэффициенту
            if (_mode == CalculatorMode.Complex && str.Contains("i"))
            {
                int iPos = str.IndexOf('i');
                _current.Insert(iPos, ch);
                return;
            }
            
            if (str == "0")
            {
                _current.Clear();
                _current.Append(ch);
            }
            else if (str == "-0")
            {
                _current.Clear();
                _current.Append('-' + ch.ToString());
            }
            else
            {
                _current.Append(ch);
            }
        }

        public void AddSign()
        {
            string str = _current.ToString();
            
            if (_mode == CalculatorMode.Fraction && str.Contains("/"))
            {
                // Меняем знак у всей дроби
                if (str.StartsWith("-"))
                    _current.Remove(0, 1);
                else if (str != "0" && str != "0/")
                    _current.Insert(0, '-');
                return;
            }
            
            if (_mode == CalculatorMode.Complex && str.Contains("i"))
            {
                int iPos = str.IndexOf('i');
                int startPos = iPos - 1;
                while (startPos >= 0 && (char.IsDigit(str[startPos]) || str[startPos] == ',' || str[startPos] == '.'))
                {
                    startPos--;
                }
                startPos++;
                
                if (startPos >= 0)
                {
                    if (startPos > 0 && str[startPos - 1] == '-')
                    {
                        _current.Remove(startPos - 1, 1);
                    }
                    else
                    {
                        _current.Insert(startPos, '-');
                    }
                }
            }
            else if (_mode == CalculatorMode.Complex && !str.Contains("i"))
            {
                if (str.StartsWith("-"))
                    _current.Remove(0, 1);
                else if (str != "0")
                    _current.Insert(0, '-');
            }
            else
            {
                if (str.StartsWith("-"))
                    _current.Remove(0, 1);
                else if (str != "0")
                    _current.Insert(0, '-');
            }
        }

        public void AddSeparator()
        {
            string str = _current.ToString();
            
            switch (_mode)
            {
                case CalculatorMode.Fraction:
                    // В режиме дробей separator - это дробная черта
                    if (!str.Contains("/"))
                        _current.Append("/");
                    break;
                case CalculatorMode.Complex:
                    if (!str.Contains(","))
                        _current.Append(",");
                    break;
                default:
                    if (!str.Contains(","))
                        _current.Append(",");
                    break;
            }
        }

        public void AddImaginaryUnit()
        {
            if (_mode != CalculatorMode.Complex)
                return;
            
            string str = _current.ToString();
            
            if (str.Contains("i"))
                return;
            
            if (str == "0" || str == "-0")
            {
                _current.Clear();
                _current.Append("i");
            }
            else
            {
                _current.Append("i");
            }
        }

        public void Backspace()
        {
            if (_current.Length > 1)
            {
                string str = _current.ToString();
                
                if (_mode == CalculatorMode.Fraction && str.EndsWith("/"))
                {
                    _current.Remove(_current.Length - 1, 1);
                }
                else if (_mode == CalculatorMode.Complex && str.EndsWith("i"))
                {
                    _current.Remove(_current.Length - 1, 1);
                    string result = _current.ToString();
                    if (string.IsNullOrEmpty(result) || result == "+" || result == "-" || result == "")
                    {
                        _current.Clear();
                        _current.Append("0");
                    }
                }
                else
                {
                    _current.Remove(_current.Length - 1, 1);
                }
            }
            else
            {
                _current.Clear();
                _current.Append("0");
            }
            
            string finalStr = _current.ToString();
            if (finalStr == "-" || finalStr == "+" || finalStr == "/" || string.IsNullOrEmpty(finalStr))
            {
                _current.Clear();
                _current.Append("0");
            }
        }

        public void Clear()
        {
            _current.Clear();
            _current.Append("0");
        }

        public void SetNumber(string s)
        {
            if (string.IsNullOrEmpty(s))
            {
                Clear();
                return;
            }
            _current.Clear();
            _current.Append(s);
        }
    }
}
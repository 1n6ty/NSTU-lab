using System;

namespace UniversalCalculator.Models
{
    public class TComplex : TANumber
    {
        private double _re;
        private double _im;

        public TComplex() : this(0, 0) { }
        public TComplex(double re, double im) { _re = re; _im = im; }
        public TComplex(string str) { FromString(str); }

        public override string ToString()
        {
            if (Math.Abs(_im) < 1e-12)
                return FormatReal(_re);
            if (Math.Abs(_re) < 1e-12)
                return FormatImaginary(_im);
            
            string sign = _im >= 0 ? "+" : "-";
            return $"{FormatReal(_re)}{sign}{FormatImaginary(Math.Abs(_im))}";
        }

        private string FormatReal(double value)
        {
            if (Math.Abs(value - Math.Round(value)) < 1e-10)
                return ((int)Math.Round(value)).ToString();
            return value.ToString(System.Globalization.CultureInfo.InvariantCulture);
        }

        private string FormatImaginary(double value)
        {
            if (Math.Abs(value - 1) < 1e-12)
                return "i";
            if (Math.Abs(value - (-1)) < 1e-12)
                return "-i";
            if (Math.Abs(value - Math.Round(value)) < 1e-10)
                return $"{(int)Math.Round(value)}i";
            return $"{value.ToString(System.Globalization.CultureInfo.InvariantCulture)}i";
        }

        public override void FromString(string str)
        {
            str = str.Replace(" ", "");
            if (string.IsNullOrEmpty(str))
            {
                _re = 0;
                _im = 0;
                return;
            }
            
            // Поддерживаем форматы: "a+bi", "a-bi", "bi", "a", "i", "-i", "xi" (где x - число)
            int iPos = str.IndexOf('i');
            if (iPos == -1)
            {
                // Только действительное число
                _re = double.Parse(str, System.Globalization.CultureInfo.InvariantCulture);
                _im = 0;
                return;
            }
            
            // Получаем часть перед i
            string beforeI = str.Substring(0, iPos);
            
            // Если перед i ничего нет (просто "i" или "+i" или "-i")
            if (string.IsNullOrEmpty(beforeI) || beforeI == "+")
            {
                _re = 0;
                _im = 1;
                return;
            }
            if (beforeI == "-")
            {
                _re = 0;
                _im = -1;
                return;
            }
            
            // Проверяем, есть ли знак + или - (для формата a+bi)
            int lastPlus = beforeI.LastIndexOf('+');
            int lastMinus = beforeI.LastIndexOf('-');
            
            if (lastPlus > 0 || lastMinus > 0)
            {
                // Формат "a+bi" или "a-bi"
                int signPos = Math.Max(lastPlus, lastMinus);
                string realPart = beforeI.Substring(0, signPos);
                string imagPart = beforeI.Substring(signPos);
                
                _re = double.Parse(realPart, System.Globalization.CultureInfo.InvariantCulture);
                
                if (string.IsNullOrEmpty(imagPart) || imagPart == "+" || imagPart == "-")
                {
                    _im = (imagPart == "-") ? -1 : 1;
                }
                else
                {
                    string imagNum = imagPart.TrimStart('+', '-');
                    if (string.IsNullOrEmpty(imagNum))
                        _im = (imagPart.StartsWith("-") ? -1 : 1);
                    else
                        _im = double.Parse(imagNum, System.Globalization.CultureInfo.InvariantCulture);
                    if (imagPart.StartsWith("-"))
                        _im = -_im;
                }
            }
            else
            {
                // Формат "xi" - чисто мнимое число
                _re = 0;
                _im = double.Parse(beforeI, System.Globalization.CultureInfo.InvariantCulture);
            }
        }

        public override TANumber Add(TANumber other) 
        { 
            var o = (TComplex)other; 
            return new TComplex(_re + o._re, _im + o._im); 
        }
        
        public override TANumber Subtract(TANumber other) 
        { 
            var o = (TComplex)other; 
            return new TComplex(_re - o._re, _im - o._im); 
        }
        
        public override TANumber Multiply(TANumber other) 
        { 
            var o = (TComplex)other; 
            return new TComplex(_re * o._re - _im * o._im, _re * o._im + _im * o._re); 
        }
        
        public override TANumber Divide(TANumber other)
        {
            var o = (TComplex)other;
            double d = o._re * o._re + o._im * o._im;
            if (Math.Abs(d) < 1e-12) throw new DivideByZeroException();
            return new TComplex((_re * o._re + _im * o._im) / d, (_im * o._re - _re * o._im) / d);
        }
        
        public override TANumber Square() 
        { 
            return new TComplex(_re * _re - _im * _im, 2 * _re * _im); 
        }
        
        public override TANumber Inverse()
        {
            double d = _re * _re + _im * _im;
            if (Math.Abs(d) < 1e-12) throw new DivideByZeroException();
            return new TComplex(_re / d, -_im / d);
        }
        
        public override bool IsZero() => Math.Abs(_re) < 1e-12 && Math.Abs(_im) < 1e-12;
        public override TANumber Copy() => new TComplex(_re, _im);
    }
}
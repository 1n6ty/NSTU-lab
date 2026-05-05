using System;

namespace UniversalCalculator.Models
{
    public class TFrac : TANumber
    {
        private long _num;
        private long _den;

        public TFrac() : this(0, 1) { }
        public TFrac(long num, long den)
        {
            if (den == 0) throw new ArgumentException("Знаменатель не может быть 0");
            _num = num;
            _den = den;
            Normalize();
        }

        public TFrac(string str)
        {
            FromString(str);
        }

        private void Normalize()
        {
            if (_den < 0) { _num = -_num; _den = -_den; }
            long g = Gcd(Math.Abs(_num), _den);
            _num /= g;
            _den /= g;
        }

        private static long Gcd(long a, long b) => b == 0 ? a : Gcd(b, a % b);

        public override string ToString() => _den == 1 ? _num.ToString() : $"{_num}/{_den}";

        public override void FromString(string str)
        {
            var parts = str.Split('/');
            _num = long.Parse(parts[0]);
            _den = parts.Length > 1 ? long.Parse(parts[1]) : 1;
            Normalize();
        }

        public override TANumber Add(TANumber other)
        {
            var o = (TFrac)other;
            return new TFrac(_num * o._den + o._num * _den, _den * o._den);
        }
        public override TANumber Subtract(TANumber other)
        {
            var o = (TFrac)other;
            return new TFrac(_num * o._den - o._num * _den, _den * o._den);
        }
        public override TANumber Multiply(TANumber other)
        {
            var o = (TFrac)other;
            return new TFrac(_num * o._num, _den * o._den);
        }
        public override TANumber Divide(TANumber other)
        {
            var o = (TFrac)other;
            if (o._num == 0) throw new DivideByZeroException();
            return new TFrac(_num * o._den, _den * o._num);
        }
        public override TANumber Square() => new TFrac(_num * _num, _den * _den);
        public override TANumber Inverse()
        {
            if (_num == 0) throw new DivideByZeroException();
            return new TFrac(_den, _num);
        }
        public override bool IsZero() => _num == 0;
        public override TANumber Copy() => new TFrac(_num, _den);
    }
}
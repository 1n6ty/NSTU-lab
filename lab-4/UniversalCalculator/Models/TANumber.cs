using System;

namespace UniversalCalculator.Models
{
    public abstract class TANumber : ICloneable
    {
        public abstract TANumber Add(TANumber other);
        public abstract TANumber Subtract(TANumber other);
        public abstract TANumber Multiply(TANumber other);
        public abstract TANumber Divide(TANumber other);
        public abstract TANumber Square();
        public abstract TANumber Inverse();
        public abstract bool IsZero();
        public abstract TANumber Copy();
        public override abstract string ToString();
        public abstract void FromString(string str);

        public object Clone() => Copy();
    }
}
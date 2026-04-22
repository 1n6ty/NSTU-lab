using System;

namespace PNumberCalculator.Models
{
    public enum Operation
    {
        None,
        Add,
        Subtract,
        Multiply,
        Divide
    }

    public enum Function
    {
        Square,
        Inverse
    }

    public class Processor<T> where T : ICloneable, new()
    {
        private T _lopRes; 
        private T _rop;   
        private Operation _operation;

        public Operation CurrentOperation => _operation;
        public T LeftOperand => _lopRes;
        public T RightOperand => _rop;

        public Processor()
        {
            _lopRes = new T();
            _rop = new T();
            _operation = Operation.None;
        }
        public void Reset()
        {
            _lopRes = new T();
            _rop = new T();
            _operation = Operation.None;
        }

        public void ClearOperation()
        {
            _operation = Operation.None;
        }

        public void SetOperation(Operation op)
        {
            _operation = op;
        }

        public void SetLeftOperand(T value)
        {
            _lopRes = (T)value.Clone();
        }

        public void SetRightOperand(T value)
        {
            _rop = (T)value.Clone();
        }

        public T ExecuteOperation()
        {
            if (_operation == Operation.None)
                return (T)_lopRes.Clone();

            dynamic left = _lopRes;
            dynamic right = _rop;
            dynamic result;

            switch (_operation)
            {
                case Operation.Add:
                    result = left.Add(right);
                    break;
                case Operation.Subtract:
                    result = left.Subtract(right);
                    break;
                case Operation.Multiply:
                    result = left.Multiply(right);
                    break;
                case Operation.Divide:
                    result = left.Divide(right);
                    break;
                default:
                    result = left;
                    break;
            }

            _lopRes = (T)result.Clone();
            return (T)_lopRes.Clone();
        }

        public T ExecuteFunction(Function func)
        {
            dynamic operand = _rop;
            dynamic result;

            switch (func)
            {
                case Function.Square:
                    result = operand.Square();
                    break;
                case Function.Inverse:
                    result = operand.Inverse();
                    break;
                default:
                    result = operand;
                    break;
            }

            _rop = (T)result.Clone();
            return (T)_rop.Clone();
        }
    }
}
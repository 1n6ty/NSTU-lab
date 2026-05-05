using System;

namespace UniversalCalculator.Models
{
    public class Processor<T> where T : TANumber, new()
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

        public void ClearOperation() => _operation = Operation.None;
        public void SetOperation(Operation op) => _operation = op;

        public void SetLeftOperand(T value) => _lopRes = (T)value.Copy();
        public void SetRightOperand(T value) => _rop = (T)value.Copy();

        public T ExecuteOperation()
        {
            if (_operation == Operation.None) return (T)_lopRes.Copy();

            TANumber result = _operation switch
            {
                Operation.Add => _lopRes.Add(_rop),
                Operation.Subtract => _lopRes.Subtract(_rop),
                Operation.Multiply => _lopRes.Multiply(_rop),
                Operation.Divide => _lopRes.Divide(_rop),
                _ => _lopRes.Copy()
            };

            _lopRes = (T)result.Copy();
            return (T)_lopRes.Copy();
        }

        public T ExecuteFunction(Function func)
        {
            TANumber result = func switch
            {
                Function.Square => _rop.Square(),
                Function.Inverse => _rop.Inverse(),
                _ => _rop.Copy()
            };

            _rop = (T)result.Copy();
            return (T)_rop.Copy();
        }
    }
}
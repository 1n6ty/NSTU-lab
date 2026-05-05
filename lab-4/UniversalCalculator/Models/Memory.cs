using System;


namespace UniversalCalculator.Models
{
    public class Memory<T> where T : TANumber, new()
    {
        public enum MemoryState { Off, On }

        private T _value;
        private MemoryState _state;

        public MemoryState State => _state;
        public T Value => _value;

        public Memory()
        {
            _value = new T();
            _state = MemoryState.Off;
        }

        public void Clear()
        {
            _value = new T();
            _state = MemoryState.Off;
        }

        public void Store(T value)
        {
            _value = (T)value.Clone();
            _state = MemoryState.On;
        }

        public T Retrieve() => (T)_value.Clone();

        public void Add(T value)
        {
                dynamic dynNumber = _value;
                dynamic dynValue = value;
                try
                {
                    _value = dynNumber.Add(dynValue);
                }
                catch
                {
                    try
                    {
                        _value = dynNumber + dynValue;
                    }
                    catch
                    {
                        throw new InvalidOperationException("Тип T должен поддерживать операцию сложения");
                    }
                }
                _state = MemoryState.On;
        }

        public string GetStateString() => _state == MemoryState.On ? "M" : "";
    }
}
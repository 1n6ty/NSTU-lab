using System;

namespace PNumberCalculator.Models
{
    public class Memory<T> where T : ICloneable, new()
    {
        public enum MemoryState
        {
            Off,
            On
        }

        private T _number;
        private MemoryState _state;

        public MemoryState State => _state;
        public T Number => _number;

        public Memory()
        {
            _number = new T();
            _state = MemoryState.Off;
        }

        // MC - очистить память
        public void Clear()
        {
            _number = new T();
            _state = MemoryState.Off;
        }

        // MS - сохранить в память
        public void Store(T value)
        {
            _number = (T)value.Clone();
            _state = MemoryState.On;
        }

        // MR - взять из памяти
        public T Retrieve()
        {
            return (T)_number.Clone();
        }

        // M+ - добавить к памяти
        public void Add(T value)
        {
            dynamic dynNumber = _number;
            dynamic dynValue = value;
            try
            {
                _number = dynNumber.Add(dynValue);
            }
            catch
            {
                try
                {
                    _number = dynNumber + dynValue;
                }
                catch
                {
                    throw new InvalidOperationException("Тип T должен поддерживать операцию сложения");
                }
            }
            _state = MemoryState.On;
        }

        public string GetStateString()
        {
            return _state == MemoryState.On ? "M" : "";
        }
    }
}
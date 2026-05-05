using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace UniversalCalculator.Models
{
    public class CalculatorControl
    {
        private Editor _editor;
        private object _processor;
        private object _memory;
        private CalculatorState _state;
        private CalculatorMode _mode;
        private List<HistoryItem> _history;
        private bool _waitingForOperand;
        private bool _equalsJustPressed;
        private TANumber _accumulator;
        private Operation _pendingOperation;
        private bool _hasPendingOperation;
        private List<string> _expressionParts;
        
        // Для повторения операции при повторном нажатии =
        private TANumber _lastOperand;
        private Operation _lastOperation;
        private bool _hasLastOperation;

        public string DisplayText => _editor.CurrentNumber;
        public string MemoryStateText => GetMemoryStateString();
        public CalculatorState State => _state;
        public int CurrentBase => _editor.CurrentBase;
        public CalculatorMode Mode => _mode;
        public List<HistoryItem> History => _history;

        public event Action<string> DisplayUpdated;
        public event Action<string> MemoryStateUpdated;
        public event Action<int> BaseChanged;
        public event Action<CalculatorMode> ModeChanged;

        public CalculatorControl(CalculatorMode mode = CalculatorMode.PNumber)
        {
            _mode = mode;
            _editor = new Editor(mode);
            _history = new List<HistoryItem>();
            _expressionParts = new List<string>();
            _waitingForOperand = true;
            _equalsJustPressed = false;
            _pendingOperation = Operation.None;
            _hasPendingOperation = false;
            _hasLastOperation = false;
            InitializeProcessorAndMemory();
            ResetAccumulator();
        }

        private void ResetAccumulator()
        {
            _accumulator = _mode switch
            {
                CalculatorMode.PNumber => new TPNumber(0),
                CalculatorMode.Fraction => new TFrac(0, 1),
                CalculatorMode.Complex => new TComplex(0, 0),
                _ => new TPNumber(0)
            };
        }

        private void InitializeProcessorAndMemory()
        {
            switch (_mode)
            {
                case CalculatorMode.PNumber:
                    _processor = new Processor<TPNumber>();
                    _memory = new Memory<TPNumber>();
                    break;
                case CalculatorMode.Fraction:
                    _processor = new Processor<TFrac>();
                    _memory = new Memory<TFrac>();
                    break;
                case CalculatorMode.Complex:
                    _processor = new Processor<TComplex>();
                    _memory = new Memory<TComplex>();
                    break;
            }
        }

        private string GetMemoryStateString()
        {
            if (_memory == null) return "";
            switch (_mode)
            {
                case CalculatorMode.PNumber:
                    return ((Memory<TPNumber>)_memory).GetStateString();
                case CalculatorMode.Fraction:
                    return ((Memory<TFrac>)_memory).GetStateString();
                case CalculatorMode.Complex:
                    return ((Memory<TComplex>)_memory).GetStateString();
                default:
                    return "";
            }
        }

        private TANumber GetCurrentNumber()
        {
            try
            {
                return _mode switch
                {
                    CalculatorMode.PNumber => new TPNumber(_editor.CurrentNumber, _editor.CurrentBase),
                    CalculatorMode.Fraction => new TFrac(_editor.CurrentNumber),
                    CalculatorMode.Complex => new TComplex(_editor.CurrentNumber),
                    _ => new TPNumber(0)
                };
            }
            catch
            {
                return _mode switch
                {
                    CalculatorMode.PNumber => new TPNumber(0),
                    CalculatorMode.Fraction => new TFrac(0, 1),
                    CalculatorMode.Complex => new TComplex(0, 0),
                    _ => new TPNumber(0)
                };
            }
        }

        private double ConvertToDecimal(string number, int fromBase)
        {
            if (string.IsNullOrWhiteSpace(number)) return 0;
            
            number = number.Trim().ToUpper();
            bool isNegative = number.StartsWith("-");
            if (isNegative) number = number.Substring(1);
            
            var parts = number.Split(',');
            double result = 0;
            
            // Целая часть
            string intPart = parts[0];
            for (int i = 0; i < intPart.Length; i++)
            {
                char c = intPart[intPart.Length - 1 - i];
                int digit = "0123456789ABCDEF".IndexOf(c);
                if (digit < 0 || digit >= fromBase) continue;
                result += digit * Math.Pow(fromBase, i);
            }
            
            // Дробная часть
            if (parts.Length > 1)
            {
                string fracPart = parts[1];
                for (int i = 0; i < fracPart.Length; i++)
                {
                    char c = fracPart[i];
                    int digit = "0123456789ABCDEF".IndexOf(c);
                    if (digit < 0 || digit >= fromBase) continue;
                    result += digit * Math.Pow(fromBase, -(i + 1));
                }
            }
            
            return isNegative ? -result : result;
        }

        private TANumber ExecuteOperation(Operation op, TANumber left, TANumber right)
        {
            return op switch
            {
                Operation.Add => left.Add(right),
                Operation.Subtract => left.Subtract(right),
                Operation.Multiply => left.Multiply(right),
                Operation.Divide => left.Divide(right),
                _ => left.Copy()
            };
        }

        public string GetCurrentExpression()
        {
            if (_expressionParts.Count == 0)
                return "";
            
            string result = "";
            for (int i = 0; i < _expressionParts.Count; i++)
            {
                result += _expressionParts[i];
                if (i < _expressionParts.Count - 1)
                    result += " ";
            }
            
            return result;
        }

        public void ChangeMode(CalculatorMode newMode)
        {
            _mode = newMode;
            _editor.SetMode(newMode, _editor.CurrentBase);
            InitializeProcessorAndMemory();
            ResetAccumulator();
            _state = CalculatorState.Start;
            _waitingForOperand = true;
            _equalsJustPressed = false;
            _expressionParts.Clear();
            _pendingOperation = Operation.None;
            _hasPendingOperation = false;
            _hasLastOperation = false;
            ModeChanged?.Invoke(newMode);
            DisplayUpdated?.Invoke(_editor.CurrentNumber);
            MemoryStateUpdated?.Invoke(GetMemoryStateString());
        }

        public void SetBase(int b)
        {
            if (b < 2 || b > 16 || _mode != CalculatorMode.PNumber) return;
            
            // Сохраняем текущее значение числа перед сменой базы
            string currentNumberStr = _editor.CurrentNumber;
            double decimalValue = 0;
            
            try
            {
                // Конвертируем текущее число в десятичное значение
                decimalValue = ConvertToDecimal(currentNumberStr, _editor.CurrentBase);
            }
            catch
            {
                decimalValue = 0;
            }
            
            // Меняем базу в редакторе
            _editor.SetBase(b);
            
            // Устанавливаем число в новой базе
            if (_editor.CurrentBase != 10)
            {
                try
                {
                    // Создаем новое число в нужной базе
                    var newNumber = new TPNumber(decimalValue, b);
                    _editor.SetNumber(newNumber.ToString());
                }
                catch
                {
                    _editor.Clear();
                }
            }
            else
            {
                // Для десятичной системы просто показываем десятичное значение
                _editor.SetNumber(decimalValue.ToString("F10").TrimEnd('0').TrimEnd(',').TrimEnd('.'));
                if (string.IsNullOrEmpty(_editor.CurrentNumber))
                    _editor.SetNumber("0");
            }
            
            // Пересчитываем аккумулятор, если в нем есть значение
            if (_accumulator is TPNumber accTP && accTP.Base != b)
            {
                double accValue = ConvertToDecimal(accTP.ToString(), accTP.Base);
                _accumulator = new TPNumber(accValue, b);
            }
            
            // Пересчитываем последний операнд, если он есть
            if (_hasLastOperation && _lastOperand is TPNumber lastTP && lastTP.Base != b)
            {
                double lastValue = ConvertToDecimal(lastTP.ToString(), lastTP.Base);
                _lastOperand = new TPNumber(lastValue, b);
            }
            
            BaseChanged?.Invoke(b);
            DisplayUpdated?.Invoke(_editor.CurrentNumber);
        }

        public void ExecuteCommand(CalculatorCommand command)
        {
            try
            {
                switch (command)
                {
                    case CalculatorCommand.Digit0: case CalculatorCommand.Digit1:
                    case CalculatorCommand.Digit2: case CalculatorCommand.Digit3:
                    case CalculatorCommand.Digit4: case CalculatorCommand.Digit5:
                    case CalculatorCommand.Digit6: case CalculatorCommand.Digit7:
                    case CalculatorCommand.Digit8: case CalculatorCommand.Digit9:
                    case CalculatorCommand.DigitA: case CalculatorCommand.DigitB:
                    case CalculatorCommand.DigitC: case CalculatorCommand.DigitD:
                    case CalculatorCommand.DigitE: case CalculatorCommand.DigitF:
                        HandleDigit((int)(command - CalculatorCommand.Digit0));
                        break;
                    case CalculatorCommand.Sign: HandleSign(); break;
                    case CalculatorCommand.Separator: HandleSeparator(); break;
                    case CalculatorCommand.Backspace: HandleBackspace(); break;
                    case CalculatorCommand.Clear: HandleClear(); break;
                    case CalculatorCommand.ClearEntry: HandleClearEntry(); break;
                    case CalculatorCommand.Add: HandleOperation(Operation.Add, "+"); break;
                    case CalculatorCommand.Subtract: HandleOperation(Operation.Subtract, "-"); break;
                    case CalculatorCommand.Multiply: HandleOperation(Operation.Multiply, "*"); break;
                    case CalculatorCommand.Divide:
                        if (_mode == CalculatorMode.Fraction)
                        {
                            HandleSeparator();
                        }
                        else
                        {
                            HandleOperation(Operation.Divide, "/");
                        }
                        break;
                    case CalculatorCommand.Equals: HandleEquals(); break;
                    case CalculatorCommand.Square: HandleFunction(Function.Square, "sqr"); break;
                    case CalculatorCommand.Inverse: HandleFunction(Function.Inverse, "1/"); break;
                    case CalculatorCommand.MemoryClear: MemoryClear(); break;
                    case CalculatorCommand.MemoryStore: HandleMemoryStore(); break;
                    case CalculatorCommand.MemoryRetrieve: HandleMemoryRetrieve(); break;
                    case CalculatorCommand.MemoryAdd: HandleMemoryAdd(); break;
                    case CalculatorCommand.Copy: HandleCopy(); break;
                    case CalculatorCommand.Paste: HandlePaste(); break;
                    case CalculatorCommand.ImaginaryUnit: HandleImaginaryUnit(); break;
                }
                DisplayUpdated?.Invoke(_editor.CurrentNumber);
                MemoryStateUpdated?.Invoke(GetMemoryStateString());
            }
            catch (Exception ex)
            {
                _state = CalculatorState.Error;
                _editor.SetNumber("Ошибка");
                DisplayUpdated?.Invoke(_editor.CurrentNumber);
                System.Diagnostics.Debug.WriteLine(ex.ToString());
            }
        }

        private void HandleDigit(int digit)
        {
            if (_mode == CalculatorMode.PNumber && digit >= _editor.CurrentBase) return;
            
            if (_equalsJustPressed)
            {
                _editor.Clear();
                _expressionParts.Clear();
                _equalsJustPressed = false;
                _hasPendingOperation = false;
                _pendingOperation = Operation.None;
                _hasLastOperation = false;
                ResetAccumulator();
            }
            
            if (_waitingForOperand || _state == CalculatorState.ResultDisplayed)
            {
                _editor.Clear();
                _waitingForOperand = false;
            }
            
            _editor.AddDigit(digit);
            _state = CalculatorState.Editing;
        }

        private void HandleSign()
        {
            _editor.AddSign();
            _state = CalculatorState.Editing;
        }

        private void HandleSeparator()
        {
            _editor.AddSeparator();
            _state = CalculatorState.Editing;
        }

        private void HandleImaginaryUnit()
        {
            if (_mode != CalculatorMode.Complex) return;
            
            if (_equalsJustPressed)
            {
                _editor.Clear();
                _expressionParts.Clear();
                _equalsJustPressed = false;
                _hasPendingOperation = false;
                _pendingOperation = Operation.None;
                _hasLastOperation = false;
                ResetAccumulator();
            }
            
            if (_waitingForOperand)
            {
                _editor.SetNumber("i");
                _expressionParts.Add("i");
                _waitingForOperand = false;
                _state = CalculatorState.Editing;
                return;
            }
            
            string current = _editor.CurrentNumber;
            if (current == "0" || current == "-0" || current == "0i")
            {
                _editor.Clear();
            }
            
            _editor.AddImaginaryUnit();
            _state = CalculatorState.Editing;
        }

        private void HandleBackspace()
        {
            _editor.Backspace();
            _state = CalculatorState.Editing;
        }

        private void HandleClear()
        {
            _editor.Clear();
            _state = CalculatorState.Start;
            _expressionParts.Clear();
            _waitingForOperand = true;
            _equalsJustPressed = false;
            _pendingOperation = Operation.None;
            _hasPendingOperation = false;
            _hasLastOperation = false;
            ResetAccumulator();
        }

        private void HandleClearEntry()
        {
            _editor.Clear();
            _state = CalculatorState.Editing;
        }

        private void HandleOperation(Operation op, string sym)
        {
            string currentNumberStr = _editor.CurrentNumber;
            TANumber currentNumber = GetCurrentNumber();
            
            // Запоминаем операцию и операнд для возможного повторения
            _lastOperation = op;
            _lastOperand = currentNumber.Copy();
            _hasLastOperation = true;
            
            if (!_waitingForOperand && !_equalsJustPressed)
            {
                if (_expressionParts.Count == 0 || _expressionParts[_expressionParts.Count - 1] != currentNumberStr)
                {
                    _expressionParts.Add(currentNumberStr);
                }
            }
            
            _expressionParts.Add(sym);
            
            if (_hasPendingOperation)
            {
                try
                {
                    _accumulator = ExecuteOperation(_pendingOperation, _accumulator, currentNumber);
                    _editor.SetNumber(_accumulator.ToString());
                }
                catch (Exception)
                {
                    _editor.SetNumber("Ошибка");
                    _state = CalculatorState.Error;
                    return;
                }
            }
            else
            {
                _accumulator = currentNumber.Copy();
            }
            
            _pendingOperation = op;
            _hasPendingOperation = true;
            _state = CalculatorState.OperationSet;
            _waitingForOperand = true;
            _equalsJustPressed = false;
        }

        private void HandleEquals()
        {
            // Если нет ожидающей операции, но есть последняя операция - повторяем её
            if (!_hasPendingOperation && _hasLastOperation)
            {
                try
                {
                    TANumber currentNum = GetCurrentNumber();
                    TANumber newResult = ExecuteOperation(_lastOperation, currentNum, _lastOperand);
                    _editor.SetNumber(newResult.ToString());
                    
                    // Обновляем выражение для отображения
                    _expressionParts.Clear();
                    _expressionParts.Add(currentNum.ToString());
                    _expressionParts.Add(GetOperationSymbol(_lastOperation));
                    _expressionParts.Add(_lastOperand.ToString());
                    _expressionParts.Add("=");
                    _expressionParts.Add(newResult.ToString());
                    
                    AddToHistory($"{currentNum} {GetOperationSymbol(_lastOperation)} {_lastOperand}", newResult.ToString());
                    
                    _accumulator = newResult.Copy();
                    _state = CalculatorState.ResultDisplayed;
                    _waitingForOperand = true;
                    _equalsJustPressed = true;
                }
                catch (Exception)
                {
                    _editor.SetNumber("Ошибка");
                    _state = CalculatorState.Error;
                }
                return;
            }
            
            if (!_hasPendingOperation)
            {
                _state = CalculatorState.ResultDisplayed;
                _waitingForOperand = true;
                _equalsJustPressed = true;
                return;
            }
            
            string currentNumberStr = _editor.CurrentNumber;
            TANumber currentNumber = GetCurrentNumber();
            
            if (!_waitingForOperand && !_equalsJustPressed)
            {
                if (_expressionParts.Count == 0 || _expressionParts[_expressionParts.Count - 1] != currentNumberStr)
                {
                    _expressionParts.Add(currentNumberStr);
                }
            }
            
            string fullExpression = string.Join(" ", _expressionParts);
            TANumber resultValue;
            
            try
            {
                resultValue = ExecuteOperation(_pendingOperation, _accumulator, currentNumber);
                _editor.SetNumber(resultValue.ToString());
                
                AddToHistory(fullExpression, resultValue.ToString());
                
                _accumulator = resultValue.Copy();
                _expressionParts.Clear();
                _expressionParts.Add(resultValue.ToString());
                
                // Запоминаем операцию и операнд для повторения
                _lastOperation = _pendingOperation;
                _lastOperand = currentNumber.Copy();
                _hasLastOperation = true;
                
                _state = CalculatorState.ResultDisplayed;
                _waitingForOperand = true;
                _equalsJustPressed = true;
                _hasPendingOperation = false;
                _pendingOperation = Operation.None;
            }
            catch (Exception)
            {
                _editor.SetNumber("Ошибка");
                _state = CalculatorState.Error;
            }
        }
        
        private string GetOperationSymbol(Operation op)
        {
            return op switch
            {
                Operation.Add => "+",
                Operation.Subtract => "-",
                Operation.Multiply => "*",
                Operation.Divide => "/",
                _ => ""
            };
        }

        private void HandleFunction(Function func, string name)
        {
            TANumber current = GetCurrentNumber();
            TANumber result;
            
            try
            {
                result = func switch
                {
                    Function.Square => current.Square(),
                    Function.Inverse => current.Inverse(),
                    _ => current.Copy()
                };
                
                _editor.SetNumber(result.ToString());
                AddToHistory($"{name}({current})", result.ToString());
                
                _accumulator = result.Copy();
                _expressionParts.Clear();
                _expressionParts.Add(result.ToString());
                _hasPendingOperation = false;
                _pendingOperation = Operation.None;
                _hasLastOperation = false;
                _state = CalculatorState.FunctionDone;
                _waitingForOperand = true;
                _equalsJustPressed = false;
            }
            catch (Exception)
            {
                _editor.SetNumber("Ошибка");
                _state = CalculatorState.Error;
            }
        }

        private void MemoryClear()
        {
            if (_memory == null) return;
            switch (_mode)
            {
                case CalculatorMode.PNumber:
                    ((Memory<TPNumber>)_memory).Clear();
                    break;
                case CalculatorMode.Fraction:
                    ((Memory<TFrac>)_memory).Clear();
                    break;
                case CalculatorMode.Complex:
                    ((Memory<TComplex>)_memory).Clear();
                    break;
            }
        }

        private void HandleMemoryStore()
        {
            TANumber current = GetCurrentNumber();
            if (_memory == null) return;
            switch (_mode)
            {
                case CalculatorMode.PNumber:
                    ((Memory<TPNumber>)_memory).Store((TPNumber)current);
                    break;
                case CalculatorMode.Fraction:
                    ((Memory<TFrac>)_memory).Store((TFrac)current);
                    break;
                case CalculatorMode.Complex:
                    ((Memory<TComplex>)_memory).Store((TComplex)current);
                    break;
            }
            // Очищаем поле ввода после сохранения в память
            _editor.Clear();
            _waitingForOperand = true;
            _state = CalculatorState.Start;
            MemoryStateUpdated?.Invoke(GetMemoryStateString());
            DisplayUpdated?.Invoke(_editor.CurrentNumber);
        }

        private void HandleMemoryRetrieve()
        {
            bool isMemoryOn = false;
            TANumber memValue = null;
            
            if (_memory != null)
            {
                switch (_mode)
                {
                    case CalculatorMode.PNumber:
                        isMemoryOn = ((Memory<TPNumber>)_memory).State == Memory<TPNumber>.MemoryState.On;
                        if (isMemoryOn) memValue = ((Memory<TPNumber>)_memory).Retrieve();
                        break;
                    case CalculatorMode.Fraction:
                        isMemoryOn = ((Memory<TFrac>)_memory).State == Memory<TFrac>.MemoryState.On;
                        if (isMemoryOn) memValue = ((Memory<TFrac>)_memory).Retrieve();
                        break;
                    case CalculatorMode.Complex:
                        isMemoryOn = ((Memory<TComplex>)_memory).State == Memory<TComplex>.MemoryState.On;
                        if (isMemoryOn) memValue = ((Memory<TComplex>)_memory).Retrieve();
                        break;
                }
            }
            
            if (isMemoryOn && memValue != null)
            {
                _editor.SetNumber(memValue.ToString());
                _waitingForOperand = false;
                _state = CalculatorState.Editing;
            }
        }

        private void HandleMemoryAdd()
        {
            TANumber current = GetCurrentNumber();
            if (_memory == null) return;
            switch (_mode)
            {
                case CalculatorMode.PNumber:
                    ((Memory<TPNumber>)_memory).Add((TPNumber)current);
                    break;
                case CalculatorMode.Fraction:
                    ((Memory<TFrac>)_memory).Add((TFrac)current);
                    break;
                case CalculatorMode.Complex:
                    ((Memory<TComplex>)_memory).Add((TComplex)current);
                    break;
            }
        }

        private void HandleCopy()
        {
            Clipboard.SetText(_editor.CurrentNumber);
        }

        private void HandlePaste()
        {
            if (Clipboard.ContainsText())
            {
                var text = Clipboard.GetText();
                try
                {
                    switch (_mode)
                    {
                        case CalculatorMode.PNumber:
                            new TPNumber(text, _editor.CurrentBase);
                            break;
                        case CalculatorMode.Fraction:
                            new TFrac(text);
                            break;
                        case CalculatorMode.Complex:
                            new TComplex(text);
                            break;
                    }
                    _editor.SetNumber(text);
                    _waitingForOperand = false;
                    _state = CalculatorState.Editing;
                }
                catch { }
            }
        }

        private void AddToHistory(string expr, string res)
        {
            _history.Add(new HistoryItem(expr, res, _editor.CurrentBase));
            if (_history.Count > 100) _history.RemoveAt(0);
        }
    }
}
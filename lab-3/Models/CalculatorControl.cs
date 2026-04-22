using System;
using System.Collections.Generic;
using System.Text;

namespace PNumberCalculator.Models
{
    public enum CalculatorState
    {
        Start,
        Editing,
        OperationSet,
        ResultDisplayed,
        FunctionDone,
        Error
    }

    public enum CalculatorCommand
    {
        Digit0, Digit1, Digit2, Digit3, Digit4, Digit5, Digit6, Digit7,
        Digit8, Digit9, DigitA, DigitB, DigitC, DigitD, DigitE, DigitF,
        Sign, Separator, Backspace, Clear, ClearEntry,
        Add, Subtract, Multiply, Divide, Equals,
        Square, Inverse,
        MemoryClear, MemoryStore, MemoryRetrieve, MemoryAdd,
        Copy, Paste
    }

    public class CalculatorControl
    {
        private Editor _editor;
        private Processor<TPNumber> _processor;
        private Memory<TPNumber> _memory;
        private CalculatorState _state;
        private List<HistoryItem> _history;
        
        private StringBuilder _currentExpression;
        private bool _waitingForOperand;
        private bool _equalsJustPressed;
        private string _lastOperationSymbol;
        private TPNumber _lastRightOperand;

        public string DisplayText => _editor.CurrentNumber;
        public string MemoryStateText => _memory.GetStateString();
        public CalculatorState State => _state;
        public int CurrentBase => _editor.CurrentBase;
        public List<HistoryItem> History => _history;

        public event Action<string> DisplayUpdated;
        public event Action<string> MemoryStateUpdated;
        public event Action<int> BaseChanged;

        public CalculatorControl()
        {
            _editor = new Editor(10);
            _processor = new Processor<TPNumber>();
            _memory = new Memory<TPNumber>();
            _state = CalculatorState.Start;
            _history = new List<HistoryItem>();
            _currentExpression = new StringBuilder();
            _waitingForOperand = true;
            _equalsJustPressed = false;
            _lastOperationSymbol = "";
            _lastRightOperand = null;
        }

        public void ExecuteCommand(CalculatorCommand command)
        {
            try
            {
                switch (command)
                {
                    case CalculatorCommand.Digit0:
                    case CalculatorCommand.Digit1:
                    case CalculatorCommand.Digit2:
                    case CalculatorCommand.Digit3:
                    case CalculatorCommand.Digit4:
                    case CalculatorCommand.Digit5:
                    case CalculatorCommand.Digit6:
                    case CalculatorCommand.Digit7:
                    case CalculatorCommand.Digit8:
                    case CalculatorCommand.Digit9:
                    case CalculatorCommand.DigitA:
                    case CalculatorCommand.DigitB:
                    case CalculatorCommand.DigitC:
                    case CalculatorCommand.DigitD:
                    case CalculatorCommand.DigitE:
                    case CalculatorCommand.DigitF:
                        HandleDigit((int)(command - CalculatorCommand.Digit0));
                        break;

                    case CalculatorCommand.Sign:
                        HandleSign();
                        break;
                    case CalculatorCommand.Separator:
                        HandleSeparator();
                        break;
                    case CalculatorCommand.Backspace:
                        HandleBackspace();
                        break;
                    case CalculatorCommand.Clear:
                        HandleClear();
                        break;
                    case CalculatorCommand.ClearEntry:
                        HandleClearEntry();
                        break;

                    case CalculatorCommand.Add:
                        HandleOperation(Operation.Add, "+");
                        break;
                    case CalculatorCommand.Subtract:
                        HandleOperation(Operation.Subtract, "-");
                        break;
                    case CalculatorCommand.Multiply:
                        HandleOperation(Operation.Multiply, "*");
                        break;
                    case CalculatorCommand.Divide:
                        HandleOperation(Operation.Divide, "/");
                        break;
                    case CalculatorCommand.Equals:
                        HandleEquals();
                        break;

                    case CalculatorCommand.Square:
                        HandleFunction(Function.Square, "Sqr");
                        break;
                    case CalculatorCommand.Inverse:
                        HandleFunction(Function.Inverse, "Rev");
                        break;

                    case CalculatorCommand.MemoryClear:
                        HandleMemoryClear();
                        break;
                    case CalculatorCommand.MemoryStore:
                        HandleMemoryStore();
                        break;
                    case CalculatorCommand.MemoryRetrieve:
                        HandleMemoryRetrieve();
                        break;
                    case CalculatorCommand.MemoryAdd:
                        HandleMemoryAdd();
                        break;

                    case CalculatorCommand.Copy:
                        HandleCopy();
                        break;
                    case CalculatorCommand.Paste:
                        HandlePaste();
                        break;
                }

                DisplayUpdated?.Invoke(_editor.CurrentNumber);
                MemoryStateUpdated?.Invoke(_memory.GetStateString());
            }
            catch (Exception)
            {
                _state = CalculatorState.Error;
                _editor.SetNumber("Ошибка");
                DisplayUpdated?.Invoke(_editor.CurrentNumber);
            }
        }

        private void HandleDigit(int digit)
        {
            if (digit >= _editor.CurrentBase)
                return;

            if (_equalsJustPressed)
            {
                _editor.Clear();
                _currentExpression.Clear();
                _equalsJustPressed = false;
                _lastOperationSymbol = "";
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

        private void HandleBackspace()
        {
            _editor.Backspace();
            _state = CalculatorState.Editing;
        }

        private void HandleClear()
        {
            _editor.Clear();
            _processor.Reset();
            _state = CalculatorState.Start;
            _currentExpression.Clear();
            _waitingForOperand = true;
            _equalsJustPressed = false;
            _lastOperationSymbol = "";
            _lastRightOperand = null;
        }

        private void HandleClearEntry()
        {
            _editor.Clear();
            _state = CalculatorState.Editing;
        }

        private void HandleOperation(Operation op, string opSymbol)
        {
            TPNumber currentNumber = GetCurrentNumber();

            if (_processor.CurrentOperation != Operation.None && !_waitingForOperand)
            {
                _processor.SetRightOperand(currentNumber);
                TPNumber result = _processor.ExecuteOperation();
                _editor.SetNumber(result.ToString());
                _processor.SetLeftOperand(result);
                _currentExpression.Append(currentNumber.ToString());
            }
            else
            {
                _processor.SetLeftOperand(currentNumber);
                _currentExpression.Clear();
                _currentExpression.Append(currentNumber.ToString());
            }

            _processor.SetOperation(op);
            _lastOperationSymbol = opSymbol;
            _currentExpression.Append($" {opSymbol} ");
            _state = CalculatorState.OperationSet;
            _waitingForOperand = true;
            _equalsJustPressed = false;
        }

        private void HandleEquals()
        {
            TPNumber rightOperand;
            string expression;
            string leftPart;

            if (_equalsJustPressed && _lastRightOperand != null)
            {
                rightOperand = _lastRightOperand.Copy();
                leftPart = _editor.CurrentNumber;
                expression = $"{leftPart} {_lastOperationSymbol} {rightOperand}";
                
                _processor.SetRightOperand(rightOperand);
            }
            else
            {
                rightOperand = GetCurrentNumber();

                if (_processor.CurrentOperation == Operation.None)
                {
                    _state = CalculatorState.ResultDisplayed;
                    _waitingForOperand = true;
                    _equalsJustPressed = true;
                    return;
                }

                _processor.SetRightOperand(rightOperand);
                _lastRightOperand = rightOperand.Copy();
                
                expression = _currentExpression.ToString() + rightOperand.ToString();
            }

            TPNumber result = _processor.ExecuteOperation();
            _editor.SetNumber(result.ToString());

            // Сохраняем в историю
            AddToHistory(expression, result.ToString());

            _currentExpression.Clear();
            _currentExpression.Append(result.ToString());

            _state = CalculatorState.ResultDisplayed;
            _waitingForOperand = true;
            _equalsJustPressed = true;
        }

        private void HandleFunction(Function func, string funcName)
        {
            TPNumber currentNumber = GetCurrentNumber();

            string expression = $"{funcName}({currentNumber})";

            _processor.SetRightOperand(currentNumber);
            TPNumber result = _processor.ExecuteFunction(func);
            _editor.SetNumber(result.ToString());

            AddToHistory(expression, result.ToString());

            _state = CalculatorState.FunctionDone;
            _waitingForOperand = true;
            _equalsJustPressed = false;
            _lastOperationSymbol = "";
            _lastRightOperand = null;
        }

        private TPNumber GetCurrentNumber()
        {
            try
            {
                return new TPNumber(_editor.CurrentNumber, _editor.CurrentBase);
            }
            catch
            {
                return new TPNumber(0, _editor.CurrentBase);
            }
        }

        public void SetBase(int baseNum)
        {
            if (baseNum < 2 || baseNum > 16)
                return;

            try
            {
                TPNumber current = GetCurrentNumber();
                current.ChangeBase(baseNum);
                _editor.SetBase(baseNum);
                _editor.SetNumber(current.ToString());
            }
            catch
            {
                _editor.SetBase(baseNum);
                _editor.Clear();
            }

            BaseChanged?.Invoke(baseNum);
            DisplayUpdated?.Invoke(_editor.CurrentNumber);
        }

        private void HandleMemoryClear()
        {
            _memory.Clear();
        }

        private void HandleMemoryStore()
        {
            _memory.Store(GetCurrentNumber());
        }

        private void HandleMemoryRetrieve()
        {
            if (_memory.State == Memory<TPNumber>.MemoryState.On)
            {
                TPNumber memValue = _memory.Retrieve();
                _editor.SetNumber(memValue.ToString());
                _waitingForOperand = false;
                _state = CalculatorState.Editing;
                DisplayUpdated?.Invoke(_editor.CurrentNumber);
            }
        }

        private void HandleMemoryAdd()
        {
            TPNumber current = GetCurrentNumber();
            _memory.Add(current);
        }

        private void HandleCopy()
        {
            System.Windows.Forms.Clipboard.SetText(_editor.CurrentNumber);
        }

        private void HandlePaste()
        {
            if (System.Windows.Forms.Clipboard.ContainsText())
            {
                string text = System.Windows.Forms.Clipboard.GetText();
                try
                {
                    TPNumber temp = new TPNumber(text, _editor.CurrentBase);
                    _editor.SetNumber(temp.ToString());
                    _waitingForOperand = false;
                    _state = CalculatorState.Editing;
                }
                catch
                {

                }
            }
        }

        private void AddToHistory(string expression, string result)
        {
            _history.Add(new HistoryItem(expression, result, _editor.CurrentBase));

            if (_history.Count > 100)
                _history.RemoveAt(0);
        }

        public string GetCurrentExpression()
        {
            if (_currentExpression.Length == 0)
                return "";

            return _currentExpression.ToString() +
                   (_processor.CurrentOperation != Operation.None && !_waitingForOperand ? _editor.CurrentNumber : "");
        }
    }
}
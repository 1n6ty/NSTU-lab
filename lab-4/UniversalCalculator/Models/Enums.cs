namespace UniversalCalculator.Models
{
    public enum CalculatorMode
    {
        PNumber,
        Fraction,
        Complex
    }

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
        Copy, Paste,
        ImaginaryUnit
    }

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

    
}
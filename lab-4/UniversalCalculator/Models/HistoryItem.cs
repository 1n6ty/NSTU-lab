using System;

namespace UniversalCalculator.Models
{
    public class HistoryItem
    {
        public DateTime Timestamp { get; set; }
        public string Expression { get; set; }
        public string Result { get; set; }
        public int Base { get; set; }

        public HistoryItem(string expression, string result, int baseNum)
        {
            Timestamp = DateTime.Now;
            Expression = expression;
            Result = result;
            Base = baseNum;
        }

        public override string ToString()
        {
            string baseInfo = Base != 10 ? $" (осн. {Base})" : "";
            return $"[{Timestamp:HH:mm:ss}] {Expression} = {Result}{baseInfo}";
        }
    }
}
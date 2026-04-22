using System;

namespace SimpleWarehouse
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("🏭 Складская система");
            Console.WriteLine("1. Приемка товара");
            Console.WriteLine("2. Отгрузка товара");
            Console.WriteLine("3. Поиск товара");
            Console.Write("Выберите действие: ");
            
            var choice = Console.ReadLine();
            Console.WriteLine($"Вы выбрали: {choice}");
        }
    }
}
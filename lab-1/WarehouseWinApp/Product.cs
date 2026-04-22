using System;

namespace WarehouseWinApp
{
    public class Product
    {
        public int Id { get; set; }
        public string Name { get; set; }
        public string Article { get; set; }
        public string Category { get; set; }
        public string Unit { get; set; }
        public decimal Quantity { get; set; }
        public string Cell { get; set; }
        public decimal Price { get; set; }
        public DateTime CreatedDate { get; set; }

        public Product()
        {
            CreatedDate = DateTime.Now;
            Unit = "шт";
        }

        public string FullInfo => $"{Article} - {Name} ({Quantity} {Unit})";
    }
}
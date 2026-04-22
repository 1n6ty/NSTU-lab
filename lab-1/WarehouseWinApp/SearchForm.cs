using System;
using System.Data;
using System.Windows.Forms;

namespace WarehouseWinApp
{
    public partial class SearchForm : Form
    {
        public SearchForm()
        {
            InitializeComponent();
            LoadAllProducts();
        }

        private void LoadAllProducts()
        {
            string query = "SELECT * FROM Products ORDER BY Name";
            dataGridView1.DataSource = DatabaseHelper.GetDataTable(query);
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            string searchText = txtSearch.Text.Trim();
            
            if (string.IsNullOrEmpty(searchText))
            {
                LoadAllProducts();
                return;
            }

            string query = @"SELECT * FROM Products 
                            WHERE Name LIKE @search 
                            OR Article LIKE @search
                            OR Category LIKE @search";
            
            var param = new SqlParameter("@search", $"%{searchText}%");
            dataGridView1.DataSource = DatabaseHelper.GetDataTable(query, param);
            
            lblResultCount.Text = $"Найдено: {dataGridView1.Rows.Count}";
        }

        private void dataGridView1_CellClick(object sender, DataGridViewCellEventArgs e)
        {
            if (e.RowIndex >= 0)
            {
                DataGridViewRow row = dataGridView1.Rows[e.RowIndex];
                
                string details = $"Товар: {row.Cells["Name"].Value}\n" +
                                $"Артикул: {row.Cells["Article"].Value}\n" +
                                $"Категория: {row.Cells["Category"].Value}\n" +
                                $"Цена: {row.Cells["Price"].Value} ₽";
                
                MessageBox.Show(details, "Информация о товаре");
            }
        }
    }
}
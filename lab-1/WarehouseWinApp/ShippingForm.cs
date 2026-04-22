using System;
using System.Data;
using System.Windows.Forms;

namespace WarehouseWinApp
{
    public partial class ShippingForm : Form
    {
        private DataTable productsTable;

        public ShippingForm()
        {
            InitializeComponent();
            productsTable = new DataTable();
            productsTable.Columns.Add("ProductName", typeof(string));
            productsTable.Columns.Add("Quantity", typeof(decimal));
            productsTable.Columns.Add("Cell", typeof(string));
            
            dataGridView1.DataSource = productsTable;
        }

        private void btnAddProduct_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtProductName.Text) ||
                string.IsNullOrEmpty(txtQuantity.Text) ||
                string.IsNullOrEmpty(txtCell.Text))
            {
                MessageBox.Show("Заполните все поля!", "Ошибка", 
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            DataRow row = productsTable.NewRow();
            row["ProductName"] = txtProductName.Text;
            row["Quantity"] = Convert.ToDecimal(txtQuantity.Text);
            row["Cell"] = txtCell.Text.ToUpper();
            productsTable.Rows.Add(row);

            txtProductName.Clear();
            txtQuantity.Clear();
            txtCell.Clear();
            txtProductName.Focus();
        }

        private void btnShip_Click(object sender, EventArgs e)
        {
            if (productsTable.Rows.Count == 0)
            {
                MessageBox.Show("Добавьте товары для отгрузки!", "Ошибка",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (string.IsNullOrEmpty(txtCustomer.Text))
            {
                MessageBox.Show("Укажите получателя!", "Ошибка",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            MessageBox.Show($"Отгрузка для {txtCustomer.Text} оформлена!", "Успех",
                MessageBoxButtons.OK, MessageBoxIcon.Information);

            this.Close();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            productsTable.Clear();
            txtCustomer.Clear();
        }
    }
}
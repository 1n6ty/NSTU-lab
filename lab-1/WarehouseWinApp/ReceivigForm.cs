using System;
using System.Data;
using System.Windows.Forms;

namespace WarehouseWinApp
{
    public partial class ReceivingForm : Form
    {
        private DataTable productsTable;

        public ReceivingForm()
        {
            InitializeComponent();
            productsTable = new DataTable();
            productsTable.Columns.Add("ProductName", typeof(string));
            productsTable.Columns.Add("Quantity", typeof(decimal));
            productsTable.Columns.Add("Price", typeof(decimal));
            productsTable.Columns.Add("Cell", typeof(string));
            productsTable.Columns.Add("Total", typeof(decimal), "Quantity * Price");
            
            dataGridView1.DataSource = productsTable;
        }

        private void btnAddProduct_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(txtProductName.Text) ||
                string.IsNullOrEmpty(txtQuantity.Text) ||
                string.IsNullOrEmpty(txtPrice.Text) ||
                string.IsNullOrEmpty(txtCell.Text))
            {
                MessageBox.Show("Заполните все поля!", "Ошибка", 
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            DataRow row = productsTable.NewRow();
            row["ProductName"] = txtProductName.Text;
            row["Quantity"] = Convert.ToDecimal(txtQuantity.Text);
            row["Price"] = Convert.ToDecimal(txtPrice.Text);
            row["Cell"] = txtCell.Text.ToUpper();
            productsTable.Rows.Add(row);

            ClearInputs();
            UpdateTotal();
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (productsTable.Rows.Count == 0)
            {
                MessageBox.Show("Добавьте хотя бы один товар!", "Ошибка",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (string.IsNullOrEmpty(txtInvoiceNumber.Text))
            {
                MessageBox.Show("Введите номер накладной!", "Ошибка",
                    MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            // Сохраняем в базу данных (упрощенно)
            string query = @"
                INSERT INTO Invoices (Number, Date, Total) 
                VALUES (@num, @date, @total)";

            DatabaseHelper.ExecuteNonQuery(query,
                new SqlParameter("@num", txtInvoiceNumber.Text),
                new SqlParameter("@date", DateTime.Now),
                new SqlParameter("@total", lblTotal.Text));

            MessageBox.Show($"Документ {txtInvoiceNumber.Text} сохранен!", "Успех",
                MessageBoxButtons.OK, MessageBoxIcon.Information);

            this.Close();
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            productsTable.Clear();
            ClearInputs();
            lblTotal.Text = "0.00";
        }

        private void ClearInputs()
        {
            txtProductName.Clear();
            txtQuantity.Clear();
            txtPrice.Clear();
            txtCell.Clear();
            txtProductName.Focus();
        }

        private void UpdateTotal()
        {
            decimal total = 0;
            foreach (DataRow row in productsTable.Rows)
            {
                total += Convert.ToDecimal(row["Total"]);
            }
            lblTotal.Text = total.ToString("F2");
        }
    }
}
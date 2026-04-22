namespace WarehouseWinApp
{
    partial class ReceivingForm
    {
        private System.ComponentModel.IContainer components = null;
        private TextBox txtInvoiceNumber;
        private TextBox txtProductName;
        private TextBox txtQuantity;
        private TextBox txtPrice;
        private TextBox txtCell;
        private Button btnAddProduct;
        private Button btnSave;
        private Button btnClear;
        private DataGridView dataGridView1;
        private Label lblTotal;
        private Label lblInvoiceNumber;
        private Label lblProductName;
        private Label lblQuantity;
        private Label lblPrice;
        private Label lblCell;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        private void InitializeComponent()
        {
            this.txtInvoiceNumber = new TextBox();
            this.txtProductName = new TextBox();
            this.txtQuantity = new TextBox();
            this.txtPrice = new TextBox();
            this.txtCell = new TextBox();
            this.btnAddProduct = new Button();
            this.btnSave = new Button();
            this.btnClear = new Button();
            this.dataGridView1 = new DataGridView();
            this.lblTotal = new Label();
            this.lblInvoiceNumber = new Label();
            this.lblProductName = new Label();
            this.lblQuantity = new Label();
            this.lblPrice = new Label();
            this.lblCell = new Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();

            // Заголовок формы
            this.Text = "📦 Приемка товара";
            this.Size = new System.Drawing.Size(800, 600);
            this.StartPosition = FormStartPosition.CenterParent;

            // lblInvoiceNumber
            this.lblInvoiceNumber.Text = "Номер накладной:";
            this.lblInvoiceNumber.Location = new System.Drawing.Point(20, 20);
            this.lblInvoiceNumber.Size = new System.Drawing.Size(120, 25);

            // txtInvoiceNumber
            this.txtInvoiceNumber.Location = new System.Drawing.Point(150, 20);
            this.txtInvoiceNumber.Size = new System.Drawing.Size(200, 25);

            // lblProductName
            this.lblProductName.Text = "Товар:";
            this.lblProductName.Location = new System.Drawing.Point(20, 60);
            this.lblProductName.Size = new System.Drawing.Size(120, 25);

            // txtProductName
            this.txtProductName.Location = new System.Drawing.Point(150, 60);
            this.txtProductName.Size = new System.Drawing.Size(200, 25);

            // lblQuantity
            this.lblQuantity.Text = "Количество:";
            this.lblQuantity.Location = new System.Drawing.Point(20, 100);
            this.lblQuantity.Size = new System.Drawing.Size(120, 25);

            // txtQuantity
            this.txtQuantity.Location = new System.Drawing.Point(150, 100);
            this.txtQuantity.Size = new System.Drawing.Size(100, 25);

            // lblPrice
            this.lblPrice.Text = "Цена:";
            this.lblPrice.Location = new System.Drawing.Point(260, 100);
            this.lblPrice.Size = new System.Drawing.Size(50, 25);

            // txtPrice
            this.txtPrice.Location = new System.Drawing.Point(320, 100);
            this.txtPrice.Size = new System.Drawing.Size(100, 25);

            // lblCell
            this.lblCell.Text = "Ячейка:";
            this.lblCell.Location = new System.Drawing.Point(20, 140);
            this.lblCell.Size = new System.Drawing.Size(120, 25);

            // txtCell
            this.txtCell.Location = new System.Drawing.Point(150, 140);
            this.txtCell.Size = new System.Drawing.Size(100, 25);
            this.txtCell.Text = "A-01-01";

            // btnAddProduct
            this.btnAddProduct.Text = "+ Добавить товар";
            this.btnAddProduct.Location = new System.Drawing.Point(260, 140);
            this.btnAddProduct.Size = new System.Drawing.Size(150, 30);
            this.btnAddProduct.BackColor = System.Drawing.Color.FromArgb(76, 175, 80);
            this.btnAddProduct.ForeColor = System.Drawing.Color.White;
            this.btnAddProduct.Click += new EventHandler(this.btnAddProduct_Click);

            // dataGridView1
            this.dataGridView1.Location = new System.Drawing.Point(20, 190);
            this.dataGridView1.Size = new System.Drawing.Size(740, 280);
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;

            // lblTotal
            this.lblTotal.Text = "Итого: 0.00 ₽";
            this.lblTotal.Font = new System.Drawing.Font("Arial", 14F, System.Drawing.FontStyle.Bold);
            this.lblTotal.Location = new System.Drawing.Point(20, 480);
            this.lblTotal.Size = new System.Drawing.Size(300, 30);
            this.lblTotal.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;

            // btnSave
            this.btnSave.Text = "💾 Сохранить накладную";
            this.btnSave.Location = new System.Drawing.Point(400, 480);
            this.btnSave.Size = new System.Drawing.Size(180, 40);
            this.btnSave.BackColor = System.Drawing.Color.FromArgb(33, 150, 243);
            this.btnSave.ForeColor = System.Drawing.Color.White;
            this.btnSave.Click += new EventHandler(this.btnSave_Click);

            // btnClear
            this.btnClear.Text = "✖ Очистить";
            this.btnClear.Location = new System.Drawing.Point(590, 480);
            this.btnClear.Size = new System.Drawing.Size(100, 40);
            this.btnClear.BackColor = System.Drawing.Color.FromArgb(244, 67, 54);
            this.btnClear.ForeColor = System.Drawing.Color.White;
            this.btnClear.Click += new EventHandler(this.btnClear_Click);

            // Добавление контролов
            this.Controls.Add(this.lblInvoiceNumber);
            this.Controls.Add(this.txtInvoiceNumber);
            this.Controls.Add(this.lblProductName);
            this.Controls.Add(this.txtProductName);
            this.Controls.Add(this.lblQuantity);
            this.Controls.Add(this.txtQuantity);
            this.Controls.Add(this.lblPrice);
            this.Controls.Add(this.txtPrice);
            this.Controls.Add(this.lblCell);
            this.Controls.Add(this.txtCell);
            this.Controls.Add(this.btnAddProduct);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.lblTotal);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.btnClear);

            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
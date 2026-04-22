namespace WarehouseWinApp
{
    partial class ShippingForm
    {
        private System.ComponentModel.IContainer components = null;
        private TextBox txtCustomer;
        private TextBox txtProductName;
        private TextBox txtQuantity;
        private TextBox txtCell;
        private Button btnAddProduct;
        private Button btnShip;
        private Button btnClear;
        private DataGridView dataGridView1;
        private Label lblCustomer;
        private Label lblProductName;
        private Label lblQuantity;
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
            this.txtCustomer = new TextBox();
            this.txtProductName = new TextBox();
            this.txtQuantity = new TextBox();
            this.txtCell = new TextBox();
            this.btnAddProduct = new Button();
            this.btnShip = new Button();
            this.btnClear = new Button();
            this.dataGridView1 = new DataGridView();
            this.lblCustomer = new Label();
            this.lblProductName = new Label();
            this.lblQuantity = new Label();
            this.lblCell = new Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();

            this.Text = "🚚 Отгрузка товара";
            this.Size = new System.Drawing.Size(800, 600);
            this.StartPosition = FormStartPosition.CenterParent;

            // lblCustomer
            this.lblCustomer.Text = "Получатель:";
            this.lblCustomer.Location = new System.Drawing.Point(20, 20);
            this.lblCustomer.Size = new System.Drawing.Size(100, 25);

            // txtCustomer
            this.txtCustomer.Location = new System.Drawing.Point(130, 20);
            this.txtCustomer.Size = new System.Drawing.Size(300, 25);
            this.txtCustomer.Text = "ООО Покупатель";

            // lblProductName
            this.lblProductName.Text = "Товар:";
            this.lblProductName.Location = new System.Drawing.Point(20, 60);
            this.lblProductName.Size = new System.Drawing.Size(100, 25);

            // txtProductName
            this.txtProductName.Location = new System.Drawing.Point(130, 60);
            this.txtProductName.Size = new System.Drawing.Size(200, 25);

            // lblQuantity
            this.lblQuantity.Text = "Кол-во:";
            this.lblQuantity.Location = new System.Drawing.Point(340, 60);
            this.lblQuantity.Size = new System.Drawing.Size(50, 25);

            // txtQuantity
            this.txtQuantity.Location = new System.Drawing.Point(400, 60);
            this.txtQuantity.Size = new System.Drawing.Size(100, 25);

            // lblCell
            this.lblCell.Text = "Ячейка:";
            this.lblCell.Location = new System.Drawing.Point(20, 100);
            this.lblCell.Size = new System.Drawing.Size(100, 25);

            // txtCell
            this.txtCell.Location = new System.Drawing.Point(130, 100);
            this.txtCell.Size = new System.Drawing.Size(100, 25);
            this.txtCell.Text = "A-01-01";

            // btnAddProduct
            this.btnAddProduct.Text = "+ Добавить";
            this.btnAddProduct.Location = new System.Drawing.Point(240, 100);
            this.btnAddProduct.Size = new System.Drawing.Size(100, 30);
            this.btnAddProduct.BackColor = System.Drawing.Color.FromArgb(76, 175, 80);
            this.btnAddProduct.ForeColor = System.Drawing.Color.White;
            this.btnAddProduct.Click += new EventHandler(this.btnAddProduct_Click);

            // dataGridView1
            this.dataGridView1.Location = new System.Drawing.Point(20, 150);
            this.dataGridView1.Size = new System.Drawing.Size(740, 300);
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.ReadOnly = true;

            // btnShip
            this.btnShip.Text = "✅ Оформить отгрузку";
            this.btnShip.Location = new System.Drawing.Point(400, 470);
            this.btnShip.Size = new System.Drawing.Size(180, 40);
            this.btnShip.BackColor = System.Drawing.Color.FromArgb(33, 150, 243);
            this.btnShip.ForeColor = System.Drawing.Color.White;
            this.btnShip.Click += new EventHandler(this.btnShip_Click);

            // btnClear
            this.btnClear.Text = "✖ Очистить";
            this.btnClear.Location = new System.Drawing.Point(590, 470);
            this.btnClear.Size = new System.Drawing.Size(100, 40);
            this.btnClear.BackColor = System.Drawing.Color.FromArgb(244, 67, 54);
            this.btnClear.ForeColor = System.Drawing.Color.White;
            this.btnClear.Click += new EventHandler(this.btnClear_Click);

            this.Controls.Add(this.lblCustomer);
            this.Controls.Add(this.txtCustomer);
            this.Controls.Add(this.lblProductName);
            this.Controls.Add(this.txtProductName);
            this.Controls.Add(this.lblQuantity);
            this.Controls.Add(this.txtQuantity);
            this.Controls.Add(this.lblCell);
            this.Controls.Add(this.txtCell);
            this.Controls.Add(this.btnAddProduct);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.btnShip);
            this.Controls.Add(this.btnClear);

            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
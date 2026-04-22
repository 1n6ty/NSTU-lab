namespace WarehouseWinApp
{
    partial class MainForm
    {
        private System.ComponentModel.IContainer components = null;
        private Button btnReceiving;
        private Button btnShipping;
        private Button btnSearch;
        private Button btnExit;
        private Label lblTitle;

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
            this.btnReceiving = new Button();
            this.btnShipping = new Button();
            this.btnSearch = new Button();
            this.btnExit = new Button();
            this.lblTitle = new Label();
            this.SuspendLayout();

            // lblTitle
            this.lblTitle.Text = "🏭 СКЛАДСКАЯ СИСТЕМА";
            this.lblTitle.Font = new System.Drawing.Font("Arial", 18F, System.Drawing.FontStyle.Bold);
            this.lblTitle.Size = new System.Drawing.Size(400, 40);
            this.lblTitle.Location = new System.Drawing.Point(80, 30);
            this.lblTitle.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;

            // btnReceiving
            this.btnReceiving.Text = "📦 ПРИЕМКА ТОВАРА";
            this.btnReceiving.Size = new System.Drawing.Size(200, 60);
            this.btnReceiving.Location = new System.Drawing.Point(120, 100);
            this.btnReceiving.Font = new System.Drawing.Font("Arial", 12F);
            this.btnReceiving.BackColor = System.Drawing.Color.FromArgb(76, 175, 80);
            this.btnReceiving.ForeColor = System.Drawing.Color.White;
            this.btnReceiving.Click += new EventHandler(this.btnReceiving_Click);

            // btnShipping
            this.btnShipping.Text = "🚚 ОТГРУЗКА ТОВАРА";
            this.btnShipping.Size = new System.Drawing.Size(200, 60);
            this.btnShipping.Location = new System.Drawing.Point(120, 180);
            this.btnShipping.Font = new System.Drawing.Font("Arial", 12F);
            this.btnShipping.BackColor = System.Drawing.Color.FromArgb(33, 150, 243);
            this.btnShipping.ForeColor = System.Drawing.Color.White;
            this.btnShipping.Click += new EventHandler(this.btnShipping_Click);

            // btnSearch
            this.btnSearch.Text = "🔍 ПОИСК ТОВАРА";
            this.btnSearch.Size = new System.Drawing.Size(200, 60);
            this.btnSearch.Location = new System.Drawing.Point(120, 260);
            this.btnSearch.Font = new System.Drawing.Font("Arial", 12F);
            this.btnSearch.BackColor = System.Drawing.Color.FromArgb(255, 152, 0);
            this.btnSearch.ForeColor = System.Drawing.Color.White;
            this.btnSearch.Click += new EventHandler(this.btnSearch_Click);

            // btnExit
            this.btnExit.Text = "✖ ВЫХОД";
            this.btnExit.Size = new System.Drawing.Size(200, 40);
            this.btnExit.Location = new System.Drawing.Point(120, 340);
            this.btnExit.Font = new System.Drawing.Font("Arial", 10F);
            this.btnExit.BackColor = System.Drawing.Color.FromArgb(244, 67, 54);
            this.btnExit.ForeColor = System.Drawing.Color.White;
            this.btnExit.Click += new EventHandler(this.btnExit_Click);

            // MainForm
            this.ClientSize = new System.Drawing.Size(450, 450);
            this.Controls.Add(this.lblTitle);
            this.Controls.Add(this.btnReceiving);
            this.Controls.Add(this.btnShipping);
            this.Controls.Add(this.btnSearch);
            this.Controls.Add(this.btnExit);
            this.Text = "Складская система";
            this.StartPosition = FormStartPosition.CenterScreen;
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.ResumeLayout(false);
        }
    }
}
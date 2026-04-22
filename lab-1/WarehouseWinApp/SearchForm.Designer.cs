namespace WarehouseWinApp
{
    partial class SearchForm
    {
        private System.ComponentModel.IContainer components = null;
        private TextBox txtSearch;
        private Button btnSearch;
        private DataGridView dataGridView1;
        private Label lblResultCount;
        private Label lblSearch;

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
            this.txtSearch = new TextBox();
            this.btnSearch = new Button();
            this.dataGridView1 = new DataGridView();
            this.lblResultCount = new Label();
            this.lblSearch = new Label();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            this.SuspendLayout();

            this.Text = "🔍 Поиск товара";
            this.Size = new System.Drawing.Size(900, 600);
            this.StartPosition = FormStartPosition.CenterParent;

            // lblSearch
            this.lblSearch.Text = "Поиск:";
            this.lblSearch.Location = new System.Drawing.Point(20, 20);
            this.lblSearch.Size = new System.Drawing.Size(50, 30);

            // txtSearch
            this.txtSearch.Location = new System.Drawing.Point(80, 20);
            this.txtSearch.Size = new System.Drawing.Size(300, 25);

            // btnSearch
            this.btnSearch.Text = "Найти";
            this.btnSearch.Location = new System.Drawing.Point(390, 20);
            this.btnSearch.Size = new System.Drawing.Size(100, 30);
            this.btnSearch.BackColor = System.Drawing.Color.FromArgb(33, 150, 243);
            this.btnSearch.ForeColor = System.Drawing.Color.White;
            this.btnSearch.Click += new EventHandler(this.btnSearch_Click);

            // dataGridView1
            this.dataGridView1.Location = new System.Drawing.Point(20, 70);
            this.dataGridView1.Size = new System.Drawing.Size(840, 460);
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.Fill;
            this.dataGridView1.CellClick += new DataGridViewCellEventHandler(this.dataGridView1_CellClick);

            // lblResultCount
            this.lblResultCount.Text = "Найдено: 0";
            this.lblResultCount.Location = new System.Drawing.Point(20, 540);
            this.lblResultCount.Size = new System.Drawing.Size(200, 30);

            this.Controls.Add(this.lblSearch);
            this.Controls.Add(this.txtSearch);
            this.Controls.Add(this.btnSearch);
            this.Controls.Add(this.dataGridView1);
            this.Controls.Add(this.lblResultCount);

            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();
        }
    }
}
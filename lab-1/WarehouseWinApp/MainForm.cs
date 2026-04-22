using System;
using System.Windows.Forms;

namespace WarehouseWinApp
{
    public partial class MainForm : Form
    {
        public MainForm()
        {
            InitializeComponent();
        }

        private void btnReceiving_Click(object sender, EventArgs e)
        {
            ReceivingForm form = new ReceivingForm();
            form.ShowDialog();
        }

        private void btnShipping_Click(object sender, EventArgs e)
        {
            ShippingForm form = new ShippingForm();
            form.ShowDialog();
        }

        private void btnSearch_Click(object sender, EventArgs e)
        {
            SearchForm form = new SearchForm();
            form.ShowDialog();
        }

        private void btnExit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }
    }
}
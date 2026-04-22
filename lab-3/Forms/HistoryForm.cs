using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using PNumberCalculator.Models;

namespace PNumberCalculator.Forms
{
    public partial class HistoryForm : Form
    {
        private ListBox historyListBox;
        private List<HistoryItem> _history;
        private Button clearButton;
        private Button closeButton;

        public HistoryForm(List<HistoryItem> history)
        {
            _history = history;
            InitializeComponent();
            RefreshHistory();
        }

        private void InitializeComponent()
        {
            this.Text = "История вычислений";
            this.Size = new Size(400, 400);
            this.MinimumSize = new Size(300, 250);
            this.StartPosition = FormStartPosition.CenterParent;
            this.FormBorderStyle = FormBorderStyle.SizableToolWindow;

            historyListBox = new ListBox
            {
                Location = new Point(10, 10),
                Size = new Size(364, 300),
                Font = new Font("Consolas", 10),
                Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right
            };
            this.Controls.Add(historyListBox);

            var buttonPanel = new Panel
            {
                Location = new Point(0, 320),
                Size = new Size(384, 40),
                Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right
            };

            clearButton = new Button
            {
                Text = "Очистить",
                Location = new Point(100, 5),
                Size = new Size(80, 25),
                Anchor = AnchorStyles.Bottom
            };
            clearButton.Click += (s, e) =>
            {
                _history.Clear();
                RefreshHistory();
            };
            buttonPanel.Controls.Add(clearButton);

            closeButton = new Button
            {
                Text = "Закрыть",
                Location = new Point(190, 5),
                Size = new Size(80, 25),
                Anchor = AnchorStyles.Bottom
            };
            closeButton.Click += (s, e) => this.Close();
            buttonPanel.Controls.Add(closeButton);

            this.Controls.Add(buttonPanel);
        }

        public void RefreshHistory()
        {
            historyListBox.Items.Clear();
            
            for (int i = _history.Count - 1; i >= 0; i--)
            {
                historyListBox.Items.Add(_history[i].ToString());
            }
        }
    }
}
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using UniversalCalculator.Models;

namespace UniversalCalculator.Forms
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
            this.Size = new Size(450, 450);
            this.MinimumSize = new Size(350, 300);
            this.StartPosition = FormStartPosition.CenterParent;
            this.FormBorderStyle = FormBorderStyle.SizableToolWindow;
            this.BackColor = SystemColors.Control;

            // Список для отображения истории
            historyListBox = new ListBox
            {
                Location = new Point(12, 12),
                Size = new Size(410, 350),
                Font = new Font("Consolas", 10),
                HorizontalScrollbar = true,
                Anchor = AnchorStyles.Top | AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right
            };
            this.Controls.Add(historyListBox);

            // Панель с кнопками
            var buttonPanel = new Panel
            {
                Height = 40,
                Anchor = AnchorStyles.Bottom | AnchorStyles.Left | AnchorStyles.Right
            };

            clearButton = new Button
            {
                Text = "Очистить историю",
                Size = new Size(140, 30),
                Location = new Point(100, 5),
                Anchor = AnchorStyles.Bottom,
                Font = new Font("Segoe UI", 9)
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
                Size = new Size(100, 30),
                Location = new Point(260, 5),
                Anchor = AnchorStyles.Bottom,
                Font = new Font("Segoe UI", 9)
            };
            closeButton.Click += (s, e) => this.Close();
            buttonPanel.Controls.Add(closeButton);

            this.Controls.Add(buttonPanel);

            // Расположение панели кнопок
            this.Resize += (s, e) =>
            {
                buttonPanel.Location = new Point(0, this.ClientSize.Height - 50);
                buttonPanel.Width = this.ClientSize.Width;
                historyListBox.Height = this.ClientSize.Height - 75;
            };

            // Начальное расположение
            this.Load += (s, e) =>
            {
                buttonPanel.Location = new Point(0, this.ClientSize.Height - 50);
                buttonPanel.Width = this.ClientSize.Width;
                historyListBox.Height = this.ClientSize.Height - 75;
            };
        }

        /// <summary>
        /// Обновляет отображение истории в ListBox
        /// </summary>
        public void RefreshHistory()
        {
            historyListBox.Items.Clear();

            if (_history.Count == 0)
            {
                historyListBox.Items.Add("История пуста");
                return;
            }

            // Показываем от новых к старым (сверху вниз)
            for (int i = _history.Count - 1; i >= 0; i--)
            {
                historyListBox.Items.Add(_history[i].ToString());
            }

            // Прокручиваем к началу списка
            if (historyListBox.Items.Count > 0)
            {
                historyListBox.TopIndex = 0;
            }
        }
    }
}
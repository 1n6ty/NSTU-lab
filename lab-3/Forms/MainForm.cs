using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using PNumberCalculator.Models;

namespace PNumberCalculator.Forms{
    public partial class MainForm : Form{
        private CalculatorControl _calculator;
        private Dictionary<Keys, CalculatorCommand> _keyMapping;
        private List<Button> _digitButtons;
        private List<Button> _hexButtons;
        private HistoryForm _historyForm;

        // Компоненты формы
        private TextBox displayTextBox;
        private Label memoryLabel;
        private Label expressionLabel;
        private ComboBox baseComboBox;
        private MenuStrip menuStrip;
        private TableLayoutPanel buttonPanel;

        public MainForm(){
            InitializeComponent();
            InitializeCalculator();
            SetupKeyMapping();
            UpdateDigitButtonsState();
        }

        private void InitializeComponent(){
            this.Text = "Калькулятор p-ичных чисел";
            this.Size = new Size(380, 580);
            this.MinimumSize = new Size(350, 550);
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.StartPosition = FormStartPosition.CenterScreen;
            this.BackColor = SystemColors.Control;
            this.KeyPreview = true;
            this.KeyPress += Form_KeyPress;
            this.KeyDown += Form_KeyDown;

            // Меню
            menuStrip = new MenuStrip();
            
            var editMenu = new ToolStripMenuItem("Правка");
            var copyItem = new ToolStripMenuItem("Копировать", null, (s, e) => _calculator.ExecuteCommand(CalculatorCommand.Copy));
            copyItem.ShortcutKeys = Keys.Control | Keys.C;
            var pasteItem = new ToolStripMenuItem("Вставить", null, (s, e) => _calculator.ExecuteCommand(CalculatorCommand.Paste));
            pasteItem.ShortcutKeys = Keys.Control | Keys.V;
            editMenu.DropDownItems.Add(copyItem);
            editMenu.DropDownItems.Add(pasteItem);
            
            var viewMenu = new ToolStripMenuItem("Вид");
            var historyItem = new ToolStripMenuItem("История", null, (s, e) => ShowHistory());
            viewMenu.DropDownItems.Add(historyItem);
            
            var helpMenu = new ToolStripMenuItem("Справка");
            var aboutItem = new ToolStripMenuItem("О программе", null, (s, e) => ShowAbout());
            helpMenu.DropDownItems.Add(aboutItem);
            
            menuStrip.Items.Add(editMenu);
            menuStrip.Items.Add(viewMenu);
            menuStrip.Items.Add(helpMenu);
            
            this.Controls.Add(menuStrip);
            this.MainMenuStrip = menuStrip;

            // Панель дисплея
            var displayPanel = new Panel{
                Location = new Point(5, 30),
                Size = new Size(354, 60),
                BackColor = Color.White,
                BorderStyle = BorderStyle.FixedSingle
            };

            expressionLabel = new Label{
                Location = new Point(5, 3),
                Size = new Size(340, 15),
                TextAlign = ContentAlignment.TopRight,
                Font = new Font("Segoe UI", 8),
                ForeColor = Color.Gray
            };
            displayPanel.Controls.Add(expressionLabel);

            displayTextBox = new TextBox{
                Location = new Point(5, 20),
                Size = new Size(340, 28),
                Font = new Font("Segoe UI", 16, FontStyle.Bold),
                TextAlign = HorizontalAlignment.Right,
                ReadOnly = true,
                BorderStyle = BorderStyle.None,
                BackColor = Color.White,
                Text = "0"
            };
            displayPanel.Controls.Add(displayTextBox);

            memoryLabel = new Label{
                Location = new Point(5, 3),
                Size = new Size(20, 15),
                Font = new Font("Segoe UI", 8, FontStyle.Bold),
                ForeColor = Color.Blue,
                Text = ""
            };
            displayPanel.Controls.Add(memoryLabel);
            memoryLabel.BringToFront();

            this.Controls.Add(displayPanel);

            // Выбор основания
            var baseLabel = new Label{
                Location = new Point(10, 95),
                Size = new Size(80, 20),
                Text = "Основание :",
                Font = new Font("Segoe UI", 9)
            };
            this.Controls.Add(baseLabel);

            baseComboBox = new ComboBox{
                Location = new Point(95, 93),
                Size = new Size(60, 20),
                DropDownStyle = ComboBoxStyle.DropDownList,
                Font = new Font("Segoe UI", 9)
            };
            for (int i = 2; i <= 16; i++)
                baseComboBox.Items.Add(i);
            baseComboBox.SelectedIndex = 8; // 10
            baseComboBox.SelectedIndexChanged += BaseComboBox_SelectedIndexChanged;
            this.Controls.Add(baseComboBox);

            // Панель кнопок
            buttonPanel = new TableLayoutPanel
            {
                Location = new Point(5, 120),
                Size = new Size(354, 400),
                ColumnCount = 5,
                RowCount = 7,
                BackColor = Color.Transparent
            };

            for (int i = 0; i < 5; i++)
                buttonPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 20));

            for (int i = 0; i < 7; i++)
                buttonPanel.RowStyles.Add(new RowStyle(SizeType.Absolute, 50));

            CreateButtons();
            this.Controls.Add(buttonPanel);
        }

        private void CreateButtons()
        {
            _digitButtons = new List<Button>();
            _hexButtons = new List<Button>();

            buttonPanel.ColumnCount = 5;
            buttonPanel.ColumnStyles.Clear();
            for (int i = 0; i < 5; i++)
                buttonPanel.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 20));

            buttonPanel.RowStyles.Clear();
            for (int i = 0; i < 7; i++)
                buttonPanel.RowStyles.Add(new RowStyle(SizeType.Percent, 100f / 7));

            buttonPanel.Controls.Clear();

            AddButton("MC", 0, 0, CalculatorCommand.MemoryClear);
            AddButton("MR", 0, 1, CalculatorCommand.MemoryRetrieve);
            AddButton("MS", 0, 2, CalculatorCommand.MemoryStore);
            AddButton("M+", 0, 3, CalculatorCommand.MemoryAdd);
            AddButton("←", 0, 4, CalculatorCommand.Backspace);

            AddDigitButton("0", 1, 0, 0);
            AddButton("CE", 1, 1, CalculatorCommand.ClearEntry);
            AddButton("C", 1, 2, CalculatorCommand.Clear);
            AddButton("Sqr", 1, 3, CalculatorCommand.Square);
            AddButton("Rev", 1, 4, CalculatorCommand.Inverse);

            AddDigitButton("1", 4, 0, 1);
            AddDigitButton("2", 4, 1, 2);
            AddDigitButton("3", 4, 2, 3);
            
            AddButton("*", 2, 3, CalculatorCommand.Multiply);
            AddButton("/", 2, 4, CalculatorCommand.Divide);

            AddDigitButton("4", 3, 0, 4);
            AddDigitButton("5", 3, 1, 5);
            AddDigitButton("6", 3, 2, 6);
            AddButton("+", 3, 3, CalculatorCommand.Add);
            AddButton("-", 3, 4, CalculatorCommand.Subtract);

            AddDigitButton("7", 2, 0, 7);
            AddDigitButton("8", 2, 1, 8);
            AddDigitButton("9", 2, 2, 9);
            AddButton(",", 4, 3, CalculatorCommand.Separator);
            AddButton("+/-", 4, 4, CalculatorCommand.Sign);

            AddHexButton("A", 5, 0, 10);
            AddHexButton("B", 5, 1, 11);
            AddHexButton("C", 5, 2, 12);
            AddButton("=", 5, 4, CalculatorCommand.Equals);
            buttonPanel.SetRowSpan(buttonPanel.GetControlFromPosition(4, 5), 2);

            AddHexButton("D", 6, 0, 13);
            AddHexButton("E", 6, 1, 14);
            AddHexButton("F", 6, 2, 15);
        }

        private void AddButton(string text, int row, int col, CalculatorCommand command)
        {
            var btn = new Button
            {
                Text = text,
                Dock = DockStyle.Fill,
                Font = new Font("Segoe UI", 10, FontStyle.Regular),
                Margin = new Padding(2),
                BackColor = Color.White,
                FlatStyle = FlatStyle.Flat,
                Tag = command
            };
            btn.FlatAppearance.BorderColor = Color.Gray;
            btn.Click += (s, e) => _calculator.ExecuteCommand((CalculatorCommand)((Button)s).Tag);
            buttonPanel.Controls.Add(btn, col, row);
        }

        private void AddDigitButton(string text, int row, int col, int digitValue)
        {
            var btn = new Button
            {
                Text = text,
                Dock = DockStyle.Fill,
                Font = new Font("Segoe UI", 10, FontStyle.Regular),
                Margin = new Padding(2),
                BackColor = Color.LightSteelBlue,
                FlatStyle = FlatStyle.Flat,
                Tag = digitValue
            };
            btn.FlatAppearance.BorderColor = Color.Gray;
            btn.Click += (s, e) =>
            {
                int digit = (int)((Button)s).Tag;
                CalculatorCommand cmd;
                if (digit == 10) cmd = CalculatorCommand.DigitA;
                else if (digit == 11) cmd = CalculatorCommand.DigitB;
                else if (digit == 12) cmd = CalculatorCommand.DigitC;
                else if (digit == 13) cmd = CalculatorCommand.DigitD;
                else if (digit == 14) cmd = CalculatorCommand.DigitE;
                else if (digit == 15) cmd = CalculatorCommand.DigitF;
                else cmd = (CalculatorCommand)((int)CalculatorCommand.Digit0 + digit);
                
                _calculator.ExecuteCommand(cmd);
            };
            buttonPanel.Controls.Add(btn, col, row);
            _digitButtons.Add(btn);
        }

        private void AddHexButton(string text, int row, int col, int digitValue)
        {
            var btn = new Button
            {
                Text = text,
                Dock = DockStyle.Fill,
                Font = new Font("Segoe UI", 10, FontStyle.Regular),
                Margin = new Padding(2),
                BackColor = Color.LightSteelBlue,
                FlatStyle = FlatStyle.Flat,
                Tag = digitValue
            };
            btn.FlatAppearance.BorderColor = Color.Gray;
            btn.Click += (s, e) =>
            {
                int digit = (int)((Button)s).Tag;
                CalculatorCommand cmd;
                if (digit == 10) cmd = CalculatorCommand.DigitA;
                else if (digit == 11) cmd = CalculatorCommand.DigitB;
                else if (digit == 12) cmd = CalculatorCommand.DigitC;
                else if (digit == 13) cmd = CalculatorCommand.DigitD;
                else if (digit == 14) cmd = CalculatorCommand.DigitE;
                else if (digit == 15) cmd = CalculatorCommand.DigitF;
                else cmd = (CalculatorCommand)((int)CalculatorCommand.Digit0 + digit);
                
                _calculator.ExecuteCommand(cmd);
            };
            buttonPanel.Controls.Add(btn, col, row);
            _hexButtons.Add(btn);
        }

        private void InitializeCalculator()
        {
            _calculator = new CalculatorControl();
            _calculator.DisplayUpdated += (text) =>
            {
                displayTextBox.Text = text;
                expressionLabel.Text = _calculator.GetCurrentExpression();
            };
            _calculator.MemoryStateUpdated += (state) => memoryLabel.Text = state;
            _calculator.BaseChanged += (b) => UpdateDigitButtonsState();
            
            _calculator.SetBase(10);
            UpdateDigitButtonsState();
        }

        private void SetupKeyMapping()
        {
            _keyMapping = new Dictionary<Keys, CalculatorCommand>
            {
                { Keys.D0, CalculatorCommand.Digit0 },
                { Keys.D1, CalculatorCommand.Digit1 },
                { Keys.D2, CalculatorCommand.Digit2 },
                { Keys.D3, CalculatorCommand.Digit3 },
                { Keys.D4, CalculatorCommand.Digit4 },
                { Keys.D5, CalculatorCommand.Digit5 },
                { Keys.D6, CalculatorCommand.Digit6 },
                { Keys.D7, CalculatorCommand.Digit7 },
                { Keys.D8, CalculatorCommand.Digit8 },
                { Keys.D9, CalculatorCommand.Digit9 },
                { Keys.NumPad0, CalculatorCommand.Digit0 },
                { Keys.NumPad1, CalculatorCommand.Digit1 },
                { Keys.NumPad2, CalculatorCommand.Digit2 },
                { Keys.NumPad3, CalculatorCommand.Digit3 },
                { Keys.NumPad4, CalculatorCommand.Digit4 },
                { Keys.NumPad5, CalculatorCommand.Digit5 },
                { Keys.NumPad6, CalculatorCommand.Digit6 },
                { Keys.NumPad7, CalculatorCommand.Digit7 },
                { Keys.NumPad8, CalculatorCommand.Digit8 },
                { Keys.NumPad9, CalculatorCommand.Digit9 },
                { Keys.A, CalculatorCommand.DigitA },
                { Keys.B, CalculatorCommand.DigitB },
                { Keys.C, CalculatorCommand.DigitC },
                { Keys.D, CalculatorCommand.DigitD },
                { Keys.E, CalculatorCommand.DigitE },
                { Keys.F, CalculatorCommand.DigitF },
                { Keys.Add, CalculatorCommand.Add },
                { Keys.Subtract, CalculatorCommand.Subtract },
                { Keys.Multiply, CalculatorCommand.Multiply },
                { Keys.Divide, CalculatorCommand.Divide },
                { Keys.Back, CalculatorCommand.Backspace },
                { Keys.Delete, CalculatorCommand.ClearEntry },
                { Keys.Escape, CalculatorCommand.Clear },
                { Keys.Enter, CalculatorCommand.Equals },
                { Keys.Oemcomma, CalculatorCommand.Separator },
                { Keys.Decimal, CalculatorCommand.Separator },
                { Keys.OemPeriod, CalculatorCommand.Separator }
            };
        }

        private void Form_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode >= Keys.D0 && e.KeyCode <= Keys.D9)
            {
                int digit = e.KeyCode - Keys.D0;
                if (digit >= _calculator.CurrentBase)
                {
                    e.SuppressKeyPress = true;
                    return;
                }
            }
            else if (e.KeyCode >= Keys.A && e.KeyCode <= Keys.F)
            {
                int digit = 10 + (e.KeyCode - Keys.A);
                if (digit >= _calculator.CurrentBase)
                {
                    e.SuppressKeyPress = true;
                    return;
                }
            }

            if (_keyMapping.TryGetValue(e.KeyCode, out CalculatorCommand command))
            {
                _calculator.ExecuteCommand(command);
                e.Handled = true;
                e.SuppressKeyPress = true;
            }
        }

        private void Form_KeyPress(object sender, KeyPressEventArgs e)
        {
            char c = char.ToUpper(e.KeyChar);
            
            if (char.IsLetterOrDigit(c))
            {
                int digit = "0123456789ABCDEF".IndexOf(c);
                if (digit >= 0 && digit >= _calculator.CurrentBase)
                {
                    e.Handled = true;
                    return;
                }
            }
            
            if (c == '=' || c == '\r')
            {
                _calculator.ExecuteCommand(CalculatorCommand.Equals);
                e.Handled = true;
            }
            else if (c == ',')
            {
                _calculator.ExecuteCommand(CalculatorCommand.Separator);
                e.Handled = true;
            }
            else if (c == '+')
            {
                _calculator.ExecuteCommand(CalculatorCommand.Add);
                e.Handled = true;
            }
            else if (c == '-')
            {
                _calculator.ExecuteCommand(CalculatorCommand.Subtract);
                e.Handled = true;
            }
            else if (c == '*')
            {
                _calculator.ExecuteCommand(CalculatorCommand.Multiply);
                e.Handled = true;
            }
            else if (c == '/')
            {
                _calculator.ExecuteCommand(CalculatorCommand.Divide);
                e.Handled = true;
            }
            else if (c == '\b')
            {
                _calculator.ExecuteCommand(CalculatorCommand.Backspace);
                e.Handled = true;
            }
        }

        private void BaseComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            int newBase = (int)baseComboBox.SelectedItem;
            _calculator.SetBase(newBase);
        }

        private void UpdateDigitButtonsState()
        {
            int currentBase = _calculator.CurrentBase;
            
            foreach (var btn in _digitButtons)
            {
                if (btn.Tag is int digit)
                {
                    bool isEnabled = digit < currentBase;
                    btn.Enabled = isEnabled;
                    btn.BackColor = isEnabled ? Color.LightSteelBlue : Color.LightGray;
                }
            }
            
            foreach (var btn in _hexButtons)
            {
                if (btn.Tag is int digit)
                {
                    bool isEnabled = digit < currentBase;
                    btn.Enabled = isEnabled;
                    btn.BackColor = isEnabled ? Color.LightSteelBlue : Color.LightGray;
                }
            }
        }

        private void ShowHistory()
        {
            if (_historyForm == null || _historyForm.IsDisposed)
            {
                _historyForm = new HistoryForm(_calculator.History);
                _historyForm.Owner = this;
            }
            _historyForm.Show();
            _historyForm.RefreshHistory();
        }

        private void ShowAbout()
        {
            MessageBox.Show(
                "Калькулятор p-ичных чисел\n\n" +
                "Выполняет операции над числами в системах счисления\n" +
                "с основанием от 2 до 16.\n\n" +
                "Версия 1.0",
                "О программе",
                MessageBoxButtons.OK,
                MessageBoxIcon.Information);
        }
    }
}
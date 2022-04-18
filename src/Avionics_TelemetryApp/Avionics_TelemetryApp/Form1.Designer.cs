namespace Avionics_TelemetryApp
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this._serialTextBox = new System.Windows.Forms.TextBox();
            this.battVoltLabel = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this._portComboBox = new System.Windows.Forms.ComboBox();
            this.label2 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // _serialTextBox
            // 
            this._serialTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._serialTextBox.Font = new System.Drawing.Font("Consolas", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this._serialTextBox.Location = new System.Drawing.Point(17, 38);
            this._serialTextBox.Margin = new System.Windows.Forms.Padding(8);
            this._serialTextBox.Multiline = true;
            this._serialTextBox.Name = "_serialTextBox";
            this._serialTextBox.ReadOnly = true;
            this._serialTextBox.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this._serialTextBox.Size = new System.Drawing.Size(766, 395);
            this._serialTextBox.TabIndex = 1;
            this._serialTextBox.WordWrap = false;
            // 
            // battVoltLabel
            // 
            this.battVoltLabel.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.battVoltLabel.Location = new System.Drawing.Point(130, 9);
            this.battVoltLabel.Name = "battVoltLabel";
            this.battVoltLabel.Size = new System.Drawing.Size(73, 21);
            this.battVoltLabel.TabIndex = 2;
            this.battVoltLabel.Text = "0.00 V";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Segoe UI", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point);
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(121, 21);
            this.label1.TabIndex = 0;
            this.label1.Text = "Battery voltage: ";
            // 
            // _portComboBox
            // 
            this._portComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this._portComboBox.FormattingEnabled = true;
            this._portComboBox.Items.AddRange(new object[] {
            "COM4",
            "COM5"});
            this._portComboBox.Location = new System.Drawing.Point(662, 9);
            this._portComboBox.Name = "_portComboBox";
            this._portComboBox.Size = new System.Drawing.Size(121, 23);
            this._portComboBox.TabIndex = 3;
            this._portComboBox.SelectedValueChanged += new System.EventHandler(this.portComboBox_SelectedValueChanged);
            // 
            // label2
            // 
            this.label2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(624, 14);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(32, 15);
            this.label2.TabIndex = 4;
            this.label2.Text = "Port:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.label2);
            this.Controls.Add(this._portComboBox);
            this.Controls.Add(this._serialTextBox);
            this.Controls.Add(this.battVoltLabel);
            this.Controls.Add(this.label1);
            this.Name = "Form1";
            this.Text = "Avionics Telemetry";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private TextBox _serialTextBox;
        private Label battVoltLabel;
        private Label label1;
        private ComboBox _portComboBox;
        private Label label2;
    }
}
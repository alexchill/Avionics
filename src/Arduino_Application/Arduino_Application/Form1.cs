using System.IO.Ports;

namespace Arduino_Application
{
    public partial class Form1 : Form
    {
        SerialPort _serialPort;

        public Form1()
        {
            InitializeComponent();

            _serialPort = new SerialPort("COM5", 115200);
            _serialPort.DataReceived += SerialPort_DataReceived;

            // Set the read/write timeouts
            _serialPort.ReadTimeout = 500;
            _serialPort.WriteTimeout = 500;

            _serialPort.Open();
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string line = _serialPort.ReadLine();

            if (line.StartsWith("Slider val:"))
            {
                string[] parts = line.Split(":");
                string voltage = $"{parts[1].Trim()} V";

                if (!battVoltLabel.InvokeRequired)
                {
                    battVoltLabel.Text = voltage;
                }
                else
                {
                    battVoltLabel.Invoke(() => battVoltLabel.Text = voltage);
                }

                return;
            }

            if (!serialTextBox.InvokeRequired)
            {
                serialTextBox.AppendText(line);
            }
            else
            {
                serialTextBox.Invoke(() => serialTextBox.AppendText(line + "\n"));
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            _serialPort.DataReceived -= SerialPort_DataReceived;
            _serialPort.Close();
        }
    }
}
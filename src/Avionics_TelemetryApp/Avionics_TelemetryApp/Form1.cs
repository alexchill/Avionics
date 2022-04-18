using System.IO.Ports;

namespace Avionics_TelemetryApp
{
    public partial class Form1 : Form
    {
        private SerialPort _serialPort;
        private const string _defaultCommPort = "COM5";

        public Form1()
        {
            InitializeComponent();
            _portComboBox.Text = _defaultCommPort;
            InitializeCommPort(_defaultCommPort);
        }

        private void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string line = _serialPort.ReadLine();

            if (line.StartsWith("Slider val:"))
            {
                string[] parts = line.Split(":");
                string voltage = parts[1].Trim();
                double? volts = double.TryParse(voltage, out double res) ? res : null;

                if (!battVoltLabel.InvokeRequired)
                {
                    battVoltLabel.Text = voltage;
                }
                else
                {
                    battVoltLabel.Invoke(() =>
                    {
                        battVoltLabel.Text = $"{voltage} V";
                        if (volts.HasValue)
                        {
                            Color bgColor = (volts > 12) ? Color.LightGreen : (volts > 9 ? Color.Yellow : Color.Red);
                            battVoltLabel.BackColor = bgColor;
                        }
                    });
                }

                return;
            }

            if (!_serialTextBox.InvokeRequired)
            {
                _serialTextBox.AppendText(line);
            }
            else
            {
                _serialTextBox.Invoke(() => _serialTextBox.AppendText(line + "\n"));
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            CloseCommPort();
        }

        private void portComboBox_SelectedValueChanged(object sender, EventArgs e)
        {
            if (_serialPort == null) return;

            string newline = Environment.NewLine;

            _serialTextBox.AppendText($"-------------------------------------------------{newline}");
            _serialTextBox.AppendText($"Switching to {_portComboBox.Text}{newline}");
            InitializeCommPort(_portComboBox.Text);
            if (_serialPort.IsOpen)
            {
                _serialTextBox.AppendText($"Now listening to {_serialPort.PortName}{newline}");
            }

            _serialTextBox.AppendText($"-------------------------------------------------{newline}");
        }

        private void InitializeCommPort(string portName)
        {
            try
            {
                if (_serialPort != null && _serialPort.IsOpen)
                {
                    CloseCommPort();
                }

                _serialPort = new SerialPort(portName, 115200);
                _serialPort.DataReceived += SerialPort_DataReceived;

                // Set the read/write timeouts
                _serialPort.ReadTimeout = 500;
                _serialPort.WriteTimeout = 500;

                _serialPort.Open();
            }
            catch (Exception ex)
            {
                _serialTextBox.AppendText($"Error: {ex.Message}{Environment.NewLine}");
            }
        }

        private void CloseCommPort()
        {
            _serialPort.DataReceived -= SerialPort_DataReceived;
            _serialPort.Close();
        }
    }
}
﻿namespace Breaknes
{
	public partial class FormIOAddDevice : Form
	{
		public IOConfigDevice? device_to_add = null;

		public FormIOAddDevice()
		{
			InitializeComponent();

			comboBox1.SelectedIndex = 0;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			UInt32 device_id = ComboIndexToDeviceID();
			string name = textBox1.Text;

			if (device_id != 0 && name != "")
			{
				device_to_add = new IOConfigDevice();
				device_to_add.name = name;
				device_to_add.device_id = device_id;
			}
			Close();
		}

		private UInt32 ComboIndexToDeviceID()
		{
			switch (comboBox1.SelectedIndex)
			{
				case 0: return 0x00000001;
				case 1: return 0x00000002;
				case 2: return 0x00000003;
				case 3: return 0x00000004;
				case 4: return 0x00010001;
				case 5: return 0x00010002;
				case 6: return 0x00010003;
				case 7: return 0x00010004;
			}
			return 0;
		}
	}
}

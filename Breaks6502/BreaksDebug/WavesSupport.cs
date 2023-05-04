
using SharpTools;

namespace BreaksDebug
{
	public partial class FormMain : Form
	{
		Dictionary<string, List<LogicValue>> waves = new();
		bool Paused = true;
		bool SimulationStarted = true;

		private void ResetWaves()
		{
			ValueChangeData[] vcd = new ValueChangeData[0];
			wavesControl1.PlotWaves(vcd, 0);
			waves = new();
		}

		private void toolStripButton8_Click(object sender, EventArgs e)
		{
			ResetWaves();
		}

		private void UpdateWaves()
		{
			// PHI0

			var phi0 = ToLogicValue((byte)BreaksCore.GetDebugInfoByName(BreaksCore.DebugInfoType.DebugInfoType_Board, BreaksCore.BOARD_CATEGORY, "PHI0"));
			if (!waves.ContainsKey("PHI0"))
			{
				waves.Add("PHI0", new List<LogicValue>());
			}
			waves["PHI0"].Add(phi0);

			// Update dump

			var info = BreaksCore.GetDebugInfo(BreaksCore.DebugInfoType.DebugInfoType_Core);
			foreach (var entry in info)
			{
				if (SignalFilter(entry))
				{
					if (!waves.ContainsKey(entry.name))
					{
						waves.Add(entry.name, new List<LogicValue>());
					}
					waves[entry.name].Add(ToLogicValue((byte)entry.value));
				}
			}

			// Update waves control

			ValueChangeData[] vcd = new ValueChangeData[waves.Count];

			int i = 0;
			foreach (var signal in waves.Keys)
			{
				vcd[i] = new ValueChangeData();
				vcd[i].name = signal;
				vcd[i].values = waves[signal].ToArray();
				i++;
			}

			wavesControl1.PlotWaves(vcd, 0);
		}

		private LogicValue ToLogicValue(byte val)
		{
			if (val == 0) return LogicValue.Zero;
			else if (val == 1) return LogicValue.One;
			else if (val == 0xff) return LogicValue.Z;
			else return LogicValue.X;
		}

		bool SignalFilter(BreaksCore.DebugInfoEntry entry)
		{
			if (entry.bits != 1)
				return false;

			//if (entry.category == BreaksCore.PPU_CLKS_CATEGORY && toolStripButton7.Checked) return true;

			return true;
		}

		/// <summary>
		/// Snatch Waves
		/// </summary>
		/// <param name="sender"></param>
		/// <param name="e"></param>
		private void toolStripButton7_Click(object sender, EventArgs e)
		{
			if (Paused && SimulationStarted)
			{
				if (wavesControl1.IsSelectedSomething())
				{
					long bias;
					ValueChangeData[] data = wavesControl1.SnatchSelection(out bias);
					wavesControl1.ClearSelection();
					FormSnatchWaves snatch = new FormSnatchWaves(data);
					snatch.Show();
				}
				else
				{
					MessageBox.Show("Select something first with the left mouse button. A box will appear, then click on Snatch.", "Message", MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
				}
			}
		}

		private void toolStripButton9_Click(object sender, EventArgs e)
		{

		}

		private void toolStripButton10_Click(object sender, EventArgs e)
		{

		}

		private void toolStripButton11_Click(object sender, EventArgs e)
		{

		}

		private void toolStripButton12_Click(object sender, EventArgs e)
		{

		}

		private void toolStripButton13_Click(object sender, EventArgs e)
		{

		}

	}
}

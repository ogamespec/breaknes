using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Collections;

using System.Drawing.Drawing2D;
using System.IO;
using System.Numerics;

// Samples are stored in ScanBuffer in raw form. After processing, the Scan enters the Field in a ready for output form (RGB).

namespace PPUPlayer
{
	public partial class FormMain : Form
	{
		PPUPlayerInterop.VideoSignalFeatures ppu_features;
		int SamplesPerScan;
		bool RawMode = false;

		PPUPlayerInterop.VideoOutSample[] ScanBuffer;
		int WritePtr = 0;
		bool SyncFound = false;
		int SyncPos = -1;

		Color[] field = new Color[256 * 240];
		int CurrentScan = 0;

		// Visualization settings

		static int PixelsPerSample = 1;     // Discrete pixels (Bitmap)
		static int ScaleY = 5;
		//static int PPUPicturePortion = 256 * SamplesPerPCLK;
		Color scanBgColor = Color.Gray;
		Color scanColor = Color.AliceBlue;
		Color pictureDelimiterColor = Color.Tomato;

		Bitmap? scan_pic = null;
		Bitmap? field_pic = null;

		void ResetVisualize(bool RAWMode)
		{
			PPUPlayerInterop.GetSignalFeatures(out ppu_features);

			SamplesPerScan = ppu_features.PixelsPerScan * ppu_features.SamplesPerPCLK;
			ScanBuffer = new PPUPlayerInterop.VideoOutSample[2 * SamplesPerScan];
			WritePtr = 0;

			RawMode = RAWMode;
			PPUPlayerInterop.SetRAWColorMode(RAWMode);

			SyncFound = false;
			SyncPos = -1;
			CurrentScan = 0;

			scan_pic = null;
			field_pic = null;
			GC.Collect();
		}

		void ProcessSample(PPUPlayerInterop.VideoOutSample sample)
		{
			ScanBuffer[WritePtr] = sample;

			// Check that the sample is HSync.

			bool sync = false;

			if (RawMode)
			{
				sync = (sample.raw & 0b1000000000) != 0;
			}
			else
			{
				if (sample.composite <= ppu_features.SyncLevel && ppu_features.Composite != 0)
				{
					sync = true;
				}
				else if (sample.syncLevel != 0 && ppu_features.Composite == 0)
				{
					sync = true;
				}
			}

			// If the beginning of HSync is found - remember its offset in the input buffer.

			if (sync && !SyncFound)
			{
				SyncPos = WritePtr;
				SyncFound = true;
			}

			// Advance write pointer

			WritePtr++;

			// If HSync is found and the number of samples is more than enough, process the scan.

			if ( SyncFound && (SyncPos + WritePtr) >= SamplesPerScan )
			{
				if (RawMode)
				{
					ProcessScanRAW();
				}
				else
				{
					if (ppu_features.Composite != 0)
					{
						ProcessScanComposite();
					}
					else
					{
						ProcessScanRGB();
					}
				}

				SyncFound = false;
				WritePtr = 0;
			}
		}

		void ProcessScanRAW()
		{
			int ReadPtr = SyncPos;
			PPUPlayerInterop.VideoOutSample[] batch = new PPUPlayerInterop.VideoOutSample[ppu_features.SamplesPerPCLK];

			if ((ScanBuffer[ReadPtr].raw & 0b1000000000) != 0)
			{
				ReadPtr++;
			}

			for (int i = 0; i < 256; i++)
			{
				for (int n = 0; n < ppu_features.SamplesPerPCLK; n++)
				{
					batch[n] = ScanBuffer[ReadPtr++];
				}

				if (CurrentScan < 240)
				{
					byte r, g, b;
					PPUPlayerInterop.ConvertRAWToRGB(batch[0].raw, out r, out g, out b);

					field[CurrentScan * 256 + i] = Color.FromArgb(r, g, b);
				}
			}

			CurrentScan++;
			if (CurrentScan >= ppu_features.ScansPerField)
			{
				VisualizeField();
				CurrentScan = 0;
			}
		}

		void ProcessScanRGB()
		{
			// TBD.
		}

		void ProcessScanComposite()
		{
			// TBD.
		}

		/// <summary>
		/// The appearance of the signal repeats the familiar pictures from Wikipedia. Upper level IRE = 200 (with reserve), 0 = Blank Level, IRE = -50 lower level.
		/// Therefore, the height of the picture is 250 IRE.
		/// When converting the sample to the IRE level, it is additionally shifted to make it look nice.
		/// </summary>
		void VisualizeScan()
		{
			int w = SamplesPerScan * PixelsPerSample;
			int h = 250 * ScaleY;
			float IRE = (ppu_features.WhiteLevel - ppu_features.BurstLevel) / 100.0f;

			//if (scan_pic == null)
			{
				scan_pic = new(w, h, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			}

			Bitmap pic = scan_pic;
			Graphics gr = Graphics.FromImage(pic);
			gr.Clear(scanBgColor);
			Pen pen = new(scanColor);
			Point prevPt = new(-SamplesPerScan, 0);

			//for (int n=0; n<SamplesPerScan; n++)
			//{
			//	float sample = Scan[n];
			//	float ires = ((sample - BlankLevel) / IRE) * ScaleY;

			//	Point pt = new(PixelsPerSample * n, h - ((int)ires + 200));
			//	gr.DrawLine(pen, prevPt, pt);
			//	prevPt = pt;
			//}

			//// Draw the separator for the visible part of the signal.
			//gr.DrawLine(new(pictureDelimiterColor),
			//	new(PPUPicturePortion * PixelsPerSample, 0),
			//	new(PPUPicturePortion * PixelsPerSample, h - 1));

			pictureBoxScan.Image = scan_pic;
			gr.Dispose();
		}

		void VisualizeField()
		{
			int w = 256;
			int h = 240;

			//if (field_pic == null)
			{
				field_pic = new(w, h, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
			}

			Bitmap pic = field_pic;
			Graphics gr = Graphics.FromImage(pic);

			for (int y=0; y<h; y++)
			{
				for (int x=0; x<w; x++)
				{
					Color p = field[w * y + x];
					gr.FillRectangle(new SolidBrush(p), x, y, 1, 1);
				}
			}

			pictureBoxField.Image = field_pic;
			gr.Dispose();
		}
	}
}

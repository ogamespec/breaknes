#pragma once

namespace Breaknes
{
	struct RGB_Triplet
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
	};

	class Board
	{
	protected:

		// These basic chips are on all variations of motherboards, so we make them available to all inherited classes.

		M6502Core::M6502* core = nullptr;
		APUSim::APU* apu = nullptr;
		PPUSim::PPU* ppu = nullptr;

		BaseLogic::TriState CLK = BaseLogic::TriState::Zero;

		// CPU Bus
		uint8_t data_bus = 0;
		bool data_bus_dirty = false;
		uint16_t addr_bus = 0;

		APUSim::AudioOutSignal aux{};
		PPUSim::VideoOutSignal vidSample{};

		// The cartridge slot supports hotplugging during simulation.

		AbstractCartridge* cart = nullptr;
		ConnectorType p1_type = ConnectorType::None;

		// Pre-calculated PPU palette

		RGB_Triplet* pal = nullptr;
		bool pal_cached = false;

		BaseLogic::TriState gnd = BaseLogic::TriState::Zero;
		BaseLogic::TriState vdd = BaseLogic::TriState::One;

	public:
		Board(APUSim::Revision apu_rev, PPUSim::Revision ppu_rev, ConnectorType p1);
		virtual ~Board();

		/// <summary>
		/// Simulate 1 half cycle of the test board with NSFPlayer. The simulation of the signal edge is not supported, this is overkill.
		/// </summary>
		virtual void Step() = 0;

		/// <summary>
		/// "Insert" the cartridge as a .nes ROM. In this implementation we are simply trying to instantiate an NROM, but in a more advanced emulation, Cartridge Factory will take care of "inserting" the cartridge.
		/// </summary>
		/// <param name="nesImage">A pointer to the ROM image in memory.</param>
		/// <param name="nesImageSize">ROM image size in bytes.</param>
		/// <returns></returns>
		virtual int InsertCartridge(uint8_t* nesImage, size_t nesImageSize);

		/// <summary>
		/// Remove the cartridge. Logically this means that all terminals associated with the cartridge take the value of `z`.
		/// </summary>
		virtual void EjectCartridge();

		/// <summary>
		/// Make the board /RES pins = 0 for a few CLK half cycles so that the APU/PPU resets all of its internal circuits.
		/// </summary>
		virtual void Reset();

		/// <summary>
		/// The parent application can check that the board is in the reset process and ignore the audio/video signal for that time.
		/// </summary>
		/// <returns></returns>
		virtual bool InResetState();

		/// <summary>
		/// Get the values of the ACLK cycle counter.
		/// </summary>
		/// <returns></returns>
		virtual size_t GetACLKCounter();

		/// <summary>
		/// Get the value of the 6502 core cycle counter (PHI Counter)
		/// </summary>
		/// <returns></returns>
		virtual size_t GetPHICounter();

		/// <summary>
		/// Get the current resulting AUX value in normalized [0.0; 1.0] format.
		/// </summary>
		/// <returns></returns>
		virtual void SampleAudioSignal(float* sample);

		/// <summary>
		/// Load the whole NSF data image to the BankedSRAM device.
		/// </summary>
		/// <param name="data">nsf data offset +0x80</param>
		/// <param name="data_size">nsf data size</param>
		/// <param name="load_address">nsf load address (from header)</param>
		virtual void LoadNSFData(uint8_t* data, size_t data_size, uint16_t load_address);

		/// <summary>
		/// Enable the bank switching circuit for the BankedSRAM device.
		/// </summary>
		/// <param name="enable"></param>
		virtual void EnableNSFBanking(bool enable);

		/// <summary>
		/// Load APU/PPU registers dump
		/// </summary>
		/// <param name="data">RegDumpEntry records</param>
		/// <param name="data_size">Dump size (bytes)</param>
		virtual void LoadRegDump(uint8_t* data, size_t data_size);

		/// <summary>
		/// Get audio signal settings that help with its rendering on the consumer side.
		/// </summary>
		/// <param name="features"></param>
		virtual void GetApuSignalFeatures(APUSim::AudioSignalFeatures* features);

		/// <summary>
		/// Get the "pixel" counter. Keep in mind that pixels refers to an abstract entity representing the visible or invisible part of the video signal.
		/// </summary>
		/// <returns></returns>
		virtual size_t GetPCLKCounter();

		/// <summary>
		/// Get 1 sample of the video signal.
		/// </summary>
		/// <param name="sample"></param>
		virtual void SampleVideoSignal(PPUSim::VideoOutSignal* sample);

		/// <summary>
		/// Get the direct value from the PPU H counter.
		/// </summary>
		/// <returns></returns>
		virtual size_t GetHCounter();

		/// <summary>
		/// Get the direct value from the PPU V counter.
		/// </summary>
		/// <returns></returns>
		virtual size_t GetVCounter();

		/// <summary>
		/// Forcibly enable rendering ($2001[3] = $2001[4] always equals 1). 
		/// Used for debugging PPU signals, when the CPU I/F register dump is limited, or when you want to get faster simulation results. 
		/// Keep in mind that with permanently enabled rendering the PPU becomes unstable and this hack should be applied when you know what you're doing.
		/// </summary>
		/// <param name="enable"></param>
		virtual void RenderAlwaysEnabled(bool enable);

		/// <summary>
		/// Get video signal settings that help with its rendering on the consumer side.
		/// </summary>
		/// <param name="features"></param>
		virtual void GetPpuSignalFeatures(PPUSim::VideoSignalFeatures* features);

		/// <summary>
		/// Convert the raw color to RGB. Can be used for palette generation or PPU video output in RAW mode.
		/// The SYNC level (RAW.Sync) check must be done from the outside.
		/// </summary>
		virtual void ConvertRAWToRGB(uint16_t raw, uint8_t* r, uint8_t* g, uint8_t* b);

		/// <summary>
		/// Use RAW color output. 
		/// RAW color refers to the Chroma/Luma combination that comes to the video generator and the Emphasis bit combination.
		/// </summary>
		/// <param name="enable"></param>
		virtual void SetRAWColorMode(bool enable);

		/// <summary>
		/// Set one of the ways to decay OAM cells.
		/// </summary>
		virtual void SetOamDecayBehavior(PPUSim::OAMDecayBehavior behavior);

		/// <summary>
		/// Set the noise for the composite video signal (in volts).
		/// </summary>
		/// <param name="volts"></param>
		virtual void SetNoiseLevel(float volts);

		/// <summary>
		/// Return all core debugging information for BreaksDebug.
		/// </summary>
		/// <param name="info"></param>
		virtual void GetAllCoreDebugInfo(M6502Core::DebugInfo* info);
	};
}

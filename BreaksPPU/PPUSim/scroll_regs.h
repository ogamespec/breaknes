// Scrolling Registers

#pragma once

namespace PPUSim
{
	class SCC_FF
	{
		BaseLogic::FF ff;

	public:
		void sim(BaseLogic::TriState val_in, BaseLogic::TriState n_DBE, BaseLogic::TriState RC, BaseLogic::TriState & val_out);
	};

	class ScrollRegs
	{
		friend PPUSimUnitTest::UnitTest;
		PPU* ppu = nullptr;

		SCC_FF FineH[3]{};
		SCC_FF FineV[3]{};
		SCC_FF NTV;
		SCC_FF NTH;
		SCC_FF TileV[5]{};
		SCC_FF TileH[5]{};

		bool not_derived = true;
		uint8_t Prev_DB = 0xff;

	public:
		ScrollRegs(PPU* parent);
		~ScrollRegs();

		void sim();
	};
}

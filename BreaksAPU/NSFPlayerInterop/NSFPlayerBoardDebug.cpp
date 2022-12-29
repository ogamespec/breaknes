// This module aggregates all the debugging mechanisms of NSFPlayer board.

#include "pch.h"

using namespace BaseLogic;

#define BANKED_SRAM_NAME "BankedSRAM"
#define CORE_WIRES_CATEGORY "Core Wires"
#define CORE_REGS_CATEGORY "Core Regs"
#define APU_WIRES_CATEGORY "APU Wires"
#define APU_REGS_CATEGORY "APU Regs"
#define BOARD_CATEGORY "NSFPlayer Board"

namespace NSFPlayer
{
	void Board::AddBoardMemDescriptors()
	{
		// Banked SRAM

		MemDesciptor* sramRegion = new MemDesciptor;
		memset(sramRegion, 0, sizeof(MemDesciptor));
		strcpy_s(sramRegion->name, sizeof(sramRegion->name), BANKED_SRAM_NAME);
		sramRegion->size = (int32_t)sram->Dbg_GetSize();
		dbg_hub->AddMemRegion(sramRegion, DumpSRAM, WriteSRAM, this, false);
	}

	struct SignalOffsetPair
	{
		const char* name;
		const size_t offset;
		const uint8_t bits;
	};

	SignalOffsetPair core_wires[] = {
		"/PRDY", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_PRDY), 1,
		"/NMIP", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_NMIP), 1,
		"/IRQP", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_IRQP), 1,
		"RESP", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::RESP), 1,
		"BRK6E", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::BRK6E), 1,
		"BRK7", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::BRK7), 1,
		"DORES", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DORES), 1,
		"/DONMI", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_DONMI), 1,
		"/T2", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_T2), 1,
		"/T3", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_T3), 1,
		"/T4", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_T4), 1,
		"/T5", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_T5), 1,
		"T0", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::T0), 1,
		"/T0", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_T0), 1,
		"/T1X", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_T1X), 1,
		"Z_IR", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_IR), 1,
		"FETCH", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::FETCH), 1,
		"/ready", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_ready), 1,
		"WR", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::WR), 1,
		"TRES2", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::TRES2), 1,
		"ACRL1", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ACRL1), 1,
		"ACRL2", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ACRL2), 1,
		"T1", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::T1), 1,
		"T5", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::T5), 1,
		"T6", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::T6), 1,
		"ENDS", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ENDS), 1,
		"ENDX", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ENDX), 1,
		"TRES1", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::TRES1), 1,
		"TRESX", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::TRESX), 1,
		"BRFW", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::BRFW), 1,
		"/BRTAKEN", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_BRTAKEN), 1,
		"ACR", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ACR), 1,
		"AVR", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::AVR), 1,

		"Y_SB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Y_SB), 1,
		"SB_Y", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SB_Y), 1,
		"X_SB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::X_SB), 1,
		"SB_X", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SB_X), 1,
		"S_ADL", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::S_ADL), 1,
		"S_SB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::S_SB), 1,
		"SB_S", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SB_S), 1,
		"S_S", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::S_S), 1,
		"NDB_ADD", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::NDB_ADD), 1,
		"DB_ADD", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DB_ADD), 1,
		"Z_ADD", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_ADD), 1,
		"SB_ADD", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SB_ADD), 1,
		"ADL_ADD", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADL_ADD), 1,
		"ANDS", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ANDS), 1,
		"EORS", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::EORS), 1,
		"ORS", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ORS), 1,
		"SRS", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SRS), 1,
		"SUMS", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SUMS), 1,
		"ADD_SB7", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADD_SB7), 1,
		"ADD_SB06", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADD_SB06), 1,
		"ADD_ADL", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADD_ADL), 1,
		"SB_AC", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SB_AC), 1,
		"AC_SB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::AC_SB), 1,
		"AC_DB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::AC_DB), 1,
		"ADH_PCH", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADH_PCH), 1,
		"PCH_PCH", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::PCH_PCH), 1,
		"PCH_ADH", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::PCH_ADH), 1,
		"PCH_DB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::PCH_DB), 1,
		"ADL_PCL", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADL_PCL), 1,
		"PCL_PCL", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::PCL_PCL), 1,
		"PCL_ADL", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::PCL_ADL), 1,
		"PCL_DB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::PCL_DB), 1,
		"ADH_ABH", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADH_ABH), 1,
		"ADL_ABL", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ADL_ABL), 1,
		"Z_ADL0", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_ADL0), 1,
		"Z_ADL1", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_ADL1), 1,
		"Z_ADL2", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_ADL2), 1,
		"Z_ADH0", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_ADH0), 1,
		"Z_ADH17", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_ADH17), 1,
		"SB_DB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SB_DB), 1,
		"SB_ADH", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::SB_ADH), 1,
		"DL_ADL", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DL_ADL), 1,
		"DL_ADH", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DL_ADH), 1,
		"DL_DB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DL_DB), 1,

		"P_DB", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::P_DB), 1,
		"DB_P", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DB_P), 1,
		"DBZ_Z", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DBZ_Z), 1,
		"DB_N", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DB_N), 1,
		"IR5_C", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::IR5_C), 1,
		"DB_C", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DB_C), 1,
		"ACR_C", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::ACR_C), 1,
		"IR5_D", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::IR5_D), 1,
		"IR5_I", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::IR5_I), 1,
		"DB_V", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::DB_V), 1,
		"AVR_V", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::AVR_V), 1,
		"Z_V", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::Z_V), 1,
		"n_ACIN", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_ACIN), 1,
		"n_DAA", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_DAA), 1,
		"n_DSA", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_DSA), 1,
		"n_1PC", offsetof(M6502Core::DebugInfo, M6502Core::DebugInfo::n_1PC), 1,
	};

	SignalOffsetPair core_regs[] = {
		"A", offsetof(M6502Core::UserRegs, A), 8,
		"X", offsetof(M6502Core::UserRegs, X), 8,
		"Y", offsetof(M6502Core::UserRegs, Y), 8,
		"S", offsetof(M6502Core::UserRegs, S), 8,
		"C_OUT", offsetof(M6502Core::UserRegs, C_OUT), 8,
		"Z_OUT", offsetof(M6502Core::UserRegs, Z_OUT), 8,
		"I_OUT", offsetof(M6502Core::UserRegs, I_OUT), 8,
		"D_OUT", offsetof(M6502Core::UserRegs, D_OUT), 8,
		"V_OUT", offsetof(M6502Core::UserRegs, V_OUT), 8,
		"N_OUT", offsetof(M6502Core::UserRegs, N_OUT), 8,
		"PCH", offsetof(M6502Core::UserRegs, PCH), 8,
		"PCL", offsetof(M6502Core::UserRegs, PCL), 8,
	};

	SignalOffsetPair apu_wires[] = {
		"/CLK", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_CLK), 1,
		"PHI0", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::PHI0), 1,
		"PHI1", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::PHI1), 1,
		"PHI2", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::PHI2), 1,
		"RDY", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::RDY), 1,
		"ACLK", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::ACLK), 1,
		"/ACLK", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_ACLK), 1,
		"RES", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::RES), 1,
		"/M2", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_M2), 1,
		"/NMI", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_NMI), 1,
		"/IRQ", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_IRQ), 1,
		"INT", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::INT), 1,
		"/LFO1", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_LFO1), 1,
		"/LFO2", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_LFO2), 1,
		"R/W", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::RnW), 1,
		"SPR/CPU", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::SPR_CPU), 1,
		"SPR/PPU", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::SPR_PPU), 1,
		"RW", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::RW), 1,
		"RD", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::RD), 1,
		"WR", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::WR), 1,
		"#DMC/AB", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_DMC_AB), 1,
		"RUNDMC", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::RUNDMC), 1,
		"DMCINT", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::DMCINT), 1,
		"DMCRDY", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::DMCRDY), 1,
		"/R4015", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_R4015), 1,
		"/R4016", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_R4016), 1,
		"/R4017", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_R4017), 1,
		"/R4018", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_R4018), 1,
		"/R4019", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_R4019), 1,
		"/R401A", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_R401A), 1,
		"W4000", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4000), 1,
		"W4001", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4001), 1,
		"W4002", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4002), 1,
		"W4003", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4003), 1,
		"W4004", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4004), 1,
		"W4005", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4005), 1,
		"W4006", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4006), 1,
		"W4007", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4007), 1,
		"W4008", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4008), 1,
		"W400A", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W400A), 1,
		"W400B", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W400B), 1,
		"W400C", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W400C), 1,
		"W400E", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W400E), 1,
		"W400F", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W400F), 1,
		"W4010", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4010), 1,
		"W4011", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4011), 1,
		"W4012", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4012), 1,
		"W4013", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4013), 1,
		"W4014", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4014), 1,
		"W4015", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4015), 1,
		"W4016", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4016), 1,
		"W4017", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W4017), 1,
		"W401A", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::W401A), 1,
		"SQA/LC", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::SQA_LC), 1,
		"SQB/LC", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::SQB_LC), 1,
		"TRI/LC", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::TRI_LC), 1,
		"RND/LC", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::RND_LC), 1,
		"NOSQA", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::NOSQA), 1,
		"NOSQB", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::NOSQB), 1,
		"NOTRI", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::NOTRI), 1,
		"NORND", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::NORND), 1,
		"DBG", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::DBG), 1,
		"/DBGRD", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::n_DBGRD), 1,
		"LOCK", offsetof(APUSim::APU_Interconnects, APUSim::APU_Interconnects::LOCK), 1,
	};

	SignalOffsetPair apu_regs[] = {
		"Bogus", offsetof(APUSim::APU_Registers, Bogus), 8,

		// TBD: Add here all the debugging APU entities as they appear.
	};

	SignalOffsetPair board_signals[] = {
		"BoardCLK", offsetof(BoardDebugInfo, CLK), 1,

		// TBD: Add here all the debugging APU entities as they appear.
	};

	void Board::AddDebugInfoProviders()
	{
		for (size_t n = 0; n < _countof(core_wires); n++)
		{
			SignalOffsetPair* sp = &core_wires[n];

			DebugInfoEntry* entry = new DebugInfoEntry;
			memset(entry, 0, sizeof(DebugInfoEntry));
			strcpy_s(entry->category, sizeof(entry->category), CORE_WIRES_CATEGORY);
			strcpy_s(entry->name, sizeof(entry->name), sp->name);
			dbg_hub->AddDebugInfo(DebugInfoType::DebugInfoType_Core, entry, GetCoreDebugInfo, this);
		}

		for (size_t n = 0; n < _countof(core_regs); n++)
		{
			SignalOffsetPair* sp = &core_regs[n];

			DebugInfoEntry* entry = new DebugInfoEntry;
			memset(entry, 0, sizeof(DebugInfoEntry));
			strcpy_s(entry->category, sizeof(entry->category), CORE_REGS_CATEGORY);
			strcpy_s(entry->name, sizeof(entry->name), sp->name);
			dbg_hub->AddDebugInfo(DebugInfoType::DebugInfoType_CoreRegs, entry, GetCoreRegsDebugInfo, this);
		}

		for (size_t n = 0; n < _countof(apu_wires); n++)
		{
			SignalOffsetPair* sp = &apu_wires[n];

			DebugInfoEntry* entry = new DebugInfoEntry;
			memset(entry, 0, sizeof(DebugInfoEntry));
			strcpy_s(entry->category, sizeof(entry->category), APU_WIRES_CATEGORY);
			strcpy_s(entry->name, sizeof(entry->name), sp->name);
			dbg_hub->AddDebugInfo(DebugInfoType::DebugInfoType_APU, entry, GetApuDebugInfo, this);
		}

		for (size_t n = 0; n < _countof(apu_regs); n++)
		{
			SignalOffsetPair* sp = &apu_regs[n];

			DebugInfoEntry* entry = new DebugInfoEntry;
			memset(entry, 0, sizeof(DebugInfoEntry));
			strcpy_s(entry->category, sizeof(entry->category), APU_REGS_CATEGORY);
			strcpy_s(entry->name, sizeof(entry->name), sp->name);
			dbg_hub->AddDebugInfo(DebugInfoType::DebugInfoType_APURegs, entry, GetApuRegsDebugInfo, this);
		}

		for (size_t n = 0; n < _countof(board_signals); n++)
		{
			SignalOffsetPair* sp = &board_signals[n];

			DebugInfoEntry* entry = new DebugInfoEntry;
			memset(entry, 0, sizeof(DebugInfoEntry));
			strcpy_s(entry->category, sizeof(entry->category), BOARD_CATEGORY);
			strcpy_s(entry->name, sizeof(entry->name), sp->name);
			dbg_hub->AddDebugInfo(DebugInfoType::DebugInfoType_Board, entry, GetBoardDebugInfo, this);
		}
	}

	uint8_t Board::DumpSRAM(void* opaque, size_t addr)
	{
		Board* board = (Board*)opaque;
		return board->sram->Dbg_ReadByte(addr);
	}

	void Board::WriteSRAM(void* opaque, size_t addr, uint8_t data)
	{
		Board* board = (Board*)opaque;
		board->sram->Dbg_WriteByte(addr, data);
	}

	uint32_t Board::GetCoreDebugInfo(void* opaque, DebugInfoEntry* entry, uint8_t& bits)
	{
		Board* board = (Board*)opaque;

		for (size_t n = 0; n < _countof(core_wires); n++)
		{
			SignalOffsetPair* sp = &core_wires[n];

			if (!strcmp(sp->name, entry->name))
			{
				M6502Core::DebugInfo wires{};
				board->core->getDebug(&wires);

				uint8_t* ptr = (uint8_t*)&wires;

				bits = sp->bits;
				return ptr[sp->offset];
			}
		}

		return 0;
	}

	uint32_t Board::GetCoreRegsDebugInfo(void* opaque, DebugInfoEntry* entry, uint8_t& bits)
	{
		Board* board = (Board*)opaque;

		for (size_t n = 0; n < _countof(core_regs); n++)
		{
			SignalOffsetPair* sp = &core_regs[n];

			if (!strcmp(sp->name, entry->name))
			{
				M6502Core::UserRegs regs{};
				board->core->getUserRegs(&regs);

				bits = sp->bits;
				uint8_t* ptr = (uint8_t*)&regs + sp->offset;
				return *(uint32_t*)ptr;
			}
		}

		return 0;
	}

	uint32_t Board::GetApuDebugInfo(void* opaque, DebugInfoEntry* entry, uint8_t& bits)
	{
		Board* board = (Board*)opaque;

		for (size_t n = 0; n < _countof(apu_wires); n++)
		{
			SignalOffsetPair* sp = &apu_wires[n];

			if (!strcmp(sp->name, entry->name))
			{
				APUSim::APU_Interconnects wires{};
				board->apu->GetDebugInfo_Wires(wires);

				uint8_t* ptr = (uint8_t*)&wires;

				bits = sp->bits;
				return ptr[sp->offset];
			}
		}

		return 0;
	}

	uint32_t Board::GetApuRegsDebugInfo(void* opaque, DebugInfoEntry* entry, uint8_t& bits)
	{
		Board* board = (Board*)opaque;

		for (size_t n = 0; n < _countof(apu_regs); n++)
		{
			SignalOffsetPair* sp = &apu_regs[n];

			if (!strcmp(sp->name, entry->name))
			{
				APUSim::APU_Registers regs{};
				board->apu->GetDebugInfo_Regs(regs);

				bits = sp->bits;
				uint8_t* ptr = (uint8_t*)&regs + sp->offset;
				return *(uint32_t*)ptr;
			}
		}

		return 0;
	}

	void Board::GetDebugInfo(BoardDebugInfo& info)
	{
		info.CLK = CLK;
	}

	uint32_t Board::GetBoardDebugInfo(void* opaque, DebugInfoEntry* entry, uint8_t& bits)
	{
		Board* board = (Board*)opaque;

		for (size_t n = 0; n < _countof(board_signals); n++)
		{
			SignalOffsetPair* sp = &board_signals[n];

			if (!strcmp(sp->name, entry->name))
			{
				BoardDebugInfo info{};
				board->GetDebugInfo(info);

				bits = sp->bits;
				uint8_t* ptr = (uint8_t*)&info + sp->offset;
				return *(uint32_t*)ptr;
			}
		}

		return 0;
	}
}

// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>

#include <cstring>

#include "instructions.h"
#include "opcodes.h"
#include "registers.h"
#include "util.h"

using namespace BinaryNinja;
using namespace Tricore;

class TricoreArchitecture : public Architecture {
 public:
  explicit TricoreArchitecture(const std::string& name) : Architecture(name) {}
  static BNRegisterInfo RegisterInfo(uint32_t fullWidthReg, size_t offset,
                                     size_t size, bool zeroExtend = false) {
    BNRegisterInfo result;
    result.fullWidthRegister = fullWidthReg;
    result.offset =
        offset;  // in bytes (TODO: correct? if not fix GetRegisterInfo)
    result.size = size;
    result.extend = zeroExtend ? ZeroExtendToFullWidth : NoExtend;
    return result;
  }

  size_t GetAddressSize() const override { return 4; }

  BNEndianness GetEndianness() const override { return LittleEndian; }

  size_t GetDefaultIntegerSize() const override { return 4; }

  size_t GetInstructionAlignment() const override { return 2; }

  size_t GetMaxInstructionLength() const override { return 4; }

  bool ConvertToNop(uint8_t* data, uint64_t, size_t len) override {
    auto wdata = reinterpret_cast<const uint16_t*>(data);

    // 32-bit
    if (const uint16_t raw = *wdata; raw & 0x01u) {
      constexpr uint32_t nop32 = 0x0000000d;
      if (len < sizeof(nop32)) return false;
      for (size_t i = 0; i < len / sizeof(nop32); i++)
        reinterpret_cast<uint32_t*>(data)[i] = nop32;
      return true;
    }
    // 16-bit
    constexpr uint16_t nop16 = 0x0000;
    if (len < sizeof(nop16)) return false;
    for (size_t i = 0; i < len / sizeof(nop16); i++)
      reinterpret_cast<uint16_t*>(data)[i] = nop16;
    return true;
  }

  std::vector<uint32_t> GetAllRegisters() override {
    return std::vector<uint32_t>{
        TRICORE_REG_D0,   TRICORE_REG_D1,  TRICORE_REG_D2,  TRICORE_REG_D3,
        TRICORE_REG_D4,   TRICORE_REG_D5,  TRICORE_REG_D6,  TRICORE_REG_D7,
        TRICORE_REG_D8,   TRICORE_REG_D9,  TRICORE_REG_D10, TRICORE_REG_D11,
        TRICORE_REG_D12,  TRICORE_REG_D13, TRICORE_REG_D14, TRICORE_REG_D15,
        TRICORE_REG_A0,   TRICORE_REG_A1,  TRICORE_REG_A2,  TRICORE_REG_A3,
        TRICORE_REG_A4,   TRICORE_REG_A5,  TRICORE_REG_A6,  TRICORE_REG_A7,
        TRICORE_REG_A8,   TRICORE_REG_A9,  TRICORE_REG_A10, TRICORE_REG_A11,
        TRICORE_REG_A12,  TRICORE_REG_A13, TRICORE_REG_A14, TRICORE_REG_A15,
        TRICORE_REG_PCXI, TRICORE_REG_PSW,
        TRICORE_REG_PC  // TODO: add other system control registers?
    };
  }

  std::vector<uint32_t> GetGlobalRegisters() override {
    return std::vector<uint32_t>{
        TRICORE_REG_A0,
        TRICORE_REG_A1,
        TRICORE_REG_A8,
        TRICORE_REG_A9,
    };
  }

  BNRegisterInfo GetRegisterInfo(uint32_t rid) override {
    std::vector<uint32_t> regs = GetAllRegisters();
    // TODO: This is a temporary fix to test--need to fix this for syscontrol
    // regs
    if (rid >= TRICORE_REG_D0 && rid <= TRICORE_REG_A15) {
      // All registers are full-length (4 bytes)
      return RegisterInfo(rid, 0, 4);
    } else {
      return RegisterInfo(0, 0, 0);
    }
  }

  std::string GetRegisterName(uint32_t rid) override {
    const char* result = Instruction::RegToStr(rid);
    if (result == nullptr) {
      result = "GetRegisterName: INVALID_REG_ID";
    }
    return result;
  }

  bool GetInstructionInfo(const uint8_t* data, uint64_t addr, size_t maxLen,
                          InstructionInfo& result) override {
    auto wdata = (const uint16_t*)data;
    uint16_t raw = *wdata;
    uint8_t seg;
    uint16_t caddr;
    uint32_t target;

    // Set a default length in case we don't find a valid instruction
    // (this keeps binja from throwing invalid instr. length errors)
    result.length = 4;

    // 32-bit instruction
    if (raw & 0x01u) {
      // There are two instructions of the BRN format, which has a 7-bit primary
      // opcode (instead of 8-bit)
      if ((raw & 0x7Fu) == OP1_x6F_BRN) {
        uint32_t op2 = Instruction::ExtractSecondaryOpcode(raw & 0x7Fu, data);
        if (op2 == JZT_DA_N_DISP15) {
          return Jzt::Info(data, addr, maxLen, result);
        } else if (op2 == JNZT_DA_N_DISP15) {
          return Jnzt::Info(data, addr, maxLen, result);
        }
        return false;
      }
      // This will get op2 if it exists
      uint32_t op2 = Instruction::ExtractSecondaryOpcode(raw & 0xFFu, data);
      op2 &= 0xFFu;  // secondary opcodes are not > 0xFF, even though some
                     // formats have up to 12 bits
      switch (raw & 0xFFu) {
        // 4-byte non-branching, single opcode
        case LDB_DA_AB_OFF16:   // TCv1.6 ISA only
        case LDBU_DA_AB_OFF16:  // TCv1.6 ISA only
        case LDH_DA_AB_OFF16:   // TCv1.6 ISA only
        case LDHU_DA_AB_OFF16:  // TCv1.6 ISA only
        case STB_AB_OFF16_DA:   // TCv1.6 ISA only
        case STH_AB_OFF16_DA:   // TCv1.6 ISA only
        case STA_AB_OFF16_AA:   // TCv1.6 ISA only
        case ADDI_DC_DA_CONST16:
        case ADDIH_DC_DA_CONST16:
        case ADDIHA_AC_AA_CONST16:
        case LDA_AA_AB_OFF16:
        case LDW_DA_AB_OFF16:
        case LEA_AA_AB_OFF16:
        case MFCR_DC_CONST16:
        case MOV_DC_CONST16:
        case MOV_EC_CONST16:  // TCv1.6 ISA only
        case MOVU_DC_CONST16:
        case MOVH_DC_CONST16:
        case MOVHA_AC_CONST16:
        case MTCR_CONST16_DA:
        case STW_AB_OFF16_DA:
          result.length = 4;
          return true;

        // 4-byte branching, single opcode
        case CALL_DISP24:
          return Call::Info(data, addr, maxLen, result);
        case CALLA_DISP24:
          return Calla::Info(data, addr, maxLen, result);
        case FCALL_DISP24:  // TCv1.6 ISA only
          return Fcall::Info(data, addr, maxLen, result);
        case FCALLA_DISP24:  // TCv1.6 ISA only
          return Fcalla::Info(data, addr, maxLen, result);
        case J_DISP24:
          return J::Info(data, addr, maxLen, result);
        case JA_DISP24:
          return Ja::Info(data, addr, maxLen, result);
        case JL_DISP24:
          return Jl::Info(data, addr, maxLen, result);
        case JLA_DISP24:
          return Jla::Info(data, addr, maxLen, result);

        // 4-byte, dual opcode
        case OP1_x01_RR:
          switch (op2) {
            case ADDA_AC_AA_AB:
            case ADDSCA_AC_AB_DA_N:
            case ADDSCAT_AC_AB_DA:
            case EQA_DC_AA_AB:
            case EQZA_DC_AA:
            case GEA_DC_AA_AB:
            case LTA_DC_AA_AB:
            case MOVA_AC_DB:
            case MOVAA_AC_AB:
            case MOVD_DC_AB:
            case NEA_DC_AA_AB:
            case NEZA_DC_AA:
            case SUBA_AC_AA_AB:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x03_RRR2:
          switch (op2) {
            case MADD_DC_DD_DA_DB:
            case MADD_EC_ED_DA_DB:
            case MADDS_DC_DD_DA_DB:
            case MADDS_EC_ED_DA_DB:
            case MADDU_EC_ED_DA_DB:
            case MADDSU_DC_DD_DA_DB:
            case MADDSU_EC_ED_DA_DB:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x05_ABS:
          switch (op2) {
            case LDB_DA_OFF18:
            case LDBU_DA_OFF18:
            case LDH_DA_OFF18:
            case LDHU_DA_OFF18:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x07_BIT:
          switch (op2) {
            case NANDT_DC_DA_POS1_DB_POS2:
            case ORNT_DC_DA_POS1_DB_POS2:
            case XNORT_DC_DA_POS1_DB_POS2:
            case XORT_DC_DA_POS1_DB_POS2:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x09_BO:
          switch (op2) {
            case LDA_AA_AB_OFF10:
            case LDA_AA_AB_OFF10_POSTINC:
            case LDA_AA_AB_OFF10_PREINC:
            case LDB_DA_AB_OFF10:
            case LDB_DA_AB_OFF10_POSTINC:
            case LDB_DA_AB_OFF10_PREINC:
            case LDBU_DA_AB_OFF10:
            case LDBU_DA_AB_OFF10_POSTINC:
            case LDBU_DA_AB_OFF10_PREINC:
            case LDD_EA_AB_OFF10:
            case LDD_EA_AB_OFF10_POSTINC:
            case LDD_EA_AB_OFF10_PREINC:
            case LDDA_PA_AB_OFF10:
            case LDDA_PA_AB_OFF10_POSTINC:
            case LDDA_PA_AB_OFF10_PREINC:
            case LDH_DA_AB_OFF10:
            case LDH_DA_AB_OFF10_POSTINC:
            case LDH_DA_AB_OFF10_PREINC:
            case LDHU_DA_AB_OFF10:
            case LDHU_DA_AB_OFF10_POSTINC:
            case LDHU_DA_AB_OFF10_PREINC:
            case LDQ_DA_AB_OFF10:
            case LDQ_DA_AB_OFF10_POSTINC:
            case LDQ_DA_AB_OFF10_PREINC:
            case LDW_DA_AB_OFF10:
            case LDW_DA_AB_OFF10_POSTINC:
            case LDW_DA_AB_OFF10_PREINC:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x0B_RR:
          switch (op2) {
            case ABS_DC_DB:
            case ABSB_DC_DB:
            case ABSH_DC_DB:
            case ABSDIF_DC_DA_DB:
            case ABSDIFB_DC_DA_DB:
            case ABSDIFH_DC_DA_DB:
            case ABSDIFS_DC_DA_DB:
            case ABSDIFSH_DC_DA_DB:
            case ABSS_DC_DB:
            case ABSSH_DC_DB:
            case ADD_DC_DA_DB:
            case ADDB_DC_DA_DB:
            case ADDH_DC_DA_DB:
            case ADDC_DC_DA_DB:
            case ADDS_DC_DA_DB:
            case ADDSH_DC_DA_DB:
            case ADDSHU_DC_DA_DB:
            case ADDSU_DC_DA_DB:
            case ADDX_DC_DA_DB:
            case ANDEQ_DC_DA_DB:
            case ANDGE_DC_DA_DB:
            case ANDGEU_DC_DA_DB:
            case ANDLT_DC_DA_DB:
            case ANDLTU_DC_DA_DB:
            case ANDNE_DC_DA_DB:
            case EQ_DC_DA_DB:
            case EQB_DC_DA_DB:
            case EQH_DC_DA_DB:
            case EQW_DC_DA_DB:
            case EQANYB_DC_DA_DB:
            case EQANYH_DC_DA_DB:
            case GE_DC_DA_DB:
            case GEU_DC_DA_DB:
            case LT_DC_DA_DB:
            case LTU_DC_DA_DB:
            case LTB_DC_DA_DB:
            case LTBU_DC_DA_DB:
            case LTH_DC_DA_DB:
            case LTHU_DC_DA_DB:
            case LTW_DC_DA_DB:
            case LTWU_DC_DA_DB:
            case MAX_DC_DA_DB:
            case MAXU_DC_DA_DB:
            case MAXB_DC_DA_DB:
            case MAXBU_DC_DA_DB:
            case MAXH_DC_DA_DB:
            case MAXHU_DC_DA_DB:
            case MIN_DC_DA_DB:
            case MINU_DC_DA_DB:
            case MINB_DC_DA_DB:
            case MINBU_DC_DA_DB:
            case MINH_DC_DA_DB:
            case MINHU_DC_DA_DB:
            case MOV_DC_DB:
            case MOV_EC_DB:     // TCv1.6 ISA only
            case MOV_EC_DA_DB:  // TCv1.6 ISA only
            case NE_DC_DA_DB:
            case OREQ_DC_DA_DB:
            case ORGE_DC_DA_DB:
            case ORGEU_DC_DA_DB:
            case ORLT_DC_DA_DB:
            case ORLTU_DC_DA_DB:
            case ORNE_DC_DA_DB:
            case SATB_DC_DA:
            case SATBU_DC_DA:
            case SATH_DC_DA:
            case SATHU_DC_DA:
            case SHEQ_DC_DA_DB:
            case SHGE_DC_DA_DB:
            case SHGEU_DC_DA_DB:
            case SHLT_DC_DA_DB:
            case SHLTU_DC_DA_DB:
            case SHNE_DC_DA_DB:
            case SUB_DC_DA_DB:
            case SUBB_DC_DA_DB:
            case SUBH_DC_DA_DB:
            case SUBC_DC_DA_DB:
            case SUBS_DC_DA_DB:
            case SUBSU_DC_DA_DB:
            case SUBSH_DC_DA_DB:
            case SUBSHU_DC_DA_DB:
            case SUBX_DC_DA_DB:
            case XOREQ_DC_DA_DB:
            case XORGE_DC_DA_DB:
            case XORGEU_DC_DA_DB:
            case XORLT_DC_DA_DB:
            case XORLTU_DC_DA_DB:
            case XORNE_DC_DA_DB:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x0D_SYS:
          switch (op2) {
            case DEBUG32:
            case DISABLE:
            case DSYNC:
            case ENABLE:
            case ISYNC:
            case NOP32:
            case RSLCX:
            case SVLCX:
              result.length = 4;
              return true;
            case TRAPSV:
              return Trapsv::Info(data, addr, maxLen, result);
            case TRAPV:
              return Trapv::Info(data, addr, maxLen, result);
            case RET32:
            case RFE32:
            case RFM:
            case FRET32:
              result.AddBranch(FunctionReturn);
              result.length = 4;
              return true;
            case DISABLE_DA:
            case RESTORE_DA:
            case WAIT:  // TCv1.6 ISA only
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x0F_RR:
          switch (op2) {
            case AND_DC_DA_DB:
            case ANDN_DC_DA_DB:
            case CLO_DC_DA:
            case CLOH_DC_DA:
            case CLS_DC_DA:
            case CLSH_DC_DA:
            case CLZ_DC_DA:
            case CLZH_DC_DA:
            case NAND_DC_DA_DB:
            case NOR_DC_DA_DB:
            case OR_DC_DA_DB:
            case ORN_DC_DA_DB:
            case SH_DC_DA_DB:
            case SHH_DC_DA_DB:
            case SHA_DC_DA_DB:
            case SHAH_DC_DA_DB:
            case SHAS_DC_DA_DB:
            case XNOR_DC_DA_DB:
            case XOR_DC_DA_DB:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x13_RCR:
          switch (op2) {
            case MADD_DC_DD_DA_CONST9:
            case MADD_EC_ED_DA_CONST9:
            case MADDS_DC_DD_DA_CONST9:
            case MADDS_EC_ED_DA_CONST9:
            case MADDU_EC_ED_DA_CONST9:
            case MADDSU_DC_DD_DA_CONST9:
            case MADDSU_EC_ED_DA_CONST9:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x15_ABS:
          switch (op2) {
            case LDLCX_OFF18:
            case LDUCX_OFF18:
            case STLCX_OFF18:
            case STUCX_OFF18:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x17_RRRR:
          switch (op2) {
            case DEXTR_DC_DA_DB_DD:
            case EXTR_DC_DA_ED:
            case EXTRU_DC_DA_ED:
            case INSERT_DC_DA_DB_ED:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x1F_BRR:
          switch (op2) {
            case JNED_DA_DB_DISP15:
              return Jned::Info(data, addr, maxLen, result);
            case JNEI_DA_DB_DISP15:
              return Jnei::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_x23_RRR2:
          switch (op2) {
            case MSUB_DC_DD_DA_DB:
            case MSUB_EC_ED_DA_DB:
            case MSUBS_DC_DD_DA_DB:
            case MSUBS_EC_ED_DA_DB:
            case MSUBU_EC_ED_DA_DB:
            case MSUBSU_DC_DD_DA_DB:
            case MSUBSU_EC_ED_DA_DB:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x25_ABS:
          switch (op2) {
            case STB_OFF18_DA:
            case STH_OFF18_DA:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x27_BIT:
          switch (op2) {
            case SHANDT_DC_DA_POS1_DB_POS2:
            case SHANDNT_DC_DA_POS1_DB_POS2:
            case SHNORT_DC_DA_POS1_DB_POS2:
            case SHORT_DC_DA_POS1_DB_POS2:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x29_BO:
          switch (op2) {
            case LDA_AA_PB:
            case LDA_AA_PB_OFF10:
            case LDB_DA_PB:
            case LDB_DA_PB_OFF10:
            case LDBU_DA_PB:
            case LDBU_DA_PB_OFF10:
            case LDD_EA_PB:
            case LDD_EA_PB_OFF10:
            case LDDA_PA_PB:
            case LDDA_PA_PB_OFF10:
            case LDH_DA_PB:
            case LDH_DA_PB_OFF10:
            case LDHU_DA_PB:
            case LDHU_DA_PB_OFF10:
            case LDQ_DA_PB:
            case LDQ_DA_PB_OFF10:
            case LDW_DA_PB:
            case LDW_DA_PB_OFF10:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x2B_RRR:
          switch (op2) {
            case CADD_DC_DD_DA_DB:
            case CADDN_DC_DD_DA_DB:
            case CSUB_DC_DD_DA_DB:
            case CSUBN_DC_DD_DA_DB:
            case SEL_DC_DD_DA_DB:
            case SELN_DC_DD_DA_DB:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x2D_RR:
          switch (op2) {
            case CALLI_AA:
              return Calli::Info(data, addr, maxLen, result);
            case FCALLI_AA:
              return Fcalli::Info(data, addr, maxLen, result);
            case JI_AA_32:
              return Ji::Info(data, addr, maxLen, result);
            case JLI_AA:
              return Jli::Info(data, addr, maxLen, result);
            default:
              break;
          }
        case OP1_x2F_SYS:
          if (op2 == RSTV) {
            result.length = 4;
            return true;
          }
          return false;
        case OP1_x33_RCR:
          switch (op2) {
            case MSUB_DC_DD_DA_CONST9:
            case MSUB_EC_ED_DA_CONST9:
            case MSUBS_DC_DD_DA_CONST9:
            case MSUBS_EC_ED_DA_CONST9:
            case MSUBU_EC_ED_DA_CONST9:
            case MSUBSU_DC_DD_DA_CONST9:
            case MSUBSU_EC_ED_DA_CONST9:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x37_RRPW:
          switch (op2) {
            case EXTR_DC_DA_POS_WIDTH:
            case EXTRU_DC_DA_POS_WIDTH:
            case IMASK_EC_DB_POS_WIDTH:
            case INSERT_DC_DA_DB_POS_WIDTH:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x3F_BRR:
          switch (op2) {
            case JLT_DA_DB_DISP15:
            case JLTU_DA_DB_DISP15:
              return Jlt::Info(data, addr, maxLen, result);
            default:
              break;
          }
        case OP1_x43_RRR1:
          switch (op2) {
            case MADDQ_DC_DD_DA_DB_N:
            case MADDQ_EC_ED_DA_DB_N:
            case MADDQ_DC_DD_DA_DBL_N:
            case MADDQ_EC_ED_DA_DBL_N:
            case MADDQ_DC_DD_DA_DBU_N:
            case MADDQ_EC_ED_DA_DBU_N:
            case MADDQ_DC_DD_DAL_DBL_N:
            case MADDQ_EC_ED_DAL_DBL_N:
            case MADDQ_DC_DD_DAU_DBU_N:
            case MADDQ_EC_ED_DAU_DBU_N:
            case MADDSQ_DC_DD_DA_DB_N:
            case MADDSQ_EC_ED_DA_DB_N:
            case MADDSQ_DC_DD_DA_DBL_N:
            case MADDSQ_EC_ED_DA_DBL_N:
            case MADDSQ_DC_DD_DA_DBU_N:
            case MADDSQ_EC_ED_DA_DBU_N:
            case MADDSQ_DC_DD_DAL_DBL_N:
            case MADDSQ_EC_ED_DAL_DBL_N:
            case MADDSQ_DC_DD_DAU_DBU_N:
            case MADDSQ_EC_ED_DAU_DBU_N:
            case MADDRQ_DC_DD_DAL_DBL_N:
            case MADDRQ_DC_DD_DAU_DBU_N:
            case MADDRSQ_DC_DD_DAL_DBL_N:
            case MADDRSQ_DC_DD_DAU_DBU_N:
            case MADDRH_DC_ED_DA_DBUL_N:
            case MADDRSH_DC_ED_DA_DBUL_N:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x45_ABS:
          if (op2 == LDQ_DA_OFF18) {
            result.length = 4;
            return true;
          }
          return false;
        case OP1_x47_BIT:
          switch (op2) {
            case ANDANDT_DC_DA_POS1_DB_POS2:
            case ANDANDNT_DC_DA_POS1_DB_POS2:
            case ANDNORT_DC_DA_POS1_DB_POS2:
            case ANDORT_DC_DA_POS1_DB_POS2:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x49_BO:
          switch (op2) {
            case LDLCX_AB_OFF10:
            case LDMST_AB_OFF10_EA:
            case LDMST_AB_OFF10_EA_POSTINC:
            case LDMST_AB_OFF10_EA_PREINC:
            case LDUCX_AB_OFF10:
            case LEA_AA_AB_OFF10:
            case STLCX_AB_OFF10:
            case STUCX_AB_OFF10:
            case SWAPW_AB_OFF10_DA:
            case SWAPW_AB_OFF10_DA_POSTINC:
            case SWAPW_AB_OFF10_DA_PREINC:
            case CMPSWAPW_AB_OFF10_EA:          // TCv1.6 ISA only
            case CMPSWAPW_AB_OFF10_EA_POSTINC:  // TCv1.6 ISA only
            case CMPSWAPW_AB_OFF10_EA_PREINC:   // TCv1.6 ISA only
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x4B_RR:
          switch (op2) {
            case BMERGE_DC_DA_DB:
            case BSPLIT_EC_DA:
            case DVINITB_EC_DA_DB:
            case DVINITBU_EC_DA_DB:
            case DVINITH_EC_DA_DB:
            case DVINITHU_EC_DA_DB:
            case DVINIT_EC_DA_DB:
            case DVINITU_EC_DA_DB:
            case DIV_EC_DA_DB:
            case DIVU_EC_DA_DB:
            case CRC32_DC_DB_DA:  // TCv1.6 ISA only
            case PARITY_DC_DA:
            case UNPACK_EC_DA:
            // FPU Instructions
            case CMPF_DC_DA_DB:
            case DIVF_DC_DA_DB:
            case FTOI_DC_DA:
            case FTOIZ_DC_DA:
            case FTOQ31_DC_DA_DB:
            case FTOQ31Z_DC_DA_DB:
            case FTOU_DC_DA:
            case FTOUZ_DC_DA:
            case ITOF_DC_DA:
            case MULF_DC_DA_DB:
            case Q31TOF_DC_DA_DB:
            case QSEEDF_DC_DA:
            case UPDFL_DA:
            case UTOF_DC_DA:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x53_RC:
          switch (op2) {
            case MUL_DC_DA_CONST9:
            case MUL_EC_DA_CONST9:
            case MULS_DC_DA_CONST9:
            case MULU_EC_DA_CONST9:
            case MULSU_DC_DA_CONST9:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x57_RRRW:
          switch (op2) {
            case EXTR_DC_DA_DD_WIDTH:
            case EXTRU_DC_DA_DD_WIDTH:
            case IMASK_EC_DB_DD_WIDTH:
            case INSERT_DC_DA_DB_DD_WIDTH:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x5F_BRR:
          switch (op2) {
            case JEQ_DA_DB_DISP15:
              return Jeq::Info(data, addr, maxLen, result);
            case JNE_DA_DB_DISP15:
              return Jne::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_x63_RRR1:
          switch (op2) {
            case MSUBQ_DC_DD_DA_DB_N:
            case MSUBQ_EC_ED_DA_DB_N:
            case MSUBQ_DC_DD_DA_DBL_N:
            case MSUBQ_EC_ED_DA_DBL_N:
            case MSUBQ_DC_DD_DA_DBU_N:
            case MSUBQ_EC_ED_DA_DBU_N:
            case MSUBQ_DC_DD_DAL_DBL_N:
            case MSUBQ_EC_ED_DAL_DBL_N:
            case MSUBQ_DC_DD_DAU_DBU_N:
            case MSUBQ_EC_ED_DAU_DBU_N:
            case MSUBSQ_DC_DD_DA_DB_N:
            case MSUBSQ_EC_ED_DA_DB_N:
            case MSUBSQ_DC_DD_DA_DBL_N:
            case MSUBSQ_EC_ED_DA_DBL_N:
            case MSUBSQ_DC_DD_DA_DBU_N:
            case MSUBSQ_EC_ED_DA_DBU_N:
            case MSUBSQ_DC_DD_DAL_DBL_N:
            case MSUBSQ_EC_ED_DAL_DBL_N:
            case MSUBSQ_DC_DD_DAU_DBU_N:
            case MSUBSQ_EC_ED_DAU_DBU_N:
            case MSUBRH_DC_ED_DA_DBUL_N:
            case MSUBRSH_DC_ED_DA_DBUL_N:
            case MSUBRQ_DC_DD_DAL_DBL_N:
            case MSUBRQ_DC_DD_DAU_DBU_N:
            case MSUBRSQ_DC_DD_DAL_DBL_N:
            case MSUBRSQ_DC_DD_DAU_DBU_N:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x65_ABS:
          if (op2 == STQ_OFF18_DA) {
            result.length = 4;
            return true;
          }
          return false;
        case OP1_x67_BIT:
          switch (op2) {
            case INST_DC_DA_POS1_DB_POS2:
            case INSNT_DC_DA_POS1_DB_POS2:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x69_BO:
          switch (op2) {
            case LDMST_PB_EA:
            case LDMST_PB_OFF10_EA:
            case SWAPW_PB_DA:
            case SWAPW_PB_OFF10_DA:
            case CMPSWAPW_PB_EA:        // TCv1.6 ISA only
            case CMPSWAPW_PB_OFF10_EA:  // TCv1.6 ISA only
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x6B_RRR:
          switch (op2) {
            case DVADJ_EC_ED_DB:
            case DVSTEP_EC_ED_DB:
            case DVSTEPU_EC_ED_DB:
            case IXMAX_EC_ED_DB:
            case IXMAXU_EC_ED_DB:
            case IXMIN_EC_ED_DB:
            case IXMINU_EC_ED_DB:
            case PACK_DC_ED_DA:
            // FPU Instructions
            case ADDF_DC_DD_DA:
            case MADDF_DC_DD_DA_DB:
            case MSUBF_DC_DD_DA_DB:
            case SUBF_DC_DD_DA:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x73_RR2:
          switch (op2) {
            case MUL_DC_DA_DB:
            case MUL_EC_DA_DB:
            case MULS_DC_DA_DB:
            case MULU_EC_DA_DB:
            case MULSU_DC_DA_DB:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x75_RR:
          switch (op2) {
            // MMU Instructions
            case TLBDEMAP_DA:
            case TLBFLUSHA:
            case TLBFLUSHB:
            case TLBMAP_EA:
            case TLBPROBEA_DA:
            case TLBPROBEI_DA:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x77_RRPW:
          if (op2 == DEXTR_DC_DA_DB_POS) {
            result.length = 4;
            return true;
          }
          return false;
        case OP1_x7D_BRR:
          switch (op2) {
            case JEQA_AA_AB_DISP15:
              return Jeqa::Info(data, addr, maxLen, result);
            case JNEA_AA_AB_DISP15:
              return Jnea::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_x7F_BRR:
          switch (op2) {
            case JGE_DA_DB_DISP15:
            case JGEU_DA_DB_DISP15:
              return Jge::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_x83_RRR1:
          switch (op2) {
            case MADDH_EC_ED_DA_DBLL_N:
            case MADDH_EC_ED_DA_DBLU_N:
            case MADDH_EC_ED_DA_DBUL_N:
            case MADDH_EC_ED_DA_DBUU_N:
            case MADDSH_EC_ED_DA_DBLL_N:
            case MADDSH_EC_ED_DA_DBLU_N:
            case MADDSH_EC_ED_DA_DBUL_N:
            case MADDSH_EC_ED_DA_DBUU_N:
            case MADDMH_EC_ED_DA_DBLL_N:
            case MADDMH_EC_ED_DA_DBLU_N:
            case MADDMH_EC_ED_DA_DBUL_N:
            case MADDMH_EC_ED_DA_DBUU_N:
            case MADDMSH_EC_ED_DA_DBLL_N:
            case MADDMSH_EC_ED_DA_DBLU_N:
            case MADDMSH_EC_ED_DA_DBUL_N:
            case MADDMSH_EC_ED_DA_DBUU_N:
            case MADDRH_DC_DD_DA_DBLL_N:
            case MADDRH_DC_DD_DA_DBLU_N:
            case MADDRH_DC_DD_DA_DBUL_N:
            case MADDRSH_DC_DD_DA_DBUU_N:
            case MADDRH_DC_DD_DA_DBUU_N:
            case MADDRSH_DC_DD_DA_DBLL_N:
            case MADDRSH_DC_DD_DA_DBLU_N:
            case MADDRSH_DC_DD_DA_DBUL_N:
              result.length = 4;
              return true;
            default:
              return true;
          }
        case OP1_x85_ABS:
          switch (op2) {
            case LDA_AA_OFF18:
            case LDD_EA_OFF18:
            case LDDA_PA_OFF18:
            case LDW_DA_OFF18:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x87_BIT:
          switch (op2) {
            case ANDT_DC_DA_POS1_DB_POS2:
            case ANDNT_DC_DA_POS1_DB_POS2:
            case NORT_DC_DA_POS1_DB_POS2:
            case ORT_DC_DA_POS1_DB_POS2:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x89_BO:
          switch (op2) {
            case CACHEAI_AB_OFF10:
            case CACHEAI_AB_OFF10_POSTINC:
            case CACHEAI_AB_OFF10_PREINC:
            case CACHEAW_AB_OFF10:
            case CACHEAW_AB_OFF10_POSTINC:
            case CACHEAW_AB_OFF10_PREINC:
            case CACHEAWI_AB_OFF10:
            case CACHEAWI_AB_OFF10_POSTINC:
            case CACHEAWI_AB_OFF10_PREINC:
            case CACHEIW_AB_OFF10:
            case CACHEIW_AB_OFF10_POSTINC:
            case CACHEIW_AB_OFF10_PREINC:
            case CACHEIWI_AB_OFF10:
            case CACHEIWI_AB_OFF10_POSTINC:
            case CACHEIWI_AB_OFF10_PREINC:
            case CACHEII_AB_OFF10:
            case CACHEII_AB_OFF10_POSTINC:
            case CACHEII_AB_OFF10_PREINC:
            case STA_AB_OFF10_AA:
            case STA_AB_OFF10_AA_POSTINC:
            case STA_AB_OFF10_AA_PREINC:
            case STB_AB_OFF10_DA:
            case STB_AB_OFF10_DA_POSTINC:
            case STB_AB_OFF10_DA_PREINC:
            case STD_AB_OFF10_EA:
            case STD_AB_OFF10_EA_POSTINC:
            case STD_AB_OFF10_EA_PREINC:
            case STDA_AB_OFF10_PA:
            case STDA_AB_OFF10_PA_POSTINC:
            case STDA_AB_OFF10_PA_PREINC:
            case STH_AB_OFF10_DA:
            case STH_AB_OFF10_DA_POSTINC:
            case STH_AB_OFF10_DA_PREINC:
            case STQ_AB_OFF10_DA:
            case STQ_AB_OFF10_DA_POSTINC:
            case STQ_AB_OFF10_DA_PREINC:
            case STW_AB_OFF10_DA:
            case STW_AB_OFF10_DA_POSTINC:
            case STW_AB_OFF10_DA_PREINC:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x8B_RC:
          switch (op2) {
            case ABSDIF_DC_DA_CONST9:
            case ABSDIFS_DC_DA_CONST9:
            case ADD_DC_DA_CONST9:
            case ADDC_DC_DA_CONST9:
            case ADDS_DC_DA_CONST9:
            case ADDSU_DC_DA_CONST9:
            case ADDX_DC_DA_CONST9:
            case ANDEQ_DC_DA_CONST9:
            case ANDGE_DC_DA_CONST9:
            case ANDGEU_DC_DA_CONST9:
            case ANDLT_DC_DA_CONST9:
            case ANDLTU_DC_DA_CONST9:
            case ANDNE_DC_DA_CONST9:
            case EQ_DC_DA_CONST9:
            case EQANYB_DC_DA_CONST9:
            case EQANYH_DC_DA_CONST9:
            case GE_DC_DA_CONST9:
            case GEU_DC_DA_CONST9:
            case LT_DC_DA_CONST9:
            case LTU_DC_DA_CONST9:
            case MAX_DC_DA_CONST9:
            case MAXU_DC_DA_CONST9:
            case MIN_DC_DA_CONST9:
            case MINU_DC_DA_CONST9:
            case NE_DC_DA_CONST9:
            case OREQ_DC_DA_CONST9:
            case ORGE_DC_DA_CONST9:
            case ORGEU_DC_DA_CONST9:
            case ORLT_DC_DA_CONST9:
            case ORLTU_DC_DA_CONST9:
            case ORNE_DC_DA_CONST9:
            case RSUB_DC_DA_CONST9:
            case RSUBS_DC_DA_CONST9:
            case RSUBSU_DC_DA_CONST9:
            case SHEQ_DC_DA_CONST9:
            case SHGE_DC_DA_CONST9:
            case SHGEU_DC_DA_CONST9:
            case SHLT_DC_DA_CONST9:
            case SHLTU_DC_DA_CONST9:
            case SHNE_DC_DA_CONST9:
            case XOREQ_DC_DA_CONST9:
            case XORGE_DC_DA_CONST9:
            case XORGEU_DC_DA_CONST9:
            case XORLT_DC_DA_CONST9:
            case XORLTU_DC_DA_CONST9:
            case XORNE_DC_DA_CONST9:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x8F_RC:
          switch (op2) {
            case AND_DC_DA_CONST9:
            case ANDN_DC_DA_CONST9:
            case NAND_DC_DA_CONST9:
            case NOR_DC_DA_CONST9:
            case OR_DC_DA_CONST9:
            case ORN_DC_DA_CONST9:
            case SH_DC_DA_CONST9:
            case SHH_DC_DA_CONST9:
            case SHA_DC_DA_CONST9:
            case SHAH_DC_DA_CONST9:
            case SHAS_DC_DA_CONST9:
            case XNOR_DC_DA_CONST9:
            case XOR_DC_DA_CONST9:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x93_RR1:
          switch (op2) {
            case MULQ_DC_DA_DB_N:
            case MULQ_EC_DA_DB_N:
            case MULQ_DC_DA_DBL_N:
            case MULQ_EC_DA_DBL_N:
            case MULQ_DC_DA_DBU_N:
            case MULQ_EC_DA_DBU_N:
            case MULQ_DC_DAL_DBL_N:
            case MULQ_DC_DAU_DBU_N:
            case MULRQ_DC_DAL_DBL_N:
            case MULRQ_DC_DAU_DBU_N:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_x97_RCRR:
          if (op2 == INSERT_DC_DA_CONST4_ED) {
            result.length = 4;
            return true;
          }
        case OP1_x9F_BRC:
          switch (op2) {
            case JNED_DA_CONST4_DISP15:
              return Jned::Info(data, addr, maxLen, result);
            case JNEI_DA_CONST4_DISP15:
              return Jnei::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_xA3_RRR1:
          switch (op2) {
            case MSUBH_EC_ED_DA_DBLL_N:
            case MSUBH_EC_ED_DA_DBLU_N:
            case MSUBH_EC_ED_DA_DBUL_N:
            case MSUBH_EC_ED_DA_DBUU_N:
            case MSUBSH_EC_ED_DA_DBLL_N:
            case MSUBSH_EC_ED_DA_DBLU_N:
            case MSUBSH_EC_ED_DA_DBUL_N:
            case MSUBSH_EC_ED_DA_DBUU_N:
            case MSUBMH_EC_ED_DA_DBLL_N:
            case MSUBMH_EC_ED_DA_DBLU_N:
            case MSUBMH_EC_ED_DA_DBUL_N:
            case MSUBMH_EC_ED_DA_DBUU_N:
            case MSUBMSH_EC_ED_DA_DBLL_N:
            case MSUBMSH_EC_ED_DA_DBLU_N:
            case MSUBMSH_EC_ED_DA_DBUL_N:
            case MSUBMSH_EC_ED_DA_DBUU_N:
            case MSUBRH_DC_DD_DA_DBLL_N:
            case MSUBRH_DC_DD_DA_DBLU_N:
            case MSUBRH_DC_DD_DA_DBUL_N:
            case MSUBRH_DC_DD_DA_DBUU_N:
            case MSUBRSH_DC_DD_DA_DBLL_N:
            case MSUBRSH_DC_DD_DA_DBLU_N:
            case MSUBRSH_DC_DD_DA_DBUL_N:
            case MSUBRSH_DC_DD_DA_DBUU_N:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xA5_ABS:
          switch (op2) {
            case STA_OFF18_AA:
            case STD_OFF18_EA:
            case STDA_OFF18_PA:
            case STW_OFF18_DA:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xA7_BIT:
          switch (op2) {
            case SHNANDT_DC_DA_POS1_DB_POS2:
            case SHORNT_DC_DA_POS1_DB_POS2:
            case SHXNORT_DC_DA_POS1_DB_POS2:
            case SHXORT_DC_DA_POS1_DB_POS2:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xA9_BO:
          switch (op2) {
            case CACHEAI_PB:
            case CACHEAI_PB_OFF10:
            case CACHEAW_PB:
            case CACHEAW_PB_OFF10:
            case CACHEAWI_PB:
            case CACHEAWI_PB_OFF10:
            case STA_PB_AA:
            case STA_PB_OFF10_AA:
            case STB_PB_DA:
            case STB_PB_OFF10_DA:
            case STD_PB_EA:
            case STD_PB_OFF10_EA:
            case STDA_PB_PA:
            case STDA_PB_OFF10_PA:
            case STH_PB_DA:
            case STH_PB_OFF10_DA:
            case STQ_PB_DA:
            case STQ_PB_OFF10_DA:
            case STW_PB_DA:
            case STW_PB_OFF10_DA:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xAB_RCR:
          switch (op2) {
            case CADD_DC_DD_DA_CONST9:
            case CADDN_DC_DD_DA_CONST9:
            case SEL_DC_DD_DA_CONST9:
            case SELN_DC_DD_DA_CONST9:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xAD_RC:
          switch (op2) {
            // TODO: handle branching instruction below?
            case BISR_CONST9:
            case SYSCALL_CONST9:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xB3_RR1:
          switch (op2) {
            case MULH_EC_DA_DBLL_N:
            case MULH_EC_DA_DBLU_N:
            case MULH_EC_DA_DBUL_N:
            case MULH_EC_DA_DBUU_N:
            case MULMH_EC_DA_DBLL_N:
            case MULMH_EC_DA_DBLU_N:
            case MULMH_EC_DA_DBUL_N:
            case MULMH_EC_DA_DBUU_N:
            case MULRH_DC_DA_DBLL_N:
            case MULRH_DC_DA_DBLU_N:
            case MULRH_DC_DA_DBUL_N:
            case MULRH_DC_DA_DBUU_N:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xB7_RCPW:
          switch (op2) {
            case IMASK_EC_CONST4_POS_WIDTH:
            case INSERT_DC_DA_CONST4_POS_WIDTH:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xBD_BRR:
          switch (op2) {
            case JNZA_AA_DISP15:
              return Jnza::Info(data, addr, maxLen, result);
            case JZA_AA_DISP15:
              return Jza::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_xBF_BRC:
          switch (op2) {
            case JLT_DA_CONST4_DISP15:
            case JLTU_DA_CONST4_DISP15:
              return Jlt::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_xC3_RRR1:
          switch (op2) {
            case MADDSUH_EC_ED_DA_DBLL_N:
            case MADDSUH_EC_ED_DA_DBLU_N:
            case MADDSUH_EC_ED_DA_DBUL_N:
            case MADDSUH_EC_ED_DA_DBUU_N:
            case MADDSUSH_EC_ED_DA_DBLL_N:
            case MADDSUSH_EC_ED_DA_DBLU_N:
            case MADDSUSH_EC_ED_DA_DBUL_N:
            case MADDSUSH_EC_ED_DA_DBUU_N:
            case MADDSUMH_EC_ED_DA_DBLL_N:
            case MADDSUMH_EC_ED_DA_DBLU_N:
            case MADDSUMH_EC_ED_DA_DBUL_N:
            case MADDSUMH_EC_ED_DA_DBUU_N:
            case MADDSUMSH_EC_ED_DA_DBLL_N:
            case MADDSUMSH_EC_ED_DA_DBLU_N:
            case MADDSUMSH_EC_ED_DA_DBUL_N:
            case MADDSUMSH_EC_ED_DA_DBUU_N:
            case MADDSURH_DC_DD_DA_DBLL_N:
            case MADDSURH_DC_DD_DA_DBLU_N:
            case MADDSURH_DC_DD_DA_DBUL_N:
            case MADDSURH_DC_DD_DA_DBUU_N:
            case MADDSURSH_DC_DD_DA_DBLL_N:
            case MADDSURSH_DC_DD_DA_DBLU_N:
            case MADDSURSH_DC_DD_DA_DBUL_N:
            case MADDSURSH_DC_DD_DA_DBUU_N:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xC5_ABS:
          if (op2 == LEA_AA_OFF18) {
            result.length = 4;
            return true;
          }
          return false;
        case OP1_xC7_BIT:
          switch (op2) {
            case ORANDT_DC_DA_POS1_DB_POS2:
            case ORANDNT_DC_DA_POS1_DB_POS2:
            case ORNORT_DC_DA_POS1_DB_POS2:
            case ORORT_DC_DA_POS1_DB_POS2:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xD5_ABSB:
          if (op2 == STT_OFF18_BPOS3_B) {
            result.length = 4;
            return true;
          }
          return false;
        case OP1_xD7_RCRW:
          switch (op2) {
            case IMASK_EC_CONST4_POS_WIDTH:
            case INSERT_DC_DA_CONST4_DD_WIDTH:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xDF_BRC:
          switch (op2) {
            case JEQ_DA_CONST4_DISP15:
              return Jeq::Info(data, addr, maxLen, result);
            case JNE_DA_CONST4_DISP15:
              return Jne::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_xE3_RRR1:
          switch (op2) {
            case MSUBADH_EC_ED_DA_DBLL_N:
            case MSUBADH_EC_ED_DA_DBLU_N:
            case MSUBADH_EC_ED_DA_DBUL_N:
            case MSUBADH_EC_ED_DA_DBUU_N:
            case MSUBADSH_EC_ED_DA_DBLL_N:
            case MSUBADSH_EC_ED_DA_DBLU_N:
            case MSUBADSH_EC_ED_DA_DBUL_N:
            case MSUBADSH_EC_ED_DA_DBUU_N:
            case MSUBADMH_EC_ED_DA_DBLL_N:
            case MSUBADMH_EC_ED_DA_DBLU_N:
            case MSUBADMH_EC_ED_DA_DBUL_N:
            case MSUBADMH_EC_ED_DA_DBUU_N:
            case MSUBADMSH_EC_ED_DA_DBLL_N:
            case MSUBADMSH_EC_ED_DA_DBLU_N:
            case MSUBADMSH_EC_ED_DA_DBUL_N:
            case MSUBADMSH_EC_ED_DA_DBUU_N:
            case MSUBADRH_DC_DD_DA_DBLL_N:
            case MSUBADRH_DC_DD_DA_DBLU_N:
            case MSUBADRH_DC_DD_DA_DBUL_N:
            case MSUBADRH_DC_DD_DA_DBUU_N:
            case MSUBADRSH_DC_DD_DA_DBLL_N:
            case MSUBADRSH_DC_DD_DA_DBLU_N:
            case MSUBADRSH_DC_DD_DA_DBUL_N:
            case MSUBADRSH_DC_DD_DA_DBUU_N:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xE5_ABS:
          switch (op2) {
            case LDMST_OFF18_EA:
            case SWAPW_OFF18_DA:
              result.length = 4;
              return true;
            default:
              return false;
          }
        case OP1_xFD_BRR:
          switch (op2) {
            case LOOP_AB_DISP15:
              return Loop::Info(data, addr, maxLen, result);
            case LOOPU_DISP15:
              return Loopu::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        case OP1_xFF_BRC:
          switch (op2) {
            case JGE_DA_CONST4_DISP15:
            case JGEU_DA_CONST4_DISP15:
              return Jge::Info(data, addr, maxLen, result);
            default:
              return false;
          }
        default:
          return false;
      }
    }
    // 16-bit instruction
    else {
      // The ADDSCA 16-bit instruction is weird and has a smaller primary opcode
      // (SRRS format)
      if ((raw & 0x3Fu) == ADDSCA_AA_AB_D15_N) {
        result.length = 2;
        return true;
      }
      uint8_t op1 = raw & 0xFFu;
      // This will get op2 if it exists
      uint32_t op2 = Instruction::ExtractSecondaryOpcode(op1, data);
      op2 &= 0xFFu;  // secondary opcodes are not > 0xFF, even though some
                     // formats have up to 12 bits
      switch (op1) {
        // 2-byte non-branching, single opcode
        case ADD_DA_CONST4:
        case ADD_DA_D15_CONST4:
        case ADD_D15_DA_CONST4:
        case ADD_DA_DB:
        case ADD_DA_D15_DB:
        case ADD_D15_DA_DB:
        case ADDA_AA_CONST4:
        case ADDA_AA_AB:
        case ADDS_DA_DB:
        case AND_D15_CONST8:
        case AND_DA_DB:
        case BISR_CONST8:
        case CADD_DA_D15_CONST4:
        case CADDN_DA_D15_CONST4:
        case CMOV_DA_D15_CONST4:
        case CMOV_DA_D15_DB:
        case CMOVN_DA_D15_CONST4:
        case CMOVN_DA_D15_DB:
        case EQ_D15_DA_CONST4:
        case EQ_D15_DA_DB:
        case LDA_A15_A10_CONST8:
        case LDA_AC_AB:
        case LDA_AC_AB_POSTINC:
        case LDA_AC_A15_OFF4:
        case LDA_A15_AB_OFF4:
        case LDBU_DC_AB:
        case LDBU_DC_AB_POSTINC:
        case LDBU_DC_A15_OFF4:
        case LDBU_D15_AB_OFF4:
        case LDH_DC_AB:
        case LDH_DC_AB_POSTINC:
        case LDH_DC_A15_OFF4:
        case LDH_D15_AB_OFF4:
        case LDW_D15_A10_CONST8:
        case LDW_DC_AB:
        case LDW_DC_AB_POSTINC:
        case LDW_DC_A15_OFF4:
        case LDW_D15_AB_OFF4:
        case LT_D15_DA_CONST4:
        case LT_D15_DA_DB:
        case MOV_D15_CONST8:
        case MOV_DA_CONST4:
        case MOV_EA_CONST4:  // TCv1.6 ISA only
        case MOV_DA_DB:
        case MOVA_AA_CONST4:
        case MOVA_AA_DB:
        case MOVAA_AA_AB:
        case MOVD_DA_AB:
        case MUL_DA_DB:
        case OR_D15_CONST8:
        case OR_DA_DB:
        case SH_DA_CONST4:
        case SHA_DA_CONST4:
        case STA_A10_CONST8_A15:
        case STA_AB_OFF4_A15:
        case STA_AB_AA:
        case STA_AB_AA_POSTINC:
        case STA_A15_OFF4_AA:
        case STB_AB_OFF4_D15:
        case STB_AB_DA:
        case STB_AB_DA_POSTINC:
        case STB_A15_OFF4_DA:
        case STH_AB_OFF4_D15:
        case STH_AB_DA:
        case STH_AB_DA_POSTINC:
        case STH_A15_OFF4_DA:
        case STW_A10_CONST8_D15:
        case STW_AB_OFF4_D15:
        case STW_AB_DA:
        case STW_AB_DA_POSTINC:
        case STW_A15_OFF4_DA:
        case SUB_DA_DB:
        case SUB_DA_D15_DB:
        case SUB_D15_DA_DB:
        case SUBA_A10_CONST8:
        case SUBS_DA_DB:
        case XOR_DA_DB:
          result.length = 2;
          return true;

        // 2-byte dual opcode
        case OP1_x00_SR:
          switch (op2) {
            case DEBUG16:
            case NOP16:
              result.length = 2;
              return true;
            case RET16:
            case RFE16:
            case FRET16:
              result.AddBranch(FunctionReturn);
              result.length = 2;
              return true;
            default:
              return false;
          }
        case OP1_x46_SR:
          switch (op2) {
            case NOT_DA:
              result.length = 2;
              return true;
            default:
              return false;
          }
        case OP1_x32_SR:
          switch (op2) {
            case RSUB_DA:
            case SATB_DA:
            case SATBU_DA:
            case SATH_DA:
            case SATHU_DA:
              result.length = 2;
              return true;
            default:
              return false;
          }
        case OP1_xDC_SR:
          switch (op2) {
            case JI_AA_16:
              return Ji::Info(data, addr, maxLen, result);
            default:
              return false;
          }

        // 2-byte branching
        case CALL_DISP8:
          return Call::Info(data, addr, maxLen, result);
        case J_DISP8:
          return J::Info(data, addr, maxLen, result);
        case JEQ_D15_CONST4_DISP4:
        case JEQ_D15_DB_DISP4:
        case JEQ_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
        case JEQ_D15_DB_DISP4_EX:      // TCv1.6 ISA only
          return Jeq::Info(data, addr, maxLen, result);
        case JGEZ_DB_DISP4:
          return Jgez::Info(data, addr, maxLen, result);
        case JGTZ_DB_DISP4:
          return Jgtz::Info(data, addr, maxLen, result);
        case JLEZ_DB_DISP4:
          return Jlez::Info(data, addr, maxLen, result);
        case JLTZ_DB_DISP4:
          return Jltz::Info(data, addr, maxLen, result);
        case JNE_D15_CONST4_DISP4:
        case JNE_D15_DB_DISP4:
        case JNE_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
        case JNE_D15_DB_DISP4_EX:      // TCv1.6 ISA only
          return Jne::Info(data, addr, maxLen, result);
        case JNZ_D15_DISP8:
        case JNZ_DB_DISP4:
          return Jnz::Info(data, addr, maxLen, result);
        case JNZA_AB_DISP4:
          return Jnza::Info(data, addr, maxLen, result);
        case JNZT_D15_N_DISP4:
          return Jnzt::Info(data, addr, maxLen, result);
        case JZ_D15_DISP8:
        case JZ_DB_DISP4:
          return Jz::Info(data, addr, maxLen, result);
        case JZA_AB_DISP4:
          return Jza::Info(data, addr, maxLen, result);
        case JZT_D15_N_DISP4:
          return Jzt::Info(data, addr, maxLen, result);
        case LOOP_AB_DISP4:
          return Loop::Info(data, addr, maxLen, result);
        default:
          return false;
      }
    }
  }

  bool GetInstructionText(const uint8_t* data, uint64_t addr, size_t& len,
                          std::vector<InstructionTextToken>& result) override {
    auto wdata = (const uint16_t*)data;
    uint16_t raw = *wdata;
    uint8_t seg;
    uint16_t caddr;
    uint32_t target;
    std::string proc_name = this->GetName();

    // 32-bit instruction
    if (raw & 0x01u) {
      // There are two instructions of the BRN format, which has a 7-bit primary
      // opcode (instead of 8-bit)
      if ((raw & 0x7Fu) == OP1_x6F_BRN) {
        uint32_t op2 = Instruction::ExtractSecondaryOpcode(raw & 0x7Fu, data);
        if (op2 == JZT_DA_N_DISP15) {
          return Jzt::Text(raw & 0x7Fu, op2, data, addr, len, result);
        }
        if (op2 == JNZT_DA_N_DISP15) {
          return Jnzt::Text(raw & 0x7Fu, op2, data, addr, len, result);
        }
        return false;
      }
      uint8_t op1 = raw & 0xFFu;
      // This will get op2 if it exists
      uint32_t op2 = Instruction::ExtractSecondaryOpcode(op1, data);
      op2 &= 0xFFu;  // secondary opcodes are not > 0xFF, even though some
                     // formats have up to 12 bits
      switch (op1) {
        // 4-byte non-branching, single opcode
        case ADDI_DC_DA_CONST16:
          return Addi::Text(op1, op2, data, addr, len, result);
        case ADDIH_DC_DA_CONST16:
          return Addih::Text(op1, op2, data, addr, len, result);
        case ADDIHA_AC_AA_CONST16:
          return Addiha::Text(op1, op2, data, addr, len, result);
        case LDA_AA_AB_OFF16:
          return Lda::Text(op1, op2, data, addr, len, result);
        case LDB_DA_AB_OFF16:   // TCv1.6 ISA only
        case LDBU_DA_AB_OFF16:  // TCv1.6 ISA only
          return Ldb::Text(op1, op2, data, addr, len, result);
        case LDH_DA_AB_OFF16:  // TCv1.6 ISA only
          return Ldh::Text(op1, op2, data, addr, len, result);
        case LDHU_DA_AB_OFF16:  // TCv1.6 ISA only
          return Ldhu::Text(op1, op2, data, addr, len, result);
        case LDW_DA_AB_OFF16:
          return Ldw::Text(op1, op2, data, addr, len, result);
        case LEA_AA_AB_OFF16:
          return Lea::Text(op1, op2, data, addr, len, result);
        case MFCR_DC_CONST16:
          return Mfcr::Text(op1, op2, data, addr, len, result);
        case MOV_DC_CONST16:
        case MOV_EC_CONST16:  // TCv1.6 ISA only
          return Mov::Text(op1, op2, data, addr, len, result);
        case MOVU_DC_CONST16:
          return Movu::Text(op1, op2, data, addr, len, result);
        case MOVH_DC_CONST16:
          return Movh::Text(op1, op2, data, addr, len, result);
        case MOVHA_AC_CONST16:
          return Movha::Text(op1, op2, data, addr, len, result);
        case MTCR_CONST16_DA:
          return Mtcr::Text(op1, op2, data, addr, len, result);
        case STB_AB_OFF16_DA:
          return Stb::Text(op1, op2, data, addr, len, result);
        case STH_AB_OFF16_DA:
          return Sth::Text(op1, op2, data, addr, len, result);
        case STA_AB_OFF16_AA:
          return Sta::Text(op1, op2, data, addr, len, result);
        case STW_AB_OFF16_DA:
          return Stw::Text(op1, op2, data, addr, len, result);

        // 4-byte branching, single opcode
        case CALL_DISP24:
          return Call::Text(op1, op2, data, addr, len, result);
        case CALLA_DISP24:
          return Calla::Text(op1, op2, data, addr, len, result);
        case FCALL_DISP24:
          return Fcall::Text(op1, op2, data, addr, len, result);
        case FCALLA_DISP24:
          return Fcalla::Text(op1, op2, data, addr, len, result);
        case J_DISP24:
          return J::Text(op1, op2, data, addr, len, result);
        case JA_DISP24:
          return Ja::Text(op1, op2, data, addr, len, result);
        case JL_DISP24:
          return Jl::Text(op1, op2, data, addr, len, result);
        case JLA_DISP24:
          return Jla::Text(op1, op2, data, addr, len, result);

        // 4-byte, dual opcode
        case OP1_x01_RR:
          switch (op2) {
            case ADDA_AC_AA_AB:
              return Adda::Text(op1, op2, data, addr, len, result);
            case ADDSCA_AC_AB_DA_N:
            case ADDSCAT_AC_AB_DA:
              return Addscaat::Text(op1, op2, data, addr, len, result);
            case EQA_DC_AA_AB:
              return Eqa::Text(op1, op2, data, addr, len, result);
            case EQZA_DC_AA:
              return Eqza::Text(op1, op2, data, addr, len, result);
            case GEA_DC_AA_AB:
              return Gea::Text(op1, op2, data, addr, len, result);
            case LTA_DC_AA_AB:
              return Lta::Text(op1, op2, data, addr, len, result);
            case MOVA_AC_DB:
              return Mova::Text(op1, op2, data, addr, len, result);
            case MOVAA_AC_AB:
              return Movaa::Text(op1, op2, data, addr, len, result);
            case MOVD_DC_AB:
              return Movd::Text(op1, op2, data, addr, len, result);
            case NEA_DC_AA_AB:
              return Nea::Text(op1, op2, data, addr, len, result);
            case NEZA_DC_AA:
              return Neza::Text(op1, op2, data, addr, len, result);
            case SUBA_AC_AA_AB:
              return Suba::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x03_RRR2:
          switch (op2) {
            case MADD_DC_DD_DA_DB:
            case MADD_EC_ED_DA_DB:
            case MADDS_DC_DD_DA_DB:
            case MADDS_EC_ED_DA_DB:
              return Madd::Text(op1, op2, data, addr, len, result);
            case MADDU_EC_ED_DA_DB:
            case MADDSU_DC_DD_DA_DB:
            case MADDSU_EC_ED_DA_DB:
              return Maddu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x05_ABS:
          switch (op2) {
            case LDB_DA_OFF18:
            case LDBU_DA_OFF18:
              return Ldb::Text(op1, op2, data, addr, len, result);
            case LDH_DA_OFF18:
              return Ldh::Text(op1, op2, data, addr, len, result);
            case LDHU_DA_OFF18:
              return Ldhu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x07_BIT:
          switch (op2) {
            case NANDT_DC_DA_POS1_DB_POS2:
              return Nandt::Text(op1, op2, data, addr, len, result);
            case ORNT_DC_DA_POS1_DB_POS2:
              return Ornt::Text(op1, op2, data, addr, len, result);
            case XNORT_DC_DA_POS1_DB_POS2:
              return Xnort::Text(op1, op2, data, addr, len, result);
            case XORT_DC_DA_POS1_DB_POS2:
              return Xort::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x09_BO:
          switch (op2) {
            case LDA_AA_AB_OFF10:
            case LDA_AA_AB_OFF10_POSTINC:
            case LDA_AA_AB_OFF10_PREINC:
              return Lda::Text(op1, op2, data, addr, len, result);
            case LDB_DA_AB_OFF10:
            case LDB_DA_AB_OFF10_POSTINC:
            case LDB_DA_AB_OFF10_PREINC:
            case LDBU_DA_AB_OFF10:
            case LDBU_DA_AB_OFF10_POSTINC:
            case LDBU_DA_AB_OFF10_PREINC:
              return Ldb::Text(op1, op2, data, addr, len, result);
            case LDD_EA_AB_OFF10:
            case LDD_EA_AB_OFF10_POSTINC:
            case LDD_EA_AB_OFF10_PREINC:
              return Ldd::Text(op1, op2, data, addr, len, result);
            case LDDA_PA_AB_OFF10:
            case LDDA_PA_AB_OFF10_POSTINC:
            case LDDA_PA_AB_OFF10_PREINC:
              return Ldda::Text(op1, op2, data, addr, len, result);
            case LDH_DA_AB_OFF10:
            case LDH_DA_AB_OFF10_POSTINC:
            case LDH_DA_AB_OFF10_PREINC:
              return Ldh::Text(op1, op2, data, addr, len, result);
            case LDHU_DA_AB_OFF10:
            case LDHU_DA_AB_OFF10_POSTINC:
            case LDHU_DA_AB_OFF10_PREINC:
              return Ldhu::Text(op1, op2, data, addr, len, result);
            case LDQ_DA_AB_OFF10:
            case LDQ_DA_AB_OFF10_POSTINC:
            case LDQ_DA_AB_OFF10_PREINC:
              return Ldq::Text(op1, op2, data, addr, len, result);
            case LDW_DA_AB_OFF10:
            case LDW_DA_AB_OFF10_POSTINC:
            case LDW_DA_AB_OFF10_PREINC:
              return Ldw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x0B_RR:
          switch (op2) {
            case ABS_DC_DB:
              return Abs::Text(op1, op2, data, addr, len, result);
            case ABSB_DC_DB:
            case ABSH_DC_DB:
              return Absbh::Text(op1, op2, data, addr, len, result);
            case ABSDIF_DC_DA_DB:
              return Absdif::Text(op1, op2, data, addr, len, result);
            case ABSDIFB_DC_DA_DB:
            case ABSDIFH_DC_DA_DB:
              return Absdifbh::Text(op1, op2, data, addr, len, result);
            case ABSDIFS_DC_DA_DB:
              return Absdifs::Text(op1, op2, data, addr, len, result);
            case ABSDIFSH_DC_DA_DB:
              return Absdifsh::Text(op1, op2, data, addr, len, result);
            case ABSS_DC_DB:
              return Abss::Text(op1, op2, data, addr, len, result);
            case ABSSH_DC_DB:
              return Abssh::Text(op1, op2, data, addr, len, result);
            case ADD_DC_DA_DB:
              return Add::Text(op1, op2, data, addr, len, result);
            case ADDB_DC_DA_DB:
            case ADDH_DC_DA_DB:
              return Addbh::Text(op1, op2, data, addr, len, result);
            case ADDC_DC_DA_DB:
              return Addc::Text(op1, op2, data, addr, len, result);
            case ADDS_DC_DA_DB:
              return Adds::Text(op1, op2, data, addr, len, result);
            case ADDSH_DC_DA_DB:
            case ADDSHU_DC_DA_DB:
              return Addshhu::Text(op1, op2, data, addr, len, result);
            case ADDSU_DC_DA_DB:
              return Addsu::Text(op1, op2, data, addr, len, result);
            case ADDX_DC_DA_DB:
              return Addx::Text(op1, op2, data, addr, len, result);
            case ANDEQ_DC_DA_DB:
              return Andeq::Text(op1, op2, data, addr, len, result);
            case ANDGE_DC_DA_DB:
            case ANDGEU_DC_DA_DB:
              return AndgeGeU::Text(op1, op2, data, addr, len, result);
            case ANDLT_DC_DA_DB:
            case ANDLTU_DC_DA_DB:
              return AndltLtU::Text(op1, op2, data, addr, len, result);
            case ANDNE_DC_DA_DB:
              return Andne::Text(op1, op2, data, addr, len, result);
            case EQ_DC_DA_DB:
              return Eq::Text(op1, op2, data, addr, len, result);
            case EQB_DC_DA_DB:
            case EQH_DC_DA_DB:
            case EQW_DC_DA_DB:
              return Eqbhw::Text(op1, op2, data, addr, len, result);
            case EQANYB_DC_DA_DB:
            case EQANYH_DC_DA_DB:
              return Eqanybh::Text(op1, op2, data, addr, len, result);
            case GE_DC_DA_DB:
            case GEU_DC_DA_DB:
              return Ge::Text(op1, op2, data, addr, len, result);
            case LT_DC_DA_DB:
            case LTU_DC_DA_DB:
              return Lt::Text(op1, op2, data, addr, len, result);
            case LTB_DC_DA_DB:
            case LTBU_DC_DA_DB:
              return Ltb::Text(op1, op2, data, addr, len, result);
            case LTH_DC_DA_DB:
            case LTHU_DC_DA_DB:
              return Lth::Text(op1, op2, data, addr, len, result);
            case LTW_DC_DA_DB:
            case LTWU_DC_DA_DB:
              return Ltw::Text(op1, op2, data, addr, len, result);
            case MAX_DC_DA_DB:
            case MAXU_DC_DA_DB:
              return Max::Text(op1, op2, data, addr, len, result);
            case MAXB_DC_DA_DB:
            case MAXBU_DC_DA_DB:
              return Maxb::Text(op1, op2, data, addr, len, result);
            case MAXH_DC_DA_DB:
            case MAXHU_DC_DA_DB:
              return Maxh::Text(op1, op2, data, addr, len, result);
            case MIN_DC_DA_DB:
            case MINU_DC_DA_DB:
              return Min::Text(op1, op2, data, addr, len, result);
            case MINB_DC_DA_DB:
            case MINBU_DC_DA_DB:
              return Minb::Text(op1, op2, data, addr, len, result);
            case MINH_DC_DA_DB:
            case MINHU_DC_DA_DB:
              return Minh::Text(op1, op2, data, addr, len, result);
            case MOV_DC_DB:
            case MOV_EC_DB:     // TCv1.6 ISA only
            case MOV_EC_DA_DB:  // TCv1.6 ISA only
              return Mov::Text(op1, op2, data, addr, len, result);
            case NE_DC_DA_DB:
              return Ne::Text(op1, op2, data, addr, len, result);
            case OREQ_DC_DA_DB:
              return Oreq::Text(op1, op2, data, addr, len, result);
            case ORGE_DC_DA_DB:
            case ORGEU_DC_DA_DB:
              return OrgeGeu::Text(op1, op2, data, addr, len, result);
            case ORLT_DC_DA_DB:
            case ORLTU_DC_DA_DB:
              return OrltLtu::Text(op1, op2, data, addr, len, result);
            case ORNE_DC_DA_DB:
              return Orne::Text(op1, op2, data, addr, len, result);
            case SATB_DC_DA:
              return Satb::Text(op1, op2, data, addr, len, result);
            case SATBU_DC_DA:
              return Satbu::Text(op1, op2, data, addr, len, result);
            case SATH_DC_DA:
              return Sath::Text(op1, op2, data, addr, len, result);
            case SATHU_DC_DA:
              return Sathu::Text(op1, op2, data, addr, len, result);
            case SHEQ_DC_DA_DB:
              return Sheq::Text(op1, op2, data, addr, len, result);
            case SHGE_DC_DA_DB:
            case SHGEU_DC_DA_DB:
              return ShgeGeu::Text(op1, op2, data, addr, len, result);
            case SHLT_DC_DA_DB:
            case SHLTU_DC_DA_DB:
              return ShltLtu::Text(op1, op2, data, addr, len, result);
            case SHNE_DC_DA_DB:
              return Shne::Text(op1, op2, data, addr, len, result);
            case SUB_DC_DA_DB:
              return Sub::Text(op1, op2, data, addr, len, result);
            case SUBB_DC_DA_DB:
            case SUBH_DC_DA_DB:
              return Subbh::Text(op1, op2, data, addr, len, result);
            case SUBC_DC_DA_DB:
              return Subc::Text(op1, op2, data, addr, len, result);
            case SUBS_DC_DA_DB:
            case SUBSU_DC_DA_DB:
              return Subsu::Text(op1, op2, data, addr, len, result);
            case SUBSH_DC_DA_DB:
            case SUBSHU_DC_DA_DB:
              return SubshHu::Text(op1, op2, data, addr, len, result);
            case SUBX_DC_DA_DB:
              return Subx::Text(op1, op2, data, addr, len, result);
            case XOREQ_DC_DA_DB:
              return Xoreq::Text(op1, op2, data, addr, len, result);
            case XORGE_DC_DA_DB:
            case XORGEU_DC_DA_DB:
              return Xorge::Text(op1, op2, data, addr, len, result);
            case XORLT_DC_DA_DB:
            case XORLTU_DC_DA_DB:
              return Xorlt::Text(op1, op2, data, addr, len, result);
            case XORNE_DC_DA_DB:
              return Xorne::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x0D_SYS:
          switch (op2) {
            case DEBUG32:
              return Debug::Text(op1, op2, data, addr, len, result);
            case DISABLE:
              return Disable::Text(op1, op2, data, addr, len, result);
            case DISABLE_DA:  // TCv1.6 ISA only
              return DisableDa::Text(op1, op2, data, addr, len, result);
            case DSYNC:
              return Dsync::Text(op1, op2, data, addr, len, result);
            case ENABLE:
              return Enable::Text(op1, op2, data, addr, len, result);
            case ISYNC:
              return Isync::Text(op1, op2, data, addr, len, result);
            case NOP32:
              return Nop::Text(op1, op2, data, addr, len, result);
            case RESTORE_DA:  // TCv1.6 ISA only
              return RestoreDa::Text(op1, op2, data, addr, len, result);
            case RET32:
              return Ret::Text(op1, op2, data, addr, len, result);
            case FRET32:  // TCv1.6 ISA only
              return Fret::Text(op1, op2, data, addr, len, result);
            case RFE32:
              return Rfe::Text(op1, op2, data, addr, len, result);
            case RFM:
              return Rfm::Text(op1, op2, data, addr, len, result);
            case RSLCX:
              return Rslcx::Text(op1, op2, data, addr, len, result);
            case SVLCX:
              return Svlcx::Text(op1, op2, data, addr, len, result);
            case TRAPSV:
              return Trapsv::Text(op1, op2, data, addr, len, result);
            case TRAPV:
              return Trapv::Text(op1, op2, data, addr, len, result);
            case WAIT:  // TCv1.6 ISA only
              return Wait::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x0F_RR:
          switch (op2) {
            case AND_DC_DA_DB:
              return And::Text(op1, op2, data, addr, len, result);
            case ANDN_DC_DA_DB:
              return Andn::Text(op1, op2, data, addr, len, result);
            case CLO_DC_DA:
              return Clo::Text(op1, op2, data, addr, len, result);
            case CLOH_DC_DA:
              return Cloh::Text(op1, op2, data, addr, len, result);
            case CLS_DC_DA:
              return Cls::Text(op1, op2, data, addr, len, result);
            case CLSH_DC_DA:
              return Clsh::Text(op1, op2, data, addr, len, result);
            case CLZ_DC_DA:
              return Clz::Text(op1, op2, data, addr, len, result);
            case CLZH_DC_DA:
              return Clzh::Text(op1, op2, data, addr, len, result);
            case NAND_DC_DA_DB:
              return Nand::Text(op1, op2, data, addr, len, result);
            case NOR_DC_DA_DB:
              return Nor::Text(op1, op2, data, addr, len, result);
            case OR_DC_DA_DB:
              return Or::Text(op1, op2, data, addr, len, result);
            case ORN_DC_DA_DB:
              return Orn::Text(op1, op2, data, addr, len, result);
            case SH_DC_DA_DB:
              return Sh::Text(op1, op2, data, addr, len, result);
            case SHH_DC_DA_DB:
              return Shh::Text(op1, op2, data, addr, len, result);
            case SHA_DC_DA_DB:
              return Sha::Text(op1, op2, data, addr, len, result);
            case SHAH_DC_DA_DB:
              return Shah::Text(op1, op2, data, addr, len, result);
            case SHAS_DC_DA_DB:
              return Shas::Text(op1, op2, data, addr, len, result);
            case XNOR_DC_DA_DB:
              return Xnor::Text(op1, op2, data, addr, len, result);
            case XOR_DC_DA_DB:
              return Xor::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x13_RCR:
          switch (op2) {
            case MADD_DC_DD_DA_CONST9:
            case MADD_EC_ED_DA_CONST9:
            case MADDS_DC_DD_DA_CONST9:
            case MADDS_EC_ED_DA_CONST9:
              return Madd::Text(op1, op2, data, addr, len, result);
            case MADDU_EC_ED_DA_CONST9:
            case MADDSU_DC_DD_DA_CONST9:
            case MADDSU_EC_ED_DA_CONST9:
              return Maddu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x15_ABS:
          switch (op2) {
            case LDLCX_OFF18:
              return Ldlcx::Text(op1, op2, data, addr, len, result);
            case LDUCX_OFF18:
              return Lducx::Text(op1, op2, data, addr, len, result);
            case STLCX_OFF18:
              return Stlcx::Text(op1, op2, data, addr, len, result);
            case STUCX_OFF18:
              return Stucx::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x17_RRRR:
          switch (op2) {
            case DEXTR_DC_DA_DB_DD:
              return Dextr::Text(op1, op2, data, addr, len, result);
            case EXTR_DC_DA_ED:
            case EXTRU_DC_DA_ED:
              return Extr::Text(op1, op2, data, addr, len, result);
            case INSERT_DC_DA_DB_ED:
              return Insert::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x1F_BRR:
          switch (op2) {
            case JNED_DA_DB_DISP15:
              return Jned::Text(op1, op2, data, addr, len, result);
            case JNEI_DA_DB_DISP15:
              return Jnei::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x23_RRR2:
          switch (op2) {
            case MSUB_DC_DD_DA_DB:
            case MSUB_EC_ED_DA_DB:
            case MSUBS_DC_DD_DA_DB:
            case MSUBS_EC_ED_DA_DB:
              return Msub::Text(op1, op2, data, addr, len, result);
            case MSUBU_EC_ED_DA_DB:
            case MSUBSU_DC_DD_DA_DB:
            case MSUBSU_EC_ED_DA_DB:
              return Msubu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x25_ABS:
          switch (op2) {
            case STB_OFF18_DA:
              return Stb::Text(op1, op2, data, addr, len, result);
            case STH_OFF18_DA:
              return Sth::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x27_BIT:
          switch (op2) {
            case SHANDT_DC_DA_POS1_DB_POS2:
            case SHANDNT_DC_DA_POS1_DB_POS2:
            case SHNORT_DC_DA_POS1_DB_POS2:
            case SHORT_DC_DA_POS1_DB_POS2:
              return ShAndOrNorXorT::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x29_BO:
          switch (op2) {
            case LDA_AA_PB:
            case LDA_AA_PB_OFF10:
              return Lda::Text(op1, op2, data, addr, len, result);
            case LDB_DA_PB:
            case LDB_DA_PB_OFF10:
            case LDBU_DA_PB:
            case LDBU_DA_PB_OFF10:
              return Ldb::Text(op1, op2, data, addr, len, result);
            case LDD_EA_PB:
            case LDD_EA_PB_OFF10:
              return Ldd::Text(op1, op2, data, addr, len, result);
            case LDDA_PA_PB:
            case LDDA_PA_PB_OFF10:
              return Ldda::Text(op1, op2, data, addr, len, result);
            case LDH_DA_PB:
            case LDH_DA_PB_OFF10:
              return Ldh::Text(op1, op2, data, addr, len, result);
            case LDHU_DA_PB:
            case LDHU_DA_PB_OFF10:
              return Ldhu::Text(op1, op2, data, addr, len, result);
            case LDQ_DA_PB:
            case LDQ_DA_PB_OFF10:
              return Ldq::Text(op1, op2, data, addr, len, result);
            case LDW_DA_PB:
            case LDW_DA_PB_OFF10:
              return Ldw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x2B_RRR:
          switch (op2) {
            case CADD_DC_DD_DA_DB:
              return Cadd::Text(op1, op2, data, addr, len, result);
            case CADDN_DC_DD_DA_DB:
              return Caddn::Text(op1, op2, data, addr, len, result);
            case CSUB_DC_DD_DA_DB:
              return Csub::Text(op1, op2, data, addr, len, result);
            case CSUBN_DC_DD_DA_DB:
              return Csubn::Text(op1, op2, data, addr, len, result);
            case SEL_DC_DD_DA_DB:
              return Sel::Text(op1, op2, data, addr, len, result);
            case SELN_DC_DD_DA_DB:
              return Seln::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x2D_RR:
          switch (op2) {
            case CALLI_AA:
              return Calli::Text(op1, op2, data, addr, len, result);
            case FCALLI_AA:
              return Fcalli::Text(op1, op2, data, addr, len, result);
            case JI_AA_32:
              return Ji::Text(op1, op2, data, addr, len, result);
            case JLI_AA:
              return Jli::Text(op1, op2, data, addr, len, result);
            default:
              break;
          }
        case OP1_x2F_SYS:
          if (op2 == RSTV) {
            return Rstv::Text(op1, op2, data, addr, len, result);
          }
          return false;
        case OP1_x33_RCR:
          switch (op2) {
            case MSUB_DC_DD_DA_CONST9:
            case MSUB_EC_ED_DA_CONST9:
            case MSUBS_DC_DD_DA_CONST9:
            case MSUBS_EC_ED_DA_CONST9:
              return Msub::Text(op1, op2, data, addr, len, result);
            case MSUBU_EC_ED_DA_CONST9:
            case MSUBSU_DC_DD_DA_CONST9:
            case MSUBSU_EC_ED_DA_CONST9:
              return Msubu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x37_RRPW:
          switch (op2) {
            case EXTR_DC_DA_POS_WIDTH:
            case EXTRU_DC_DA_POS_WIDTH:
              return Extr::Text(op1, op2, data, addr, len, result);
            case IMASK_EC_DB_POS_WIDTH:
              return Imask::Text(op1, op2, data, addr, len, result);
            case INSERT_DC_DA_DB_POS_WIDTH:
              return Insert::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x3F_BRR:
          switch (op2) {
            case JLT_DA_DB_DISP15:
            case JLTU_DA_DB_DISP15:
              return Jlt::Text(op1, op2, data, addr, len, result);
            default:
              break;
          }
        case OP1_x43_RRR1:
          switch (op2) {
            case MADDQ_DC_DD_DA_DB_N:
            case MADDQ_EC_ED_DA_DB_N:
            case MADDQ_DC_DD_DA_DBL_N:
            case MADDQ_EC_ED_DA_DBL_N:
            case MADDQ_DC_DD_DA_DBU_N:
            case MADDQ_EC_ED_DA_DBU_N:
            case MADDQ_DC_DD_DAL_DBL_N:
            case MADDQ_EC_ED_DAL_DBL_N:
            case MADDQ_DC_DD_DAU_DBU_N:
            case MADDQ_EC_ED_DAU_DBU_N:
            case MADDSQ_DC_DD_DA_DB_N:
            case MADDSQ_EC_ED_DA_DB_N:
            case MADDSQ_DC_DD_DA_DBL_N:
            case MADDSQ_EC_ED_DA_DBL_N:
            case MADDSQ_DC_DD_DA_DBU_N:
            case MADDSQ_EC_ED_DA_DBU_N:
            case MADDSQ_DC_DD_DAL_DBL_N:
            case MADDSQ_EC_ED_DAL_DBL_N:
            case MADDSQ_DC_DD_DAU_DBU_N:
            case MADDSQ_EC_ED_DAU_DBU_N:
              return Maddq::Text(op1, op2, data, addr, len, result);
            case MADDRQ_DC_DD_DAL_DBL_N:
            case MADDRQ_DC_DD_DAU_DBU_N:
            case MADDRSQ_DC_DD_DAL_DBL_N:
            case MADDRSQ_DC_DD_DAU_DBU_N:
              return Maddrq::Text(op1, op2, data, addr, len, result);
            case MADDRH_DC_ED_DA_DBUL_N:
            case MADDRSH_DC_ED_DA_DBUL_N:
              return MaddrhSh::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x45_ABS:
          if (op2 == LDQ_DA_OFF18) {
            return Ldq::Text(op1, op2, data, addr, len, result);
          }
          return false;
        case OP1_x47_BIT:
          switch (op2) {
            case ANDANDT_DC_DA_POS1_DB_POS2:
            case ANDANDNT_DC_DA_POS1_DB_POS2:
            case ANDNORT_DC_DA_POS1_DB_POS2:
            case ANDORT_DC_DA_POS1_DB_POS2:
              return AndAndnNorOrT::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x49_BO:
          switch (op2) {
            case CMPSWAPW_AB_OFF10_EA:          // TCv1.6 ISA only
            case CMPSWAPW_AB_OFF10_EA_PREINC:   // TCv1.6 ISA only
            case CMPSWAPW_AB_OFF10_EA_POSTINC:  // TCv1.6 ISA only
              return Cmpswap::Text(op1, op2, data, addr, len, result);
            case LDLCX_AB_OFF10:
              return Ldlcx::Text(op1, op2, data, addr, len, result);
            case LDMST_AB_OFF10_EA:
            case LDMST_AB_OFF10_EA_POSTINC:
            case LDMST_AB_OFF10_EA_PREINC:
              return Ldmst::Text(op1, op2, data, addr, len, result);
            case LDUCX_AB_OFF10:
              return Lducx::Text(op1, op2, data, addr, len, result);
            case LEA_AA_AB_OFF10:
              return Lea::Text(op1, op2, data, addr, len, result);
            case STLCX_AB_OFF10:
              return Stlcx::Text(op1, op2, data, addr, len, result);
            case STUCX_AB_OFF10:
              return Stucx::Text(op1, op2, data, addr, len, result);
            case SWAPW_AB_OFF10_DA:
            case SWAPW_AB_OFF10_DA_POSTINC:
            case SWAPW_AB_OFF10_DA_PREINC:
              return Swapw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x4B_RR:
          switch (op2) {
            case BMERGE_DC_DA_DB:
              return Bmerge::Text(op1, op2, data, addr, len, result);
            case BSPLIT_EC_DA:
              return Bsplit::Text(op1, op2, data, addr, len, result);
            case CRC32_DC_DB_DA:  // TCv1.6 ISA only
              return Crc32::Text(op1, op2, data, addr, len, result);
            case DVINITB_EC_DA_DB:
            case DVINITBU_EC_DA_DB:
            case DVINITH_EC_DA_DB:
            case DVINITHU_EC_DA_DB:
            case DVINIT_EC_DA_DB:
            case DVINITU_EC_DA_DB:
              return Dvinit::Text(op1, op2, data, addr, len, result);
            case DIV_EC_DA_DB:
            case DIVU_EC_DA_DB:
              return Div::Text(op1, op2, data, addr, len, result);
            case PARITY_DC_DA:
              return Parity::Text(op1, op2, data, addr, len, result);
            case UNPACK_EC_DA:
              return Unpack::Text(op1, op2, data, addr, len, result);
            // FPU Instructions
            case CMPF_DC_DA_DB:
              return Cmpf::Text(op1, op2, data, addr, len, result);
            case DIVF_DC_DA_DB:
              return Divf::Text(op1, op2, data, addr, len, result);
            case FTOI_DC_DA:
              return Ftoi::Text(op1, op2, data, addr, len, result);
            case FTOIZ_DC_DA:
              return Ftoiz::Text(op1, op2, data, addr, len, result);
            case FTOQ31_DC_DA_DB:
              return Ftoq31::Text(op1, op2, data, addr, len, result);
            case FTOQ31Z_DC_DA_DB:
              return Ftoq31z::Text(op1, op2, data, addr, len, result);
            case FTOU_DC_DA:
              return Ftou::Text(op1, op2, data, addr, len, result);
            case FTOUZ_DC_DA:
              return Ftouz::Text(op1, op2, data, addr, len, result);
            case ITOF_DC_DA:
              return Itof::Text(op1, op2, data, addr, len, result);
            case MULF_DC_DA_DB:
              return Mulf::Text(op1, op2, data, addr, len, result);
            case Q31TOF_DC_DA_DB:
              return Q31tof::Text(op1, op2, data, addr, len, result);
            case QSEEDF_DC_DA:
              return Qseedf::Text(op1, op2, data, addr, len, result);
            case UPDFL_DA:
              return Updfl::Text(op1, op2, data, addr, len, result);
            case UTOF_DC_DA:
              return Utof::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x53_RC:
          switch (op2) {
            case MUL_DC_DA_CONST9:
            case MUL_EC_DA_CONST9:
            case MULS_DC_DA_CONST9:
              return Mul::Text(op1, op2, data, addr, len, result);
            case MULU_EC_DA_CONST9:
            case MULSU_DC_DA_CONST9:
              return Mulu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x57_RRRW:
          switch (op2) {
            case EXTR_DC_DA_DD_WIDTH:
            case EXTRU_DC_DA_DD_WIDTH:
              return Extr::Text(op1, op2, data, addr, len, result);
            case IMASK_EC_DB_DD_WIDTH:
              return Imask::Text(op1, op2, data, addr, len, result);
            case INSERT_DC_DA_DB_DD_WIDTH:
              return Insert::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x5F_BRR:
          switch (op2) {
            case JEQ_DA_DB_DISP15:
              return Jeq::Text(op1, op2, data, addr, len, result);
            case JNE_DA_DB_DISP15:
              return Jne::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x63_RRR1:
          switch (op2) {
            case MSUBQ_DC_DD_DA_DB_N:
            case MSUBQ_EC_ED_DA_DB_N:
            case MSUBQ_DC_DD_DA_DBL_N:
            case MSUBQ_EC_ED_DA_DBL_N:
            case MSUBQ_DC_DD_DA_DBU_N:
            case MSUBQ_EC_ED_DA_DBU_N:
            case MSUBQ_DC_DD_DAL_DBL_N:
            case MSUBQ_EC_ED_DAL_DBL_N:
            case MSUBQ_DC_DD_DAU_DBU_N:
            case MSUBQ_EC_ED_DAU_DBU_N:
            case MSUBSQ_DC_DD_DA_DB_N:
            case MSUBSQ_EC_ED_DA_DB_N:
            case MSUBSQ_DC_DD_DA_DBL_N:
            case MSUBSQ_EC_ED_DA_DBL_N:
            case MSUBSQ_DC_DD_DA_DBU_N:
            case MSUBSQ_EC_ED_DA_DBU_N:
            case MSUBSQ_DC_DD_DAL_DBL_N:
            case MSUBSQ_EC_ED_DAL_DBL_N:
            case MSUBSQ_DC_DD_DAU_DBU_N:
            case MSUBSQ_EC_ED_DAU_DBU_N:
              return Msubq::Text(op1, op2, data, addr, len, result);
            case MSUBRH_DC_ED_DA_DBUL_N:
            case MSUBRSH_DC_ED_DA_DBUL_N:
              return MsubrhSh::Text(op1, op2, data, addr, len, result);
            case MSUBRQ_DC_DD_DAL_DBL_N:
            case MSUBRQ_DC_DD_DAU_DBU_N:
            case MSUBRSQ_DC_DD_DAL_DBL_N:
            case MSUBRSQ_DC_DD_DAU_DBU_N:
              return Msubrq::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x65_ABS:
          if (op2 == STQ_OFF18_DA) {
            return Stq::Text(op1, op2, data, addr, len, result);
          }
          return false;
        case OP1_x67_BIT:
          switch (op2) {
            case INST_DC_DA_POS1_DB_POS2:
            case INSNT_DC_DA_POS1_DB_POS2:
              return InstNt::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x69_BO:
          switch (op2) {
            case CMPSWAPW_PB_EA:        // TCv1.6 ISA only
            case CMPSWAPW_PB_OFF10_EA:  // TCv1.6 ISA only
              return Cmpswap::Text(op1, op2, data, addr, len, result);
            case LDMST_PB_EA:
            case LDMST_PB_OFF10_EA:
              return Ldmst::Text(op1, op2, data, addr, len, result);
            case SWAPW_PB_DA:
            case SWAPW_PB_OFF10_DA:
              return Swapw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x6B_RRR:
          switch (op2) {
            case DVADJ_EC_ED_DB:
              return Dvadj::Text(op1, op2, data, addr, len, result);
            case DVSTEP_EC_ED_DB:
            case DVSTEPU_EC_ED_DB:
              return Dvstep::Text(op1, op2, data, addr, len, result);
            case IXMAX_EC_ED_DB:
            case IXMAXU_EC_ED_DB:
              return Ixmax::Text(op1, op2, data, addr, len, result);
            case IXMIN_EC_ED_DB:
            case IXMINU_EC_ED_DB:
              return Ixmin::Text(op1, op2, data, addr, len, result);
            case PACK_DC_ED_DA:
              return Pack::Text(op1, op2, data, addr, len, result);
            // FPU Instructions
            case ADDF_DC_DD_DA:
              return Addf::Text(op1, op2, data, addr, len, result);
            case MADDF_DC_DD_DA_DB:
              return Maddf::Text(op1, op2, data, addr, len, result);
            case MSUBF_DC_DD_DA_DB:
              return Msubf::Text(op1, op2, data, addr, len, result);
            case SUBF_DC_DD_DA:
              return Subf::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x73_RR2:
          switch (op2) {
            case MUL_DC_DA_DB:
            case MUL_EC_DA_DB:
            case MULS_DC_DA_DB:
              return Mul::Text(op1, op2, data, addr, len, result);
            case MULU_EC_DA_DB:
            case MULSU_DC_DA_DB:
              return Mulu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x75_RR:
          switch (op2) {
            // MMU Instructions
            case TLBDEMAP_DA:
              return Tlbdemap::Text(op1, op2, data, addr, len, result);
            case TLBFLUSHA:
            case TLBFLUSHB:
              return Tlbflush::Text(op1, op2, data, addr, len, result);
            case TLBMAP_EA:
              return Tlbmap::Text(op1, op2, data, addr, len, result);
            case TLBPROBEA_DA:
              return Tlbprobea::Text(op1, op2, data, addr, len, result);
            case TLBPROBEI_DA:
              return Tlbprobei::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x77_RRPW:
          if (op2 == DEXTR_DC_DA_DB_POS) {
            return Dextr::Text(op1, op2, data, addr, len, result);
          }
          return false;
        case OP1_x7D_BRR:
          switch (op2) {
            case JEQA_AA_AB_DISP15:
              return Jeqa::Text(op1, op2, data, addr, len, result);
            case JNEA_AA_AB_DISP15:
              return Jnea::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x7F_BRR:
          switch (op2) {
            case JGE_DA_DB_DISP15:
            case JGEU_DA_DB_DISP15:
              return Jge::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x83_RRR1:
          switch (op2) {
            case MADDH_EC_ED_DA_DBLL_N:
            case MADDH_EC_ED_DA_DBLU_N:
            case MADDH_EC_ED_DA_DBUL_N:
            case MADDH_EC_ED_DA_DBUU_N:
            case MADDSH_EC_ED_DA_DBLL_N:
            case MADDSH_EC_ED_DA_DBLU_N:
            case MADDSH_EC_ED_DA_DBUL_N:
            case MADDSH_EC_ED_DA_DBUU_N:
              return Maddh::Text(op1, op2, data, addr, len, result);
            case MADDMH_EC_ED_DA_DBLL_N:
            case MADDMH_EC_ED_DA_DBLU_N:
            case MADDMH_EC_ED_DA_DBUL_N:
            case MADDMH_EC_ED_DA_DBUU_N:
            case MADDMSH_EC_ED_DA_DBLL_N:
            case MADDMSH_EC_ED_DA_DBLU_N:
            case MADDMSH_EC_ED_DA_DBUL_N:
            case MADDMSH_EC_ED_DA_DBUU_N:
              return MaddmhSh::Text(op1, op2, data, addr, len, result);
            case MADDRH_DC_DD_DA_DBLL_N:
            case MADDRH_DC_DD_DA_DBLU_N:
            case MADDRH_DC_DD_DA_DBUL_N:
            case MADDRSH_DC_DD_DA_DBUU_N:
            case MADDRH_DC_DD_DA_DBUU_N:
            case MADDRSH_DC_DD_DA_DBLL_N:
            case MADDRSH_DC_DD_DA_DBLU_N:
            case MADDRSH_DC_DD_DA_DBUL_N:
              return MaddrhSh::Text(op1, op2, data, addr, len, result);
            default:
              return true;
          }
        case OP1_x85_ABS:
          switch (op2) {
            case LDA_AA_OFF18:
              return Lda::Text(op1, op2, data, addr, len, result);
            case LDD_EA_OFF18:
              return Ldd::Text(op1, op2, data, addr, len, result);
            case LDDA_PA_OFF18:
              return Ldda::Text(op1, op2, data, addr, len, result);
            case LDW_DA_OFF18:
              return Ldw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x87_BIT:
          switch (op2) {
            case ANDT_DC_DA_POS1_DB_POS2:
              return Andt::Text(op1, op2, data, addr, len, result);
            case ANDNT_DC_DA_POS1_DB_POS2:
              return Andnt::Text(op1, op2, data, addr, len, result);
            case NORT_DC_DA_POS1_DB_POS2:
              return Nort::Text(op1, op2, data, addr, len, result);
            case ORT_DC_DA_POS1_DB_POS2:
              return Ort::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x89_BO:
          switch (op2) {
            case CACHEAI_AB_OFF10:
            case CACHEAI_AB_OFF10_POSTINC:
            case CACHEAI_AB_OFF10_PREINC:
              return Cacheai::Text(op1, op2, data, addr, len, result);
            case CACHEAW_AB_OFF10:
            case CACHEAW_AB_OFF10_POSTINC:
            case CACHEAW_AB_OFF10_PREINC:
              return Cacheaw::Text(op1, op2, data, addr, len, result);
            case CACHEAWI_AB_OFF10:
            case CACHEAWI_AB_OFF10_POSTINC:
            case CACHEAWI_AB_OFF10_PREINC:
              return Cacheawi::Text(op1, op2, data, addr, len, result);
            case CACHEII_AB_OFF10:
            case CACHEII_AB_OFF10_POSTINC:
            case CACHEII_AB_OFF10_PREINC:
              return Cacheii::Text(op1, op2, data, addr, len, result);
            case CACHEIW_AB_OFF10:
            case CACHEIW_AB_OFF10_POSTINC:
            case CACHEIW_AB_OFF10_PREINC:
              return Cacheiw::Text(op1, op2, data, addr, len, result);
            case CACHEIWI_AB_OFF10:
            case CACHEIWI_AB_OFF10_POSTINC:
            case CACHEIWI_AB_OFF10_PREINC:
              return Cacheiwi::Text(op1, op2, data, addr, len, result);
            case STA_AB_OFF10_AA:
            case STA_AB_OFF10_AA_POSTINC:
            case STA_AB_OFF10_AA_PREINC:
              return Sta::Text(op1, op2, data, addr, len, result);
            case STB_AB_OFF10_DA:
            case STB_AB_OFF10_DA_POSTINC:
            case STB_AB_OFF10_DA_PREINC:
              return Stb::Text(op1, op2, data, addr, len, result);
            case STD_AB_OFF10_EA:
            case STD_AB_OFF10_EA_POSTINC:
            case STD_AB_OFF10_EA_PREINC:
              return Std::Text(op1, op2, data, addr, len, result);
            case STDA_AB_OFF10_PA:
            case STDA_AB_OFF10_PA_POSTINC:
            case STDA_AB_OFF10_PA_PREINC:
              return Stda::Text(op1, op2, data, addr, len, result);
            case STH_AB_OFF10_DA:
            case STH_AB_OFF10_DA_POSTINC:
            case STH_AB_OFF10_DA_PREINC:
              return Sth::Text(op1, op2, data, addr, len, result);
            case STQ_AB_OFF10_DA:
            case STQ_AB_OFF10_DA_POSTINC:
            case STQ_AB_OFF10_DA_PREINC:
              return Stq::Text(op1, op2, data, addr, len, result);
            case STW_AB_OFF10_DA:
            case STW_AB_OFF10_DA_POSTINC:
            case STW_AB_OFF10_DA_PREINC:
              return Stw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x8B_RC:
          switch (op2) {
            case ABSDIF_DC_DA_CONST9:
              return Absdifsh::Text(op1, op2, data, addr, len, result);
            case ABSDIFS_DC_DA_CONST9:
              return Absdifs::Text(op1, op2, data, addr, len, result);
            case ADD_DC_DA_CONST9:
              return Add::Text(op1, op2, data, addr, len, result);
            case ADDC_DC_DA_CONST9:
              return Addc::Text(op1, op2, data, addr, len, result);
            case ADDS_DC_DA_CONST9:
              return Adds::Text(op1, op2, data, addr, len, result);
            case ADDSU_DC_DA_CONST9:
              return Addsu::Text(op1, op2, data, addr, len, result);
            case ADDX_DC_DA_CONST9:
              return Addx::Text(op1, op2, data, addr, len, result);
            case ANDEQ_DC_DA_CONST9:
              return Andeq::Text(op1, op2, data, addr, len, result);
            case ANDGE_DC_DA_CONST9:
            case ANDGEU_DC_DA_CONST9:
              return AndgeGeU::Text(op1, op2, data, addr, len, result);
            case ANDLT_DC_DA_CONST9:
            case ANDLTU_DC_DA_CONST9:
              return AndltLtU::Text(op1, op2, data, addr, len, result);
            case ANDNE_DC_DA_CONST9:
              return Andne::Text(op1, op2, data, addr, len, result);
            case EQ_DC_DA_CONST9:
              return Eq::Text(op1, op2, data, addr, len, result);
            case EQANYB_DC_DA_CONST9:
            case EQANYH_DC_DA_CONST9:
              return Eqanybh::Text(op1, op2, data, addr, len, result);
            case GE_DC_DA_CONST9:
            case GEU_DC_DA_CONST9:
              return Ge::Text(op1, op2, data, addr, len, result);
            case LT_DC_DA_CONST9:
            case LTU_DC_DA_CONST9:
              return Lt::Text(op1, op2, data, addr, len, result);
            case MAX_DC_DA_CONST9:
            case MAXU_DC_DA_CONST9:
              return Max::Text(op1, op2, data, addr, len, result);
            case MIN_DC_DA_CONST9:
            case MINU_DC_DA_CONST9:
              return Min::Text(op1, op2, data, addr, len, result);
            case NE_DC_DA_CONST9:
              return Ne::Text(op1, op2, data, addr, len, result);
            case OREQ_DC_DA_CONST9:
              return Oreq::Text(op1, op2, data, addr, len, result);
            case ORGE_DC_DA_CONST9:
            case ORGEU_DC_DA_CONST9:
              return OrgeGeu::Text(op1, op2, data, addr, len, result);
            case ORLT_DC_DA_CONST9:
            case ORLTU_DC_DA_CONST9:
              return OrltLtu::Text(op1, op2, data, addr, len, result);
            case ORNE_DC_DA_CONST9:
              return Orne::Text(op1, op2, data, addr, len, result);
            case RSUB_DC_DA_CONST9:
              return Rsub::Text(op1, op2, data, addr, len, result);
            case RSUBS_DC_DA_CONST9:
            case RSUBSU_DC_DA_CONST9:
              return Rsubs::Text(op1, op2, data, addr, len, result);
            case SHEQ_DC_DA_CONST9:
              return Sheq::Text(op1, op2, data, addr, len, result);
            case SHGE_DC_DA_CONST9:
            case SHGEU_DC_DA_CONST9:
              return ShgeGeu::Text(op1, op2, data, addr, len, result);
            case SHLT_DC_DA_CONST9:
            case SHLTU_DC_DA_CONST9:
              return ShltLtu::Text(op1, op2, data, addr, len, result);
            case SHNE_DC_DA_CONST9:
              return Shne::Text(op1, op2, data, addr, len, result);
            case XOREQ_DC_DA_CONST9:
              return Xoreq::Text(op1, op2, data, addr, len, result);
            case XORGE_DC_DA_CONST9:
            case XORGEU_DC_DA_CONST9:
              return Xorge::Text(op1, op2, data, addr, len, result);
            case XORLT_DC_DA_CONST9:
            case XORLTU_DC_DA_CONST9:
              return Xorlt::Text(op1, op2, data, addr, len, result);
            case XORNE_DC_DA_CONST9:
              return Xorne::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x8F_RC:
          switch (op2) {
            case AND_DC_DA_CONST9:
              return And::Text(op1, op2, data, addr, len, result);
            case ANDN_DC_DA_CONST9:
              return Andn::Text(op1, op2, data, addr, len, result);
            case NAND_DC_DA_CONST9:
              return Nand::Text(op1, op2, data, addr, len, result);
            case NOR_DC_DA_CONST9:
              return Nor::Text(op1, op2, data, addr, len, result);
            case OR_DC_DA_CONST9:
              return Or::Text(op1, op2, data, addr, len, result);
            case ORN_DC_DA_CONST9:
              return Orn::Text(op1, op2, data, addr, len, result);
            case SH_DC_DA_CONST9:
              return Sh::Text(op1, op2, data, addr, len, result);
            case SHH_DC_DA_CONST9:
              return Shh::Text(op1, op2, data, addr, len, result);
            case SHA_DC_DA_CONST9:
              return Sha::Text(op1, op2, data, addr, len, result);
            case SHAH_DC_DA_CONST9:
              return Shah::Text(op1, op2, data, addr, len, result);
            case SHAS_DC_DA_CONST9:
              return Shas::Text(op1, op2, data, addr, len, result);
            case XNOR_DC_DA_CONST9:
              return Xnor::Text(op1, op2, data, addr, len, result);
            case XOR_DC_DA_CONST9:
              return Xor::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x93_RR1:
          switch (op2) {
            case MULQ_DC_DA_DB_N:
            case MULQ_EC_DA_DB_N:
            case MULQ_DC_DA_DBL_N:
            case MULQ_EC_DA_DBL_N:
            case MULQ_DC_DA_DBU_N:
            case MULQ_EC_DA_DBU_N:
            case MULQ_DC_DAL_DBL_N:
            case MULQ_DC_DAU_DBU_N:
              return Mulq::Text(op1, op2, data, addr, len, result);
            case MULRQ_DC_DAL_DBL_N:
            case MULRQ_DC_DAU_DBU_N:
              return Mulrq::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x97_RCRR:
          if (op2 == INSERT_DC_DA_CONST4_ED) {
            return Insert::Text(op1, op2, data, addr, len, result);
          }
        case OP1_x9F_BRC:
          switch (op2) {
            case JNED_DA_CONST4_DISP15:
              return Jned::Text(op1, op2, data, addr, len, result);
            case JNEI_DA_CONST4_DISP15:
              return Jnei::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xA3_RRR1:
          switch (op2) {
            case MSUBH_EC_ED_DA_DBLL_N:
            case MSUBH_EC_ED_DA_DBLU_N:
            case MSUBH_EC_ED_DA_DBUL_N:
            case MSUBH_EC_ED_DA_DBUU_N:
            case MSUBSH_EC_ED_DA_DBLL_N:
            case MSUBSH_EC_ED_DA_DBLU_N:
            case MSUBSH_EC_ED_DA_DBUL_N:
            case MSUBSH_EC_ED_DA_DBUU_N:
              return Msubh::Text(op1, op2, data, addr, len, result);
            case MSUBMH_EC_ED_DA_DBLL_N:
            case MSUBMH_EC_ED_DA_DBLU_N:
            case MSUBMH_EC_ED_DA_DBUL_N:
            case MSUBMH_EC_ED_DA_DBUU_N:
            case MSUBMSH_EC_ED_DA_DBLL_N:
            case MSUBMSH_EC_ED_DA_DBLU_N:
            case MSUBMSH_EC_ED_DA_DBUL_N:
            case MSUBMSH_EC_ED_DA_DBUU_N:
              return MsubmhSh::Text(op1, op2, data, addr, len, result);
            case MSUBRH_DC_DD_DA_DBLL_N:
            case MSUBRH_DC_DD_DA_DBLU_N:
            case MSUBRH_DC_DD_DA_DBUL_N:
            case MSUBRH_DC_DD_DA_DBUU_N:
            case MSUBRSH_DC_DD_DA_DBLL_N:
            case MSUBRSH_DC_DD_DA_DBLU_N:
            case MSUBRSH_DC_DD_DA_DBUL_N:
            case MSUBRSH_DC_DD_DA_DBUU_N:
              return MsubrhSh::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xA5_ABS:
          switch (op2) {
            case STA_OFF18_AA:
              return Sta::Text(op1, op2, data, addr, len, result);
            case STD_OFF18_EA:
              return Std::Text(op1, op2, data, addr, len, result);
            case STDA_OFF18_PA:
              return Stda::Text(op1, op2, data, addr, len, result);
            case STW_OFF18_DA:
              return Stw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xA7_BIT:
          switch (op2) {
            case SHNANDT_DC_DA_POS1_DB_POS2:
            case SHORNT_DC_DA_POS1_DB_POS2:
            case SHXNORT_DC_DA_POS1_DB_POS2:
            case SHXORT_DC_DA_POS1_DB_POS2:
              return ShAndOrNorXorT::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xA9_BO:
          switch (op2) {
            case CACHEAI_PB:
            case CACHEAI_PB_OFF10:
              return Cacheawi::Text(op1, op2, data, addr, len, result);
            case CACHEAW_PB:
            case CACHEAW_PB_OFF10:
              return Cacheawi::Text(op1, op2, data, addr, len, result);
            case CACHEAWI_PB:
            case CACHEAWI_PB_OFF10:
              return Cacheawi::Text(op1, op2, data, addr, len, result);
            case STA_PB_AA:
            case STA_PB_OFF10_AA:
              return Sta::Text(op1, op2, data, addr, len, result);
            case STB_PB_DA:
            case STB_PB_OFF10_DA:
              return Stb::Text(op1, op2, data, addr, len, result);
            case STD_PB_EA:
            case STD_PB_OFF10_EA:
              return Std::Text(op1, op2, data, addr, len, result);
            case STDA_PB_PA:
            case STDA_PB_OFF10_PA:
              return Stda::Text(op1, op2, data, addr, len, result);
            case STH_PB_DA:
            case STH_PB_OFF10_DA:
              return Sth::Text(op1, op2, data, addr, len, result);
            case STQ_PB_DA:
            case STQ_PB_OFF10_DA:
              return Stq::Text(op1, op2, data, addr, len, result);
            case STW_PB_DA:
            case STW_PB_OFF10_DA:
              return Stw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xAB_RCR:
          switch (op2) {
            case CADD_DC_DD_DA_CONST9:
              return Cadd::Text(op1, op2, data, addr, len, result);
            case CADDN_DC_DD_DA_CONST9:
              return Caddn::Text(op1, op2, data, addr, len, result);
            case SEL_DC_DD_DA_CONST9:
              return Sel::Text(op1, op2, data, addr, len, result);
            case SELN_DC_DD_DA_CONST9:
              return Seln::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xAD_RC:
          switch (op2) {
            case BISR_CONST9:
              return Bisr::Text(op1, op2, data, addr, len, result);
            case SYSCALL_CONST9:
              return Syscall::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xB3_RR1:
          switch (op2) {
            case MULH_EC_DA_DBLL_N:
            case MULH_EC_DA_DBLU_N:
            case MULH_EC_DA_DBUL_N:
            case MULH_EC_DA_DBUU_N:
              return Mulh::Text(op1, op2, data, addr, len, result);
            case MULMH_EC_DA_DBLL_N:
            case MULMH_EC_DA_DBLU_N:
            case MULMH_EC_DA_DBUL_N:
            case MULMH_EC_DA_DBUU_N:
              return Mulmh::Text(op1, op2, data, addr, len, result);
            case MULRH_DC_DA_DBLL_N:
            case MULRH_DC_DA_DBLU_N:
            case MULRH_DC_DA_DBUL_N:
            case MULRH_DC_DA_DBUU_N:
              return Mulrh::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xB7_RCPW:
          switch (op2) {
            case IMASK_EC_CONST4_POS_WIDTH:
              return Imask::Text(op1, op2, data, addr, len, result);
            case INSERT_DC_DA_CONST4_POS_WIDTH:
              return Insert::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xBD_BRR:
          switch (op2) {
            case JNZA_AA_DISP15:
              return Jnza::Text(op1, op2, data, addr, len, result);
            case JZA_AA_DISP15:
              return Jza::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xBF_BRC:
          switch (op2) {
            case JLT_DA_CONST4_DISP15:
            case JLTU_DA_CONST4_DISP15:
              return Jlt::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xC3_RRR1:
          switch (op2) {
            case MADDSUH_EC_ED_DA_DBLL_N:
            case MADDSUH_EC_ED_DA_DBLU_N:
            case MADDSUH_EC_ED_DA_DBUL_N:
            case MADDSUH_EC_ED_DA_DBUU_N:
            case MADDSUSH_EC_ED_DA_DBLL_N:
            case MADDSUSH_EC_ED_DA_DBLU_N:
            case MADDSUSH_EC_ED_DA_DBUL_N:
            case MADDSUSH_EC_ED_DA_DBUU_N:
              return MaddsuhSh::Text(op1, op2, data, addr, len, result);
            case MADDSUMH_EC_ED_DA_DBLL_N:
            case MADDSUMH_EC_ED_DA_DBLU_N:
            case MADDSUMH_EC_ED_DA_DBUL_N:
            case MADDSUMH_EC_ED_DA_DBUU_N:
            case MADDSUMSH_EC_ED_DA_DBLL_N:
            case MADDSUMSH_EC_ED_DA_DBLU_N:
            case MADDSUMSH_EC_ED_DA_DBUL_N:
            case MADDSUMSH_EC_ED_DA_DBUU_N:
              return MaddsumhSh::Text(op1, op2, data, addr, len, result);
            case MADDSURH_DC_DD_DA_DBLL_N:
            case MADDSURH_DC_DD_DA_DBLU_N:
            case MADDSURH_DC_DD_DA_DBUL_N:
            case MADDSURH_DC_DD_DA_DBUU_N:
            case MADDSURSH_DC_DD_DA_DBLL_N:
            case MADDSURSH_DC_DD_DA_DBLU_N:
            case MADDSURSH_DC_DD_DA_DBUL_N:
            case MADDSURSH_DC_DD_DA_DBUU_N:
              return MaddsurhSh::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xC5_ABS:
          if (op2 == LEA_AA_OFF18) {
            return Lea::Text(op1, op2, data, addr, len, result);
          }
          return false;
        case OP1_xC7_BIT:
          switch (op2) {
            case ORANDT_DC_DA_POS1_DB_POS2:
            case ORANDNT_DC_DA_POS1_DB_POS2:
            case ORNORT_DC_DA_POS1_DB_POS2:
            case ORORT_DC_DA_POS1_DB_POS2:
              return OrAndtAndntNortOrt::Text(op1, op2, data, addr, len,
                                              result);
            default:
              return false;
          }
        case OP1_xD5_ABSB:
          if (op2 == STT_OFF18_BPOS3_B) {
            return Stt::Text(op1, op2, data, addr, len, result);
          }
          return false;
        case OP1_xD7_RCRW:
          switch (op2) {
            case IMASK_EC_CONST4_POS_WIDTH:
              return Imask::Text(op1, op2, data, addr, len, result);
            case INSERT_DC_DA_CONST4_DD_WIDTH:
              return Insert::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xDF_BRC:
          switch (op2) {
            case JEQ_DA_CONST4_DISP15:
              return Jeq::Text(op1, op2, data, addr, len, result);
            case JNE_DA_CONST4_DISP15:
              return Jne::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xE3_RRR1:
          switch (op2) {
            case MSUBADH_EC_ED_DA_DBLL_N:
            case MSUBADH_EC_ED_DA_DBLU_N:
            case MSUBADH_EC_ED_DA_DBUL_N:
            case MSUBADH_EC_ED_DA_DBUU_N:
            case MSUBADSH_EC_ED_DA_DBLL_N:
            case MSUBADSH_EC_ED_DA_DBLU_N:
            case MSUBADSH_EC_ED_DA_DBUL_N:
            case MSUBADSH_EC_ED_DA_DBUU_N:
              return MsubadhSh::Text(op1, op2, data, addr, len, result);
            case MSUBADMH_EC_ED_DA_DBLL_N:
            case MSUBADMH_EC_ED_DA_DBLU_N:
            case MSUBADMH_EC_ED_DA_DBUL_N:
            case MSUBADMH_EC_ED_DA_DBUU_N:
            case MSUBADMSH_EC_ED_DA_DBLL_N:
            case MSUBADMSH_EC_ED_DA_DBLU_N:
            case MSUBADMSH_EC_ED_DA_DBUL_N:
            case MSUBADMSH_EC_ED_DA_DBUU_N:
              return MsubadmhSh::Text(op1, op2, data, addr, len, result);
            case MSUBADRH_DC_DD_DA_DBLL_N:
            case MSUBADRH_DC_DD_DA_DBLU_N:
            case MSUBADRH_DC_DD_DA_DBUL_N:
            case MSUBADRH_DC_DD_DA_DBUU_N:
            case MSUBADRSH_DC_DD_DA_DBLL_N:
            case MSUBADRSH_DC_DD_DA_DBLU_N:
            case MSUBADRSH_DC_DD_DA_DBUL_N:
            case MSUBADRSH_DC_DD_DA_DBUU_N:
              return MsubadrhSh::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xE5_ABS:
          switch (op2) {
            case LDMST_OFF18_EA:
              return Ldmst::Text(op1, op2, data, addr, len, result);
            case SWAPW_OFF18_DA:
              return Swapw::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xFD_BRR:
          switch (op2) {
            case LOOP_AB_DISP15:
              return Loop::Text(op1, op2, data, addr, len, result);
            case LOOPU_DISP15:
              return Loopu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xFF_BRC:
          switch (op2) {
            case JGE_DA_CONST4_DISP15:
            case JGEU_DA_CONST4_DISP15:
              return Jge::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        default:
          return false;
      }
    }
    // 16-bit instruction
    else {
      // The ADDSCA 16-bit instruction is weird and has a smaller primary opcode
      // (SRRS format)
      if ((raw & 0x3Fu) == ADDSCA_AA_AB_D15_N) {
        return Addscaat::Text(raw & 0x3Fu, 0xFF, data, addr, len, result);
      }
      uint8_t op1 = raw & 0xFFu;
      // This will get op2 if it exists
      uint32_t op2 = Instruction::ExtractSecondaryOpcode(raw & 0xFFu, data);
      op2 &= 0xFFu;  // secondary opcodes are not > 0xFF, even though some
                     // formats have up to 12 bits
      switch (op1) {
        // 2-byte non-branching, single opcode
        case ADD_DA_CONST4:
        case ADD_DA_D15_CONST4:
        case ADD_D15_DA_CONST4:
        case ADD_DA_DB:
        case ADD_DA_D15_DB:
        case ADD_D15_DA_DB:
          return Add::Text(op1, op2, data, addr, len, result);
        case ADDA_AA_CONST4:
        case ADDA_AA_AB:
          return Adda::Text(op1, op2, data, addr, len, result);
        case ADDS_DA_DB:
          return Adds::Text(op1, op2, data, addr, len, result);
        case AND_D15_CONST8:
        case AND_DA_DB:
          return And::Text(op1, op2, data, addr, len, result);
        case BISR_CONST8:
          return Bisr::Text(op1, op2, data, addr, len, result);
        case CADD_DA_D15_CONST4:
          return Cadd::Text(op1, op2, data, addr, len, result);
        case CADDN_DA_D15_CONST4:
          return Caddn::Text(op1, op2, data, addr, len, result);
        case CMOV_DA_D15_CONST4:
        case CMOV_DA_D15_DB:
          return Cmov::Text(op1, op2, data, addr, len, result);
        case CMOVN_DA_D15_CONST4:
        case CMOVN_DA_D15_DB:
          return Cmovn::Text(op1, op2, data, addr, len, result);
        case EQ_D15_DA_CONST4:
        case EQ_D15_DA_DB:
          return Eq::Text(op1, op2, data, addr, len, result);
        case LDA_A15_A10_CONST8:
        case LDA_AC_AB:
        case LDA_AC_AB_POSTINC:
        case LDA_AC_A15_OFF4:
        case LDA_A15_AB_OFF4:
          return Lda::Text(op1, op2, data, addr, len, result);
        case LDBU_DC_AB:
        case LDBU_DC_AB_POSTINC:
        case LDBU_DC_A15_OFF4:
        case LDBU_D15_AB_OFF4:
          return Ldb::Text(op1, op2, data, addr, len, result);
        case LDH_DC_AB:
        case LDH_DC_AB_POSTINC:
        case LDH_DC_A15_OFF4:
        case LDH_D15_AB_OFF4:
          return Ldh::Text(op1, op2, data, addr, len, result);
        case LDW_D15_A10_CONST8:
        case LDW_DC_AB:
        case LDW_DC_AB_POSTINC:
        case LDW_DC_A15_OFF4:
        case LDW_D15_AB_OFF4:
          return Ldw::Text(op1, op2, data, addr, len, result);
        case LT_D15_DA_CONST4:
        case LT_D15_DA_DB:
          return Lt::Text(op1, op2, data, addr, len, result);
        case MOV_D15_CONST8:
        case MOV_DA_CONST4:
        case MOV_EA_CONST4:  // TCv1.6 ISA only
        case MOV_DA_DB:
          return Mov::Text(op1, op2, data, addr, len, result);
        case MOVA_AA_CONST4:
        case MOVA_AA_DB:
          return Mova::Text(op1, op2, data, addr, len, result);
        case MOVAA_AA_AB:
          return Movaa::Text(op1, op2, data, addr, len, result);
        case MOVD_DA_AB:
          return Movd::Text(op1, op2, data, addr, len, result);
        case MUL_DA_DB:
          return Mul::Text(op1, op2, data, addr, len, result);
        case OR_D15_CONST8:
        case OR_DA_DB:
          return Or::Text(op1, op2, data, addr, len, result);
        case SH_DA_CONST4:
          return Sh::Text(op1, op2, data, addr, len, result);
        case SHA_DA_CONST4:
          return Sha::Text(op1, op2, data, addr, len, result);
        case STA_A10_CONST8_A15:
        case STA_AB_OFF4_A15:
        case STA_AB_AA:
        case STA_AB_AA_POSTINC:
        case STA_A15_OFF4_AA:
          return Sta::Text(op1, op2, data, addr, len, result);
        case STB_AB_OFF4_D15:
        case STB_AB_DA:
        case STB_AB_DA_POSTINC:
        case STB_A15_OFF4_DA:
          return Stb::Text(op1, op2, data, addr, len, result);
        case STH_AB_OFF4_D15:
        case STH_AB_DA:
        case STH_AB_DA_POSTINC:
        case STH_A15_OFF4_DA:
          return Sth::Text(op1, op2, data, addr, len, result);
        case STW_A10_CONST8_D15:
        case STW_AB_OFF4_D15:
        case STW_AB_DA:
        case STW_AB_DA_POSTINC:
        case STW_A15_OFF4_DA:
          return Stw::Text(op1, op2, data, addr, len, result);
        case SUB_DA_DB:
        case SUB_DA_D15_DB:
        case SUB_D15_DA_DB:
          return Sub::Text(op1, op2, data, addr, len, result);
        case SUBA_A10_CONST8:
          return Suba::Text(op1, op2, data, addr, len, result);
        case SUBS_DA_DB:
          return Subsu::Text(op1, op2, data, addr, len, result);
        case XOR_DA_DB:
          return Xor::Text(op1, op2, data, addr, len, result);

        // 2-byte non-branching, dual opcode
        case OP1_x00_SR:
          switch (op2) {
            case DEBUG16:
              return Debug::Text(op1, op2, data, addr, len, result);
            case NOP16:
              return Nop::Text(op1, op2, data, addr, len, result);
            case RET16:
              return Ret::Text(op1, op2, data, addr, len, result);
            case FRET16:
              return Fret::Text(op1, op2, data, addr, len, result);
            case RFE16:
              return Rfe::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x46_SR:
          switch (op2) {
            case NOT_DA:
              return Not::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_x32_SR:
          switch (op2) {
            case RSUB_DA:
              return Rsub::Text(op1, op2, data, addr, len, result);
            case SATB_DA:
              return Satb::Text(op1, op2, data, addr, len, result);
            case SATBU_DA:
              return Satbu::Text(op1, op2, data, addr, len, result);
            case SATH_DA:
              return Sath::Text(op1, op2, data, addr, len, result);
            case SATHU_DA:
              return Sathu::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }
        case OP1_xDC_SR:
          switch (op2) {
            case JI_AA_16:
              return Ji::Text(op1, op2, data, addr, len, result);
            default:
              return false;
          }

        // 2-byte branching
        case CALL_DISP8:
          return Call::Text(op1, op2, data, addr, len, result);
        case J_DISP8:
          return J::Text(op1, op2, data, addr, len, result);
        case JEQ_D15_CONST4_DISP4:
        case JEQ_D15_DB_DISP4:
        case JEQ_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
        case JEQ_D15_DB_DISP4_EX:      // TCv1.6 ISA only
          return Jeq::Text(op1, op2, data, addr, len, result);
        case JGEZ_DB_DISP4:
          return Jgez::Text(op1, op2, data, addr, len, result);
        case JGTZ_DB_DISP4:
          return Jgtz::Text(op1, op2, data, addr, len, result);
        case JLEZ_DB_DISP4:
          return Jlez::Text(op1, op2, data, addr, len, result);
        case JLTZ_DB_DISP4:
          return Jltz::Text(op1, op2, data, addr, len, result);
        case JNE_D15_CONST4_DISP4:
        case JNE_D15_DB_DISP4:
        case JNE_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
        case JNE_D15_DB_DISP4_EX:      // TCv1.6 ISA only
          return Jne::Text(op1, op2, data, addr, len, result);
        case JNZ_D15_DISP8:
        case JNZ_DB_DISP4:
          return Jnz::Text(op1, op2, data, addr, len, result);
        case JNZA_AB_DISP4:
          return Jnza::Text(op1, op2, data, addr, len, result);
        case JNZT_D15_N_DISP4:
          return Jnzt::Text(op1, op2, data, addr, len, result);
        case JZ_D15_DISP8:
        case JZ_DB_DISP4:
          return Jz::Text(op1, op2, data, addr, len, result);
        case JZA_AB_DISP4:
          return Jza::Text(op1, op2, data, addr, len, result);
        case JZT_D15_N_DISP4:
          return Jzt::Text(op1, op2, data, addr, len, result);
        case LOOP_AB_DISP4:
          return Loop::Text(op1, op2, data, addr, len, result);
        default:
          return false;
      }
    }
  }

  bool GetInstructionLowLevelIL(const uint8_t* data, uint64_t addr, size_t& len,
                                LowLevelILFunction& il) override {
    auto wdata = (const uint16_t*)data;
    uint16_t raw = *wdata;
    uint8_t seg;
    uint16_t caddr;
    uint32_t target;
    std::string proc_name = this->GetName();

    // 32-bit instruction
    if (raw & 0x01u) {
      // There are two instructions of the BRN format, which has a 7-bit primary
      // opcode (instead of 8-bit)
      if ((raw & 0x7Fu) == OP1_x6F_BRN) {
        uint32_t op2 = Instruction::ExtractSecondaryOpcode(raw & 0x7Fu, data);
        if (op2 == JZT_DA_N_DISP15) {
          return Jzt::Lift(raw & 0x7Fu, op2, data, addr, len, il, this);
        }
        if (op2 == JNZT_DA_N_DISP15) {
          return Jnzt::Lift(raw & 0x7Fu, op2, data, addr, len, il, this);
        }
        return false;
      }
      uint8_t op1 = raw & 0xFFu;
      // This will get op2 if it exists
      uint32_t op2 = Instruction::ExtractSecondaryOpcode(op1, data);
      op2 &= 0xFFu;  // secondary opcodes are not > 0xFF, even though some
                     // formats have up to 12 bits
      switch (op1) {
        // 4-byte non-branching, single opcode
        case ADDI_DC_DA_CONST16:
          return Addi::Lift(op1, op2, data, addr, len, il);
        case ADDIH_DC_DA_CONST16:
          return Addih::Lift(op1, op2, data, addr, len, il);
        case ADDIHA_AC_AA_CONST16:
          return Addiha::Lift(op1, op2, data, addr, len, il);
        case LDA_AA_AB_OFF16:
          return Lda::Lift(op1, op2, data, addr, len, il);
        case LDB_DA_AB_OFF16:   // TCv1.6 ISA only
        case LDBU_DA_AB_OFF16:  // TCv1.6 ISA only
          return Ldb::Lift(op1, op2, data, addr, len, il);
        case LDH_DA_AB_OFF16:  // TCv1.6 ISA only
          return Ldh::Lift(op1, op2, data, addr, len, il);
        case LDHU_DA_AB_OFF16:  // TCv1.6 ISA only
          return Ldhu::Lift(op1, op2, data, addr, len, il);
        case LDW_DA_AB_OFF16:
          return Ldw::Lift(op1, op2, data, addr, len, il);
        case LEA_AA_AB_OFF16:
          return Lea::Lift(op1, op2, data, addr, len, il);
        case MFCR_DC_CONST16:
          return Mfcr::Lift(op1, op2, data, addr, len, il);
        case MOV_DC_CONST16:
        case MOV_EC_CONST16:  // TCv1.6 ISA only
          return Mov::Lift(op1, op2, data, addr, len, il);
        case MOVU_DC_CONST16:
          return Movu::Lift(op1, op2, data, addr, len, il);
        case MOVH_DC_CONST16:
          return Movh::Lift(op1, op2, data, addr, len, il);
        case MOVHA_AC_CONST16:
          return Movha::Lift(op1, op2, data, addr, len, il);
        case MTCR_CONST16_DA:
          return Mtcr::Lift(op1, op2, data, addr, len, il);
        case STB_AB_OFF16_DA:
          return Stb::Lift(op1, op2, data, addr, len, il);
        case STH_AB_OFF16_DA:
          return Sth::Lift(op1, op2, data, addr, len, il);
        case STA_AB_OFF16_AA:
          return Sta::Lift(op1, op2, data, addr, len, il);
        case STW_AB_OFF16_DA:
          return Stw::Lift(op1, op2, data, addr, len, il);

        // 4-byte branching, single opcode
        case CALL_DISP24:
          return Call::Lift(op1, op2, data, addr, len, il);
        case CALLA_DISP24:
          return Calla::Lift(op1, op2, data, addr, len, il);
        case FCALL_DISP24:
          return Fcall::Lift(op1, op2, data, addr, len, il);
        case FCALLA_DISP24:
          return Fcalla::Lift(op1, op2, data, addr, len, il);
        case J_DISP24:
          return J::Lift(op1, op2, data, addr, len, il);
        case JA_DISP24:
          return Ja::Lift(op1, op2, data, addr, len, il);
        case JL_DISP24:
          return Jl::Lift(op1, op2, data, addr, len, il);
        case JLA_DISP24:
          return Jla::Lift(op1, op2, data, addr, len, il);

        // 4-byte, dual opcode
        case OP1_x01_RR:
          switch (op2) {
            case ADDA_AC_AA_AB:
              return Adda::Lift(op1, op2, data, addr, len, il);
            case ADDSCA_AC_AB_DA_N:
            case ADDSCAT_AC_AB_DA:
              return Addscaat::Lift(op1, op2, data, addr, len, il);
            case EQA_DC_AA_AB:
              return Eqa::Lift(op1, op2, data, addr, len, il);
            case EQZA_DC_AA:
              return Eqza::Lift(op1, op2, data, addr, len, il);
            case GEA_DC_AA_AB:
              return Gea::Lift(op1, op2, data, addr, len, il);
            case LTA_DC_AA_AB:
              return Lta::Lift(op1, op2, data, addr, len, il);
            case MOVA_AC_DB:
              return Mova::Lift(op1, op2, data, addr, len, il);
            case MOVAA_AC_AB:
              return Movaa::Lift(op1, op2, data, addr, len, il);
            case MOVD_DC_AB:
              return Movd::Lift(op1, op2, data, addr, len, il);
            case NEA_DC_AA_AB:
              return Nea::Lift(op1, op2, data, addr, len, il);
            case NEZA_DC_AA:
              return Neza::Lift(op1, op2, data, addr, len, il);
            case SUBA_AC_AA_AB:
              return Suba::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x03_RRR2:
          switch (op2) {
            case MADD_DC_DD_DA_DB:
            case MADD_EC_ED_DA_DB:
            case MADDS_DC_DD_DA_DB:
            case MADDS_EC_ED_DA_DB:
              return Madd::Lift(op1, op2, data, addr, len, il);
            case MADDU_EC_ED_DA_DB:
            case MADDSU_DC_DD_DA_DB:
            case MADDSU_EC_ED_DA_DB:
              return Maddu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x05_ABS:
          switch (op2) {
            case LDB_DA_OFF18:
            case LDBU_DA_OFF18:
              return Ldb::Lift(op1, op2, data, addr, len, il);
            case LDH_DA_OFF18:
              return Ldh::Lift(op1, op2, data, addr, len, il);
            case LDHU_DA_OFF18:
              return Ldhu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x07_BIT:
          switch (op2) {
            case NANDT_DC_DA_POS1_DB_POS2:
              return Nandt::Lift(op1, op2, data, addr, len, il);
            case ORNT_DC_DA_POS1_DB_POS2:
              return Ornt::Lift(op1, op2, data, addr, len, il);
            case XNORT_DC_DA_POS1_DB_POS2:
              return Xnort::Lift(op1, op2, data, addr, len, il);
            case XORT_DC_DA_POS1_DB_POS2:
              return Xort::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x09_BO:
          switch (op2) {
            case LDA_AA_AB_OFF10:
            case LDA_AA_AB_OFF10_POSTINC:
            case LDA_AA_AB_OFF10_PREINC:
              return Lda::Lift(op1, op2, data, addr, len, il);
            case LDB_DA_AB_OFF10:
            case LDB_DA_AB_OFF10_POSTINC:
            case LDB_DA_AB_OFF10_PREINC:
            case LDBU_DA_AB_OFF10:
            case LDBU_DA_AB_OFF10_POSTINC:
            case LDBU_DA_AB_OFF10_PREINC:
              return Ldb::Lift(op1, op2, data, addr, len, il);
            case LDD_EA_AB_OFF10:
            case LDD_EA_AB_OFF10_POSTINC:
            case LDD_EA_AB_OFF10_PREINC:
              return Ldd::Lift(op1, op2, data, addr, len, il);
            case LDDA_PA_AB_OFF10:
            case LDDA_PA_AB_OFF10_POSTINC:
            case LDDA_PA_AB_OFF10_PREINC:
              return Ldda::Lift(op1, op2, data, addr, len, il);
            case LDH_DA_AB_OFF10:
            case LDH_DA_AB_OFF10_POSTINC:
            case LDH_DA_AB_OFF10_PREINC:
              return Ldh::Lift(op1, op2, data, addr, len, il);
            case LDHU_DA_AB_OFF10:
            case LDHU_DA_AB_OFF10_POSTINC:
            case LDHU_DA_AB_OFF10_PREINC:
              return Ldhu::Lift(op1, op2, data, addr, len, il);
            case LDQ_DA_AB_OFF10:
            case LDQ_DA_AB_OFF10_POSTINC:
            case LDQ_DA_AB_OFF10_PREINC:
              return Ldq::Lift(op1, op2, data, addr, len, il);
            case LDW_DA_AB_OFF10:
            case LDW_DA_AB_OFF10_POSTINC:
            case LDW_DA_AB_OFF10_PREINC:
              return Ldw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x0B_RR:
          switch (op2) {
            case ABS_DC_DB:
              return Abs::Lift(op1, op2, data, addr, len, il);
            case ABSB_DC_DB:
            case ABSH_DC_DB:
              return Absbh::Lift(op1, op2, data, addr, len, il);
            case ABSDIF_DC_DA_DB:
              return Absdif::Lift(op1, op2, data, addr, len, il);
            case ABSDIFB_DC_DA_DB:
            case ABSDIFH_DC_DA_DB:
              return Absdifbh::Lift(op1, op2, data, addr, len, il);
            case ABSDIFS_DC_DA_DB:
              return Absdifs::Lift(op1, op2, data, addr, len, il);
            case ABSDIFSH_DC_DA_DB:
              return Absdifsh::Lift(op1, op2, data, addr, len, il);
            case ABSS_DC_DB:
              return Abss::Lift(op1, op2, data, addr, len, il);
            case ABSSH_DC_DB:
              return Abssh::Lift(op1, op2, data, addr, len, il);
            case ADD_DC_DA_DB:
              return Add::Lift(op1, op2, data, addr, len, il);
            case ADDB_DC_DA_DB:
            case ADDH_DC_DA_DB:
              return Addbh::Lift(op1, op2, data, addr, len, il);
            case ADDC_DC_DA_DB:
              return Addc::Lift(op1, op2, data, addr, len, il);
            case ADDS_DC_DA_DB:
              return Adds::Lift(op1, op2, data, addr, len, il);
            case ADDSH_DC_DA_DB:
            case ADDSHU_DC_DA_DB:
              return Addshhu::Lift(op1, op2, data, addr, len, il);
            case ADDSU_DC_DA_DB:
              return Addsu::Lift(op1, op2, data, addr, len, il);
            case ADDX_DC_DA_DB:
              return Addx::Lift(op1, op2, data, addr, len, il);
            case ANDEQ_DC_DA_DB:
              return Andeq::Lift(op1, op2, data, addr, len, il);
            case ANDGE_DC_DA_DB:
            case ANDGEU_DC_DA_DB:
              return AndgeGeU::Lift(op1, op2, data, addr, len, il);
            case ANDLT_DC_DA_DB:
            case ANDLTU_DC_DA_DB:
              return AndltLtU::Lift(op1, op2, data, addr, len, il);
            case ANDNE_DC_DA_DB:
              return Andne::Lift(op1, op2, data, addr, len, il);
            case EQ_DC_DA_DB:
              return Eq::Lift(op1, op2, data, addr, len, il);
            case EQB_DC_DA_DB:
            case EQH_DC_DA_DB:
            case EQW_DC_DA_DB:
              return Eqbhw::Lift(op1, op2, data, addr, len, il);
            case EQANYB_DC_DA_DB:
            case EQANYH_DC_DA_DB:
              return Eqanybh::Lift(op1, op2, data, addr, len, il);
            case GE_DC_DA_DB:
            case GEU_DC_DA_DB:
              return Ge::Lift(op1, op2, data, addr, len, il);
            case LT_DC_DA_DB:
            case LTU_DC_DA_DB:
              return Lt::Lift(op1, op2, data, addr, len, il);
            case LTB_DC_DA_DB:
            case LTBU_DC_DA_DB:
              return Ltb::Lift(op1, op2, data, addr, len, il);
            case LTH_DC_DA_DB:
            case LTHU_DC_DA_DB:
              return Lth::Lift(op1, op2, data, addr, len, il);
            case LTW_DC_DA_DB:
            case LTWU_DC_DA_DB:
              return Ltw::Lift(op1, op2, data, addr, len, il);
            case MAX_DC_DA_DB:
            case MAXU_DC_DA_DB:
              return Max::Lift(op1, op2, data, addr, len, il);
            case MAXB_DC_DA_DB:
            case MAXBU_DC_DA_DB:
              return Maxb::Lift(op1, op2, data, addr, len, il);
            case MAXH_DC_DA_DB:
            case MAXHU_DC_DA_DB:
              return Maxh::Lift(op1, op2, data, addr, len, il);
            case MIN_DC_DA_DB:
            case MINU_DC_DA_DB:
              return Min::Lift(op1, op2, data, addr, len, il);
            case MINB_DC_DA_DB:
            case MINBU_DC_DA_DB:
              return Minb::Lift(op1, op2, data, addr, len, il);
            case MINH_DC_DA_DB:
            case MINHU_DC_DA_DB:
              return Minh::Lift(op1, op2, data, addr, len, il);
            case MOV_DC_DB:
            case MOV_EC_DB:     // TCv1.6 ISA only
            case MOV_EC_DA_DB:  // TCv1.6 ISA only
              return Mov::Lift(op1, op2, data, addr, len, il);
            case NE_DC_DA_DB:
              return Ne::Lift(op1, op2, data, addr, len, il);
            case OREQ_DC_DA_DB:
              return Oreq::Lift(op1, op2, data, addr, len, il);
            case ORGE_DC_DA_DB:
            case ORGEU_DC_DA_DB:
              return OrgeGeu::Lift(op1, op2, data, addr, len, il);
            case ORLT_DC_DA_DB:
            case ORLTU_DC_DA_DB:
              return OrltLtu::Lift(op1, op2, data, addr, len, il);
            case ORNE_DC_DA_DB:
              return Orne::Lift(op1, op2, data, addr, len, il);
            case SATB_DC_DA:
              return Satb::Lift(op1, op2, data, addr, len, il);
            case SATBU_DC_DA:
              return Satbu::Lift(op1, op2, data, addr, len, il);
            case SATH_DC_DA:
              return Sath::Lift(op1, op2, data, addr, len, il);
            case SATHU_DC_DA:
              return Sathu::Lift(op1, op2, data, addr, len, il);
            case SHEQ_DC_DA_DB:
              return Sheq::Lift(op1, op2, data, addr, len, il);
            case SHGE_DC_DA_DB:
            case SHGEU_DC_DA_DB:
              return ShgeGeu::Lift(op1, op2, data, addr, len, il);
            case SHLT_DC_DA_DB:
            case SHLTU_DC_DA_DB:
              return ShltLtu::Lift(op1, op2, data, addr, len, il);
            case SHNE_DC_DA_DB:
              return Shne::Lift(op1, op2, data, addr, len, il);
            case SUB_DC_DA_DB:
              return Sub::Lift(op1, op2, data, addr, len, il);
            case SUBB_DC_DA_DB:
            case SUBH_DC_DA_DB:
              return Subbh::Lift(op1, op2, data, addr, len, il);
            case SUBC_DC_DA_DB:
              return Subc::Lift(op1, op2, data, addr, len, il);
            case SUBS_DC_DA_DB:
            case SUBSU_DC_DA_DB:
              return Subsu::Lift(op1, op2, data, addr, len, il);
            case SUBSH_DC_DA_DB:
            case SUBSHU_DC_DA_DB:
              return SubshHu::Lift(op1, op2, data, addr, len, il);
            case SUBX_DC_DA_DB:
              return Subx::Lift(op1, op2, data, addr, len, il);
            case XOREQ_DC_DA_DB:
              return Xoreq::Lift(op1, op2, data, addr, len, il);
            case XORGE_DC_DA_DB:
            case XORGEU_DC_DA_DB:
              return Xorge::Lift(op1, op2, data, addr, len, il);
            case XORLT_DC_DA_DB:
            case XORLTU_DC_DA_DB:
              return Xorlt::Lift(op1, op2, data, addr, len, il);
            case XORNE_DC_DA_DB:
              return Xorne::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x0D_SYS:
          switch (op2) {
            case DEBUG32:
              return Debug::Lift(op1, op2, data, addr, len, il);
            case DISABLE:
              return Disable::Lift(op1, op2, data, addr, len, il);
            case DISABLE_DA:  // TCv1.6 ISA only
              return DisableDa::Lift(op1, op2, data, addr, len, il);
            case DSYNC:
              return Dsync::Lift(op1, op2, data, addr, len, il);
            case ENABLE:
              return Enable::Lift(op1, op2, data, addr, len, il);
            case ISYNC:
              return Isync::Lift(op1, op2, data, addr, len, il);
            case NOP32:
              return Nop::Lift(op1, op2, data, addr, len, il);
            case RESTORE_DA:  // TCv1.6 ISA only
              return RestoreDa::Lift(op1, op2, data, addr, len, il);
            case RET32:
              return Ret::Lift(op1, op2, data, addr, len, il);
            case FRET32:  // TCv1.6 ISA only
              return Fret::Lift(op1, op2, data, addr, len, il);
            case RFE32:
              return Rfe::Lift(op1, op2, data, addr, len, il);
            case RFM:
              return Rfm::Lift(op1, op2, data, addr, len, il);
            case RSLCX:
              return Rslcx::Lift(op1, op2, data, addr, len, il);
            case SVLCX:
              return Svlcx::Lift(op1, op2, data, addr, len, il);
            case TRAPSV:
              return Trapsv::Lift(op1, op2, data, addr, len, il);
            case TRAPV:
              return Trapv::Lift(op1, op2, data, addr, len, il);
            case WAIT:  // TCv1.6 ISA only
              return Wait::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x0F_RR:
          switch (op2) {
            case AND_DC_DA_DB:
              return And::Lift(op1, op2, data, addr, len, il);
            case ANDN_DC_DA_DB:
              return Andn::Lift(op1, op2, data, addr, len, il);
            case CLO_DC_DA:
              return Clo::Lift(op1, op2, data, addr, len, il);
            case CLOH_DC_DA:
              return Cloh::Lift(op1, op2, data, addr, len, il);
            case CLS_DC_DA:
              return Cls::Lift(op1, op2, data, addr, len, il);
            case CLSH_DC_DA:
              return Clsh::Lift(op1, op2, data, addr, len, il);
            case CLZ_DC_DA:
              return Clz::Lift(op1, op2, data, addr, len, il);
            case CLZH_DC_DA:
              return Clzh::Lift(op1, op2, data, addr, len, il);
            case NAND_DC_DA_DB:
              return Nand::Lift(op1, op2, data, addr, len, il);
            case NOR_DC_DA_DB:
              return Nor::Lift(op1, op2, data, addr, len, il);
            case OR_DC_DA_DB:
              return Or::Lift(op1, op2, data, addr, len, il);
            case ORN_DC_DA_DB:
              return Orn::Lift(op1, op2, data, addr, len, il);
            case SH_DC_DA_DB:
              return Sh::Lift(op1, op2, data, addr, len, il);
            case SHH_DC_DA_DB:
              return Shh::Lift(op1, op2, data, addr, len, il);
            case SHA_DC_DA_DB:
              return Sha::Lift(op1, op2, data, addr, len, il);
            case SHAH_DC_DA_DB:
              return Shah::Lift(op1, op2, data, addr, len, il);
            case SHAS_DC_DA_DB:
              return Shas::Lift(op1, op2, data, addr, len, il);
            case XNOR_DC_DA_DB:
              return Xnor::Lift(op1, op2, data, addr, len, il);
            case XOR_DC_DA_DB:
              return Xor::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x13_RCR:
          switch (op2) {
            case MADD_DC_DD_DA_CONST9:
            case MADD_EC_ED_DA_CONST9:
            case MADDS_DC_DD_DA_CONST9:
            case MADDS_EC_ED_DA_CONST9:
              return Madd::Lift(op1, op2, data, addr, len, il);
            case MADDU_EC_ED_DA_CONST9:
            case MADDSU_DC_DD_DA_CONST9:
            case MADDSU_EC_ED_DA_CONST9:
              return Maddu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x15_ABS:
          switch (op2) {
            case LDLCX_OFF18:
              return Ldlcx::Lift(op1, op2, data, addr, len, il);
            case LDUCX_OFF18:
              return Lducx::Lift(op1, op2, data, addr, len, il);
            case STLCX_OFF18:
              return Stlcx::Lift(op1, op2, data, addr, len, il);
            case STUCX_OFF18:
              return Stucx::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x17_RRRR:
          switch (op2) {
            case DEXTR_DC_DA_DB_DD:
              return Dextr::Lift(op1, op2, data, addr, len, il);
            case EXTR_DC_DA_ED:
            case EXTRU_DC_DA_ED:
              return Extr::Lift(op1, op2, data, addr, len, il);
            case INSERT_DC_DA_DB_ED:
              return Insert::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x1F_BRR:
          switch (op2) {
            case JNED_DA_DB_DISP15:
              return Jned::Lift(op1, op2, data, addr, len, il, this);
            case JNEI_DA_DB_DISP15:
              return Jnei::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_x23_RRR2:
          switch (op2) {
            case MSUB_DC_DD_DA_DB:
            case MSUB_EC_ED_DA_DB:
            case MSUBS_DC_DD_DA_DB:
            case MSUBS_EC_ED_DA_DB:
              return Msub::Lift(op1, op2, data, addr, len, il);
            case MSUBU_EC_ED_DA_DB:
            case MSUBSU_DC_DD_DA_DB:
            case MSUBSU_EC_ED_DA_DB:
              return Msubu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x25_ABS:
          switch (op2) {
            case STB_OFF18_DA:
              return Stb::Lift(op1, op2, data, addr, len, il);
            case STH_OFF18_DA:
              return Sth::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x27_BIT:
          switch (op2) {
            case SHANDT_DC_DA_POS1_DB_POS2:
            case SHANDNT_DC_DA_POS1_DB_POS2:
            case SHNORT_DC_DA_POS1_DB_POS2:
            case SHORT_DC_DA_POS1_DB_POS2:
              return ShAndOrNorXorT::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x29_BO:
          switch (op2) {
            case LDA_AA_PB:
            case LDA_AA_PB_OFF10:
              return Lda::Lift(op1, op2, data, addr, len, il);
            case LDB_DA_PB:
            case LDB_DA_PB_OFF10:
            case LDBU_DA_PB:
            case LDBU_DA_PB_OFF10:
              return Ldb::Lift(op1, op2, data, addr, len, il);
            case LDD_EA_PB:
            case LDD_EA_PB_OFF10:
              return Ldd::Lift(op1, op2, data, addr, len, il);
            case LDDA_PA_PB:
            case LDDA_PA_PB_OFF10:
              return Ldda::Lift(op1, op2, data, addr, len, il);
            case LDH_DA_PB:
            case LDH_DA_PB_OFF10:
              return Ldh::Lift(op1, op2, data, addr, len, il);
            case LDHU_DA_PB:
            case LDHU_DA_PB_OFF10:
              return Ldhu::Lift(op1, op2, data, addr, len, il);
            case LDQ_DA_PB:
            case LDQ_DA_PB_OFF10:
              return Ldq::Lift(op1, op2, data, addr, len, il);
            case LDW_DA_PB:
            case LDW_DA_PB_OFF10:
              return Ldw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x2B_RRR:
          switch (op2) {
            case CADD_DC_DD_DA_DB:
              return Cadd::Lift(op1, op2, data, addr, len, il);
            case CADDN_DC_DD_DA_DB:
              return Caddn::Lift(op1, op2, data, addr, len, il);
            case CSUB_DC_DD_DA_DB:
              return Csub::Lift(op1, op2, data, addr, len, il);
            case CSUBN_DC_DD_DA_DB:
              return Csubn::Lift(op1, op2, data, addr, len, il);
            case SEL_DC_DD_DA_DB:
              return Sel::Lift(op1, op2, data, addr, len, il);
            case SELN_DC_DD_DA_DB:
              return Seln::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x2D_RR:
          switch (op2) {
            case CALLI_AA:
              return Calli::Lift(op1, op2, data, addr, len, il);
            case FCALLI_AA:
              return Fcalli::Lift(op1, op2, data, addr, len, il);
            case JI_AA_32:
              return Ji::Lift(op1, op2, data, addr, len, il);
            case JLI_AA:
              return Jli::Lift(op1, op2, data, addr, len, il);
            default:
              break;
          }
        case OP1_x2F_SYS:
          if (op2 == RSTV) {
            return Rstv::Lift(op1, op2, data, addr, len, il);
          }
          return false;
        case OP1_x33_RCR:
          switch (op2) {
            case MSUB_DC_DD_DA_CONST9:
            case MSUB_EC_ED_DA_CONST9:
            case MSUBS_DC_DD_DA_CONST9:
            case MSUBS_EC_ED_DA_CONST9:
              return Msub::Lift(op1, op2, data, addr, len, il);
            case MSUBU_EC_ED_DA_CONST9:
            case MSUBSU_DC_DD_DA_CONST9:
            case MSUBSU_EC_ED_DA_CONST9:
              return Msubu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x37_RRPW:
          switch (op2) {
            case EXTR_DC_DA_POS_WIDTH:
            case EXTRU_DC_DA_POS_WIDTH:
              return Extr::Lift(op1, op2, data, addr, len, il);
            case IMASK_EC_DB_POS_WIDTH:
              return Imask::Lift(op1, op2, data, addr, len, il);
            case INSERT_DC_DA_DB_POS_WIDTH:
              return Insert::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x3F_BRR:
          switch (op2) {
            case JLT_DA_DB_DISP15:
            case JLTU_DA_DB_DISP15:
              return Jlt::Lift(op1, op2, data, addr, len, il, this);
            default:
              break;
          }
        case OP1_x43_RRR1:
          switch (op2) {
            case MADDQ_DC_DD_DA_DB_N:
            case MADDQ_EC_ED_DA_DB_N:
            case MADDQ_DC_DD_DA_DBL_N:
            case MADDQ_EC_ED_DA_DBL_N:
            case MADDQ_DC_DD_DA_DBU_N:
            case MADDQ_EC_ED_DA_DBU_N:
            case MADDQ_DC_DD_DAL_DBL_N:
            case MADDQ_EC_ED_DAL_DBL_N:
            case MADDQ_DC_DD_DAU_DBU_N:
            case MADDQ_EC_ED_DAU_DBU_N:
            case MADDSQ_DC_DD_DA_DB_N:
            case MADDSQ_EC_ED_DA_DB_N:
            case MADDSQ_DC_DD_DA_DBL_N:
            case MADDSQ_EC_ED_DA_DBL_N:
            case MADDSQ_DC_DD_DA_DBU_N:
            case MADDSQ_EC_ED_DA_DBU_N:
            case MADDSQ_DC_DD_DAL_DBL_N:
            case MADDSQ_EC_ED_DAL_DBL_N:
            case MADDSQ_DC_DD_DAU_DBU_N:
            case MADDSQ_EC_ED_DAU_DBU_N:
              return Maddq::Lift(op1, op2, data, addr, len, il);
            case MADDRQ_DC_DD_DAL_DBL_N:
            case MADDRQ_DC_DD_DAU_DBU_N:
            case MADDRSQ_DC_DD_DAL_DBL_N:
            case MADDRSQ_DC_DD_DAU_DBU_N:
              return Maddrq::Lift(op1, op2, data, addr, len, il);
            case MADDRH_DC_ED_DA_DBUL_N:
            case MADDRSH_DC_ED_DA_DBUL_N:
              return MaddrhSh::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x45_ABS:
          if (op2 == LDQ_DA_OFF18) {
            return Ldq::Lift(op1, op2, data, addr, len, il);
          }
          return false;
        case OP1_x47_BIT:
          switch (op2) {
            case ANDANDT_DC_DA_POS1_DB_POS2:
            case ANDANDNT_DC_DA_POS1_DB_POS2:
            case ANDNORT_DC_DA_POS1_DB_POS2:
            case ANDORT_DC_DA_POS1_DB_POS2:
              return AndAndnNorOrT::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x49_BO:
          switch (op2) {
            case CMPSWAPW_AB_OFF10_EA:          // TCv1.6 ISA only
            case CMPSWAPW_AB_OFF10_EA_PREINC:   // TCv1.6 ISA only
            case CMPSWAPW_AB_OFF10_EA_POSTINC:  // TCv1.6 ISA only
              return Cmpswap::Lift(op1, op2, data, addr, len, il);
            case LDLCX_AB_OFF10:
              return Ldlcx::Lift(op1, op2, data, addr, len, il);
            case LDMST_AB_OFF10_EA:
            case LDMST_AB_OFF10_EA_POSTINC:
            case LDMST_AB_OFF10_EA_PREINC:
              return Ldmst::Lift(op1, op2, data, addr, len, il);
            case LDUCX_AB_OFF10:
              return Lducx::Lift(op1, op2, data, addr, len, il);
            case LEA_AA_AB_OFF10:
              return Lea::Lift(op1, op2, data, addr, len, il);
            case STLCX_AB_OFF10:
              return Stlcx::Lift(op1, op2, data, addr, len, il);
            case STUCX_AB_OFF10:
              return Stucx::Lift(op1, op2, data, addr, len, il);
            case SWAPW_AB_OFF10_DA:
            case SWAPW_AB_OFF10_DA_POSTINC:
            case SWAPW_AB_OFF10_DA_PREINC:
              return Swapw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x4B_RR:
          switch (op2) {
            case BMERGE_DC_DA_DB:
              return Bmerge::Lift(op1, op2, data, addr, len, il);
            case BSPLIT_EC_DA:
              return Bsplit::Lift(op1, op2, data, addr, len, il);
            case CRC32_DC_DB_DA:  // TCv1.6 ISA only
              return Crc32::Lift(op1, op2, data, addr, len, il);
            case DVINITB_EC_DA_DB:
            case DVINITBU_EC_DA_DB:
            case DVINITH_EC_DA_DB:
            case DVINITHU_EC_DA_DB:
            case DVINIT_EC_DA_DB:
            case DVINITU_EC_DA_DB:
              return Dvinit::Lift(op1, op2, data, addr, len, il);
            case DIV_EC_DA_DB:
            case DIVU_EC_DA_DB:
              return Div::Lift(op1, op2, data, addr, len, il);
            case PARITY_DC_DA:
              return Parity::Lift(op1, op2, data, addr, len, il);
            case UNPACK_EC_DA:
              return Unpack::Lift(op1, op2, data, addr, len, il);
            // FPU Instructions
            case CMPF_DC_DA_DB:
              return Cmpf::Lift(op1, op2, data, addr, len, il);
            case DIVF_DC_DA_DB:
              return Divf::Lift(op1, op2, data, addr, len, il);
            case FTOI_DC_DA:
              return Ftoi::Lift(op1, op2, data, addr, len, il);
            case FTOIZ_DC_DA:
              return Ftoiz::Lift(op1, op2, data, addr, len, il);
            case FTOQ31_DC_DA_DB:
              return Ftoq31::Lift(op1, op2, data, addr, len, il);
            case FTOQ31Z_DC_DA_DB:
              return Ftoq31z::Lift(op1, op2, data, addr, len, il);
            case FTOU_DC_DA:
              return Ftou::Lift(op1, op2, data, addr, len, il);
            case FTOUZ_DC_DA:
              return Ftouz::Lift(op1, op2, data, addr, len, il);
            case ITOF_DC_DA:
              return Itof::Lift(op1, op2, data, addr, len, il);
            case MULF_DC_DA_DB:
              return Mulf::Lift(op1, op2, data, addr, len, il);
            case Q31TOF_DC_DA_DB:
              return Q31tof::Lift(op1, op2, data, addr, len, il);
            case QSEEDF_DC_DA:
              return Qseedf::Lift(op1, op2, data, addr, len, il);
            case UPDFL_DA:
              return Updfl::Lift(op1, op2, data, addr, len, il);
            case UTOF_DC_DA:
              return Utof::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x53_RC:
          switch (op2) {
            case MUL_DC_DA_CONST9:
            case MUL_EC_DA_CONST9:
            case MULS_DC_DA_CONST9:
              return Mul::Lift(op1, op2, data, addr, len, il);
            case MULU_EC_DA_CONST9:
            case MULSU_DC_DA_CONST9:
              return Mulu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x57_RRRW:
          switch (op2) {
            case EXTR_DC_DA_DD_WIDTH:
            case EXTRU_DC_DA_DD_WIDTH:
              return Extr::Lift(op1, op2, data, addr, len, il);
            case IMASK_EC_DB_DD_WIDTH:
              return Imask::Lift(op1, op2, data, addr, len, il);
            case INSERT_DC_DA_DB_DD_WIDTH:
              return Insert::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x5F_BRR:
          switch (op2) {
            case JEQ_DA_DB_DISP15:
              return Jeq::Lift(op1, op2, data, addr, len, il, this);
            case JNE_DA_DB_DISP15:
              return Jne::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_x63_RRR1:
          switch (op2) {
            case MSUBQ_DC_DD_DA_DB_N:
            case MSUBQ_EC_ED_DA_DB_N:
            case MSUBQ_DC_DD_DA_DBL_N:
            case MSUBQ_EC_ED_DA_DBL_N:
            case MSUBQ_DC_DD_DA_DBU_N:
            case MSUBQ_EC_ED_DA_DBU_N:
            case MSUBQ_DC_DD_DAL_DBL_N:
            case MSUBQ_EC_ED_DAL_DBL_N:
            case MSUBQ_DC_DD_DAU_DBU_N:
            case MSUBQ_EC_ED_DAU_DBU_N:
            case MSUBSQ_DC_DD_DA_DB_N:
            case MSUBSQ_EC_ED_DA_DB_N:
            case MSUBSQ_DC_DD_DA_DBL_N:
            case MSUBSQ_EC_ED_DA_DBL_N:
            case MSUBSQ_DC_DD_DA_DBU_N:
            case MSUBSQ_EC_ED_DA_DBU_N:
            case MSUBSQ_DC_DD_DAL_DBL_N:
            case MSUBSQ_EC_ED_DAL_DBL_N:
            case MSUBSQ_DC_DD_DAU_DBU_N:
            case MSUBSQ_EC_ED_DAU_DBU_N:
              return Msubq::Lift(op1, op2, data, addr, len, il);
            case MSUBRH_DC_ED_DA_DBUL_N:
            case MSUBRSH_DC_ED_DA_DBUL_N:
              return MsubrhSh::Lift(op1, op2, data, addr, len, il);
            case MSUBRQ_DC_DD_DAL_DBL_N:
            case MSUBRQ_DC_DD_DAU_DBU_N:
            case MSUBRSQ_DC_DD_DAL_DBL_N:
            case MSUBRSQ_DC_DD_DAU_DBU_N:
              return Msubrq::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x65_ABS:
          if (op2 == STQ_OFF18_DA) {
            return Stq::Lift(op1, op2, data, addr, len, il);
          }
          return false;
        case OP1_x67_BIT:
          switch (op2) {
            case INST_DC_DA_POS1_DB_POS2:
            case INSNT_DC_DA_POS1_DB_POS2:
              return InstNt::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x69_BO:
          switch (op2) {
            case CMPSWAPW_PB_EA:        // TCv1.6 ISA only
            case CMPSWAPW_PB_OFF10_EA:  // TCv1.6 ISA only
              return Cmpswap::Lift(op1, op2, data, addr, len, il);
            case LDMST_PB_EA:
            case LDMST_PB_OFF10_EA:
              return Ldmst::Lift(op1, op2, data, addr, len, il);
            case SWAPW_PB_DA:
            case SWAPW_PB_OFF10_DA:
              return Swapw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x6B_RRR:
          switch (op2) {
            case DVADJ_EC_ED_DB:
              return Dvadj::Lift(op1, op2, data, addr, len, il);
            case DVSTEP_EC_ED_DB:
            case DVSTEPU_EC_ED_DB:
              return Dvstep::Lift(op1, op2, data, addr, len, il);
            case IXMAX_EC_ED_DB:
            case IXMAXU_EC_ED_DB:
              return Ixmax::Lift(op1, op2, data, addr, len, il);
            case IXMIN_EC_ED_DB:
            case IXMINU_EC_ED_DB:
              return Ixmin::Lift(op1, op2, data, addr, len, il);
            case PACK_DC_ED_DA:
              return Pack::Lift(op1, op2, data, addr, len, il);
            // FPU Instructions
            case ADDF_DC_DD_DA:
              return Addf::Lift(op1, op2, data, addr, len, il);
            case MADDF_DC_DD_DA_DB:
              return Maddf::Lift(op1, op2, data, addr, len, il);
            case MSUBF_DC_DD_DA_DB:
              return Msubf::Lift(op1, op2, data, addr, len, il);
            case SUBF_DC_DD_DA:
              return Subf::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x73_RR2:
          switch (op2) {
            case MUL_DC_DA_DB:
            case MUL_EC_DA_DB:
            case MULS_DC_DA_DB:
              return Mul::Lift(op1, op2, data, addr, len, il);
            case MULU_EC_DA_DB:
            case MULSU_DC_DA_DB:
              return Mulu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x75_RR:
          switch (op2) {
            // MMU Instructions
            case TLBDEMAP_DA:
              return Tlbdemap::Lift(op1, op2, data, addr, len, il);
            case TLBFLUSHA:
            case TLBFLUSHB:
              return Tlbflush::Lift(op1, op2, data, addr, len, il);
            case TLBMAP_EA:
              return Tlbmap::Lift(op1, op2, data, addr, len, il);
            case TLBPROBEA_DA:
              return Tlbprobea::Lift(op1, op2, data, addr, len, il);
            case TLBPROBEI_DA:
              return Tlbprobei::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x77_RRPW:
          if (op2 == DEXTR_DC_DA_DB_POS) {
            return Dextr::Lift(op1, op2, data, addr, len, il);
          }
          return false;
        case OP1_x7D_BRR:
          switch (op2) {
            case JEQA_AA_AB_DISP15:
              return Jeqa::Lift(op1, op2, data, addr, len, il, this);
            case JNEA_AA_AB_DISP15:
              return Jnea::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_x7F_BRR:
          switch (op2) {
            case JGE_DA_DB_DISP15:
            case JGEU_DA_DB_DISP15:
              return Jge::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_x83_RRR1:
          switch (op2) {
            case MADDH_EC_ED_DA_DBLL_N:
            case MADDH_EC_ED_DA_DBLU_N:
            case MADDH_EC_ED_DA_DBUL_N:
            case MADDH_EC_ED_DA_DBUU_N:
            case MADDSH_EC_ED_DA_DBLL_N:
            case MADDSH_EC_ED_DA_DBLU_N:
            case MADDSH_EC_ED_DA_DBUL_N:
            case MADDSH_EC_ED_DA_DBUU_N:
              return Maddh::Lift(op1, op2, data, addr, len, il);
            case MADDMH_EC_ED_DA_DBLL_N:
            case MADDMH_EC_ED_DA_DBLU_N:
            case MADDMH_EC_ED_DA_DBUL_N:
            case MADDMH_EC_ED_DA_DBUU_N:
            case MADDMSH_EC_ED_DA_DBLL_N:
            case MADDMSH_EC_ED_DA_DBLU_N:
            case MADDMSH_EC_ED_DA_DBUL_N:
            case MADDMSH_EC_ED_DA_DBUU_N:
              return MaddmhSh::Lift(op1, op2, data, addr, len, il);
            case MADDRH_DC_DD_DA_DBLL_N:
            case MADDRH_DC_DD_DA_DBLU_N:
            case MADDRH_DC_DD_DA_DBUL_N:
            case MADDRSH_DC_DD_DA_DBUU_N:
            case MADDRH_DC_DD_DA_DBUU_N:
            case MADDRSH_DC_DD_DA_DBLL_N:
            case MADDRSH_DC_DD_DA_DBLU_N:
            case MADDRSH_DC_DD_DA_DBUL_N:
              return MaddrhSh::Lift(op1, op2, data, addr, len, il);
            default:
              return true;
          }
        case OP1_x85_ABS:
          switch (op2) {
            case LDA_AA_OFF18:
              return Lda::Lift(op1, op2, data, addr, len, il);
            case LDD_EA_OFF18:
              return Ldd::Lift(op1, op2, data, addr, len, il);
            case LDDA_PA_OFF18:
              return Ldda::Lift(op1, op2, data, addr, len, il);
            case LDW_DA_OFF18:
              return Ldw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x87_BIT:
          switch (op2) {
            case ANDT_DC_DA_POS1_DB_POS2:
              return Andt::Lift(op1, op2, data, addr, len, il);
            case ANDNT_DC_DA_POS1_DB_POS2:
              return Andnt::Lift(op1, op2, data, addr, len, il);
            case NORT_DC_DA_POS1_DB_POS2:
              return Nort::Lift(op1, op2, data, addr, len, il);
            case ORT_DC_DA_POS1_DB_POS2:
              return Ort::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x89_BO:
          switch (op2) {
            case CACHEAI_AB_OFF10:
            case CACHEAI_AB_OFF10_POSTINC:
            case CACHEAI_AB_OFF10_PREINC:
              return Cacheai::Lift(op1, op2, data, addr, len, il);
            case CACHEAW_AB_OFF10:
            case CACHEAW_AB_OFF10_POSTINC:
            case CACHEAW_AB_OFF10_PREINC:
              return Cacheaw::Lift(op1, op2, data, addr, len, il);
            case CACHEAWI_AB_OFF10:
            case CACHEAWI_AB_OFF10_POSTINC:
            case CACHEAWI_AB_OFF10_PREINC:
              return Cacheawi::Lift(op1, op2, data, addr, len, il);
            case CACHEIW_AB_OFF10:
            case CACHEIW_AB_OFF10_POSTINC:
            case CACHEIW_AB_OFF10_PREINC:
              return Cacheiw::Lift(op1, op2, data, addr, len, il);
            case CACHEIWI_AB_OFF10:
            case CACHEIWI_AB_OFF10_POSTINC:
            case CACHEIWI_AB_OFF10_PREINC:
              return Cacheiwi::Lift(op1, op2, data, addr, len, il);
            case STA_AB_OFF10_AA:
            case STA_AB_OFF10_AA_POSTINC:
            case STA_AB_OFF10_AA_PREINC:
              return Sta::Lift(op1, op2, data, addr, len, il);
            case STB_AB_OFF10_DA:
            case STB_AB_OFF10_DA_POSTINC:
            case STB_AB_OFF10_DA_PREINC:
              return Stb::Lift(op1, op2, data, addr, len, il);
            case STD_AB_OFF10_EA:
            case STD_AB_OFF10_EA_POSTINC:
            case STD_AB_OFF10_EA_PREINC:
              return Std::Lift(op1, op2, data, addr, len, il);
            case STDA_AB_OFF10_PA:
            case STDA_AB_OFF10_PA_POSTINC:
            case STDA_AB_OFF10_PA_PREINC:
              return Stda::Lift(op1, op2, data, addr, len, il);
            case STH_AB_OFF10_DA:
            case STH_AB_OFF10_DA_POSTINC:
            case STH_AB_OFF10_DA_PREINC:
              return Sth::Lift(op1, op2, data, addr, len, il);
            case STQ_AB_OFF10_DA:
            case STQ_AB_OFF10_DA_POSTINC:
            case STQ_AB_OFF10_DA_PREINC:
              return Stq::Lift(op1, op2, data, addr, len, il);
            case STW_AB_OFF10_DA:
            case STW_AB_OFF10_DA_POSTINC:
            case STW_AB_OFF10_DA_PREINC:
              return Stw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x8B_RC:
          switch (op2) {
            case ABSDIF_DC_DA_CONST9:
              return Absdifsh::Lift(op1, op2, data, addr, len, il);
            case ABSDIFS_DC_DA_CONST9:
              return Absdifs::Lift(op1, op2, data, addr, len, il);
            case ADD_DC_DA_CONST9:
              return Add::Lift(op1, op2, data, addr, len, il);
            case ADDC_DC_DA_CONST9:
              return Addc::Lift(op1, op2, data, addr, len, il);
            case ADDS_DC_DA_CONST9:
              return Adds::Lift(op1, op2, data, addr, len, il);
            case ADDSU_DC_DA_CONST9:
              return Addsu::Lift(op1, op2, data, addr, len, il);
            case ADDX_DC_DA_CONST9:
              return Addx::Lift(op1, op2, data, addr, len, il);
            case ANDEQ_DC_DA_CONST9:
              return Andeq::Lift(op1, op2, data, addr, len, il);
            case ANDGE_DC_DA_CONST9:
            case ANDGEU_DC_DA_CONST9:
              return AndgeGeU::Lift(op1, op2, data, addr, len, il);
            case ANDLT_DC_DA_CONST9:
            case ANDLTU_DC_DA_CONST9:
              return AndltLtU::Lift(op1, op2, data, addr, len, il);
            case ANDNE_DC_DA_CONST9:
              return Andne::Lift(op1, op2, data, addr, len, il);
            case EQ_DC_DA_CONST9:
              return Eq::Lift(op1, op2, data, addr, len, il);
            case EQANYB_DC_DA_CONST9:
            case EQANYH_DC_DA_CONST9:
              return Eqanybh::Lift(op1, op2, data, addr, len, il);
            case GE_DC_DA_CONST9:
            case GEU_DC_DA_CONST9:
              return Ge::Lift(op1, op2, data, addr, len, il);
            case LT_DC_DA_CONST9:
            case LTU_DC_DA_CONST9:
              return Lt::Lift(op1, op2, data, addr, len, il);
            case MAX_DC_DA_CONST9:
            case MAXU_DC_DA_CONST9:
              return Max::Lift(op1, op2, data, addr, len, il);
            case MIN_DC_DA_CONST9:
            case MINU_DC_DA_CONST9:
              return Min::Lift(op1, op2, data, addr, len, il);
            case NE_DC_DA_CONST9:
              return Ne::Lift(op1, op2, data, addr, len, il);
            case OREQ_DC_DA_CONST9:
              return Oreq::Lift(op1, op2, data, addr, len, il);
            case ORGE_DC_DA_CONST9:
            case ORGEU_DC_DA_CONST9:
              return OrgeGeu::Lift(op1, op2, data, addr, len, il);
            case ORLT_DC_DA_CONST9:
            case ORLTU_DC_DA_CONST9:
              return OrltLtu::Lift(op1, op2, data, addr, len, il);
            case ORNE_DC_DA_CONST9:
              return Orne::Lift(op1, op2, data, addr, len, il);
            case RSUB_DC_DA_CONST9:
              return Rsub::Lift(op1, op2, data, addr, len, il);
            case RSUBS_DC_DA_CONST9:
            case RSUBSU_DC_DA_CONST9:
              return Rsubs::Lift(op1, op2, data, addr, len, il);
            case SHEQ_DC_DA_CONST9:
              return Sheq::Lift(op1, op2, data, addr, len, il);
            case SHGE_DC_DA_CONST9:
            case SHGEU_DC_DA_CONST9:
              return ShgeGeu::Lift(op1, op2, data, addr, len, il);
            case SHLT_DC_DA_CONST9:
            case SHLTU_DC_DA_CONST9:
              return ShltLtu::Lift(op1, op2, data, addr, len, il);
            case SHNE_DC_DA_CONST9:
              return Shne::Lift(op1, op2, data, addr, len, il);
            case XOREQ_DC_DA_CONST9:
              return Xoreq::Lift(op1, op2, data, addr, len, il);
            case XORGE_DC_DA_CONST9:
            case XORGEU_DC_DA_CONST9:
              return Xorge::Lift(op1, op2, data, addr, len, il);
            case XORLT_DC_DA_CONST9:
            case XORLTU_DC_DA_CONST9:
              return Xorlt::Lift(op1, op2, data, addr, len, il);
            case XORNE_DC_DA_CONST9:
              return Xorne::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x8F_RC:
          switch (op2) {
            case AND_DC_DA_CONST9:
              return And::Lift(op1, op2, data, addr, len, il);
            case ANDN_DC_DA_CONST9:
              return Andn::Lift(op1, op2, data, addr, len, il);
            case NAND_DC_DA_CONST9:
              return Nand::Lift(op1, op2, data, addr, len, il);
            case NOR_DC_DA_CONST9:
              return Nor::Lift(op1, op2, data, addr, len, il);
            case OR_DC_DA_CONST9:
              return Or::Lift(op1, op2, data, addr, len, il);
            case ORN_DC_DA_CONST9:
              return Orn::Lift(op1, op2, data, addr, len, il);
            case SH_DC_DA_CONST9:
              return Sh::Lift(op1, op2, data, addr, len, il);
            case SHH_DC_DA_CONST9:
              return Shh::Lift(op1, op2, data, addr, len, il);
            case SHA_DC_DA_CONST9:
              return Sha::Lift(op1, op2, data, addr, len, il);
            case SHAH_DC_DA_CONST9:
              return Shah::Lift(op1, op2, data, addr, len, il);
            case SHAS_DC_DA_CONST9:
              return Shas::Lift(op1, op2, data, addr, len, il);
            case XNOR_DC_DA_CONST9:
              return Xnor::Lift(op1, op2, data, addr, len, il);
            case XOR_DC_DA_CONST9:
              return Xor::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x93_RR1:
          switch (op2) {
            case MULQ_DC_DA_DB_N:
            case MULQ_EC_DA_DB_N:
            case MULQ_DC_DA_DBL_N:
            case MULQ_EC_DA_DBL_N:
            case MULQ_DC_DA_DBU_N:
            case MULQ_EC_DA_DBU_N:
            case MULQ_DC_DAL_DBL_N:
            case MULQ_DC_DAU_DBU_N:
              return Mulq::Lift(op1, op2, data, addr, len, il);
            case MULRQ_DC_DAL_DBL_N:
            case MULRQ_DC_DAU_DBU_N:
              return Mulrq::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x97_RCRR:
          if (op2 == INSERT_DC_DA_CONST4_ED) {
            return Insert::Lift(op1, op2, data, addr, len, il);
          }
        case OP1_x9F_BRC:
          switch (op2) {
            case JNED_DA_CONST4_DISP15:
              return Jned::Lift(op1, op2, data, addr, len, il, this);
            case JNEI_DA_CONST4_DISP15:
              return Jnei::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_xA3_RRR1:
          switch (op2) {
            case MSUBH_EC_ED_DA_DBLL_N:
            case MSUBH_EC_ED_DA_DBLU_N:
            case MSUBH_EC_ED_DA_DBUL_N:
            case MSUBH_EC_ED_DA_DBUU_N:
            case MSUBSH_EC_ED_DA_DBLL_N:
            case MSUBSH_EC_ED_DA_DBLU_N:
            case MSUBSH_EC_ED_DA_DBUL_N:
            case MSUBSH_EC_ED_DA_DBUU_N:
              return Msubh::Lift(op1, op2, data, addr, len, il);
            case MSUBMH_EC_ED_DA_DBLL_N:
            case MSUBMH_EC_ED_DA_DBLU_N:
            case MSUBMH_EC_ED_DA_DBUL_N:
            case MSUBMH_EC_ED_DA_DBUU_N:
            case MSUBMSH_EC_ED_DA_DBLL_N:
            case MSUBMSH_EC_ED_DA_DBLU_N:
            case MSUBMSH_EC_ED_DA_DBUL_N:
            case MSUBMSH_EC_ED_DA_DBUU_N:
              return MsubmhSh::Lift(op1, op2, data, addr, len, il);
            case MSUBRH_DC_DD_DA_DBLL_N:
            case MSUBRH_DC_DD_DA_DBLU_N:
            case MSUBRH_DC_DD_DA_DBUL_N:
            case MSUBRH_DC_DD_DA_DBUU_N:
            case MSUBRSH_DC_DD_DA_DBLL_N:
            case MSUBRSH_DC_DD_DA_DBLU_N:
            case MSUBRSH_DC_DD_DA_DBUL_N:
            case MSUBRSH_DC_DD_DA_DBUU_N:
              return MsubrhSh::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xA5_ABS:
          switch (op2) {
            case STA_OFF18_AA:
              return Sta::Lift(op1, op2, data, addr, len, il);
            case STD_OFF18_EA:
              return Std::Lift(op1, op2, data, addr, len, il);
            case STDA_OFF18_PA:
              return Stda::Lift(op1, op2, data, addr, len, il);
            case STW_OFF18_DA:
              return Stw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xA7_BIT:
          switch (op2) {
            case SHNANDT_DC_DA_POS1_DB_POS2:
            case SHORNT_DC_DA_POS1_DB_POS2:
            case SHXNORT_DC_DA_POS1_DB_POS2:
            case SHXORT_DC_DA_POS1_DB_POS2:
              return ShAndOrNorXorT::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xA9_BO:
          switch (op2) {
            case CACHEAI_PB:
            case CACHEAI_PB_OFF10:
              return Cacheawi::Lift(op1, op2, data, addr, len, il);
            case CACHEAW_PB:
            case CACHEAW_PB_OFF10:
              return Cacheawi::Lift(op1, op2, data, addr, len, il);
            case CACHEAWI_PB:
            case CACHEAWI_PB_OFF10:
              return Cacheawi::Lift(op1, op2, data, addr, len, il);
            case STA_PB_AA:
            case STA_PB_OFF10_AA:
              return Sta::Lift(op1, op2, data, addr, len, il);
            case STB_PB_DA:
            case STB_PB_OFF10_DA:
              return Stb::Lift(op1, op2, data, addr, len, il);
            case STD_PB_EA:
            case STD_PB_OFF10_EA:
              return Std::Lift(op1, op2, data, addr, len, il);
            case STDA_PB_PA:
            case STDA_PB_OFF10_PA:
              return Stda::Lift(op1, op2, data, addr, len, il);
            case STH_PB_DA:
            case STH_PB_OFF10_DA:
              return Sth::Lift(op1, op2, data, addr, len, il);
            case STQ_PB_DA:
            case STQ_PB_OFF10_DA:
              return Stq::Lift(op1, op2, data, addr, len, il);
            case STW_PB_DA:
            case STW_PB_OFF10_DA:
              return Stw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xAB_RCR:
          switch (op2) {
            case CADD_DC_DD_DA_CONST9:
              return Cadd::Lift(op1, op2, data, addr, len, il);
            case CADDN_DC_DD_DA_CONST9:
              return Caddn::Lift(op1, op2, data, addr, len, il);
            case SEL_DC_DD_DA_CONST9:
              return Sel::Lift(op1, op2, data, addr, len, il);
            case SELN_DC_DD_DA_CONST9:
              return Seln::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xAD_RC:
          switch (op2) {
            case BISR_CONST9:
              return Bisr::Lift(op1, op2, data, addr, len, il);
            case SYSCALL_CONST9:
              return Syscall::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xB3_RR1:
          switch (op2) {
            case MULH_EC_DA_DBLL_N:
            case MULH_EC_DA_DBLU_N:
            case MULH_EC_DA_DBUL_N:
            case MULH_EC_DA_DBUU_N:
              return Mulh::Lift(op1, op2, data, addr, len, il);
            case MULMH_EC_DA_DBLL_N:
            case MULMH_EC_DA_DBLU_N:
            case MULMH_EC_DA_DBUL_N:
            case MULMH_EC_DA_DBUU_N:
              return Mulmh::Lift(op1, op2, data, addr, len, il);
            case MULRH_DC_DA_DBLL_N:
            case MULRH_DC_DA_DBLU_N:
            case MULRH_DC_DA_DBUL_N:
            case MULRH_DC_DA_DBUU_N:
              return Mulrh::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xB7_RCPW:
          switch (op2) {
            case IMASK_EC_CONST4_POS_WIDTH:
              return Imask::Lift(op1, op2, data, addr, len, il);
            case INSERT_DC_DA_CONST4_POS_WIDTH:
              return Insert::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xBD_BRR:
          switch (op2) {
            case JNZA_AA_DISP15:
              return Jnza::Lift(op1, op2, data, addr, len, il, this);
            case JZA_AA_DISP15:
              return Jza::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_xBF_BRC:
          switch (op2) {
            case JLT_DA_CONST4_DISP15:
            case JLTU_DA_CONST4_DISP15:
              return Jlt::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_xC3_RRR1:
          switch (op2) {
            case MADDSUH_EC_ED_DA_DBLL_N:
            case MADDSUH_EC_ED_DA_DBLU_N:
            case MADDSUH_EC_ED_DA_DBUL_N:
            case MADDSUH_EC_ED_DA_DBUU_N:
            case MADDSUSH_EC_ED_DA_DBLL_N:
            case MADDSUSH_EC_ED_DA_DBLU_N:
            case MADDSUSH_EC_ED_DA_DBUL_N:
            case MADDSUSH_EC_ED_DA_DBUU_N:
              return MaddsuhSh::Lift(op1, op2, data, addr, len, il);
            case MADDSUMH_EC_ED_DA_DBLL_N:
            case MADDSUMH_EC_ED_DA_DBLU_N:
            case MADDSUMH_EC_ED_DA_DBUL_N:
            case MADDSUMH_EC_ED_DA_DBUU_N:
            case MADDSUMSH_EC_ED_DA_DBLL_N:
            case MADDSUMSH_EC_ED_DA_DBLU_N:
            case MADDSUMSH_EC_ED_DA_DBUL_N:
            case MADDSUMSH_EC_ED_DA_DBUU_N:
              return MaddsumhSh::Lift(op1, op2, data, addr, len, il);
            case MADDSURH_DC_DD_DA_DBLL_N:
            case MADDSURH_DC_DD_DA_DBLU_N:
            case MADDSURH_DC_DD_DA_DBUL_N:
            case MADDSURH_DC_DD_DA_DBUU_N:
            case MADDSURSH_DC_DD_DA_DBLL_N:
            case MADDSURSH_DC_DD_DA_DBLU_N:
            case MADDSURSH_DC_DD_DA_DBUL_N:
            case MADDSURSH_DC_DD_DA_DBUU_N:
              return MaddsurhSh::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xC5_ABS:
          if (op2 == LEA_AA_OFF18) {
            return Lea::Lift(op1, op2, data, addr, len, il);
          }
          return false;
        case OP1_xC7_BIT:
          switch (op2) {
            case ORANDT_DC_DA_POS1_DB_POS2:
            case ORANDNT_DC_DA_POS1_DB_POS2:
            case ORNORT_DC_DA_POS1_DB_POS2:
            case ORORT_DC_DA_POS1_DB_POS2:
              return OrAndtAndntNortOrt::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xD5_ABSB:
          if (op2 == STT_OFF18_BPOS3_B) {
            return Stt::Lift(op1, op2, data, addr, len, il);
          }
          return false;
        case OP1_xD7_RCRW:
          switch (op2) {
            case IMASK_EC_CONST4_POS_WIDTH:
              return Imask::Lift(op1, op2, data, addr, len, il);
            case INSERT_DC_DA_CONST4_DD_WIDTH:
              return Insert::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xDF_BRC:
          switch (op2) {
            case JEQ_DA_CONST4_DISP15:
              return Jeq::Lift(op1, op2, data, addr, len, il, this);
            case JNE_DA_CONST4_DISP15:
              return Jne::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        case OP1_xE3_RRR1:
          switch (op2) {
            case MSUBADH_EC_ED_DA_DBLL_N:
            case MSUBADH_EC_ED_DA_DBLU_N:
            case MSUBADH_EC_ED_DA_DBUL_N:
            case MSUBADH_EC_ED_DA_DBUU_N:
            case MSUBADSH_EC_ED_DA_DBLL_N:
            case MSUBADSH_EC_ED_DA_DBLU_N:
            case MSUBADSH_EC_ED_DA_DBUL_N:
            case MSUBADSH_EC_ED_DA_DBUU_N:
              return MsubadhSh::Lift(op1, op2, data, addr, len, il);
            case MSUBADMH_EC_ED_DA_DBLL_N:
            case MSUBADMH_EC_ED_DA_DBLU_N:
            case MSUBADMH_EC_ED_DA_DBUL_N:
            case MSUBADMH_EC_ED_DA_DBUU_N:
            case MSUBADMSH_EC_ED_DA_DBLL_N:
            case MSUBADMSH_EC_ED_DA_DBLU_N:
            case MSUBADMSH_EC_ED_DA_DBUL_N:
            case MSUBADMSH_EC_ED_DA_DBUU_N:
              return MsubadmhSh::Lift(op1, op2, data, addr, len, il);
            case MSUBADRH_DC_DD_DA_DBLL_N:
            case MSUBADRH_DC_DD_DA_DBLU_N:
            case MSUBADRH_DC_DD_DA_DBUL_N:
            case MSUBADRH_DC_DD_DA_DBUU_N:
            case MSUBADRSH_DC_DD_DA_DBLL_N:
            case MSUBADRSH_DC_DD_DA_DBLU_N:
            case MSUBADRSH_DC_DD_DA_DBUL_N:
            case MSUBADRSH_DC_DD_DA_DBUU_N:
              return MsubadrhSh::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xE5_ABS:
          switch (op2) {
            case LDMST_OFF18_EA:
              return Ldmst::Lift(op1, op2, data, addr, len, il);
            case SWAPW_OFF18_DA:
              return Swapw::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xFD_BRR:
          switch (op2) {
            case LOOP_AB_DISP15:
              return Loop::Lift(op1, op2, data, addr, len, il);
            case LOOPU_DISP15:
              return Loopu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xFF_BRC:
          switch (op2) {
            case JGE_DA_CONST4_DISP15:
            case JGEU_DA_CONST4_DISP15:
              return Jge::Lift(op1, op2, data, addr, len, il, this);
            default:
              return false;
          }
        default:
          return false;
      }
    }
    // 16-bit instruction
    else {
      // The ADDSCA 16-bit instruction is weird and has a smaller primary opcode
      // (SRRS format)
      if ((raw & 0x3Fu) == ADDSCA_AA_AB_D15_N) {
        return Addscaat::Lift(raw & 0x3Fu, 0xFF, data, addr, len, il);
      }
      uint8_t op1 = raw & 0xFFu;
      // This will get op2 if it exists
      uint32_t op2 = Instruction::ExtractSecondaryOpcode(op1, data);
      op2 &= 0xFFu;  // secondary opcodes are not > 0xFF, even though some
                     // formats have up to 12 bits
      switch (op1) {
        // 2-byte non-branching, single opcode
        case ADD_DA_CONST4:
        case ADD_DA_D15_CONST4:
        case ADD_D15_DA_CONST4:
        case ADD_DA_DB:
        case ADD_DA_D15_DB:
        case ADD_D15_DA_DB:
          return Add::Lift(op1, op2, data, addr, len, il);
        case ADDA_AA_CONST4:
        case ADDA_AA_AB:
          return Adda::Lift(op1, op2, data, addr, len, il);
        case ADDS_DA_DB:
          return Adds::Lift(op1, op2, data, addr, len, il);
        case AND_D15_CONST8:
        case AND_DA_DB:
          return And::Lift(op1, op2, data, addr, len, il);
        case BISR_CONST8:
          return Bisr::Lift(op1, op2, data, addr, len, il);
        case CADD_DA_D15_CONST4:
          return Cadd::Lift(op1, op2, data, addr, len, il);
        case CADDN_DA_D15_CONST4:
          return Caddn::Lift(op1, op2, data, addr, len, il);
        case CMOV_DA_D15_CONST4:
        case CMOV_DA_D15_DB:
          return Cmov::Lift(op1, op2, data, addr, len, il);
        case CMOVN_DA_D15_CONST4:
        case CMOVN_DA_D15_DB:
          return Cmovn::Lift(op1, op2, data, addr, len, il);
        case EQ_D15_DA_CONST4:
        case EQ_D15_DA_DB:
          return Eq::Lift(op1, op2, data, addr, len, il);
        case LDA_A15_A10_CONST8:
        case LDA_AC_AB:
        case LDA_AC_AB_POSTINC:
        case LDA_AC_A15_OFF4:
        case LDA_A15_AB_OFF4:
          return Lda::Lift(op1, op2, data, addr, len, il);
        case LDBU_DC_AB:
        case LDBU_DC_AB_POSTINC:
        case LDBU_DC_A15_OFF4:
        case LDBU_D15_AB_OFF4:
          return Ldb::Lift(op1, op2, data, addr, len, il);
        case LDH_DC_AB:
        case LDH_DC_AB_POSTINC:
        case LDH_DC_A15_OFF4:
        case LDH_D15_AB_OFF4:
          return Ldh::Lift(op1, op2, data, addr, len, il);
        case LDW_D15_A10_CONST8:
        case LDW_DC_AB:
        case LDW_DC_AB_POSTINC:
        case LDW_DC_A15_OFF4:
        case LDW_D15_AB_OFF4:
          return Ldw::Lift(op1, op2, data, addr, len, il);
        case LT_D15_DA_CONST4:
        case LT_D15_DA_DB:
          return Lt::Lift(op1, op2, data, addr, len, il);
        case MOV_D15_CONST8:
        case MOV_DA_CONST4:
        case MOV_EA_CONST4:  // TCv1.6 ISA only
        case MOV_DA_DB:
          return Mov::Lift(op1, op2, data, addr, len, il);
        case MOVA_AA_CONST4:
        case MOVA_AA_DB:
          return Mova::Lift(op1, op2, data, addr, len, il);
        case MOVAA_AA_AB:
          return Movaa::Lift(op1, op2, data, addr, len, il);
        case MOVD_DA_AB:
          return Movd::Lift(op1, op2, data, addr, len, il);
        case MUL_DA_DB:
          return Mul::Lift(op1, op2, data, addr, len, il);
        case OR_D15_CONST8:
        case OR_DA_DB:
          return Or::Lift(op1, op2, data, addr, len, il);
        case SH_DA_CONST4:
          return Sh::Lift(op1, op2, data, addr, len, il);
        case SHA_DA_CONST4:
          return Sha::Lift(op1, op2, data, addr, len, il);
        case STA_A10_CONST8_A15:
        case STA_AB_OFF4_A15:
        case STA_AB_AA:
        case STA_AB_AA_POSTINC:
        case STA_A15_OFF4_AA:
          return Sta::Lift(op1, op2, data, addr, len, il);
        case STB_AB_OFF4_D15:
        case STB_AB_DA:
        case STB_AB_DA_POSTINC:
        case STB_A15_OFF4_DA:
          return Stb::Lift(op1, op2, data, addr, len, il);
        case STH_AB_OFF4_D15:
        case STH_AB_DA:
        case STH_AB_DA_POSTINC:
        case STH_A15_OFF4_DA:
          return Sth::Lift(op1, op2, data, addr, len, il);
        case STW_A10_CONST8_D15:
        case STW_AB_OFF4_D15:
        case STW_AB_DA:
        case STW_AB_DA_POSTINC:
        case STW_A15_OFF4_DA:
          return Stw::Lift(op1, op2, data, addr, len, il);
        case SUB_DA_DB:
        case SUB_DA_D15_DB:
        case SUB_D15_DA_DB:
          return Sub::Lift(op1, op2, data, addr, len, il);
        case SUBA_A10_CONST8:
          return Suba::Lift(op1, op2, data, addr, len, il);
        case SUBS_DA_DB:
          return Subsu::Lift(op1, op2, data, addr, len, il);
        case XOR_DA_DB:
          return Xor::Lift(op1, op2, data, addr, len, il);

        // 2-byte non-branching, dual opcode
        case OP1_x00_SR:
          switch (op2) {
            case DEBUG16:
              return Debug::Lift(op1, op2, data, addr, len, il);
            case NOP16:
              return Nop::Lift(op1, op2, data, addr, len, il);
            case RET16:
              return Ret::Lift(op1, op2, data, addr, len, il);
            case FRET16:  // TCv1.6 ISA only
              return Fret::Lift(op1, op2, data, addr, len, il);
            case RFE16:
              return Rfe::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x46_SR:
          switch (op2) {
            case NOT_DA:
              return Not::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_x32_SR:
          switch (op2) {
            case RSUB_DA:
              return Rsub::Lift(op1, op2, data, addr, len, il);
            case SATB_DA:
              return Satb::Lift(op1, op2, data, addr, len, il);
            case SATBU_DA:
              return Satbu::Lift(op1, op2, data, addr, len, il);
            case SATH_DA:
              return Sath::Lift(op1, op2, data, addr, len, il);
            case SATHU_DA:
              return Sathu::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }
        case OP1_xDC_SR:
          switch (op2) {
            case JI_AA_16:
              return Ji::Lift(op1, op2, data, addr, len, il);
            default:
              return false;
          }

        // 2-byte branching
        case CALL_DISP8:
          return Call::Lift(op1, op2, data, addr, len, il);
        case J_DISP8:
          return J::Lift(op1, op2, data, addr, len, il);
        case JEQ_D15_CONST4_DISP4:
        case JEQ_D15_DB_DISP4:
        case JEQ_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
        case JEQ_D15_DB_DISP4_EX:      // TCv1.6 ISA only
          return Jeq::Lift(op1, op2, data, addr, len, il, this);
        case JGEZ_DB_DISP4:
          return Jgez::Lift(op1, op2, data, addr, len, il, this);
        case JGTZ_DB_DISP4:
          return Jgtz::Lift(op1, op2, data, addr, len, il, this);
        case JLEZ_DB_DISP4:
          return Jlez::Lift(op1, op2, data, addr, len, il, this);
        case JLTZ_DB_DISP4:
          return Jltz::Lift(op1, op2, data, addr, len, il, this);
        case JNE_D15_CONST4_DISP4:
        case JNE_D15_DB_DISP4:
        case JNE_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
        case JNE_D15_DB_DISP4_EX:      // TCv1.6 ISA only
          return Jne::Lift(op1, op2, data, addr, len, il, this);
        case JNZ_D15_DISP8:
        case JNZ_DB_DISP4:
          return Jnz::Lift(op1, op2, data, addr, len, il, this);
        case JNZA_AB_DISP4:
          return Jnza::Lift(op1, op2, data, addr, len, il, this);
        case JNZT_D15_N_DISP4:
          return Jnzt::Lift(op1, op2, data, addr, len, il, this);
        case JZ_D15_DISP8:
        case JZ_DB_DISP4:
          return Jz::Lift(op1, op2, data, addr, len, il, this);
        case JZA_AB_DISP4:
          return Jza::Lift(op1, op2, data, addr, len, il, this);
        case JZT_D15_N_DISP4:
          return Jzt::Lift(op1, op2, data, addr, len, il, this);
        case LOOP_AB_DISP4:
          return Loop::Lift(op1, op2, data, addr, len, il);
        default:
          return false;
      }
    }
  }
};

class TCArch : public TricoreArchitecture {
 public:
  explicit TCArch(const std::string& name) : TricoreArchitecture(name) {}

  uint32_t GetStackPointerRegister() override { return TRICORE_REG_A10; }
};

class TCArchCallingConvention : public CallingConvention {
 public:
  explicit TCArchCallingConvention(Architecture* arch)
      : CallingConvention(arch, "tricore-cc") {}

  std::vector<uint32_t> GetIntegerArgumentRegisters() override {
    return std::vector<uint32_t>{TRICORE_REG_D0, TRICORE_REG_D1, TRICORE_REG_D2,
                                 TRICORE_REG_D3};
  }

  std::vector<uint32_t> GetCalleeSavedRegisters() override {
    return std::vector<uint32_t>{
        TRICORE_REG_D8,  TRICORE_REG_D9,  TRICORE_REG_D10, TRICORE_REG_D11,
        TRICORE_REG_A12, TRICORE_REG_A13, TRICORE_REG_A14, TRICORE_REG_A15,
        TRICORE_REG_D12, TRICORE_REG_D13, TRICORE_REG_D14, TRICORE_REG_D15,
        TRICORE_REG_A0,  TRICORE_REG_A1,  TRICORE_REG_A8,  TRICORE_REG_A9};
  }

  std::vector<uint32_t> GetCallerSavedRegisters() override {
    return std::vector<uint32_t>{
        TRICORE_REG_A11, TRICORE_REG_A2, TRICORE_REG_A3, TRICORE_REG_D0,
        TRICORE_REG_D1,  TRICORE_REG_D2, TRICORE_REG_D3, TRICORE_REG_A4,
        TRICORE_REG_A5,  TRICORE_REG_A6, TRICORE_REG_A7, TRICORE_REG_D4,
        TRICORE_REG_D5,  TRICORE_REG_D6, TRICORE_REG_D7};
  }

  uint32_t GetIntegerReturnValueRegister() override {
    return TRICORE_REG_D2;  // What about a2?
  }
};

extern "C" {
BN_DECLARE_CORE_ABI_VERSION
BINARYNINJAPLUGIN bool CorePluginInit() {
  const auto tcarch = new TCArch("tricore");
  const auto cc = new TCArchCallingConvention(tcarch);

  Architecture::Register(tcarch);
  tcarch->RegisterCallingConvention(cc);
  tcarch->SetDefaultCallingConvention(cc);

  return true;
}
}

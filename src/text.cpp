// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>

#include <cstdint>

#include "instructions.h"
#include "opcodes.h"
#include "util.h"

#define ITEXT(m)                            \
  result.emplace_back(InstructionToken, m); \
  result.emplace_back(TextToken, " ");

using namespace std;
using namespace BinaryNinja;

namespace Tricore {
bool Abs::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR || op2 != ABS_DC_DB) {
    LogDebug("0x%lx: Abs::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDb_RR(data, len, WORD, result, "abs");
}

bool Absbh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Absbh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case ABSB_DC_DB:
      len = 4;
      return Instruction::TextOpDcDb_RR(data, len, BYTE, result, "abs.b");
    case ABSH_DC_DB:
      len = 4;
      return Instruction::TextOpDcDb_RR(data, len, HWORD, result, "abs.h");
    default:
      LogDebug("0x%lx: Absbh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Absdif::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ABSDIF_DC_DA_CONST9) {
        LogDebug("0x%lx: Absdif::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                              "absdif");
    case OP1_x0B_RR:
      if (op2 != ABSDIF_DC_DA_DB) {
        LogDebug("0x%lx: Absdif::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "absdif");
    default:
      LogDebug("0x%lx: Absdif::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Absdifbh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Absdifbh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case ABSDIFB_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "absdif.b");
    case ABSDIFH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "absdif.h");
    default:
      LogDebug("0x%lx: Absdifbh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Absdifs::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ABSDIFS_DC_DA_CONST9) {
        LogDebug("0x%lx: Absdifs::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                              "absdifs");
    case OP1_x0B_RR:
      if (op2 != ABSDIFS_DC_DA_DB) {
        LogDebug("0x%lx: Absdifs::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "absdifs");
    default:
      LogDebug("0x%lx: Absdifs::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Absdifsh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR || op2 != ABSDIFSH_DC_DA_DB) {
    LogDebug("0x%lx: Absdifsh::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "absdifs.h");
}

bool Abss::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR || op2 != ABSS_DC_DB) {
    LogDebug("0x%lx: Abss::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDb_RR(data, len, WORD, result, "abss");
}

bool Abssh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR || op2 != ABSSH_DC_DB) {
    LogDebug("0x%lx: Abssh::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDb_RR(data, len, HWORD, result, "abssh");
}

bool Add::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    // 16-bit instructions
    case ADD_DA_CONST4:
      len = 2;
      return Instruction::TextOpDaConst4_SRC(data, len, WORD, result, "add");
    case ADD_DA_D15_CONST4:
      len = 2;
      return Instruction::TextOpDaD15Const4_SRC(data, len, WORD, result, "add");
    case ADD_D15_DA_CONST4:
      len = 2;
      return Instruction::TextOpD15DaConst4_SRC(data, len, WORD, result, "add");
    case ADD_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "add");
    case ADD_DA_D15_DB:
      len = 2;
      return Instruction::TextOpDaD15Db_SRR(data, len, WORD, result, "add");
    case ADD_D15_DA_DB:
      len = 2;
      return Instruction::TextOpD15DaDb_SRR(data, len, WORD, result, "add");
    // 32-bit instructions
    case OP1_x8B_RC:
      if (op2 != ADD_DC_DA_CONST9) {
        LogDebug("0x%lx: Add::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "add");
    case OP1_x0B_RR:
      if (op2 != ADD_DC_DA_DB) {
        LogDebug("0x%lx: Add::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "add");
    default:
      LogDebug("0x%lx: Add::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Adda::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    // 16-bit instructions
    case ADDA_AA_CONST4:
      len = 2;
      return Instruction::TextOpAaConst4_SRC(data, len, WORD, result, "add.a");
    case ADDA_AA_AB:
      len = 2;
      return Instruction::TextOpAaAb_SRR(data, len, WORD, result, "add.a");
    // 32-bit instructions
    case OP1_x01_RR:
      if (op2 != ADDA_AC_AA_AB) {
        LogDebug("0x%lx: Adda::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpAcAaAb_RR(data, len, WORD, result, "add.a");
    default:
      LogDebug("0x%lx: Adda::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Addbh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Addbh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case ADDB_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "add.b");
    case ADDH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "add.h");
    default:
      LogDebug("0x%lx: Addbh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Addc::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ADDC_DC_DA_CONST9) {
        LogDebug("0x%lx: Addc::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "addc");
    case OP1_x0B_RR:
      if (op2 != ADDC_DC_DA_DB) {
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "addc");
    default:
      LogDebug("0x%lx: Addc::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Addi::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  // 32-bit, single opcode
  if (op1 != ADDI_DC_DA_CONST16) {
    LogDebug("0x%lx: Addi::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaConst16_RLC(data, len, WORD, result, "addi");
}

bool Addih::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  // 32-bit, single opcode
  if (op1 != ADDIH_DC_DA_CONST16) {
    LogDebug("0x%lx: Addih::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaConst16_RLC(data, len, HWORD, result, "addih");
}

bool Addiha::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  // 32-bit, single opcode
  if (op1 != ADDIHA_AC_AA_CONST16) {
    LogDebug("0x%lx: Addiha::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpAcAaConst16_RLC(data, len, HWORD, result,
                                            "addih.a");
}

bool Adds::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case ADDS_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "adds");
    case OP1_x8B_RC:
      if (op2 != ADDS_DC_DA_CONST9) {
        LogDebug("0x%lx: Adds::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "adds");
    case OP1_x0B_RR:
      if (op2 != ADDS_DC_DA_DB) {
        LogDebug("0x%lx: Adds::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "adds");
    default:
      LogDebug("0x%lx: Adds::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Addshhu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Addshhu::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case ADDSH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "adds.h");
    case ADDSHU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "adds.hu");
    default:
      LogDebug("0x%lx: Addshhu::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Addsu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ADDSU_DC_DA_CONST9) {
        LogDebug("0x%lx: Addsu::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "adds.u",
                                              true);
    case OP1_x0B_RR:
      if (op2 != ADDSU_DC_DA_DB) {
        LogDebug("0x%lx: Addsu::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "adds.u");
    default:
      LogDebug("0x%lx: Addsu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Addscaat::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case ADDSCA_AA_AB_D15_N:
      len = 2;
      return Instruction::TextOpAaAbD15N_SRRS(data, len, WORD, result,
                                              "addsc.a");
    case OP1_x01_RR:
      switch (op2) {
        case ADDSCA_AC_AB_DA_N:
          len = 4;
          return Instruction::TextOpAcAbDaN_RR(data, len, WORD, result,
                                               "addsc.a");
        case ADDSCAT_AC_AB_DA:
          len = 4;
          return Instruction::TextOpAcAbDa_RR(data, len, WORD, result,
                                              "addsc.at");
        default:
          LogDebug("0x%lx: Addscaat::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Addscaat::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Addx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ADDX_DC_DA_CONST9) {
        LogDebug("0x%lx: Addx::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "addx");
    case OP1_x0B_RR:
      if (op2 != ADDX_DC_DA_DB) {
        LogDebug("0x%lx: Addx::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "addx");
    default:
      LogDebug("0x%lx: Addx::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool And::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case AND_D15_CONST8:
      len = 2;
      return Instruction::TextOpD15Const8_SC(data, len, WORD, result, "and");
    case AND_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "and");
    case OP1_x8F_RC:
      if (op2 != AND_DC_DA_CONST9) {
        LogDebug("0x%lx: And::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "and",
                                              true);
    case OP1_x0F_RR:
      if (op2 != AND_DC_DA_DB) {
        LogDebug("0x%lx: And::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "and");
    default:
      LogDebug("0x%lx: And::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool AndAndnNorOrT::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                         uint64_t addr, size_t& len,
                         std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x47_BIT) {
    LogDebug("0x%lx: AndAndnNorOrT::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case ANDANDT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "and.and.t");
    case ANDANDNT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "and.andn.t");
    case ANDNORT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "and.nor.t");
    case ANDORT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "and.or.t");
    default:
      LogDebug("0x%lx: AndAndnNorOrT::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Andeq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ANDEQ_DC_DA_CONST9) {
        LogDebug("0x%lx: Andeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                              "and.eq");
    case OP1_x0B_RR:
      if (op2 != ANDEQ_DC_DA_DB) {
        LogDebug("0x%lx: Andeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "and.eq");
    default:
      LogDebug("0x%lx: Andeq::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool AndgeGeU::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ANDGE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "and.ge");
        case ANDGEU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "and.ge.u", true);
        default:
          LogDebug("0x%lx: AndgeGeU::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ANDGE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "and.ge");
        case ANDGEU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "and.ge.u");
        default:
          LogDebug("0x%lx: AndgeGeU::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: AndgeGeU::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool AndltLtU::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ANDLT_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "and.lt");
        case ANDLTU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "and.lt.u", true);
        default:
          LogDebug("0x%lx: AndltLtU::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ANDLT_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "and.lt");
        case ANDLTU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "and.lt.u");
        default:
          LogDebug("0x%lx: AndltLtU::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: AndltLtU::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Andne::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ANDNE_DC_DA_CONST9) {
        LogDebug("0x%lx: Andne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                              "and.ne");
    case OP1_x0B_RR:
      if (op2 != ANDNE_DC_DA_DB) {
        LogDebug("0x%lx: Andne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "and.ne");
    default:
      LogDebug("0x%lx: Andne::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Andt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x87_BIT || op2 != ANDT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Andt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                               "and.t");
}

bool Andn::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      if (op2 != ANDN_DC_DA_CONST9) {
        LogDebug("0x%lx: Andn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "andn",
                                              true);
    case OP1_x0F_RR:
      if (op2 != ANDN_DC_DA_DB) {
        LogDebug("0x%lx: Andn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "andn");
    default:
      LogDebug("0x%lx: Andn::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Andnt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x87_BIT || op2 != ANDNT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Andnt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                               "andn.t");
}

bool Bisr::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case BISR_CONST8:
      len = 2;
      return Instruction::TextOpConst8_SC(data, len, WORD, result, "bisr");
    case OP1_xAD_RC:
      if (op2 != BISR_CONST9) {
        LogDebug("0x%lx: Bisr::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpConst9_RC(data, len, WORD, result, "bisr");
    default:
      LogDebug("0x%lx: Bisr::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Bmerge::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != BMERGE_DC_DA_DB) {
    LogDebug("0x%lx: Bmerge::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "bmerge");
}

bool Bsplit::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != BSPLIT_EC_DA) {
    LogDebug("0x%lx: Bsplit::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpEcDa_RR(data, len, WORD, result, "bsplit");
}

bool Cacheai::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x89_BO:
      switch (op2) {
        case CACHEAI_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.i", ADDRMODE_REGULAR);
        case CACHEAI_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.i", ADDRMODE_POSTINC);
        case CACHEAI_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.i", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Cacheai::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case CACHEAI_PB:
          len = 4;
          return Instruction::TextOpPb_BO(data, len, WORD, result, "cachea.i",
                                          ADDRMODE_BREV);
        case CACHEAI_PB_OFF10:
          len = 4;
          return Instruction::TextOpPbOff10_BO(data, len, WORD, result,
                                               "cachea.i", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Cacheai::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Cacheai::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Cacheaw::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x89_BO:
      switch (op2) {
        case CACHEAW_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.w", ADDRMODE_REGULAR);
        case CACHEAW_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.w", ADDRMODE_POSTINC);
        case CACHEAW_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.w", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Cacheaw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case CACHEAW_PB:
          len = 4;
          return Instruction::TextOpPb_BO(data, len, WORD, result, "cachea.w",
                                          ADDRMODE_BREV);
        case CACHEAW_PB_OFF10:
          len = 4;
          return Instruction::TextOpPbOff10_BO(data, len, WORD, result,
                                               "cachea.w", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Cacheaw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Cacheaw::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Cacheawi::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x89_BO:
      switch (op2) {
        case CACHEAWI_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.wi", ADDRMODE_REGULAR);
        case CACHEAWI_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.wi", ADDRMODE_POSTINC);
        case CACHEAWI_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachea.wi", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Cacheawi::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case CACHEAWI_PB:
          len = 4;
          return Instruction::TextOpPb_BO(data, len, WORD, result, "cachea.wi",
                                          ADDRMODE_BREV);
        case CACHEAWI_PB_OFF10:
          len = 4;
          return Instruction::TextOpPbOff10_BO(data, len, WORD, result,
                                               "cachea.wi", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Cacheawi::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Cacheawi::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

// TCv1.6 ISA only
bool Cacheii::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x89_BO:
      switch (op2) {
        case CACHEII_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachei.i", ADDRMODE_REGULAR);
        case CACHEII_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachei.i", ADDRMODE_POSTINC);
        case CACHEII_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "cachei.i", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Cacheii::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Cacheii::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Cacheiw::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x89_BO) {
    LogDebug("0x%lx: Cacheiw::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case CACHEIW_AB_OFF10:
      len = 4;
      return Instruction::TextOpAbOff10_BO(data, len, WORD, result, "cachei.w",
                                           ADDRMODE_REGULAR);
    case CACHEIW_AB_OFF10_POSTINC:
      len = 4;
      return Instruction::TextOpAbOff10_BO(data, len, WORD, result, "cachei.w",
                                           ADDRMODE_POSTINC);
    case CACHEIW_AB_OFF10_PREINC:
      len = 4;
      return Instruction::TextOpAbOff10_BO(data, len, WORD, result, "cachei.w",
                                           ADDRMODE_PREINC);
    default:
      LogDebug("0x%lx: Cacheiw::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Cacheiwi::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x89_BO) {
    LogDebug("0x%lx: Cacheiwi::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case CACHEIWI_AB_OFF10:
      len = 4;
      return Instruction::TextOpAbOff10_BO(data, len, WORD, result, "cachei.wi",
                                           ADDRMODE_REGULAR);
    case CACHEIWI_AB_OFF10_POSTINC:
      len = 4;
      return Instruction::TextOpAbOff10_BO(data, len, WORD, result, "cachei.wi",
                                           ADDRMODE_POSTINC);
    case CACHEIWI_AB_OFF10_PREINC:
      len = 4;
      return Instruction::TextOpAbOff10_BO(data, len, WORD, result, "cachei.wi",
                                           ADDRMODE_PREINC);
    default:
      LogDebug("0x%lx: Cacheiwi::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Cadd::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case CADD_DA_D15_CONST4:
      len = 2;
      return Instruction::TextOpDaD15Const4_SRC(data, len, WORD, result,
                                                "cadd");
    case OP1_xAB_RCR:
      if (op2 != CADD_DC_DD_DA_CONST9) {
        LogDebug("0x%lx: Cadd::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                 "cadd");
    case OP1_x2B_RRR:
      if (op2 != CADD_DC_DD_DA_DB) {
        LogDebug("0x%lx: Cadd::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result, "cadd");
    default:
      LogDebug("0x%lx: Cadd::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Caddn::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case CADDN_DA_D15_CONST4:
      len = 2;
      return Instruction::TextOpDaD15Const4_SRC(data, len, WORD, result,
                                                "caddn");
    case OP1_xAB_RCR:
      if (op2 != CADDN_DC_DD_DA_CONST9) {
        LogDebug("0x%lx: Caddn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                 "caddn");
    case OP1_x2B_RRR:
      if (op2 != CADDN_DC_DD_DA_DB) {
        LogDebug("0x%lx: Caddn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result, "caddn");
    default:
      LogDebug("0x%lx: Caddn::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Call::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case CALL_DISP8:
      len = 2;
      return Instruction::TextOpDisp8_SB(data, addr, len, WORD, result, "call");
    case CALL_DISP24:
      len = 4;
      return Instruction::TextOpDisp24_B(data, addr, len, WORD, result, "call");
    default:
      LogDebug("0x%lx: Call::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Calla::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != CALLA_DISP24) {
    LogDebug("0x%lx: Calla::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDisp24_B(data, addr, len, WORD, result, "calla");
}

bool Calli::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x2D_RR || op2 != CALLI_AA) {
    LogDebug("0x%lx: Calli::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpAa_RR(data, len, WORD, result, "calli");
}

bool Clo::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0F_RR || op2 != CLO_DC_DA) {
    LogDebug("0x%lx: Clo::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "clo");
}

bool Cloh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0F_RR || op2 != CLOH_DC_DA) {
    LogDebug("0x%lx: Cloh::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  // TODO: is the width here 2 or 4? It operates on two independent half
  // words...
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "cloh");
}

bool Cls::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0F_RR || op2 != CLS_DC_DA) {
    LogDebug("0x%lx: Cls::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "cls");
}

bool Clsh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0F_RR || op2 != CLSH_DC_DA) {
    LogDebug("0x%lx: Clsh::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  // TODO: is the width here 2 or 4? It operates on two independent half
  // words...
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "clsh");
}

bool Clz::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0F_RR || op2 != CLZ_DC_DA) {
    LogDebug("0x%lx: Clz::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "clz");
}

bool Clzh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0F_RR || op2 != CLZH_DC_DA) {
    LogDebug("0x%lx: Clzh::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  // TODO: is the width here 2 or 4? It operates on two independent half
  // words...
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "clzh");
}

bool Cmov::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case CMOV_DA_D15_CONST4:
      len = 2;
      return Instruction::TextOpDaD15Const4_SRC(data, len, WORD, result,
                                                "cmov");
    case CMOV_DA_D15_DB:
      len = 2;
      return Instruction::TextOpDaD15Db_SRR(data, len, WORD, result, "cmov");
    default:
      LogDebug("0x%lx: Cmov::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Cmovn::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case CMOVN_DA_D15_CONST4:
      len = 2;
      return Instruction::TextOpDaD15Const4_SRC(data, len, WORD, result,
                                                "cmovn");
    case CMOVN_DA_D15_DB:
      len = 2;
      return Instruction::TextOpDaD15Db_SRR(data, len, WORD, result, "cmovn");
    default:
      LogDebug("0x%lx: Cmovn::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

// TCv1.6 ISA only
bool Cmpswap::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x49_BO:
      switch (op2) {
        case CMPSWAPW_AB_OFF10_EA:
          return Instruction::TextOpAbOff10Ea_BO(data, len, WORD, result,
                                                 "cmpswap.w", ADDRMODE_REGULAR);
        case CMPSWAPW_AB_OFF10_EA_PREINC:
          return Instruction::TextOpAbOff10Ea_BO(data, len, WORD, result,
                                                 "cmpswap.w", ADDRMODE_PREINC);
        case CMPSWAPW_AB_OFF10_EA_POSTINC:
          return Instruction::TextOpAbOff10Ea_BO(data, len, WORD, result,
                                                 "cmpswap.w", ADDRMODE_POSTINC);
        default:
          LogDebug("0x%lx: Cmpswap::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x69_BO:
      switch (op2) {
        case CMPSWAPW_PB_EA:
          return Instruction::TextOpPbEa_BO(data, len, WORD, result,
                                            "cmpswap.w", ADDRMODE_BREV);
        case CMPSWAPW_PB_OFF10_EA:
          return Instruction::TextOpPbOff10Ea_BO(data, len, WORD, result,
                                                 "cmpswap.w", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Cmpswap::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Cmpswap::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

// TCv1.6 ISA only
bool Crc32::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != CRC32_DC_DB_DA) {
    LogDebug("0x%lx: Crc32::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDbDa_RR(data, len, WORD, result, "crc32");
}

bool Csub::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x2B_RRR || op2 != CSUB_DC_DD_DA_DB) {
    LogDebug("0x%lx: Csub::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result, "csub");
}

bool Csubn::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x2B_RRR || op2 != CSUBN_DC_DD_DA_DB) {
    LogDebug("0x%lx: Csubn::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result, "csubn");
}

bool Debug::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x00_SR:
      if (op2 != DEBUG16) {
        LogDebug("0x%lx: Debug::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 2;
      return Instruction::TextOp_SR(data, len, WORD, result, "debug");
    case OP1_x0D_SYS:
      if (op2 != DEBUG32) {
        LogDebug("0x%lx: Debug::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOp_SYS(data, len, WORD, result, "debug");
    default:
      LogDebug("0x%lx: Debug::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Dextr::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x77_RRPW:
      if (op2 != DEXTR_DC_DA_DB_POS) {
        LogDebug("0x%lx: Dextr::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDbPos_RRPW(data, len, WORD, result,
                                               "dextr");
    case OP1_x17_RRRR:
      if (op2 != DEXTR_DC_DA_DB_DD) {
        LogDebug("0x%lx: Dextr::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDbDd_RRRR(data, len, WORD, result, "dextr");
    default:
      LogDebug("0x%lx: Dextr::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Disable::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != DISABLE) {
    LogDebug("0x%lx: Disable::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "disable");
}

// TCv1.6 ISA only
bool DisableDa::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len,
                     std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != DISABLE_DA) {
    LogDebug("0x%lx: DisableDa::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDa_SYS(data, len, WORD, result, "disable");
}

bool Dsync::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != DSYNC) {
    LogDebug("0x%lx: Dsync::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "dsync");
}

bool Div::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR) {
    LogDebug("0x%lx: Div::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  if (op2 == DIV_EC_DA_DB) {
    len = 4;
    return Instruction::TextOpEcDaDb_RR(data, len, WORD, result, "div");
  } else if (op2 == DIVU_EC_DA_DB) {
    len = 4;
    return Instruction::TextOpEcDaDb_RR(data, len, WORD, result, "div.u");
  } else {
    LogDebug("0x%lx: Div::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
}

bool Dvadj::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR || op2 != DVADJ_EC_ED_DB) {
    LogDebug("0x%lx: Dvadj::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpEcEdDb_RRR(data, len, WORD, result, "dvadj");
}

bool Dvinit::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR) {
    LogDebug("0x%lx: Dvinit::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case DVINITB_EC_DA_DB:
      len = 4;
      return Instruction::TextOpEcDaDb_RR(data, len, BYTE, result, "dvinit.b");
    case DVINITBU_EC_DA_DB:
      len = 4;
      return Instruction::TextOpEcDaDb_RR(data, len, BYTE, result, "dvinit.bu");
    case DVINITH_EC_DA_DB:
      len = 4;
      return Instruction::TextOpEcDaDb_RR(data, len, HWORD, result, "dvinit.h");
    case DVINITHU_EC_DA_DB:
      len = 4;
      return Instruction::TextOpEcDaDb_RR(data, len, HWORD, result,
                                          "dvinit.hu");
    case DVINIT_EC_DA_DB:
      len = 4;
      return Instruction::TextOpEcDaDb_RR(data, len, WORD, result, "dvinit");
    case DVINITU_EC_DA_DB:
      len = 4;
      return Instruction::TextOpEcDaDb_RR(data, len, WORD, result, "dvinit.u");
    default:
      LogDebug("0x%lx: Dvinit::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Dvstep::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR) {
    LogDebug("0x%lx: Dvstep::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case DVSTEP_EC_ED_DB:
      len = 4;
      return Instruction::TextOpEcEdDb_RRR(data, len, WORD, result, "dvstep");
    case DVSTEPU_EC_ED_DB:
      len = 4;
      return Instruction::TextOpEcEdDb_RRR(data, len, WORD, result, "dvstep.u");
    default:
      LogDebug("0x%lx: Dvstep::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Enable::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != ENABLE) {
    LogDebug("0x%lx: Enable::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "enable");
}

bool Eq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case EQ_D15_DA_CONST4:
      len = 2;
      return Instruction::TextOpD15DaConst4_SRC(data, len, WORD, result, "eq");
    case EQ_D15_DA_DB:
      len = 2;
      return Instruction::TextOpD15DaDb_SRR(data, len, WORD, result, "eq");
    case OP1_x8B_RC:
      if (op2 != EQ_DC_DA_CONST9) {
        LogDebug("0x%lx: Eq::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "eq");
    case OP1_x0B_RR:
      if (op2 != EQ_DC_DA_DB) {
        LogDebug("0x%lx: Eq::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "eq");
    default:
      LogDebug("0x%lx: Eq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Eqa::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x01_RR || op2 != EQA_DC_AA_AB) {
    LogDebug("0x%lx: Eqa::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcAaAb_RR(data, len, WORD, result, "eq.a");
}

bool Eqbhw::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Eqbhw::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    // TODO: eq.b and eq.h compare bytes/hwords at a time but for the whole
    // word, is width still WORD?
    case EQB_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "eq.b");
    case EQH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "eq.h");
    case EQW_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "eq.w");

    default:
      LogDebug("0x%lx: Eqbhw::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Eqanybh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case EQANYB_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "eqany.b");
        case EQANYH_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "eqany.h");
        default:
          LogDebug("0x%lx: Eqanybh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case EQANYB_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "eqany.b");
        case EQANYH_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "eqany.h");
        default:
          LogDebug("0x%lx: Eqanybh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Eqanybh::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Eqza::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x01_RR || op2 != EQZA_DC_AA) {
    LogDebug("0x%lx: Eqza::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcAa_RR(data, len, WORD, result, "eqz.a");
}

bool Extr::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x37_RRPW:
      switch (op2) {
        case EXTR_DC_DA_POS_WIDTH:
          len = 4;
          return Instruction::TextOpDcDaPosWidth_RRPW(data, len, WORD, result,
                                                      "extr");
        case EXTRU_DC_DA_POS_WIDTH:
          len = 4;
          return Instruction::TextOpDcDaPosWidth_RRPW(data, len, WORD, result,
                                                      "extr.u");
        default:
          LogDebug("0x%lx: Extr::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x17_RRRR:
      switch (op2) {
        case EXTR_DC_DA_ED:
          len = 4;
          return Instruction::TextOpDcDaEd_RRRR(data, len, WORD, result,
                                                "extr");
        case EXTRU_DC_DA_ED:
          len = 4;
          return Instruction::TextOpDcDaEd_RRRR(data, len, WORD, result,
                                                "extr.u");
        default:
          LogDebug("0x%lx: Extr::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x57_RRRW:
      switch (op2) {
        case EXTR_DC_DA_DD_WIDTH:
          len = 4;
          return Instruction::TextOpDcDaDdWidth_RRRW(data, len, WORD, result,
                                                     "extr");
        case EXTRU_DC_DA_DD_WIDTH:
          len = 4;
          return Instruction::TextOpDcDaDdWidth_RRRW(data, len, WORD, result,
                                                     "extr.u");
        default:
          LogDebug("0x%lx: Extr::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Extr::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Fcall::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case FCALL_DISP24:
      len = 4;
      return Instruction::TextOpDisp24_B(data, addr, len, WORD, result,
                                         "fcall");
    default:
      LogDebug("0x%lx: Fcall::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Fcalla::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != FCALLA_DISP24) {
    LogDebug("0x%lx: Fcalla::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDisp24_B(data, addr, len, WORD, result, "fcalla");
}

bool Fcalli::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x2D_RR || op2 != FCALLI_AA) {
    LogDebug("0x%lx: Fcalli::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpAa_RR(data, len, WORD, result, "fcalli");
}

bool Fret::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x00_SR:
      switch (op2) {
        case FRET16:
          len = 2;
          return Instruction::TextOp_SR(data, len, WORD, result, "fret");
        default:
          LogDebug("0x%lx: Fret::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0D_SYS:
      switch (op2) {
        case FRET32:
          len = 4;
          return Instruction::TextOp_SYS(data, len, WORD, result, "fret");
        default:
          LogDebug("0x%lx: Fret::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Fret::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ge::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case GE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "ge");
        case GEU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "ge.u", true);
        default:
          LogDebug("0x%lx: Ge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case GE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "ge");
        case GEU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "ge.u");
        default:
          LogDebug("0x%lx: Ge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ge::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Gea::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x01_RR || op2 != GEA_DC_AA_AB) {
    LogDebug("0x%lx: Gea::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcAaAb_RR(data, len, WORD, result, "ge.a");
}

bool Imask::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xB7_RCPW:
      if (op2 != IMASK_EC_CONST4_POS_WIDTH) {
        LogDebug("0x%lx: Imask::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpEcConst4PosWidth_RCPW(data, len, WORD, result,
                                                      "imask");
    case OP1_xD7_RCRW:
      if (op2 != IMASK_EC_CONST4_DD_WIDTH) {
        LogDebug("0x%lx: Imask::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpEcConst4DdWidth_RCRW(data, len, WORD, result,
                                                     "imask");
    case OP1_x37_RRPW:
      if (op2 != IMASK_EC_DB_POS_WIDTH) {
        LogDebug("0x%lx: Imask::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpEcDbPosWidth_RRPW(data, len, WORD, result,
                                                  "imask");
    case OP1_x57_RRRW:
      if (op2 != IMASK_EC_DB_DD_WIDTH) {
        LogDebug("0x%lx: Imask::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpEcDbDdWidth_RRRW(data, len, WORD, result,
                                                 "imask");
    default:
      LogDebug("0x%lx: Imask::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool InstNt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x67_BIT) {
    LogDebug("0x%lx: InstNt::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case INST_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "ins.t");
    case INSNT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "insn.t");
    default:
      LogDebug("0x%lx: InstNt::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Insert::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xB7_RCPW:
      if (op2 != INSERT_DC_DA_CONST4_POS_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst4PosWidth_RCPW(data, len, WORD, result,
                                                        "insert");
    case OP1_x97_RCRR:
      if (op2 != INSERT_DC_DA_CONST4_ED) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst4Ed_RCRR(data, len, WORD, result,
                                                  "insert");
    case OP1_xD7_RCRW:
      if (op2 != INSERT_DC_DA_CONST4_DD_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaConst4DdWidth_RCRW(data, len, WORD, result,
                                                       "insert");
    case OP1_x37_RRPW:
      if (op2 != INSERT_DC_DA_DB_POS_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDbPosWidth_RRPW(data, len, WORD, result,
                                                    "insert");
    case OP1_x17_RRRR:
      if (op2 != INSERT_DC_DA_DB_ED) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDbEd_RRRR(data, len, WORD, result,
                                              "insert");
    case OP1_x57_RRRW:
      if (op2 != INSERT_DC_DA_DB_DD_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDcDaDbDdWidth_RRRW(data, len, WORD, result,
                                                   "insert");
    default:
      LogDebug("0x%lx: Insert::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Isync::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != ISYNC) {
    LogDebug("0x%lx: Isync::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "isync");
}

bool Ixmax::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR) {
    LogDebug("0x%lx: Ixmax::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case IXMAX_EC_ED_DB:
      len = 4;
      return Instruction::TextOpEcEdDb_RRR(data, len, WORD, result, "ixmax");
    case IXMAXU_EC_ED_DB:
      len = 4;
      return Instruction::TextOpEcEdDb_RRR(data, len, WORD, result, "ixmax.u");
    default:
      LogDebug("0x%lx: Ixmax::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Ixmin::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR) {
    LogDebug("0x%lx: Ixmin::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case IXMIN_EC_ED_DB:
      len = 4;
      return Instruction::TextOpEcEdDb_RRR(data, len, WORD, result, "ixmin");
    case IXMINU_EC_ED_DB:
      len = 4;
      return Instruction::TextOpEcEdDb_RRR(data, len, WORD, result, "ixmin.u");
    default:
      LogDebug("0x%lx: Ixmin::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool J::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
             size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case J_DISP8:
      len = 2;
      return Instruction::TextOpDisp8_SB(data, addr, len, WORD, result, "j");
    case J_DISP24:
      len = 4;
      return Instruction::TextOpDisp24_B(data, addr, len, WORD, result, "j");
    default:
      LogDebug("0x%lx: J::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ja::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != JA_DISP24) {
    LogDebug("0x%lx: Ja::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDisp24_B(data, addr, len, WORD, result, "ja");
}

bool Jeq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case JEQ_D15_CONST4_DISP4:
    case JEQ_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
      len = 2;
      return Instruction::TextOpD15Const4Disp4_SBC(data, addr, len, WORD,
                                                   result, "jeq");
    case JEQ_D15_DB_DISP4:
    case JEQ_D15_DB_DISP4_EX:  // TCv1.6 ISA only
      len = 2;
      return Instruction::TextOpD15DbDisp4_SBR(data, addr, len, WORD, result,
                                               "jeq");
    case OP1_xDF_BRC:
      if (op2 != JEQ_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                   result, "jeq");
    case OP1_x5F_BRR:
      if (op2 != JEQ_DA_DB_DISP15) {
        LogDebug("0x%lx: Jeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD, result,
                                               "jeq");
    default:
      LogDebug("0x%lx: Jeq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jeqa::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x7D_BRR || op2 != JEQA_AA_AB_DISP15) {
    LogDebug("0x%lx: Jeqa::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpAaAbDisp15_BRR(data, addr, len, WORD, result,
                                           "jeqa");
}

bool Jge::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xFF_BRC:
      switch (op2) {
        case JGE_DA_CONST4_DISP15:
          len = 4;
          return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                       result, "jge");
        case JGEU_DA_CONST4_DISP15:
          len = 4;
          return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                       result, "jge.u", true);
        default:
          LogDebug("0x%lx: Jge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x7F_BRR:
      switch (op2) {
        case JGE_DA_DB_DISP15:
          len = 4;
          return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD,
                                                   result, "jge");
        case JGEU_DA_DB_DISP15:
          len = 4;
          return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD,
                                                   result, "jge.u");
        default:
          LogDebug("0x%lx: Jge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Jge::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jgez::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != JGEZ_DB_DISP4) {
    LogDebug("0x%lx: Jgez::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 2;
  return Instruction::TextOpDbDisp4_SBR(data, addr, len, WORD, result, "jgez");
}

bool Jgtz::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != JGTZ_DB_DISP4) {
    LogDebug("0x%lx: Jgtz::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 2;
  return Instruction::TextOpDbDisp4_SBR(data, addr, len, WORD, result, "jgtz");
}

bool Ji::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xDC_SR:
      if (op2 != JI_AA_16) {
        LogDebug("0x%lx: Ji::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      len = 2;
      return Instruction::TextOpAa_SR(data, len, WORD, result, "ji");
    case OP1_x2D_RR:
      if (op2 != JI_AA_32) {
        LogDebug("0x%lx: Ji::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpAa_RR(data, len, WORD, result, "ji");
    default:
      LogDebug("0x%lx: Ji::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jl::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != JL_DISP24) {
    LogDebug("0x%lx: Jl::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDisp24_B(data, addr, len, WORD, result, "jl");
}

bool Jla::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != JLA_DISP24) {
    LogDebug("0x%lx: Jla::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDisp24_B(data, addr, len, WORD, result, "jla");
}

bool Jlez::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != JLEZ_DB_DISP4) {
    LogDebug("0x%lx: Jlez::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 2;
  return Instruction::TextOpDbDisp4_SBR(data, addr, len, WORD, result, "jlez");
}

bool Jli::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x2D_RR || op2 != JLI_AA) {
    LogDebug("0x%lx: Jli::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpAa_RR(data, len, WORD, result, "jli");
}

bool Jlt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xBF_BRC:
      switch (op2) {
        case JLT_DA_CONST4_DISP15:
          len = 4;
          return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                       result, "jlt");
        case JLTU_DA_CONST4_DISP15:
          len = 4;
          return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                       result, "jlt.u", true);
        default:
          LogDebug("0x%lx: Jlt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x3F_BRR:
      switch (op2) {
        case JLT_DA_DB_DISP15:
          len = 4;
          return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD,
                                                   result, "jlt");
        case JLTU_DA_DB_DISP15:
          len = 4;
          return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD,
                                                   result, "jlt.u");
        default:
          LogDebug("0x%lx: Jlt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Jlt::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jltz::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != JLTZ_DB_DISP4) {
    LogDebug("0x%lx: Jltz::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 2;
  return Instruction::TextOpDbDisp4_SBR(data, addr, len, WORD, result, "jltz");
}

bool Jne::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xDF_BRC:
      if (op2 != JNE_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                   result, "jne");
    case OP1_x5F_BRR:
      if (op2 != JNE_DA_DB_DISP15) {
        LogDebug("0x%lx: Jne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD, result,
                                               "jne");
    case JNE_D15_CONST4_DISP4:
    case JNE_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
      len = 2;
      return Instruction::TextOpD15Const4Disp4_SBC(data, addr, len, WORD,
                                                   result, "jne");
    case JNE_D15_DB_DISP4:
    case JNE_D15_DB_DISP4_EX:  // TCv1.6 ISA only
      len = 2;
      return Instruction::TextOpD15DbDisp4_SBR(data, addr, len, WORD, result,
                                               "jne");
    default:
      LogDebug("0x%lx: Jne::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnea::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x7D_BRR || op2 != JNEA_AA_AB_DISP15) {
    LogDebug("0x%lx: Jnea::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpAaAbDisp15_BRR(data, addr, len, WORD, result,
                                           "jnea");
}

bool Jned::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x9F_BRC:
      if (op2 != JNED_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jned::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                   result, "jned");
    case OP1_x1F_BRR:
      if (op2 != JNED_DA_DB_DISP15) {
        LogDebug("0x%lx: Jned::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD, result,
                                               "jned");
    default:
      LogDebug("0x%lx: Jned::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnei::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x9F_BRC:
      if (op2 != JNEI_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jnei::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaConst4Disp15_BRC(data, addr, len, WORD,
                                                   result, "jnei");
    case OP1_x1F_BRR:
      if (op2 != JNEI_DA_DB_DISP15) {
        LogDebug("0x%lx: Jnei::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaDbDisp15_BRR(data, addr, len, WORD, result,
                                               "jnei");
    default:
      LogDebug("0x%lx: Jnei::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnz::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case JNZ_D15_DISP8:
      len = 2;
      return Instruction::TextOpD15Disp8_SB(data, addr, len, WORD, result,
                                            "jnz");
    case JNZ_DB_DISP4:
      len = 2;
      return Instruction::TextOpDbDisp4_SBR(data, addr, len, WORD, result,
                                            "jnz");
    default:
      LogDebug("0x%lx: Jnz::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnza::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case JNZA_AB_DISP4:
      len = 2;
      return Instruction::TextOpAbDisp4_SBR(data, addr, len, WORD, result,
                                            "jnz.a");
    case OP1_xBD_BRR:
      if (op2 != JNZA_AA_DISP15) {
        LogDebug("0x%lx: Jnza::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpAaDisp15_BRR(data, addr, len, WORD, result,
                                             "jnz.a");
    default:
      LogDebug("0x%lx: Jnza::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnzt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case JNZT_D15_N_DISP4:
      len = 2;
      return Instruction::TextOpD15NDisp4_SBRN(data, addr, len, WORD, result,
                                               "jnz.t");
    case OP1_x6F_BRN:
      if (op2 != JNZT_DA_N_DISP15) {
        LogDebug("0x%lx: Jnzt::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaNDisp15_BRN(data, addr, len, WORD, result,
                                              "jnz.t");
    default:
      LogDebug("0x%lx: Jnzt::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jz::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case JZ_D15_DISP8:
      len = 2;
      return Instruction::TextOpD15Disp8_SB(data, addr, len, WORD, result,
                                            "jz");
    case JZ_DB_DISP4:
      len = 2;
      return Instruction::TextOpDbDisp4_SBR(data, addr, len, WORD, result,
                                            "jz");
    default:
      LogDebug("0x%lx: Jz::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jza::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case JZA_AB_DISP4:
      len = 2;
      return Instruction::TextOpAbDisp4_SBR(data, addr, len, WORD, result,
                                            "jz.a");
    case OP1_xBD_BRR:
      if (op2 != JZA_AA_DISP15) {
        LogDebug("0x%lx: Jza::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpAaDisp15_BRR(data, addr, len, WORD, result,
                                             "jz.a");
    default:
      LogDebug("0x%lx: Jza::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jzt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case JZT_D15_N_DISP4:
      len = 2;
      return Instruction::TextOpD15NDisp4_SBRN(data, addr, len, WORD, result,
                                               "jz.t");
    case OP1_x6F_BRN:
      if (op2 != JZT_DA_N_DISP15) {
        LogDebug("0x%lx: Jzt::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpDaNDisp15_BRN(data, addr, len, WORD, result,
                                              "jz.t");
    default:
      LogDebug("0x%lx: Jzt::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Lda::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LDA_A15_A10_CONST8:
      len = 2;
      return Instruction::TextOpA15A10Const8_SC(data, len, WORD, result,
                                                "ld.a");
    case LDA_AC_AB:
      len = 2;
      return Instruction::TextOpAcAb_SLR(data, len, WORD, result, "ld.a",
                                         ADDRMODE_REGULAR);
    case LDA_AC_AB_POSTINC:
      len = 2;
      return Instruction::TextOpAcAb_SLR(data, len, WORD, result, "ld.a",
                                         ADDRMODE_POSTINC);
    case LDA_AC_A15_OFF4:
      len = 2;
      return Instruction::TextOpAcA15Off4_SLRO(data, len, WORD, result, "ld.a");
    case LDA_A15_AB_OFF4:
      len = 2;
      return Instruction::TextOpA15AbOff4_SRO(data, len, WORD, result, "ld.a");
    case LDA_AA_AB_OFF16:
      len = 4;
      return Instruction::TextOpAaAbOff16_BOL(data, len, WORD, result, "ld.a");
    case OP1_x85_ABS:
      if (op2 != LDA_AA_OFF18) {
        LogDebug("0x%lx: Lda::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpAaOff18_ABS(data, len, WORD, result, "ld.a");
    case OP1_x09_BO:
      switch (op2) {
        case LDA_AA_AB_OFF10:
          len = 4;
          return Instruction::TextOpAaAbOff10_BO(data, len, WORD, result,
                                                 "ld.a", ADDRMODE_REGULAR);
        case LDA_AA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpAaAbOff10_BO(data, len, WORD, result,
                                                 "ld.a", ADDRMODE_POSTINC);
        case LDA_AA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpAaAbOff10_BO(data, len, WORD, result,
                                                 "ld.a", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Lda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDA_AA_PB:
          len = 4;
          return Instruction::TextOpAaPb_BO(data, len, WORD, result, "ld.a",
                                            ADDRMODE_BREV);
        case LDA_AA_PB_OFF10:
          len = 4;
          return Instruction::TextOpAaPbOff10_BO(data, len, WORD, result,
                                                 "ld.a", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Lda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Lda::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldb::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LDBU_DC_AB:
      len = 2;
      return Instruction::TextOpDcAb_SLR(data, len, BYTE, result, "ld.bu",
                                         ADDRMODE_REGULAR);
    case LDBU_DC_AB_POSTINC:
      len = 2;
      return Instruction::TextOpDcAb_SLR(data, len, BYTE, result, "ld.bu",
                                         ADDRMODE_POSTINC);
    case LDBU_DC_A15_OFF4:
      len = 2;
      return Instruction::TextOpDcA15Off4_SLRO(data, len, BYTE, result,
                                               "ld.bu");
    case LDBU_D15_AB_OFF4:
      len = 2;
      return Instruction::TextOpD15AbOff4_SRO(data, len, BYTE, result, "ld.bu");
    case LDB_DA_AB_OFF16:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpDaAbOff16_BOL(data, len, BYTE, result, "ld.b");
    case LDBU_DA_AB_OFF16:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpDaAbOff16_BOL(data, len, BYTE, result, "ld.bu");
    case OP1_x05_ABS:
      switch (op2) {
        case LDB_DA_OFF18:
          len = 4;
          return Instruction::TextOpDaOff18_ABS(data, len, BYTE, result,
                                                "ld.b");
        case LDBU_DA_OFF18:
          len = 4;
          return Instruction::TextOpDaOff18_ABS(data, len, BYTE, result,
                                                "ld.bu");
        default:
          LogDebug("0x%lx: Ldb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDB_DA_AB_OFF10:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, BYTE, result,
                                                 "ld.b", ADDRMODE_REGULAR);
        case LDB_DA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, BYTE, result,
                                                 "ld.b", ADDRMODE_POSTINC);
        case LDB_DA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, BYTE, result,
                                                 "ld.b", ADDRMODE_PREINC);
        case LDBU_DA_AB_OFF10:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, BYTE, result,
                                                 "ld.bu", ADDRMODE_REGULAR);
        case LDBU_DA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, BYTE, result,
                                                 "ld.bu", ADDRMODE_POSTINC);
        case LDBU_DA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, BYTE, result,
                                                 "ld.bu", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDB_DA_PB:
          len = 4;
          return Instruction::TextOpDaPb_BO(data, len, BYTE, result, "ld.b",
                                            ADDRMODE_BREV);
        case LDB_DA_PB_OFF10:
          len = 4;
          return Instruction::TextOpDaPbOff10_BO(data, len, BYTE, result,
                                                 "ld.b", ADDRMODE_CIRC);
        case LDBU_DA_PB:
          len = 4;
          return Instruction::TextOpDaPb_BO(data, len, BYTE, result, "ld.bu",
                                            ADDRMODE_BREV);
        case LDBU_DA_PB_OFF10:
          len = 4;
          return Instruction::TextOpDaPbOff10_BO(data, len, BYTE, result,
                                                 "ld.bu", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldb::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldd::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x85_ABS:
      if (op2 != LDD_EA_OFF18) {
        LogDebug("0x%lx: Ldd::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpEaOff18_ABS(data, len, DWORD, result, "ld.d");
    case OP1_x09_BO:
      switch (op2) {
        case LDD_EA_AB_OFF10:
          len = 4;
          return Instruction::TextOpEaAbOff10_BO(data, len, DWORD, result,
                                                 "ld.d", ADDRMODE_REGULAR);
        case LDD_EA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpEaAbOff10_BO(data, len, DWORD, result,
                                                 "ld.d", ADDRMODE_POSTINC);
        case LDD_EA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpEaAbOff10_BO(data, len, DWORD, result,
                                                 "ld.d", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldd::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDD_EA_PB:
          len = 4;
          return Instruction::TextOpEaPb_BO(data, len, DWORD, result, "ld.d",
                                            ADDRMODE_BREV);
        case LDD_EA_PB_OFF10:
          len = 4;
          return Instruction::TextOpEaPbOff10_BO(data, len, DWORD, result,
                                                 "ld.d", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldd::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldd::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldda::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x85_ABS:
      if (op2 != LDDA_PA_OFF18) {
        LogDebug("0x%lx: Ldda::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      return Instruction::TextOpPaOff18_ABS(data, len, DWORD, result, "ld.da");
    case OP1_x09_BO:
      switch (op2) {
        case LDDA_PA_AB_OFF10:
          len = 4;
          return Instruction::TextOpPaAbOff10_BO(data, len, DWORD, result,
                                                 "ld.da", ADDRMODE_REGULAR);
        case LDDA_PA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpPaAbOff10_BO(data, len, DWORD, result,
                                                 "ld.da", ADDRMODE_POSTINC);
        case LDDA_PA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpPaAbOff10_BO(data, len, DWORD, result,
                                                 "ld.da", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDDA_PA_PB:
          len = 4;
          return Instruction::TextOpPaPb_BO(data, len, DWORD, result, "ld.da",
                                            ADDRMODE_BREV);
        case LDDA_PA_PB_OFF10:
          len = 4;
          return Instruction::TextOpPaPbOff10_BO(data, len, DWORD, result,
                                                 "ld.da", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldda::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LDH_DC_AB:
      len = 2;
      return Instruction::TextOpDcAb_SLR(data, len, HWORD, result, "ld.h",
                                         ADDRMODE_REGULAR);
    case LDH_DC_AB_POSTINC:
      len = 2;
      return Instruction::TextOpDcAb_SLR(data, len, HWORD, result, "ld.h",
                                         ADDRMODE_POSTINC);
    case LDH_DC_A15_OFF4:
      len = 2;
      return Instruction::TextOpDcA15Off4_SLRO(data, len, HWORD, result,
                                               "ld.h");
    case LDH_D15_AB_OFF4:
      len = 2;
      return Instruction::TextOpD15AbOff4_SRO(data, len, HWORD, result, "ld.h");
    case LDH_DA_AB_OFF16:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpDaAbOff16_BOL(data, len, HWORD, result, "ld.h");
    case OP1_x05_ABS:
      switch (op2) {
        case LDH_DA_OFF18:
          len = 4;
          return Instruction::TextOpDaOff18_ABS(data, len, HWORD, result,
                                                "ld.h");
        default:
          LogDebug("0x%lx: Ldh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDH_DA_AB_OFF10:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.h", ADDRMODE_REGULAR);
        case LDH_DA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.h", ADDRMODE_POSTINC);
        case LDH_DA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.h", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDH_DA_PB:
          len = 4;
          return Instruction::TextOpDaPb_BO(data, len, HWORD, result, "ld.h",
                                            ADDRMODE_BREV);
        case LDH_DA_PB_OFF10:
          len = 4;
          return Instruction::TextOpDaPbOff10_BO(data, len, HWORD, result,
                                                 "ld.h", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldh::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldhu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LDHU_DA_AB_OFF16:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpDaAbOff16_BOL(data, len, HWORD, result,
                                              "ld.hu");
    case OP1_x05_ABS:
      switch (op2) {
        case LDHU_DA_OFF18:
          len = 4;
          return Instruction::TextOpDaOff18_ABS(data, len, HWORD, result,
                                                "ld.hu");
        default:
          LogDebug("0x%lx: Ldhu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDHU_DA_AB_OFF10:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.hu", ADDRMODE_REGULAR);
        case LDHU_DA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.hu", ADDRMODE_POSTINC);
        case LDHU_DA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.hu", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldhu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDHU_DA_PB:
          len = 4;
          return Instruction::TextOpDaPb_BO(data, len, HWORD, result, "ld.hu",
                                            ADDRMODE_BREV);
        case LDHU_DA_PB_OFF10:
          len = 4;
          return Instruction::TextOpDaPbOff10_BO(data, len, HWORD, result,
                                                 "ld.hu", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldhu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldhu::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x45_ABS:
      switch (op2) {
        case LDQ_DA_OFF18:
          len = 4;
          return Instruction::TextOpDaOff18_ABS(data, len, HWORD, result,
                                                "ld.q");
        default:
          LogDebug("0x%lx: Ldq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDQ_DA_AB_OFF10:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.q", ADDRMODE_REGULAR);
        case LDQ_DA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.q", ADDRMODE_POSTINC);
        case LDQ_DA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, HWORD, result,
                                                 "ld.q", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDQ_DA_PB:
          len = 4;
          return Instruction::TextOpDaPb_BO(data, len, HWORD, result, "ld.q",
                                            ADDRMODE_BREV);
        case LDQ_DA_PB_OFF10:
          len = 4;
          return Instruction::TextOpDaPbOff10_BO(data, len, HWORD, result,
                                                 "ld.q", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldw::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LDW_DC_AB:
      len = 2;
      return Instruction::TextOpDcAb_SLR(data, len, WORD, result, "ld.w",
                                         ADDRMODE_REGULAR);
    case LDW_DC_AB_POSTINC:
      len = 2;
      return Instruction::TextOpDcAb_SLR(data, len, WORD, result, "ld.w",
                                         ADDRMODE_POSTINC);
    case LDW_D15_A10_CONST8:
      len = 2;
      return Instruction::TextOpD15A10Const8_SC(data, len, WORD, result,
                                                "ld.w");
    case LDW_DC_A15_OFF4:
      len = 2;
      return Instruction::TextOpDcA15Off4_SLRO(data, len, WORD, result, "ld.w");
    case LDW_D15_AB_OFF4:
      len = 2;
      return Instruction::TextOpD15AbOff4_SRO(data, len, WORD, result, "ld.w");
    case LDW_DA_AB_OFF16:
      len = 4;
      return Instruction::TextOpDaAbOff16_BOL(data, len, WORD, result, "ld.w");
    case OP1_x85_ABS:
      switch (op2) {
        case LDW_DA_OFF18:
          len = 4;
          return Instruction::TextOpDaOff18_ABS(data, len, WORD, result,
                                                "ld.w");
        default:
          LogDebug("0x%lx: Ldw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDW_DA_AB_OFF10:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, WORD, result,
                                                 "ld.w", ADDRMODE_REGULAR);
        case LDW_DA_AB_OFF10_POSTINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, WORD, result,
                                                 "ld.w", ADDRMODE_POSTINC);
        case LDW_DA_AB_OFF10_PREINC:
          len = 4;
          return Instruction::TextOpDaAbOff10_BO(data, len, WORD, result,
                                                 "ld.w", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDW_DA_PB:
          len = 4;
          return Instruction::TextOpDaPb_BO(data, len, WORD, result, "ld.w",
                                            ADDRMODE_BREV);
        case LDW_DA_PB_OFF10:
          len = 4;
          return Instruction::TextOpDaPbOff10_BO(data, len, WORD, result,
                                                 "ld.w", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldw::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ldlcx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x15_ABS:
      switch (op2) {
        case LDLCX_OFF18:
          len = 4;
          return Instruction::TextOpOff18_ABS(data, len, WORD, result, "ldlcx");
        default:
          LogDebug("0x%lx: Ldlcx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case LDLCX_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "ldlcx");
        default:
          LogDebug("0x%lx: Ldlcx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldlcx::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Ldmst::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xE5_ABS:
      switch (op2) {
        case LDMST_OFF18_EA:
          len = 4;
          return Instruction::TextOpOff18Ea_ABS(data, len, WORD, result,
                                                "ldmst");
        default:
          LogDebug("0x%lx: Ldmst::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case LDMST_AB_OFF10_EA:
          len = 4;
          return Instruction::TextOpAbOff10Ea_BO(data, len, WORD, result,
                                                 "ldmst", ADDRMODE_REGULAR);
        case LDMST_AB_OFF10_EA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Ea_BO(data, len, WORD, result,
                                                 "ldmst", ADDRMODE_POSTINC);
        case LDMST_AB_OFF10_EA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Ea_BO(data, len, WORD, result,
                                                 "ldmst", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Ldmst::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x69_BO:
      switch (op2) {
        case LDMST_PB_EA:
          len = 4;
          return Instruction::TextOpPbEa_BO(data, len, WORD, result, "ldmst",
                                            ADDRMODE_BREV);
        case LDMST_PB_OFF10_EA:
          len = 4;
          return Instruction::TextOpPbOff10Ea_BO(data, len, WORD, result,
                                                 "ldmst", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Ldmst::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ldmst::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Lducx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x15_ABS:
      switch (op2) {
        case LDUCX_OFF18:
          len = 4;
          return Instruction::TextOpOff18_ABS(data, len, WORD, result, "lducx");
        default:
          LogDebug("0x%lx: Lducx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case LDUCX_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "lducx");
        default:
          LogDebug("0x%lx: Lducx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Lducx::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Lea::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LEA_AA_AB_OFF16:
      len = 4;
      return Instruction::TextOpAaAbOff16_BOL(data, len, WORD, result, "lea");
    case OP1_xC5_ABS:
      switch (op2) {
        case LEA_AA_OFF18:
          len = 4;
          return Instruction::TextOpAaOff18_ABS(data, len, WORD, result, "lea");
        default:
          LogDebug("0x%lx: Lea::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case LEA_AA_AB_OFF10:
          len = 4;
          return Instruction::TextOpAaAbOff10_BO(data, len, WORD, result,
                                                 "lea");
        default:
          LogDebug("0x%lx: Lea::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Lea::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Loop::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LOOP_AB_DISP4:
      len = 2;
      return Instruction::TextOpAbDisp4_SBR(data, addr, len, WORD, result,
                                            "loop");
    case OP1_xFD_BRR:
      switch (op2) {
        case LOOP_AB_DISP15:
          len = 4;
          return Instruction::TextOpAbDisp15_BRR(data, addr, len, WORD, result,
                                                 "loop");
        default:
          LogDebug("0x%lx: Loop::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Loop::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Loopu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xFD_BRR:
      switch (op2) {
        case LOOPU_DISP15:
          len = 4;
          return Instruction::TextOpDisp15_BRR(data, addr, len, WORD, result,
                                               "loopu");
        default:
          LogDebug("0x%lx: Loopu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Loopu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Lt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case LT_D15_DA_CONST4:
      len = 2;
      return Instruction::TextOpD15DaConst4_SRC(data, len, WORD, result, "lt");
    case LT_D15_DA_DB:
      len = 2;
      return Instruction::TextOpD15DaDb_SRR(data, len, WORD, result, "lt");
    case OP1_x8B_RC:
      switch (op2) {
        case LT_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "lt");
        case LTU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "lt.u", true);
        default:
          LogDebug("0x%lx: Lt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case LT_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "lt");
        case LTU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "lt.u");
        default:
          LogDebug("0x%lx: Lt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Lt::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Lta::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x01_RR || op2 != LTA_DC_AA_AB) {
    LogDebug("0x%lx: Lta::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcAaAb_RR(data, len, WORD, result, "lt.a");
}

bool Ltb::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Ltb::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case LTB_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "lt.b");
    case LTBU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "lt.bu");
    default:
      LogDebug("0x%lx: Ltb::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Lth::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Lth::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case LTH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "lt.h");
    case LTHU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "lt.hu");
    default:
      LogDebug("0x%lx: Lth::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Ltw::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Ltw::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case LTW_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "lt.w");
    case LTWU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "lt.wu");
    default:
      LogDebug("0x%lx: Ltw::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Madd::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x13_RCR:
      switch (op2) {
        case MADD_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "madd");
        case MADD_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "madd");
        case MADDS_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "madds");
        case MADDS_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "madds");
        default:
          LogDebug("0x%lx: Madd::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x03_RRR2:
      switch (op2) {
        case MADD_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR2(data, len, WORD, result,
                                                  "madd");
        case MADD_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "madd");
        case MADDS_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR2(data, len, WORD, result,
                                                  "madds");
        case MADDS_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "madds");
        default:
          LogDebug("0x%lx: Madd::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Madd::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Maddh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x83_RRR1) {
    LogDebug("0x%lx: Maddh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MADDH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "madd.h");
    case MADDH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "madd.h");
    case MADDH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "madd.h");
    case MADDH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "madd.h");
    case MADDSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "madds.h");
    case MADDSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "madds.h");
    case MADDSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "madds.h");
    case MADDSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "madds.h");
    default:
      LogDebug("0x%lx: Maddh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Maddq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x43_RRR1) {
    LogDebug("0x%lx: Maddq::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MADDQ_DC_DD_DA_DB_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbN_RRR1(data, len, WORD, result,
                                               "madd.q");
    case MADDQ_EC_ED_DA_DB_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbN_RRR1(data, len, WORD, result,
                                               "madd.q");
    case MADDQ_DC_DD_DA_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDblN_RRR1(data, len, WORD, result,
                                                "madd.q");
    case MADDQ_EC_ED_DA_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDblN_RRR1(data, len, WORD, result,
                                                "madd.q");
    case MADDQ_DC_DD_DA_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuN_RRR1(data, len, WORD, result,
                                                "madd.q");
    case MADDQ_EC_ED_DA_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuN_RRR1(data, len, WORD, result,
                                                "madd.q");
    case MADDQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "madd.q");
    case MADDQ_EC_ED_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDalDblN_RRR1(data, len, WORD, result,
                                                 "madd.q");
    case MADDQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "madd.q");
    case MADDQ_EC_ED_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDauDbuN_RRR1(data, len, WORD, result,
                                                 "madd.q");
    case MADDSQ_DC_DD_DA_DB_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbN_RRR1(data, len, WORD, result,
                                               "madds.q");
    case MADDSQ_EC_ED_DA_DB_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbN_RRR1(data, len, WORD, result,
                                               "madds.q");
    case MADDSQ_DC_DD_DA_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDblN_RRR1(data, len, WORD, result,
                                                "madds.q");
    case MADDSQ_EC_ED_DA_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDblN_RRR1(data, len, WORD, result,
                                                "madds.q");
    case MADDSQ_DC_DD_DA_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuN_RRR1(data, len, WORD, result,
                                                "madds.q");
    case MADDSQ_EC_ED_DA_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuN_RRR1(data, len, WORD, result,
                                                "madds.q");
    case MADDSQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "madds.q");
    case MADDSQ_EC_ED_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDalDblN_RRR1(data, len, WORD, result,
                                                 "madds.q");
    case MADDSQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "madds.q");
    case MADDSQ_EC_ED_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDauDbuN_RRR1(data, len, WORD, result,
                                                 "madds.q");
    default:
      LogDebug("0x%lx: Maddq::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Maddu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x13_RCR:
      switch (op2) {
        case MADDU_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "madd.u", true);
        case MADDSU_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "madds.u", true);
        case MADDSU_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "madds.u", true);
        default:
          LogDebug("0x%lx: Maddu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x03_RRR2:
      switch (op2) {
        case MADDU_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "madd.u");
        case MADDSU_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR2(data, len, WORD, result,
                                                  "madds.u");
        case MADDSU_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "madds.u");
        case LTU_DC_DA_DB:
        default:
          LogDebug("0x%lx: Maddu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Maddu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool MaddmhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x83_RRR1) {
    LogDebug("0x%lx: MaddmhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MADDMH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddm.h");
    case MADDMH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddm.h");
    case MADDMH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddm.h");
    case MADDMH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddm.h");
    case MADDMSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddms.h");
    case MADDMSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddms.h");
    case MADDMSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddms.h");
    case MADDMSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddms.h");
    default:
      LogDebug("0x%lx: MaddmhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MaddrhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x83_RRR1:
      switch (op2) {
        case MADDRH_DC_DD_DA_DBLL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                     "maddr.h");
        case MADDRH_DC_DD_DA_DBLU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                     "maddr.h");
        case MADDRH_DC_DD_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "maddr.h");
        case MADDRH_DC_DD_DA_DBUU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                     "maddr.h");
        case MADDRSH_DC_DD_DA_DBLL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                     "maddrs.h");
        case MADDRSH_DC_DD_DA_DBLU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                     "maddrs.h");
        case MADDRSH_DC_DD_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "maddrs.h");
        case MADDRSH_DC_DD_DA_DBUU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                     "maddrs.h");
        default:
          LogDebug("0x%lx: MaddrhSh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x43_RRR1:
      switch (op2) {
        case MADDRH_DC_ED_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "maddr.h");
        case MADDRSH_DC_ED_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "maddrs.h");
        default:
          LogDebug("0x%lx: MaddrhSh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: MaddrhSh::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Maddrq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x43_RRR1) {
    LogDebug("0x%lx: Maddrq::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MADDRQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "maddr.q");
    case MADDRQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "maddr.q");
    case MADDRSQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "maddrs.q");
    case MADDRSQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "maddrs.q");
    default:
      LogDebug("0x%lx: Maddrq::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MaddsuhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len,
                     std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xC3_RRR1) {
    LogDebug("0x%lx: MaddsuhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MADDSUH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddsu.h");
    case MADDSUH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddsu.h");
    case MADDSUH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddsu.h");
    case MADDSUH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddsu.h");
    case MADDSUSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddsus.h");
    case MADDSUSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddsus.h");
    case MADDSUSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddsus.h");
    case MADDSUSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddsus.h");
    default:
      LogDebug("0x%lx: MaddsuhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MaddsumhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len,
                      std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xC3_RRR1) {
    LogDebug("0x%lx: MaddsumhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MADDSUMH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddsum.h");
    case MADDSUMH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddsum.h");
    case MADDSUMH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddsum.h");
    case MADDSUMH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddsum.h");
    case MADDSUMSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddsums.h");
    case MADDSUMSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddsums.h");
    case MADDSUMSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddsums.h");
    case MADDSUMSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddsums.h");
    default:
      LogDebug("0x%lx: MaddsumhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MaddsurhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len,
                      std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xC3_RRR1) {
    LogDebug("0x%lx: MaddsurhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MADDSURH_DC_DD_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddsur.h");
    case MADDSURH_DC_DD_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddsur.h");
    case MADDSURH_DC_DD_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddsur.h");
    case MADDSURH_DC_DD_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddsur.h");
    case MADDSURSH_DC_DD_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "maddsurs.h");
    case MADDSURSH_DC_DD_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "maddsurs.h");
    case MADDSURSH_DC_DD_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "maddsurs.h");
    case MADDSURSH_DC_DD_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "maddsurs.h");
    default:
      LogDebug("0x%lx: MaddsurhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Max::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case MAX_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "max");
        case MAXU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "max.u", true);
        default:
          LogDebug("0x%lx: Max::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case MAX_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "max");
        case MAXU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "max.u");
        default:
          LogDebug("0x%lx: Max::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Max::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Maxb::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Maxb::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MAXB_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "max.b");
    case MAXBU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "max.bu");
    default:
      LogDebug("0x%lx: Maxb::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Maxh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Maxh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MAXH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "max.h");
    case MAXHU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "max.hu");
    default:
      LogDebug("0x%lx: Maxh::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Mfcr::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != MFCR_DC_CONST16) {
    LogDebug("0x%lx: Mfcr::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcConst16_RLC(data, len, WORD, result, "mfcr",
                                          true);
}

bool Min::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case MIN_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "min");
        case MINU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "min.u", true);
        default:
          LogDebug("0x%lx: Min::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case MIN_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "min");
        case MINU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "min.u");
        default:
          LogDebug("0x%lx: Min::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Min::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Minb::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Minb::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MINB_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "min.b");
    case MINBU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "min.bu");
    default:
      LogDebug("0x%lx: Minb::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Minh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Minh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MINH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "min.h");
    case MINHU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "min.hu");
    default:
      LogDebug("0x%lx: Minh::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Mov::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case MOV_D15_CONST8:
      len = 2;
      return Instruction::TextOpD15Const8_SC(data, len, WORD, result, "mov");
    case MOV_DA_CONST4:
      len = 2;
      return Instruction::TextOpDaConst4_SRC(data, len, WORD, result, "mov");
    case MOV_EA_CONST4:  // TCv1.6 ISA only
      len = 2;
      return Instruction::TextOpEaConst4_SRC(data, len, WORD, result, "mov");
    case MOV_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "mov");
    case MOV_DC_CONST16:
      len = 4;
      return Instruction::TextOpDcConst16_RLC(data, len, WORD, result, "mov");
    case MOV_EC_CONST16:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpEcConst16_RLC(data, len, WORD, result, "mov");
    case OP1_x0B_RR:
      switch (op2) {
        case MOV_DC_DB:
          len = 4;
          return Instruction::TextOpDcDb_RR(data, len, WORD, result, "mov");
        case MOV_EC_DB:  // TCv1.6 ISA only
          len = 4;
          return Instruction::TextOpEcDb_RR(data, len, WORD, result, "mov");
        case MOV_EC_DA_DB:  // TCv1.6 ISA only
          len = 4;
          return Instruction::TextOpEcDaDb_RR(data, len, WORD, result, "mov");
        default:
          LogDebug("0x%lx: Mov::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Mov::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Mova::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case MOVA_AA_CONST4:
      len = 2;
      return Instruction::TextOpAaConst4_SRC(data, len, WORD, result, "mov.a",
                                             true);
    case MOVA_AA_DB:
      len = 2;
      return Instruction::TextOpAaDb_SRR(data, len, WORD, result, "mov.a");
    case OP1_x01_RR:
      switch (op2) {
        case MOVA_AC_DB:
          len = 4;
          return Instruction::TextOpAcDb_RR(data, len, WORD, result, "mov.a");
        default:
          LogDebug("0x%lx: Mova::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Mova::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Movaa::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case MOVAA_AA_AB:
      len = 2;
      return Instruction::TextOpAaAb_SRR(data, len, WORD, result, "mov.aa");
    case OP1_x01_RR:
      switch (op2) {
        case MOVAA_AC_AB:
          len = 4;
          return Instruction::TextOpAcAb_RR(data, len, WORD, result, "mov.aa");
        default:
          LogDebug("0x%lx: Movaa::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Movaa::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Movd::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case MOVD_DA_AB:
      len = 2;
      return Instruction::TextOpDaAb_SRR(data, len, WORD, result, "mov.d");
    case OP1_x01_RR:
      switch (op2) {
        case MOVD_DC_AB:
          len = 4;
          return Instruction::TextOpDcAb_RR(data, len, WORD, result, "mov.d");
        default:
          LogDebug("0x%lx: Movd::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Movd::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Movu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != MOVU_DC_CONST16) {
    LogDebug("0x%lx: Movu::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcConst16_RLC(data, len, WORD, result, "mov.u",
                                          true);
}

bool Movh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != MOVH_DC_CONST16) {
    LogDebug("0x%lx: Movh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcConst16_RLC(data, len, WORD, result, "movh");
}

bool Movha::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != MOVHA_AC_CONST16) {
    LogDebug("0x%lx: Movha::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpAcConst16_RLC(data, len, WORD, result, "movh.a");
}

bool Msub::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x33_RCR:
      switch (op2) {
        case MSUB_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "msub");
        case MSUB_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "msub");
        case MSUBS_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "msubs");
        case MSUBS_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "msubs");
        default:
          LogDebug("0x%lx: Msub::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x23_RRR2:
      switch (op2) {
        case MSUB_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR2(data, len, WORD, result,
                                                  "msub");
        case MSUB_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "msub");
        case MSUBS_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR2(data, len, WORD, result,
                                                  "msubs");
        case MSUBS_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "msubs");
        default:
          LogDebug("0x%lx: Msub::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Msub::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Msubh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xA3_RRR1) {
    LogDebug("0x%lx: Msubh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MSUBH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msub.h");
    case MSUBH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msub.h");
    case MSUBH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msub.h");
    case MSUBH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msub.h");
    case MSUBSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubs.h");
    case MSUBSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubs.h");
    case MSUBSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubs.h");
    case MSUBSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubs.h");
    default:
      LogDebug("0x%lx: Msubh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Msubq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x63_RRR1) {
    LogDebug("0x%lx: Msubq::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MSUBQ_DC_DD_DA_DB_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbN_RRR1(data, len, WORD, result,
                                               "msub.q");
    case MSUBQ_EC_ED_DA_DB_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbN_RRR1(data, len, WORD, result,
                                               "msub.q");
    case MSUBQ_DC_DD_DA_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDblN_RRR1(data, len, WORD, result,
                                                "msub.q");
    case MSUBQ_EC_ED_DA_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDblN_RRR1(data, len, WORD, result,
                                                "msub.q");
    case MSUBQ_DC_DD_DA_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuN_RRR1(data, len, WORD, result,
                                                "msub.q");
    case MSUBQ_EC_ED_DA_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuN_RRR1(data, len, WORD, result,
                                                "msub.q");
    case MSUBQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "msub.q");
    case MSUBQ_EC_ED_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDalDblN_RRR1(data, len, WORD, result,
                                                 "msub.q");
    case MSUBQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "msub.q");
    case MSUBQ_EC_ED_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDauDbuN_RRR1(data, len, WORD, result,
                                                 "msub.q");
    case MSUBSQ_DC_DD_DA_DB_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbN_RRR1(data, len, WORD, result,
                                               "msubs.q");
    case MSUBSQ_EC_ED_DA_DB_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbN_RRR1(data, len, WORD, result,
                                               "msubs.q");
    case MSUBSQ_DC_DD_DA_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDblN_RRR1(data, len, WORD, result,
                                                "msubs.q");
    case MSUBSQ_EC_ED_DA_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDblN_RRR1(data, len, WORD, result,
                                                "msubs.q");
    case MSUBSQ_DC_DD_DA_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuN_RRR1(data, len, WORD, result,
                                                "msubs.q");
    case MSUBSQ_EC_ED_DA_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuN_RRR1(data, len, WORD, result,
                                                "msubs.q");
    case MSUBSQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "msubs.q");
    case MSUBSQ_EC_ED_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpEcEdDalDblN_RRR1(data, len, WORD, result,
                                                 "msubs.q");
    case MSUBSQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "msubs.q");
    case MSUBSQ_EC_ED_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpEcEdDauDbuN_RRR1(data, len, WORD, result,
                                                 "msubs.q");
    default:
      LogDebug("0x%lx: Msubq::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Msubu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x33_RCR:
      switch (op2) {
        case MSUBU_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "msub.u", true);
        case MSUBSU_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "msubs.u", true);
        case MSUBSU_EC_ED_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcEdDaConst9_RCR(data, len, WORD, result,
                                                     "msubs.u", true);
        default:
          LogDebug("0x%lx: Msubu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x23_RRR2:
      switch (op2) {
        case MSUBU_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "msub.u");
        case MSUBSU_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR2(data, len, WORD, result,
                                                  "msubs.u");
        case MSUBSU_EC_ED_DA_DB:
          len = 4;
          return Instruction::TextOpEcEdDaDb_RRR2(data, len, WORD, result,
                                                  "msubs.u");
        default:
          LogDebug("0x%lx: Msubu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Msubu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool MsubadhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len,
                     std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xE3_RRR1) {
    LogDebug("0x%lx: MsubadhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MSUBADH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubad.h");
    case MSUBADH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubad.h");
    case MSUBADH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubad.h");
    case MSUBADH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubad.h");
    case MSUBADSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubads.h");
    case MSUBADSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubads.h");
    case MSUBADSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubads.h");
    case MSUBADSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubads.h");
    default:
      LogDebug("0x%lx: MsubadhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MsubadmhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len,
                      std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xE3_RRR1) {
    LogDebug("0x%lx: MsubadmhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MSUBADMH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubadm.h");
    case MSUBADMH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubadm.h");
    case MSUBADMH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubadm.h");
    case MSUBADMH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubadm.h");
    case MSUBADMSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubadms.h");
    case MSUBADMSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubadms.h");
    case MSUBADMSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubadms.h");
    case MSUBADMSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubadms.h");
    default:
      LogDebug("0x%lx: MsubadmhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MsubadrhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len,
                      std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xE3_RRR1) {
    LogDebug("0x%lx: MsubadrhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MSUBADRH_DC_DD_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubadr.h");
    case MSUBADRH_DC_DD_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubadr.h");
    case MSUBADRH_DC_DD_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubadr.h");
    case MSUBADRH_DC_DD_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubadr.h");
    case MSUBADRSH_DC_DD_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubadrs.h");
    case MSUBADRSH_DC_DD_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubadrs.h");
    case MSUBADRSH_DC_DD_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubadrs.h");
    case MSUBADRSH_DC_DD_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubadrs.h");
    default:
      LogDebug("0x%lx: MsubadrhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MsubmhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xA3_RRR1) {
    LogDebug("0x%lx: MsubmhSh::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case MSUBMH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubm.h");
    case MSUBMH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubm.h");
    case MSUBMH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubm.h");
    case MSUBMH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubm.h");
    case MSUBMSH_EC_ED_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbllN_RRR1(data, len, HWORD, result,
                                                 "msubms.h");
    case MSUBMSH_EC_ED_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbluN_RRR1(data, len, HWORD, result,
                                                 "msubms.h");
    case MSUBMSH_EC_ED_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                 "msubms.h");
    case MSUBMSH_EC_ED_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcEdDaDbuuN_RRR1(data, len, HWORD, result,
                                                 "msubms.h");
    default:
      LogDebug("0x%lx: MsubmhSh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool MsubrhSh::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xA3_RRR1:
      switch (op2) {
        case MSUBRH_DC_DD_DA_DBLL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                     "msubr.h");
        case MSUBRH_DC_DD_DA_DBLU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                     "msubr.h");
        case MSUBRH_DC_DD_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "msubr.h");
        case MSUBRH_DC_DD_DA_DBUU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                     "msubr.h");
        case MSUBRSH_DC_DD_DA_DBLL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbllN_RRR1(data, len, HWORD, result,
                                                     "msubrs.h");
        case MSUBRSH_DC_DD_DA_DBLU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbluN_RRR1(data, len, HWORD, result,
                                                     "msubrs.h");
        case MSUBRSH_DC_DD_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "msubrs.h");
        case MSUBRSH_DC_DD_DA_DBUU_N:
          len = 4;
          return Instruction::TextOpDcDdDaDbuuN_RRR1(data, len, HWORD, result,
                                                     "msubrs.h");
        default:
          LogDebug("0x%lx: MsubrhSh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x63_RRR1:
      switch (op2) {
        case MSUBRH_DC_ED_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "msubr.h");
        case MSUBRSH_DC_ED_DA_DBUL_N:
          len = 4;
          return Instruction::TextOpDcEdDaDbulN_RRR1(data, len, HWORD, result,
                                                     "msubrs.h");
        default:
          LogDebug("0x%lx: MsubrhSh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: MsubrhSh::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Msubrq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x63_RRR1) {
    LogDebug("0x%lx: Msubrq::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MSUBRQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "msubr.q");
    case MSUBRQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "msubr.q");
    case MSUBRSQ_DC_DD_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDdDalDblN_RRR1(data, len, WORD, result,
                                                 "msubrs.q");
    case MSUBRSQ_DC_DD_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDdDauDbuN_RRR1(data, len, WORD, result,
                                                 "msubrs.q");
    default:
      LogDebug("0x%lx: Msubrq::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Mtcr::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != MTCR_CONST16_DA) {
    LogDebug("0x%lx: Mtcr::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  return Instruction::TextOpConst16Da_RLC(data, len, WORD, result, "mtcr");
}

bool Mul::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case MUL_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "mul");
    case OP1_x53_RC:
      switch (op2) {
        case MUL_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "mul");
        case MUL_EC_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcDaConst9_RC(data, len, WORD, result,
                                                  "mul");
        case MULS_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "muls");
        default:
          LogDebug("0x%lx: Mul::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x73_RR2:
      switch (op2) {
        case MUL_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR2(data, len, WORD, result, "mul");
        case MUL_EC_DA_DB:
          len = 4;
          return Instruction::TextOpEcDaDb_RR2(data, len, WORD, result, "mul");
        case MULS_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR2(data, len, WORD, result, "muls");
        default:
          LogDebug("0x%lx: Mul::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Mul::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Mulh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xB3_RR1) {
    LogDebug("0x%lx: Mulh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MULH_EC_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcDaDbllN_RR1(data, len, HWORD, result,
                                              "mul.h");
    case MULH_EC_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcDaDbluN_RR1(data, len, HWORD, result,
                                              "mul.h");
    case MULH_EC_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcDaDbulN_RR1(data, len, HWORD, result,
                                              "mul.h");
    case MULH_EC_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcDaDbuuN_RR1(data, len, HWORD, result,
                                              "mul.h");
    default:
      LogDebug("0x%lx: Mulh::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Mulq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x93_RR1) {
    LogDebug("0x%lx: Mulq::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MULQ_DC_DA_DB_N:
      len = 4;
      return Instruction::TextOpDcDaDbN_RR1(data, len, HWORD, result, "mul.q");
    case MULQ_EC_DA_DB_N:
      len = 4;
      return Instruction::TextOpEcDaDbN_RR1(data, len, HWORD, result, "mul.q");
    case MULQ_DC_DA_DBL_N:
      len = 4;
      return Instruction::TextOpDcDaDblN_RR1(data, len, HWORD, result, "mul.q");
    case MULQ_EC_DA_DBL_N:
      len = 4;
      return Instruction::TextOpEcDaDblN_RR1(data, len, HWORD, result, "mul.q");
    case MULQ_DC_DA_DBU_N:
      len = 4;
      return Instruction::TextOpDcDaDbuN_RR1(data, len, HWORD, result, "mul.q");
    case MULQ_EC_DA_DBU_N:
      len = 4;
      return Instruction::TextOpEcDaDbuN_RR1(data, len, HWORD, result, "mul.q");
    case MULQ_DC_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDalDblN_RR1(data, len, HWORD, result,
                                              "mul.q");
    case MULQ_DC_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDauDbuN_RR1(data, len, HWORD, result,
                                              "mul.q");
    default:
      LogDebug("0x%lx: Mulq::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Mulu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x53_RC:
      switch (op2) {
        case MULU_EC_DA_CONST9:
          len = 4;
          return Instruction::TextOpEcDaConst9_RC(data, len, WORD, result,
                                                  "mul.u", true);
        case MULSU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "muls.u", true);
        default:
          LogDebug("0x%lx: Mulu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x73_RR2:
      switch (op2) {
        case MULU_EC_DA_DB:
          len = 4;
          return Instruction::TextOpEcDaDb_RR2(data, len, WORD, result,
                                               "mul.u");
        case MULSU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR2(data, len, WORD, result,
                                               "muls.u");
        default:
          LogDebug("0x%lx: Mulu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Mulu::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Mulmh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xB3_RR1) {
    LogDebug("0x%lx: Mulmh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MULMH_EC_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpEcDaDbllN_RR1(data, len, HWORD, result,
                                              "mulm.h");
    case MULMH_EC_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpEcDaDbluN_RR1(data, len, HWORD, result,
                                              "mulm.h");
    case MULMH_EC_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpEcDaDbulN_RR1(data, len, HWORD, result,
                                              "mulm.h");
    case MULMH_EC_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpEcDaDbuuN_RR1(data, len, HWORD, result,
                                              "mulm.h");
    default:
      LogDebug("0x%lx: Mulmh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Mulrh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xB3_RR1) {
    LogDebug("0x%lx: Mulrh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MULRH_DC_DA_DBLL_N:
      len = 4;
      return Instruction::TextOpDcDaDbllN_RR1(data, len, HWORD, result,
                                              "mulr.h");
    case MULRH_DC_DA_DBLU_N:
      len = 4;
      return Instruction::TextOpDcDaDbluN_RR1(data, len, HWORD, result,
                                              "mulr.h");
    case MULRH_DC_DA_DBUL_N:
      len = 4;
      return Instruction::TextOpDcDaDbulN_RR1(data, len, HWORD, result,
                                              "mulr.h");
    case MULRH_DC_DA_DBUU_N:
      len = 4;
      return Instruction::TextOpDcDaDbuuN_RR1(data, len, HWORD, result,
                                              "mulr.h");
    default:
      LogDebug("0x%lx: Mulrh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Mulrq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x93_RR1) {
    LogDebug("0x%lx: Mulrq::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MULRQ_DC_DAL_DBL_N:
      len = 4;
      return Instruction::TextOpDcDalDblN_RR1(data, len, WORD, result,
                                              "mulr.q");
    case MULRQ_DC_DAU_DBU_N:
      len = 4;
      return Instruction::TextOpDcDauDbuN_RR1(data, len, WORD, result,
                                              "mulr.q");
    default:
      LogDebug("0x%lx: Mulrq::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Nand::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case NAND_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "nand", true);
        default:
          LogDebug("0x%lx: Nand::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case NAND_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "nand");
        default:
          LogDebug("0x%lx: Nand::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Nand::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Nandt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x07_BIT || op2 != NANDT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Nandt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                               "nand.t");
}

bool Ne::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case NE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "ne");
        default:
          LogDebug("0x%lx: Ne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case NE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "ne");
        default:
          LogDebug("0x%lx: Ne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ne::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Nea::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x01_RR || op2 != NEA_DC_AA_AB) {
    LogDebug("0x%lx: Nea::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcAaAb_RR(data, len, WORD, result, "ne.a");
}

bool Neza::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x01_RR || op2 != NEZA_DC_AA) {
    LogDebug("0x%lx: Neza::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcAa_RR(data, len, WORD, result, "nez.a");
}

bool Nop::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x00_SR:
      switch (op2) {
        case NOP16:
          len = 2;
          return Instruction::TextOp_SR(data, len, WORD, result, "nop");
        default:
          LogDebug("0x%lx: Ne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0D_SYS:
      switch (op2) {
        case NOP32:
          len = 4;
          return Instruction::TextOp_SYS(data, len, WORD, result, "nop");
        default:
          LogDebug("0x%lx: Nop::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Nop::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Nor::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case NOR_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "nor", true);
        default:
          LogDebug("0x%lx: Nor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case NOR_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "nor");
        default:
          LogDebug("0x%lx: Nor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Nor::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Nort::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x87_BIT || op2 != NORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Nort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                               "nor.t");
}

bool Not::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x46_SR || op2 != NOT_DA) {
    LogDebug("0x%lx: Not::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 2;
  return Instruction::TextOpDa_SR(data, len, WORD, result, "not");
}

bool Or::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OR_D15_CONST8:
      len = 2;
      return Instruction::TextOpD15Const8_SC(data, len, WORD, result, "or");
    case OR_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "or");
    case OP1_x8F_RC:
      switch (op2) {
        case OR_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "or",
                                                  true);
        default:
          LogDebug("0x%lx: Or::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case OR_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "or");
        default:
          LogDebug("0x%lx: Or::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Or::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool OrAndtAndntNortOrt::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                              uint64_t addr, size_t& len,
                              std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xC7_BIT) {
    LogDebug("0x%lx: OrAndtAndntNortOrt::%s received invalid opcode 1 0x%x",
             addr, __func__, op1);
    return false;
  }
  switch (op2) {
    case ORANDT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "or.and.t");
    case ORANDNT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "or.andn.t");
    case ORNORT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "or.nor.t");
    case ORORT_DC_DA_POS1_DB_POS2:
      len = 4;
      return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                   "or.or.t");
    default:
      LogDebug("0x%lx: OrAndtAndntNortOrt::%s received invalid opcode 2 0x%x",
               addr, __func__, op2);
      return false;
  }
}

bool Oreq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case OREQ_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "or.eq");
        default:
          LogDebug("0x%lx: Oreq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case OREQ_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "or.eq");
        default:
          LogDebug("0x%lx: Oreq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Oreq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool OrgeGeu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ORGE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "or.ge");
        case ORGEU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "or.ge.u", true);
        default:
          LogDebug("0x%lx: OrgeGeu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ORGE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "or.ge");
        case ORGEU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "or.ge.u");
        default:
          LogDebug("0x%lx: OrgeGeu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: OrgeGeu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool OrltLtu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ORLT_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "or.lt");
        case ORLTU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "or.lt.u", true);
        default:
          LogDebug("0x%lx: OrltLtu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ORLT_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "or.lt");
        case ORLTU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "or.lt.u");
        default:
          LogDebug("0x%lx: OrltLtu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: OrltLtu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Orne::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ORNE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "or.ne");
        default:
          LogDebug("0x%lx: Orne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ORNE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "or.ne");
        default:
          LogDebug("0x%lx: Orne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Orne::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ort::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x87_BIT || op2 != ORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Ort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result, "or.t");
}

bool Orn::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case ORN_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "orn", true);
        default:
          LogDebug("0x%lx: Orn::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case ORN_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "orn");
        default:
          LogDebug("0x%lx: Orn::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Orn::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ornt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x87_BIT || op2 != ORNT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Ornt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                               "orn.t");
}

bool Pack::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR || op2 != PACK_DC_ED_DA) {
    LogDebug("0x%lx: Pack::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcEdDa_RRR(data, len, WORD, result, "pack");
}

bool Parity::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != PARITY_DC_DA) {
    LogDebug("0x%lx: Parity::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "parity");
}

// TCv1.6 ISA only
bool RestoreDa::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len,
                     std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != RESTORE_DA) {
    LogDebug("0x%lx: RestoreDa::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDa_SYS(data, len, WORD, result, "restore");
}

bool Ret::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x00_SR:
      switch (op2) {
        case RET16:
          len = 2;
          return Instruction::TextOp_SR(data, len, WORD, result, "ret");
        default:
          LogDebug("0x%lx: Ret::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0D_SYS:
      switch (op2) {
        case RET32:
          len = 4;
          return Instruction::TextOp_SYS(data, len, WORD, result, "ret");
        default:
          LogDebug("0x%lx: Ret::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Ret::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Rfe::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x00_SR:
      switch (op2) {
        case RFE16:
          len = 2;
          return Instruction::TextOp_SR(data, len, WORD, result, "rfe");
        default:
          LogDebug("0x%lx: Rfe::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0D_SYS:
      switch (op2) {
        case RFE32:
          len = 4;
          return Instruction::TextOp_SYS(data, len, WORD, result, "rfe");
        default:
          LogDebug("0x%lx: Rfe::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Rfe::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Rfm::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != RFM) {
    LogDebug("0x%lx: Rfm::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "rfm");
}

bool Rslcx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != RSLCX) {
    LogDebug("0x%lx: Rslcx::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "rslcx");
}

bool Rstv::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x2F_SYS || op2 != RSTV) {
    LogDebug("0x%lx: Rstv::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "rstv");
}

bool Rsub::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case RSUB_DA:
          len = 2;
          return Instruction::TextOpDa_SR(data, len, WORD, result, "rsub");
        default:
          LogDebug("0x%lx: Rsub::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x8B_RC:
      switch (op2) {
        case RSUB_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "rsub");
        default:
          LogDebug("0x%lx: Rsub::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Rsub::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Rsubs::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x8B_RC) {
    LogDebug("0x%lx: Rsubs::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case RSUBS_DC_DA_CONST9:
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result, "rsubs");
    case RSUBSU_DC_DA_CONST9:
      len = 4;
      return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                              "rsubs.u", true);
    default:
      LogDebug("0x%lx: Rsubs::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Satb::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATB_DA:
          len = 2;
          return Instruction::TextOpDa_SR(data, len, BYTE, result, "sat.b");
        default:
          LogDebug("0x%lx: Satb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATB_DC_DA:
          len = 4;
          return Instruction::TextOpDcDa_RR(data, len, BYTE, result, "sat.b");
        default:
          LogDebug("0x%lx: Satb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Satb::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Satbu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATBU_DA:
          len = 2;
          return Instruction::TextOpDa_SR(data, len, BYTE, result, "sat.bu");
        default:
          LogDebug("0x%lx: Satbu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATBU_DC_DA:
          len = 4;
          return Instruction::TextOpDcDa_RR(data, len, BYTE, result, "sat.bu");
        default:
          LogDebug("0x%lx: Satbu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Satbu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Sath::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATH_DA:
          len = 2;
          return Instruction::TextOpDa_SR(data, len, HWORD, result, "sat.h");
        default:
          LogDebug("0x%lx: Sath::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATH_DC_DA:
          len = 4;
          return Instruction::TextOpDcDa_RR(data, len, HWORD, result, "sat.h");
        default:
          LogDebug("0x%lx: Sath::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sath::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Sathu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATHU_DA:
          len = 2;
          return Instruction::TextOpDa_SR(data, len, HWORD, result, "sat.hu");
        default:
          LogDebug("0x%lx: Sathu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATHU_DC_DA:
          len = 4;
          return Instruction::TextOpDcDa_RR(data, len, HWORD, result, "sat.hu");
        default:
          LogDebug("0x%lx: Sathu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sathu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Sel::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xAB_RCR:
      switch (op2) {
        case SEL_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "sel");
        default:
          LogDebug("0x%lx: Sel::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x2B_RRR:
      switch (op2) {
        case SEL_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result,
                                                 "sel");
        default:
          LogDebug("0x%lx: Sel::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sel::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Seln::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xAB_RCR:
      switch (op2) {
        case SELN_DC_DD_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDdDaConst9_RCR(data, len, WORD, result,
                                                     "seln");
        default:
          LogDebug("0x%lx: Seln::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x2B_RRR:
      switch (op2) {
        case SELN_DC_DD_DA_DB:
          len = 4;
          return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result,
                                                 "seln");
        default:
          LogDebug("0x%lx: Seln::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Seln::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Sh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case SH_DA_CONST4:
      len = 2;
      return Instruction::TextOpDaConst4_SRC(data, len, WORD, result, "sh");
    case OP1_x8F_RC:
      switch (op2) {
        case SH_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sh");
        default:
          LogDebug("0x%lx: Sh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case SH_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "sh");
        default:
          LogDebug("0x%lx: Sh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sh::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Sheq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHEQ_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sh.eq");
        default:
          LogDebug("0x%lx: Sheq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHEQ_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "sh.eq");
        default:
          LogDebug("0x%lx: Sheq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sheq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool ShgeGeu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHGE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sh.ge");
        case SHGEU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sh.ge.u", true);
        default:
          LogDebug("0x%lx: ShgeGeu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHGE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "sh.ge");
        case SHGEU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "sh.ge.u");
        default:
          LogDebug("0x%lx: ShgeGeu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: ShgeGeu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Shh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case SHH_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, HWORD, result,
                                                  "sh.h");
        default:
          LogDebug("0x%lx: Shh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case SHH_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "sh.h");
        default:
          LogDebug("0x%lx: Shh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Shh::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool ShltLtu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHLT_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sh.lt");
        case SHLTU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sh.lt.u", true);
        default:
          LogDebug("0x%lx: ShltLtu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHLT_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "sh.lt");
        case SHLTU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "sh.lt.u");
        default:
          LogDebug("0x%lx: ShltLtu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: ShltLtu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Shne::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHNE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sh.ne");
        default:
          LogDebug("0x%lx: Shne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHNE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "sh.ne");
        default:
          LogDebug("0x%lx: Shne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Shne::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool ShAndOrNorXorT::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                          uint64_t addr, size_t& len,
                          std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x27_BIT:
      switch (op2) {
        case SHANDT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.and.t");
        case SHANDNT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.andn.t");
        case SHNORT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.nor.t");
        case SHORT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.or.t");
        default:
          LogDebug("0x%lx: ShAndOrNorXorT::%s received invalid opcode 2 0x%x",
                   addr, __func__, op2);
          return false;
      }
    case OP1_xA7_BIT:
      switch (op2) {
        case SHNANDT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.nand.t");
        case SHORNT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.orn.t");
        case SHXNORT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.xnor.t");
        case SHXORT_DC_DA_POS1_DB_POS2:
          len = 4;
          return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                                       "sh.xor.t");
        default:
          LogDebug("0x%lx: ShAndOrNorXorT::%s received invalid opcode 2 0x%x",
                   addr, __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: ShAndOrNorXorT::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Sha::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case SHA_DA_CONST4:
      len = 2;
      return Instruction::TextOpDaConst4_SRC(data, len, WORD, result, "sha");
    case OP1_x8F_RC:
      switch (op2) {
        case SHA_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "sha");
        default:
          LogDebug("0x%lx: Sha::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case SHA_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "sha");
        default:
          LogDebug("0x%lx: Sha::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sha::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Shah::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case SHAH_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, HWORD, result,
                                                  "sha.h");
        default:
          LogDebug("0x%lx: Shah::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case SHAH_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result,
                                              "sha.h");
        default:
          LogDebug("0x%lx: Shah::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Shah::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Shas::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case SHAS_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, HWORD, result,
                                                  "shas");
        default:
          LogDebug("0x%lx: Shas::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case SHAS_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "shas");
        default:
          LogDebug("0x%lx: Shas::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Shas::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Sta::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case STA_AB_AA:
      len = 2;
      return Instruction::TextOpAbAa_SSR(data, len, WORD, result, "st.a",
                                         ADDRMODE_REGULAR);
    case STA_AB_AA_POSTINC:
      len = 2;
      return Instruction::TextOpAbAa_SSR(data, len, WORD, result, "st.a",
                                         ADDRMODE_POSTINC);
    case STA_A10_CONST8_A15:
      len = 2;
      return Instruction::TextOpA10Const8A15_SC(data, len, WORD, result,
                                                "st.a");
    case STA_AB_OFF4_A15:
      len = 2;
      return Instruction::TextOpAbOff4A15_SRO(data, len, WORD, result, "st.a");
    case STA_A15_OFF4_AA:
      len = 2;
      return Instruction::TextOpA15Off4Aa_SSRO(data, len, WORD, result, "st.a");
    case STA_AB_OFF16_AA:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpAbOff16Aa_BOL(data, len, WORD, result, "st.a");
    case OP1_xA5_ABS:
      switch (op2) {
        case STA_OFF18_AA:
          len = 4;
          return Instruction::TextOpOff18Aa_ABS(data, len, WORD, result,
                                                "st.a");
        default:
          LogDebug("0x%lx: Sta::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STA_AB_OFF10_AA:
          len = 4;
          return Instruction::TextOpAbOff10Aa_BO(data, len, WORD, result,
                                                 "st.a", ADDRMODE_REGULAR);
        case STA_AB_OFF10_AA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Aa_BO(data, len, WORD, result,
                                                 "st.a", ADDRMODE_POSTINC);
        case STA_AB_OFF10_AA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Aa_BO(data, len, WORD, result,
                                                 "st.a", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Sta::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STA_PB_AA:
          len = 4;
          return Instruction::TextOpPbAa_BO(data, len, WORD, result, "st.a",
                                            ADDRMODE_BREV);
        case STA_PB_OFF10_AA:
          len = 4;
          return Instruction::TextOpPbOff10Aa_BO(data, len, WORD, result,
                                                 "st.a", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Sta::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sta::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Stb::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case STB_AB_DA:
      len = 2;
      return Instruction::TextOpAbDa_SSR(data, len, BYTE, result, "st.b",
                                         ADDRMODE_REGULAR);
    case STB_AB_DA_POSTINC:
      len = 2;
      return Instruction::TextOpAbDa_SSR(data, len, BYTE, result, "st.b",
                                         ADDRMODE_POSTINC);
    case STB_AB_OFF4_D15:
      len = 2;
      return Instruction::TextOpAbOff4D15_SRO(data, len, BYTE, result, "st.b");
    case STB_A15_OFF4_DA:
      len = 2;
      return Instruction::TextOpA15Off4Da_SSRO(data, len, BYTE, result, "st.b");
    case STB_AB_OFF16_DA:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpAbOff16Da_BOL(data, len, BYTE, result, "st.b");
    case OP1_x25_ABS:
      switch (op2) {
        case STB_OFF18_DA:
          len = 4;
          return Instruction::TextOpOff18Da_ABS(data, len, BYTE, result,
                                                "st.b");
        default:
          LogDebug("0x%lx: Stb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STB_AB_OFF10_DA:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, BYTE, result,
                                                 "st.b", ADDRMODE_REGULAR);
        case STB_AB_OFF10_DA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, BYTE, result,
                                                 "st.b", ADDRMODE_POSTINC);
        case STB_AB_OFF10_DA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, BYTE, result,
                                                 "st.b", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Stb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STB_PB_DA:
          len = 4;
          return Instruction::TextOpPbDa_BO(data, len, BYTE, result, "st.b",
                                            ADDRMODE_BREV);
        case STB_PB_OFF10_DA:
          len = 4;
          return Instruction::TextOpPbOff10Da_BO(data, len, BYTE, result,
                                                 "st.b", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Stb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Stb::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Std::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xA5_ABS:
      switch (op2) {
        case STD_OFF18_EA:
          len = 4;
          return Instruction::TextOpOff18Ea_ABS(data, len, DWORD, result,
                                                "st.d");
        default:
          LogDebug("0x%lx: Std::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STD_AB_OFF10_EA:
          len = 4;
          return Instruction::TextOpAbOff10Ea_BO(data, len, DWORD, result,
                                                 "st.d", ADDRMODE_REGULAR);
        case STD_AB_OFF10_EA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Ea_BO(data, len, DWORD, result,
                                                 "st.d", ADDRMODE_POSTINC);
        case STD_AB_OFF10_EA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Ea_BO(data, len, DWORD, result,
                                                 "st.d", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Std::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STD_PB_EA:
          len = 4;
          return Instruction::TextOpPbEa_BO(data, len, DWORD, result, "st.d",
                                            ADDRMODE_BREV);
        case STD_PB_OFF10_EA:
          len = 4;
          return Instruction::TextOpPbOff10Ea_BO(data, len, DWORD, result,
                                                 "st.d", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Std::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Std::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Stda::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xA5_ABS:
      switch (op2) {
        case STDA_OFF18_PA:
          len = 4;
          return Instruction::TextOpOff18Pa_ABS(data, len, DWORD, result,
                                                "st.da");
        default:
          LogDebug("0x%lx: Stda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STDA_AB_OFF10_PA:
          len = 4;
          return Instruction::TextOpAbOff10Pa_BO(data, len, DWORD, result,
                                                 "st.da", ADDRMODE_REGULAR);
        case STDA_AB_OFF10_PA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Pa_BO(data, len, DWORD, result,
                                                 "st.da", ADDRMODE_POSTINC);
        case STDA_AB_OFF10_PA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Pa_BO(data, len, DWORD, result,
                                                 "st.da", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Stda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STDA_PB_PA:
          len = 4;
          return Instruction::TextOpPbPa_BO(data, len, DWORD, result, "st.da",
                                            ADDRMODE_BREV);
        case STDA_PB_OFF10_PA:
          len = 4;
          return Instruction::TextOpPbOff10Pa_BO(data, len, DWORD, result,
                                                 "st.da", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Stda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Stda::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Sth::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case STH_AB_DA:
      len = 2;
      return Instruction::TextOpAbDa_SSR(data, len, HWORD, result, "st.h",
                                         ADDRMODE_REGULAR);
    case STH_AB_DA_POSTINC:
      len = 2;
      return Instruction::TextOpAbDa_SSR(data, len, HWORD, result, "st.h",
                                         ADDRMODE_POSTINC);
    case STH_AB_OFF4_D15:
      len = 2;
      return Instruction::TextOpAbOff4D15_SRO(data, len, HWORD, result, "st.h");
    case STH_A15_OFF4_DA:
      len = 2;
      return Instruction::TextOpA15Off4Da_SSRO(data, len, HWORD, result,
                                               "st.h");
    case STH_AB_OFF16_DA:  // TCv1.6 ISA only
      len = 4;
      return Instruction::TextOpAbOff16Da_BOL(data, len, HWORD, result, "st.h");
    case OP1_x25_ABS:
      switch (op2) {
        case STH_OFF18_DA:
          len = 4;
          return Instruction::TextOpOff18Da_ABS(data, len, HWORD, result,
                                                "st.h");
        default:
          LogDebug("0x%lx: Sth::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STH_AB_OFF10_DA:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, HWORD, result,
                                                 "st.h", ADDRMODE_REGULAR);
        case STH_AB_OFF10_DA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, HWORD, result,
                                                 "st.h", ADDRMODE_POSTINC);
        case STH_AB_OFF10_DA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, HWORD, result,
                                                 "st.h", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Sth::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STH_PB_DA:
          len = 4;
          return Instruction::TextOpPbDa_BO(data, len, HWORD, result, "st.h",
                                            ADDRMODE_BREV);
        case STH_PB_OFF10_DA:
          len = 4;
          return Instruction::TextOpPbOff10Da_BO(data, len, HWORD, result,
                                                 "st.h", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Sth::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sth::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Stq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x65_ABS:
      switch (op2) {
        case STQ_OFF18_DA:
          len = 4;
          return Instruction::TextOpOff18Da_ABS(data, len, HWORD, result,
                                                "st.q");
        default:
          LogDebug("0x%lx: Stq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STQ_AB_OFF10_DA:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, HWORD, result,
                                                 "st.q", ADDRMODE_REGULAR);
        case STQ_AB_OFF10_DA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, HWORD, result,
                                                 "st.q", ADDRMODE_POSTINC);
        case STQ_AB_OFF10_DA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, HWORD, result,
                                                 "st.q", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Stq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STQ_PB_DA:
          len = 4;
          return Instruction::TextOpPbDa_BO(data, len, HWORD, result, "st.q",
                                            ADDRMODE_BREV);
        case STQ_PB_OFF10_DA:
          len = 4;
          return Instruction::TextOpPbOff10Da_BO(data, len, HWORD, result,
                                                 "st.q", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Stq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Stq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Stt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xD5_ABSB || op2 != STT_OFF18_BPOS3_B) {
    LogDebug("0x%lx: Stt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpOff18Bpos3B_ABSB(data, len, WORD, result, "st.t");
}

bool Stw::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case STW_AB_DA:
      len = 2;
      return Instruction::TextOpAbDa_SSR(data, len, WORD, result, "st.w",
                                         ADDRMODE_REGULAR);
    case STW_AB_DA_POSTINC:
      len = 2;
      return Instruction::TextOpAbDa_SSR(data, len, WORD, result, "st.w",
                                         ADDRMODE_POSTINC);
    case STW_A10_CONST8_D15:
      len = 2;
      return Instruction::TextOpA10Const8D15_SC(data, len, WORD, result,
                                                "st.w");
    case STW_AB_OFF4_D15:
      len = 2;
      return Instruction::TextOpAbOff4D15_SRO(data, len, WORD, result, "st.w");
    case STW_A15_OFF4_DA:
      len = 2;
      return Instruction::TextOpA15Off4Da_SSRO(data, len, WORD, result, "st.w");
    case STW_AB_OFF16_DA:
      len = 4;
      return Instruction::TextOpAbOff16Da_BOL(data, len, WORD, result, "st.w");
    case OP1_xA5_ABS:
      switch (op2) {
        case STW_OFF18_DA:
          len = 4;
          return Instruction::TextOpOff18Da_ABS(data, len, WORD, result,
                                                "st.w");
        default:
          LogDebug("0x%lx: Stw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STW_AB_OFF10_DA:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, WORD, result,
                                                 "st.w", ADDRMODE_REGULAR);
        case STW_AB_OFF10_DA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, WORD, result,
                                                 "st.w", ADDRMODE_POSTINC);
        case STW_AB_OFF10_DA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, WORD, result,
                                                 "st.w", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Stw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STW_PB_DA:
          len = 4;
          return Instruction::TextOpPbDa_BO(data, len, WORD, result, "st.w",
                                            ADDRMODE_BREV);
        case STW_PB_OFF10_DA:
          len = 4;
          return Instruction::TextOpPbOff10Da_BO(data, len, WORD, result,
                                                 "st.w"),
                 ADDRMODE_CIRC;
        default:
          LogDebug("0x%lx: Stw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Stw::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Stlcx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x15_ABS:
      switch (op2) {
        case STLCX_OFF18:
          len = 4;
          return Instruction::TextOpOff18_ABS(data, len, WORD, result, "stlcx");
        default:
          LogDebug("0x%lx: Stlcx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case STLCX_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "stlcx");
        default:
          LogDebug("0x%lx: Stlcx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Stlcx::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Stucx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x15_ABS:
      switch (op2) {
        case STUCX_OFF18:
          len = 4;
          return Instruction::TextOpOff18_ABS(data, len, WORD, result, "stucx");
        default:
          LogDebug("0x%lx: Stucx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case STUCX_AB_OFF10:
          len = 4;
          return Instruction::TextOpAbOff10_BO(data, len, WORD, result,
                                               "stucx");
        default:
          LogDebug("0x%lx: Stucx::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Stucx::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Sub::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case SUB_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "sub");
    case SUB_DA_D15_DB:
      len = 2;
      return Instruction::TextOpDaD15Db_SRR(data, len, WORD, result, "sub");
    case SUB_D15_DA_DB:
      len = 2;
      return Instruction::TextOpD15DaDb_SRR(data, len, WORD, result, "sub");
    case OP1_x0B_RR:
      switch (op2) {
        case SUB_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "sub");
        default:
          LogDebug("0x%lx: Sub::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Sub::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Suba::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case SUBA_A10_CONST8:
      len = 2;
      return Instruction::TextOpA10Const8_SC(data, len, WORD, result, "sub.a");
    case OP1_x01_RR:
      switch (op2) {
        case SUBA_AC_AA_AB:
          len = 4;
          return Instruction::TextOpAcAaAb_RR(data, len, WORD, result, "sub.a");
        default:
          LogDebug("0x%lx: Suba::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Suba::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Subbh::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Subbh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case SUBB_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, BYTE, result, "sub.b");
    case SUBH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, HWORD, result, "sub.h");
    default:
      LogDebug("0x%lx: Subh::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Subc::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR || op2 != SUBC_DC_DA_DB) {
    LogDebug("0x%lx: Subc::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "subc");
}

bool Subsu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case SUBS_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "subs");
    case OP1_x0B_RR:
      switch (op2) {
        case SUBS_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "subs");
        case SUBSU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "subs.u");
        default:
          LogDebug("0x%lx: Subsu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Subsu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool SubshHu::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: SubshHu::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case SUBSH_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "subs.h");
    case SUBSHU_DC_DA_DB:
      len = 4;
      return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "subs.hu");
    default:
      LogDebug("0x%lx: SubshHu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Subx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0B_RR || op2 != SUBX_DC_DA_DB) {
    LogDebug("0x%lx: Subx::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "subx");
}

bool Svlcx::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != SVLCX) {
    LogDebug("0x%lx: Svlcx::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "svlcx");
}

bool Swapw::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_xE5_ABS:
      switch (op2) {
        case SWAPW_OFF18_DA:
          len = 4;
          return Instruction::TextOpOff18Da_ABS(data, len, WORD, result,
                                                "swap.w");
        default:
          LogDebug("0x%lx: Swapw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case SWAPW_AB_OFF10_DA:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, WORD, result,
                                                 "swap.w", ADDRMODE_REGULAR);
        case SWAPW_AB_OFF10_DA_POSTINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, WORD, result,
                                                 "swap.w", ADDRMODE_POSTINC);
        case SWAPW_AB_OFF10_DA_PREINC:
          len = 4;
          return Instruction::TextOpAbOff10Da_BO(data, len, WORD, result,
                                                 "swap.w", ADDRMODE_PREINC);
        default:
          LogDebug("0x%lx: Swapw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x69_BO:
      switch (op2) {
        case SWAPW_PB_DA:
          len = 4;
          return Instruction::TextOpPbDa_BO(data, len, WORD, result, "swap.w",
                                            ADDRMODE_BREV);
        case SWAPW_PB_OFF10_DA:
          len = 4;
          return Instruction::TextOpPbOff10Da_BO(data, len, WORD, result,
                                                 "swap.w", ADDRMODE_CIRC);
        default:
          LogDebug("0x%lx: Swapw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Subsu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Syscall::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_xAD_RC || op2 != SYSCALL_CONST9) {
    LogDebug("0x%lx: Syscall::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpConst9_RC(data, len, WORD, result, "syscall");
}

bool Trapsv::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != TRAPSV) {
    LogDebug("0x%lx: Trapsv::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "trapsv");
}

bool Trapv::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != TRAPV) {
    LogDebug("0x%lx: Trapv::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "trapv");
}

bool Unpack::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != UNPACK_EC_DA) {
    LogDebug("0x%lx: Unpack::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpEcDa_RR(data, len, WORD, result, "unpack");
}

bool Xnor::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case XNOR_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xnor", true);
        default:
          LogDebug("0x%lx: Xnor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case XNOR_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "xnor");
        default:
          LogDebug("0x%lx: Xnor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Xnor::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Xnort::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x07_BIT || op2 != XNORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Xnort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                               "xnor.t");
}

bool Xor::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case XOR_DA_DB:
      len = 2;
      return Instruction::TextOpDaDb_SRR(data, len, WORD, result, "xor");
    case OP1_x8F_RC:
      switch (op2) {
        case XOR_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xor", true);
        default:
          LogDebug("0x%lx: Xor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case XOR_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "xor");
        default:
          LogDebug("0x%lx: Xor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Xor::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Xoreq::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XOREQ_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xor.eq");
        default:
          LogDebug("0x%lx: Xoreq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XOREQ_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "xor.eq");
        default:
          LogDebug("0x%lx: Xoreq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Xoreq::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Xorge::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XORGE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xor.ge");
        case XORGEU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xor.ge.u", true);
        default:
          LogDebug("0x%lx: Xorge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XORGE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "xor.ge");
        case XORGEU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "xor.ge.u");
        default:
          LogDebug("0x%lx: Xorge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Xorge::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Xorlt::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XORLT_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xor.lt");
        case XORLTU_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xor.lt.u", true);
        default:
          LogDebug("0x%lx: Xorlt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XORLT_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "xor.lt");
        case XORLTU_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "xor.lt.u");
        default:
          LogDebug("0x%lx: Xorlt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Xorlt::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Xorne::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XORNE_DC_DA_CONST9:
          len = 4;
          return Instruction::TextOpDcDaConst9_RC(data, len, WORD, result,
                                                  "xor.ne");
        default:
          LogDebug("0x%lx: Xorne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XORNE_DC_DA_DB:
          len = 4;
          return Instruction::TextOpDcDaDb_RR(data, len, WORD, result,
                                              "xor.ne");
        default:
          LogDebug("0x%lx: Xorne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    default:
      LogDebug("0x%lx: Xorne::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Xort::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x07_BIT || op2 != XORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Xort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaPos1DbPos2_BIT(data, len, WORD, result,
                                               "xor.t");
}

/* Floating Point Instructions */
bool Addf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR || op2 != ADDF_DC_DD_DA) {
    LogDebug("0x%lx: Addf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDdDa_RRR(data, len, WORD, result, "add.f");
}

bool Cmpf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != CMPF_DC_DA_DB) {
    LogDebug("0x%lx: Cmpf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "cmp.f");
}

bool Divf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != DIVF_DC_DA_DB) {
    LogDebug("0x%lx: Divf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "div.f");
}

bool Ftoi::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != FTOI_DC_DA) {
    LogDebug("0x%lx: Ftoi::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "ftoi");
}

bool Ftoiz::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != FTOIZ_DC_DA) {
    LogDebug("0x%lx: Ftoiz::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "ftoiz");
}

bool Ftoq31::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != FTOQ31_DC_DA_DB) {
    LogDebug("0x%lx: Ftoq31::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "ftoq31");
}

bool Ftoq31z::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != FTOQ31Z_DC_DA_DB) {
    LogDebug("0x%lx: Ftoq31z::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "ftoq31z");
}

bool Ftou::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != FTOU_DC_DA) {
    LogDebug("0x%lx: Ftou::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "ftou");
}

bool Ftouz::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != FTOUZ_DC_DA) {
    LogDebug("0x%lx: Ftouz::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "ftouz");
}

bool Itof::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != ITOF_DC_DA) {
    LogDebug("0x%lx: Itof::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "Itof");
}

bool Maddf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR || op2 != MADDF_DC_DD_DA_DB) {
    LogDebug("0x%lx: Maddf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result, "madd.f");
}

bool Msubf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR || op2 != MSUBF_DC_DD_DA_DB) {
    LogDebug("0x%lx: Msubf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDdDaDb_RRR(data, len, WORD, result, "msub.f");
}

bool Mulf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != MULF_DC_DA_DB) {
    LogDebug("0x%lx: Mulf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "mul.f");
}

bool Q31tof::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != Q31TOF_DC_DA_DB) {
    LogDebug("0x%lx: Q31tof::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDaDb_RR(data, len, WORD, result, "q31tof");
}

bool Qseedf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != QSEEDF_DC_DA) {
    LogDebug("0x%lx: Qseedf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "qseed.f");
}

bool Subf::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x6B_RRR || op2 != SUBF_DC_DD_DA) {
    LogDebug("0x%lx: Subf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDdDa_RRR(data, len, WORD, result, "sub.f");
}

bool Updfl::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != UPDFL_DA) {
    LogDebug("0x%lx: Updfl::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDa_RR(data, len, WORD, result, "updfl");
}

bool Utof::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x4B_RR || op2 != UTOF_DC_DA) {
    LogDebug("0x%lx: Utof::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDcDa_RR(data, len, WORD, result, "utof");
}

/* MMU Instructions */
bool Tlbdemap::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x75_RR || op2 != TLBDEMAP_DA) {
    LogDebug("0x%lx: Tlbdemap::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDa_RR(data, len, WORD, result, "tlbdemap");
}

bool Tlbflush::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len,
                    std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x75_RR) {
    LogDebug("0x%lx: Tlbflush::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case TLBFLUSHA:
      len = 4;
      return Instruction::TextOp_RR(data, len, WORD, result, "tlbflush.a");
    case TLBFLUSHB:
      len = 4;
      return Instruction::TextOp_RR(data, len, WORD, result, "tlbflush.b");
    default:
      LogDebug("0x%lx: Tlbflush::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Tlbmap::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x75_RR || op2 != TLBMAP_EA) {
    LogDebug("0x%lx: Tlbmap::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpEa_RR(data, len, WORD, result, "tlbmap");
}

bool Tlbprobea::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len,
                     std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x75_RR || op2 != TLBPROBEA_DA) {
    LogDebug("0x%lx: Tlbprobea::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDa_RR(data, len, WORD, result, "tlbprobe.a");
}

bool Tlbprobei::Text(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len,
                     std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x75_RR || op2 != TLBPROBEI_DA) {
    LogDebug("0x%lx: Tlbprobei::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOpDa_RR(data, len, WORD, result, "tlbprobe.i");
}

// TCv1.6 ISA only
bool Wait::Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, std::vector<InstructionTextToken>& result) {
  if (op1 != OP1_x0D_SYS || op2 != WAIT) {
    LogDebug("0x%lx: Wait::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  len = 4;
  return Instruction::TextOp_SYS(data, len, WORD, result, "wait");
}
}  // namespace Tricore

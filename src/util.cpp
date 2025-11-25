// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include "util.h"

#include <cstddef>
#include <cstdint>

#include "instructions.h"
#include "opcodes.h"
#include "registers.h"

using namespace std;

#define ITEXT(m)                            \
  result.emplace_back(InstructionToken, m); \
  result.emplace_back(TextToken, " ");
#define REGTOA(reg) reg += 16;
#define REGTOE(reg) reg += 32;
#define REGTOP(reg) reg += 48;

namespace Tricore {
const char* Instruction::RegToStr(const uint32_t rid) {
  switch (rid) {
    /* Full-Width General Purpose Data Registers */
    case TRICORE_REG_D0:
      return "d0";
    case TRICORE_REG_D1:
      return "d1";
    case TRICORE_REG_D2:
      return "d2";
    case TRICORE_REG_D3:
      return "d3";
    case TRICORE_REG_D4:
      return "d4";
    case TRICORE_REG_D5:
      return "d5";
    case TRICORE_REG_D6:
      return "d6";
    case TRICORE_REG_D7:
      return "d7";
    case TRICORE_REG_D8:
      return "d8";
    case TRICORE_REG_D9:
      return "d9";
    case TRICORE_REG_D10:
      return "d10";
    case TRICORE_REG_D11:
      return "d11";
    case TRICORE_REG_D12:
      return "d12";
    case TRICORE_REG_D13:
      return "d13";
    case TRICORE_REG_D14:
      return "d14";
    case TRICORE_REG_D15:
      return "d15";

    /* Full-Width General Purpose Address Registers */
    case TRICORE_REG_A0:
      return "a0";
    case TRICORE_REG_A1:
      return "a1";
    case TRICORE_REG_A2:
      return "a2";
    case TRICORE_REG_A3:
      return "a3";
    case TRICORE_REG_A4:
      return "a4";
    case TRICORE_REG_A5:
      return "a5";
    case TRICORE_REG_A6:
      return "a6";
    case TRICORE_REG_A7:
      return "a7";
    case TRICORE_REG_A8:
      return "a8";
    case TRICORE_REG_A9:
      return "a9";
    case TRICORE_REG_A10:
      return "a10";
    case TRICORE_REG_A11:
      return "a11";
    case TRICORE_REG_A12:
      return "a12";
    case TRICORE_REG_A13:
      return "a13";
    case TRICORE_REG_A14:
      return "a14";
    case TRICORE_REG_A15:
      return "a15";

    /* 64-bit Data Registers */
    case TRICORE_REG_E0:
      return "d0/d1";
    case TRICORE_REG_E2:
      return "d2/d3";
    case TRICORE_REG_E4:
      return "d4/d5";
    case TRICORE_REG_E6:
      return "d6/d7";
    case TRICORE_REG_E8:
      return "d8/d9";
    case TRICORE_REG_E10:
      return "d10/d11";
    case TRICORE_REG_E12:
      return "d12/d13";
    case TRICORE_REG_E14:
      return "d14/d15";

    /* 64-bit Address Registers */
    case TRICORE_REG_P0:
      return "a0/a1";
    case TRICORE_REG_P2:
      return "a2/a3";
    case TRICORE_REG_P4:
      return "a4/a5";
    case TRICORE_REG_P6:
      return "a6/a7";
    case TRICORE_REG_P8:
      return "a8/a9";
    case TRICORE_REG_P10:
      return "a10/a11";
    case TRICORE_REG_P12:
      return "a12/a13";
    case TRICORE_REG_P14:
      return "a14/a15";

    /* Core Special Function Registers */
    case TRICORE_REG_PCXI:
      return "pcxi";
    case TRICORE_REG_PSW:
      return "psw";
    case TRICORE_REG_PC:
      return "pc";
    case TRICORE_REG_SYSCON:
      return "syscon";

    case TRICORE_REG_CPUID:
      return "cpuid";
    case TRICORE_REG_BIV:
      return "biv";
    case TRICORE_REG_BTV:
      return "btv";
    case TRICORE_REG_ISP:
      return "isp";
    case TRICORE_REG_ICR:
      return "icr";
    case TRICORE_REG_FCX:
      return "fcx";
    case TRICORE_REG_LCX:
      return "lcx";
    case TRICORE_REG_DIEAR:
      return "diear";
    case TRICORE_REG_DIETR:
      return "dietr";
    case TRICORE_REG_CCDIER:
      return "ccdier";
    case TRICORE_REG_MIECON:
      return "miecon";
    case TRICORE_REG_PIEAR:
      return "piear";
    case TRICORE_REG_PIETR:
      return "pietr";
    case TIRCORE_REG_CCPIER:
      return "ccpier";
    case TRICORE_REG_DBGSR:
      return "dbgsr";
    case TRICORE_REG_EXEVT:
      return "exevt";
    case TRICORE_REG_CREVT:
      return "crevt";
    case TRICORE_REG_SWEVT:
      return "swevt";
    case TRICORE_REG_TR0EVT:
      return "tr0evt";
    case TRICORE_REG_TR1EVT:
      return "tr1evt";
    case TRICORE_REG_DMS:
      return "dms";
    case TRICORE_REG_DCX:
      return "dcx";
    case TRICORE_REG_DBGTCR:
      return "dbgtcr";
    case TRICORE_REG_CCTRL:
      return "cctrl";
    case TRICORE_REG_CCNT:
      return "ccnt";
    case TRICORE_REG_ICNT:
      return "icnt";
    case TRICORE_REG_M1CNT:
      return "m1cnt";
    case TRICORE_REG_M2CNT:
      return "m2cnt";
    case TRICORE_REG_M3CNT:
      return "m3cnt";
    case TRICIRE_REG_COMPAT:
      return "compat";
    case TRICORE_REG_BMACON:
      return "bmacon";
    case TRICORE_REG_SMACON:
      return "smacon";
    case TRICORE_REG_FPUTC:
      return "fputc";
    case TRICORE_REG_FPUTPC:
      return "fputpc";
    case TRICORE_REG_FPUTOPC:
      return "fputopc";
    case TRICORE_REG_FPUTSRC1:
      return "fputsrc1";
    case TRICORE_REG_FPUTSRC2:
      return "fputsrc2";
    case TRICORE_REG_FPUTSRC3:
      return "fputsrc3";
    case TRICORE_REG_FPUIDR:
      return "fpuidr";

    default:
      return nullptr;
  }
}

uint32_t Instruction::ExtractSecondaryOpcode(uint8_t primaryOp,
                                             const uint8_t* data) {
  auto data16 = (const uint16_t*)data;
  auto data32 = (const uint32_t*)data;
  /* See TriCore Architecture Volume 2: Instruction Set, V1.3 & V1.3.1;
   * secion 2.2 - Opcode Formats */
  switch (primaryOp) {
    case OP1_x00_SR:
    case OP1_x46_SR:
    case OP1_x32_SR:
    case OP1_xDC_SR:
      return ((*data16) >> 12) & 0x0Fu;

    case OP1_x01_RR:
    case OP1_x0B_RR:
    case OP1_x0F_RR:
    case OP1_x2D_RR:
    case OP1_x4B_RR:
    case OP1_x75_RR:
      return ((*data32) >> 20) & 0xFFu;

    case OP1_x03_RRR2:
    case OP1_x23_RRR2:
      return ((*data32) >> 16) & 0xFFu;

    case OP1_x05_ABS:
    case OP1_x15_ABS:
    case OP1_x25_ABS:
    case OP1_x45_ABS:
    case OP1_x65_ABS:
    case OP1_x85_ABS:
    case OP1_xA5_ABS:
    case OP1_xC5_ABS:
    case OP1_xE5_ABS:
      return ((*data32) >> 26) & 0x03u;

    case OP1_x07_BIT:
    case OP1_x27_BIT:
    case OP1_x47_BIT:
    case OP1_x67_BIT:
    case OP1_x87_BIT:
    case OP1_xA7_BIT:
    case OP1_xC7_BIT:
      return ((*data32) >> 21) & 0x03u;

    case OP1_x09_BO:
    case OP1_x29_BO:
    case OP1_x49_BO:
    case OP1_x69_BO:
    case OP1_x89_BO:
    case OP1_xA9_BO:
      return ((*data32) >> 22) & 0x3Fu;

    case OP1_x0D_SYS:
    case OP1_x2F_SYS:
      return ((*data32) >> 22) & 0x3Fu;

    case OP1_x13_RCR:
    case OP1_x33_RCR:
    case OP1_xAB_RCR:
      return ((*data32) >> 21) & 0x07u;

    case OP1_x17_RRRR:
      return ((*data32) >> 21) & 0x07u;

    case OP1_x1F_BRR:
    case OP1_x3F_BRR:
    case OP1_x5F_BRR:
    case OP1_x7D_BRR:
    case OP1_x7F_BRR:
    case OP1_xBD_BRR:
    case OP1_xFD_BRR:
      return ((*data32) >> 31) & 0x01u;

    case OP1_x2B_RRR:
    case OP1_x6B_RRR:
      return ((*data32) >> 20) & 0x0Fu;

    case OP1_x37_RRPW:
    case OP1_x77_RRPW:
      return ((*data32) >> 21) & 0x03u;

    case OP1_x43_RRR1:
    case OP1_x63_RRR1:
    case OP1_x83_RRR1:
    case OP1_xA3_RRR1:
    case OP1_xC3_RRR1:
    case OP1_xE3_RRR1:
      return ((*data32) >> 18) & 0x3Fu;

    case OP1_x53_RC:
    case OP1_x8B_RC:
    case OP1_x8F_RC:
    case OP1_xAD_RC:
      return ((*data32) >> 21) & 0x7Fu;

    case OP1_x57_RRRW:
      return ((*data32) >> 21) & 0x07u;

    // smaller primary opcode (should have been handled in caller)
    case OP1_x6F_BRN:
      return ((*data32) >> 31) & 0x01u;

    case OP1_x73_RR2:
      return ((*data32) >> 16) & 0xFFFu;

    case OP1_x93_RR1:
    case OP1_xB3_RR1:
      return ((*data32) >> 18) & 0x3FFu;

    case OP1_x97_RCRR:
      return ((*data32) >> 21) & 0x07u;

    case OP1_x9F_BRC:
    case OP1_xBF_BRC:
    case OP1_xDF_BRC:
    case OP1_xFF_BRC:
      return ((*data32) >> 31) & 0x01u;

    case OP1_xB7_RCPW:
      return ((*data32) >> 21) & 0x03u;

    case OP1_xD5_ABSB:
      return ((*data32) >> 26) & 0x03u;

    case OP1_xD7_RCRW:
      return ((*data32) >> 21) & 0x07u;

    default:
      return 0xFFFF;  // This is not a valid secondary opcode
  }
}

void Instruction::ExtractOpfieldsSB(const uint8_t* data, uint8_t& op1,
                                    int8_t& disp8) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  op1 = raw & 0xFFu;
  disp8 = (int8_t)((raw >> 8) & 0xFFu);
}

void Instruction::ExtractOpfieldsSBC(const uint8_t* data, uint8_t& op1,
                                     int8_t& disp4, int8_t& const4) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  uint8_t tmp;
  op1 = raw & 0xFFu;

  tmp = (raw >> 8) & 0x0Fu;
  if (tmp & 0x8) {
    // sign extend (not using >> because that operator's behavior is
    // compiler-dependent)
    tmp |= 0xF0;
  }
  disp4 = (int8_t)tmp;

  tmp = (raw >> 12) & 0x0Fu;
  if (tmp & 0x8) {
    // sign extend (not using >> because that operator's behavior is
    // compiler-dependent)
    tmp |= 0xF0;
  }
  const4 = (int8_t)tmp;
}

void Instruction::ExtractOpfieldsSBR(const uint8_t* data, uint8_t& op1,
                                     int8_t& disp4, uint8_t& s2) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  uint8_t tmp;
  op1 = raw & 0xFFu;

  tmp = (raw >> 8) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  disp4 = (int8_t)tmp;

  s2 = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSBRN(const uint8_t* data, uint8_t& op1,
                                      int8_t& disp4, uint8_t& n) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  uint8_t tmp;
  op1 = raw & 0xFFu;

  tmp = (raw >> 8) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  disp4 = (int8_t)tmp;

  n = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSC(const uint8_t* data, uint8_t& op1,
                                    int8_t& const8) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  op1 = raw & 0xFFu;
  const8 = (int8_t)((raw >> 8) & 0xFFu);
}

void Instruction::ExtractOpfieldsSLR(const uint8_t* data, uint8_t& op1,
                                     uint8_t& d, uint8_t& s2) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  op1 = raw & 0xFFu;
  d = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSLRO(const uint8_t* data, uint8_t& op1,
                                      uint8_t& d, int8_t& off4) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  uint8_t tmp;
  op1 = raw & 0xFFu;
  d = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  off4 = (int8_t)tmp;
}

void Instruction::ExtractOpfieldsSR(const uint8_t* data, uint8_t& op1,
                                    uint8_t& s1d, uint8_t& op2) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  op1 = raw & 0xFFu;
  s1d = (raw >> 8) & 0x0Fu;
  op2 = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSRC(const uint8_t* data, uint8_t& op1,
                                     uint8_t& s1d, int8_t& const4) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  uint8_t tmp;
  op1 = raw & 0xFFu;
  s1d = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  const4 = (int8_t)tmp;
}

void Instruction::ExtractOpfieldsSRO(const uint8_t* data, uint8_t& op1,
                                     int8_t& off4, uint8_t& s2) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  uint8_t tmp;
  op1 = raw & 0xFFu;

  tmp = (raw >> 8) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  off4 = (int8_t)tmp;

  s2 = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSRR(const uint8_t* data, uint8_t& op1,
                                     uint8_t& s1d, uint8_t& s2) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  op1 = raw & 0xFFu;
  s1d = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSRRS(const uint8_t* data, uint8_t& op1,
                                      uint8_t& n, uint8_t& s1d, uint8_t& s2) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  op1 = raw & 0x3Fu;
  n = (raw >> 6) & 0x03u;
  s1d = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSSR(const uint8_t* data, uint8_t& op1,
                                     uint8_t& s1, uint8_t& s2) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  op1 = raw & 0xFFu;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSSRO(const uint8_t* data, uint8_t& op1,
                                      uint8_t& s1, int8_t& off4) {
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  uint8_t tmp;
  op1 = raw & 0xFFu;
  s1 = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  off4 = (int8_t)tmp;
}

void Instruction::ExtractOpfieldsABS(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1d, uint32_t& ea) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 26) & 0x03u;
  s1d = (raw >> 8) & 0x0Fu;

  // off18 seems to be used the same way everywhere, so moving the effective
  // address calculation here
  ea = ((raw << 16) & 0xF0000000u) | ((raw >> 12) & 0x3C00u) |
       ((raw >> 22) & 0x3C0u) | ((raw >> 16) & 0x3Fu);
}

void Instruction::ExtractOpfieldsABSB(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& b, uint8_t& bpos3,
                                      uint32_t& ea) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 26) & 0x03u;
  bpos3 = (raw >> 8) & 0x07u;
  b = (raw >> 11) & 0x01u;

  // off18 seems to be used the same way everywhere, so moving the effective
  // address calculation here
  ea = ((raw << 16) & 0xF0000000u) | ((raw >> 12) & 0x3C00u) |
       ((raw >> 22) & 0x3C0u) | ((raw >> 16) & 0x3Fu);
}

void Instruction::ExtractOpfieldsB(const uint8_t* data, uint8_t& op1,
                                   int32_t& disp24) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;

  disp24 = ((raw >> 16) & 0xFFFFu) | ((raw << 8) & 0xFF0000u);
  if (disp24 & 0x800000u) {
    disp24 |= 0xFF000000u;
  }
}

void Instruction::ExtractOpfieldsBIT(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                     uint8_t& pos1, uint8_t& pos2, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x03u;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  pos1 = (raw >> 16) & 0x1Fu;
  pos2 = (raw >> 23) & 0x1Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsBO(const uint8_t* data, uint8_t& op1,
                                    uint8_t& op2, uint8_t& s1d, uint8_t& s2,
                                    int16_t& off10) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint16_t tmp;
  op1 = raw & 0xFFu;
  op2 = (raw >> 22) & 0x3Fu;
  s1d = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;

  tmp = ((raw >> 16) & 0x3Fu) | ((raw >> 22) & 0x3C0u);
  if (tmp & 0x200) {
    tmp |= 0xFC00;
  }
  off10 = (int16_t)tmp;
}

void Instruction::ExtractOpfieldsBOL(const uint8_t* data, uint8_t& op1,
                                     uint8_t& s1d, uint8_t& s2,
                                     int16_t& off16) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  s1d = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  off16 = (int16_t)(((raw >> 16) & 0x3Fu) | ((raw >> 22) & 0x3C0u) |
                    ((raw >> 12) & 0xFC00u));
}

void Instruction::ExtractOpfieldsBRC(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1, int8_t& const4,
                                     int16_t& disp15) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint8_t tmp8;
  uint16_t tmp16;

  op1 = raw & 0xFFu;
  op2 = (raw >> 31) & 0x01u;
  s1 = (raw >> 8) & 0x0Fu;

  tmp8 = (raw >> 12) & 0x0Fu;
  if (tmp8 & 0x8) {
    tmp8 |= 0xF0;
  }
  const4 = (int8_t)tmp8;

  tmp16 = (raw >> 16) & 0x7FFFu;
  if (tmp16 & 0x4000) {
    tmp16 |= 0x8000;
  }
  disp15 = (int16_t)tmp16;
}

void Instruction::ExtractOpfieldsBRN(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& n, uint8_t& s1,
                                     int16_t& disp15) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint16_t tmp;
  op1 = raw & 0x7Fu;
  op2 = (raw >> 31) & 0x01u;
  s1 = (raw >> 8) & 0x0Fu;
  n = ((raw >> 12) & 0x0Fu) | ((raw >> 3) & 0x10u);

  tmp = (raw >> 16) & 0x7FFFu;
  if (tmp & 0x4000) {
    tmp |= 0x8000;
  }
  disp15 = (int16_t)tmp;
}

void Instruction::ExtractOpfieldsBRR(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                     int16_t& disp15) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint16_t tmp;
  op1 = raw & 0xFFu;
  op2 = (raw >> 31) & 0x01u;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;

  tmp = (raw >> 16) & 0x7FFFu;
  if (tmp & 0x4000) {
    tmp |= 0x8000;
  }
  disp15 = (int16_t)tmp;
}

void Instruction::ExtractOpfieldsRC(const uint8_t* data, uint8_t& op1,
                                    uint8_t& op2, uint8_t& s1, uint8_t& d,
                                    int16_t& const9) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint16_t tmp;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x7Fu;
  s1 = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x1FFu;
  if (tmp & 0x100) {
    tmp |= 0xFE00;
  }
  const9 = (int16_t)tmp;

  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRCPW(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, int8_t& const4,
                                      uint8_t& width, uint8_t& pos,
                                      uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint8_t tmp;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x03u;
  s1 = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  const4 = (int8_t)tmp;

  width = (raw >> 16) & 0x1Fu;
  pos = (raw >> 23) & 0x1Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRCR(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1, int16_t& const9,
                                     uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint16_t tmp;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x07u;
  s1 = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x1FFu;
  if (tmp & 0x100) {
    tmp |= 0xFE00;
  }
  const9 = (int16_t)tmp;

  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRCRR(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, int8_t& const4,
                                      uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint8_t tmp;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x07u;
  s1 = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  const4 = (int8_t)tmp;

  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRCRW(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, int8_t& const4,
                                      uint8_t& width, uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  uint8_t tmp;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x07u;
  s1 = (raw >> 8) & 0x0Fu;

  tmp = (raw >> 12) & 0x0Fu;
  if (tmp & 0x8) {
    tmp |= 0xF0;
  }
  const4 = (int8_t)tmp;

  width = (raw >> 16) & 0x1Fu;
  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRLC(const uint8_t* data, uint8_t& op1,
                                     uint8_t& s1, int16_t& const16,
                                     uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  s1 = (raw >> 8) & 0x0Fu;
  const16 = (int16_t)((raw >> 12) & 0xFFFFu);
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRR(const uint8_t* data, uint8_t& op1,
                                    uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                    uint8_t& n, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 20) & 0xFFu;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  n = (raw >> 16) & 0x03u;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRR1(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                     uint8_t& n, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 18) & 0x3FFu;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  n = (raw >> 16) & 0x03u;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRR2(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                     uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 16) & 0xFFFu;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRRPW(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                      uint8_t& width, uint8_t& pos,
                                      uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x03u;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  width = (raw >> 16) & 0x1Fu;
  pos = (raw >> 23) & 0x1Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRRR(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                     uint8_t& n, uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 20) & 0x0Fu;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  n = (raw >> 16) & 0x03u;
  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRRR1(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                      uint8_t& n, uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 18) & 0x3Fu;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  n = (raw >> 16) & 0x03u;
  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRRR2(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                      uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 16) & 0xFFu;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRRRR(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                      uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x03u;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsRRRW(const uint8_t* data, uint8_t& op1,
                                      uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                      uint8_t& width, uint8_t& s3, uint8_t& d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 21) & 0x03u;
  s1 = (raw >> 8) & 0x0Fu;
  s2 = (raw >> 12) & 0x0Fu;
  width = (raw >> 16) & 0x1Fu;
  s3 = (raw >> 24) & 0x0Fu;
  d = (raw >> 28) & 0x0Fu;
}

void Instruction::ExtractOpfieldsSYS(const uint8_t* data, uint8_t& op1,
                                     uint8_t& op2, uint8_t& s1d) {
  auto data32 = (const uint32_t*)data;
  uint32_t raw = *data32;
  op1 = raw & 0xFFu;
  op2 = (raw >> 22) & 0x3Fu;
  s1d = (raw >> 8) & 0x0Fu;
}

bool Instruction::TextOpDcDb_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t reg_dc, reg_db, op1, op2, s1, n;
  char buf[32];
  Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_dc);

  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpEcDb_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t reg_ec, reg_db, op1, op2, s1, n;
  char buf[32];
  Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpDcDaConst9_RC(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr, bool is_unsigned) {
  uint8_t reg_da, reg_dc, op1, op2;
  int16_t const9;
  char buf[32];

  Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
  if (is_unsigned) {
    const9 &= 0x01FF;  // clear sign-extended bits
  }
  ITEXT(instr)

  if (op1 == OP1_x8F_RC &&
      (op2 == SH_DC_DA_CONST9 || op2 == SHA_DC_DA_CONST9)) {
    const9 &= 0x3F;
    if (const9 & 0x20) {
      const9 |= 0xFFC0u;
    }
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const9)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const9 < 0 ? "-" : "",
                              (uint16_t)abs(const9));
  result.emplace_back(
      IntegerToken, buf, const9,
      sizeof(const9));  // TODO: is sizeof the right choice here?

  return true;
}

bool Instruction::TextOpDcDaDb_RR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr) {
  uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpDcDbDa_RR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr) {
  uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpDaConst4_SRC(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, bool is_unsigned) {
  uint8_t op1, reg_da;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));

  return true;
}

bool Instruction::TextOpEaConst4_SRC(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, bool is_unsigned) {
  uint8_t op1, reg_ea;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsSRC(data, op1, reg_ea, const4);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  REGTOE(reg_ea)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));

  return true;
}

bool Instruction::TextOpDaD15Const4_SRC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_da;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));

  return true;
}

bool Instruction::TextOpD15DaConst4_SRC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_da;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));

  return true;
}

bool Instruction::TextOpDaDb_SRR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr) {
  uint8_t op1, reg_da, reg_db;
  char buf[32];

  Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpDaD15Db_SRR(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, reg_da, reg_db;
  char buf[32];

  Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpD15DaDb_SRR(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, reg_da, reg_db;
  char buf[32];

  Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpAaConst4_SRC(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, bool is_unsigned) {
  uint8_t op1, reg_aa;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsSRC(data, op1, reg_aa, const4);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  REGTOA(reg_aa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "#%hu", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "#%hi", const4);
  result.emplace_back(PossibleAddressToken, buf, const4, sizeof(const4));

  return true;
}

bool Instruction::TextOpAaAb_SRR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr)

{
  uint8_t op1, reg_aa, reg_ab;
  char buf[32];

  Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_ab);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);

  return true;
}

bool Instruction::TextOpAcAaAb_RR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr) {
  uint8_t reg_ac, reg_aa, reg_ab, op1, op2, n;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n, reg_ac);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  REGTOA(reg_ac)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);

  return true;
}

bool Instruction::TextOpDcDaConst16_RLC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_da, reg_dc;
  int16_t const16;
  unsigned res;
  char buf[32];

  Instruction::ExtractOpfieldsRLC(data, op1, reg_da, const16, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");

  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const16)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const16 < 0 ? "-" : "",
                              (uint16_t)abs(const16));
  result.emplace_back(IntegerToken, buf, const16, sizeof(const16));

  return true;
}

bool Instruction::TextOpAcAaConst16_RLC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_aa, reg_ac;
  int16_t const16;
  int32_t res;
  char buf[32];

  Instruction::ExtractOpfieldsRLC(data, op1, reg_aa, const16, reg_ac);
  REGTOA(reg_aa)
  REGTOA(reg_ac)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  if (op1 == ADDIHA_AC_AA_CONST16) {
    res = const16 << 16;
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint32_t)res)
                : std::snprintf(buf, sizeof(buf), "%#x", res);
    result.emplace_back(PossibleAddressToken, buf, res, sizeof(res));
  } else {
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const16)
                : std::snprintf(buf, sizeof(buf), "%#x", const16);
    result.emplace_back(PossibleAddressToken, buf, const16, sizeof(const16));
  }

  return true;
}

bool Instruction::TextOpAaAbD15N_SRRS(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, n, reg_aa, reg_ab;
  char buf[32];

  Instruction::ExtractOpfieldsSRRS(data, op1, n, reg_aa, reg_ab);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpAcAbDaN_RR(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr) {
  uint8_t op1, op2, reg_da, reg_ab, n, reg_ac;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_ab, n, reg_ac);
  REGTOA(reg_ab)
  REGTOA(reg_ac)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpAcAbDa_RR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr) {
  uint8_t op1, op2, reg_da, reg_ab, n, reg_ac;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_ab, n, reg_ac);
  REGTOA(reg_ab)
  REGTOA(reg_ac)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpD15Const8_SC(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, bool is_unsigned) {
  uint8_t op1;
  int8_t const8;
  char buf[32];

  Instruction::ExtractOpfieldsSC(data, op1, const8);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const8)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const8 < 0 ? "-" : "",
                              (uint8_t)abs(const8));
  result.emplace_back(IntegerToken, buf, const8, sizeof(const8));

  return true;
}

bool Instruction::TextOpDcDaPos1DbPos2_BIT(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                  reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos1);
  result.emplace_back(IntegerToken, buf, pos1, sizeof(pos1));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos2);
  result.emplace_back(IntegerToken, buf, pos1, sizeof(pos2));

  return true;
}

bool Instruction::TextOpConst8_SC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned) {
  uint8_t op1;
  int8_t const8;
  char buf[32];

  Instruction::ExtractOpfieldsSC(data, op1, const8);
  ITEXT(instr)

  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const8)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const8 < 0 ? "-" : "",
                              (uint8_t)abs(const8));
  result.emplace_back(IntegerToken, buf, const8, sizeof(const8));

  return true;
}

bool Instruction::TextOpConst9_RC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned) {
  uint8_t op1, op2, s1, d;
  int16_t const9;
  char buf[32];

  Instruction::ExtractOpfieldsRC(data, op1, op2, s1, d, const9);
  if (is_unsigned) {
    const9 &= 0x01FF;
  }
  ITEXT(instr)

  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const9)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const9 < 0 ? "-" : "",
                              (uint16_t)abs(const9));
  result.emplace_back(IntegerToken, buf, const9, sizeof(const9));

  return true;
}

bool Instruction::TextOpEcDa_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t op1, op2, reg_da, s1, s2, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpAbOff10_BO(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, s1d, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, s1d, reg_ab, off10);
  REGTOA(reg_ab)
  ITEXT(instr)

  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpPb_BO(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, s1d, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, s1d, reg_pb, off10);
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");

  return true;
}

bool Instruction::TextOpPbOff10_BO(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, s1d, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, s1d, reg_pb, off10);
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpDcDdDaConst9_RCR(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, reg_da, reg_dd, reg_dc;
  int16_t const9;
  char buf[32];

  Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9, reg_dd,
                                  reg_dc);
  if (is_unsigned) {
    const9 &= 0x01FF;
  }
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const9)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const9 < 0 ? "-" : "",
                              (uint16_t)abs(const9));
  result.emplace_back(IntegerToken, buf, const9, sizeof(const9));

  return true;
}

bool Instruction::TextOpDcDdDaDb_RRR(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                  reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpDisp8_SB(const uint8_t* data, uint64_t addr, size_t len,
                                 size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr) {
  uint8_t op1;
  int8_t disp8;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsSB(data, op1, disp8);
  ITEXT(instr)

  switch (op1) {
    case CALL_DISP8:
      target = Call::GetTarget(data, addr, len);
      break;
    case J_DISP8:
      target = J::GetTarget(data, addr, len);
      break;
    default:
      break;
  }

  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpDisp24_B(const uint8_t* data, uint64_t addr, size_t len,
                                 size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr) {
  uint8_t op1;
  int32_t disp24;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsB(data, op1, disp24);
  ITEXT(instr)

  switch (op1) {
    case CALL_DISP24:
    case FCALL_DISP24:  // TCv1.6 ISA only
      target = Call::GetTarget(data, addr, len);
      break;
    case CALLA_DISP24:
    case FCALLA_DISP24:  // TCv1.6 ISA only
      target = Calla::GetTarget(data, addr, len);
      break;
    case J_DISP24:
      target = J::GetTarget(data, addr, len);
      break;
    case JA_DISP24:
      target = Ja::GetTarget(data, addr, len);
      break;
    case JL_DISP24:
      target = Jl::GetTarget(data, addr, len);
      break;
    case JLA_DISP24:
      target = Jla::GetTarget(data, addr, len);
      break;
    default:
      break;
  }

  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpAa_SR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr) {
  uint8_t op1, reg_aa, op2;
  char buf[32];

  Instruction::ExtractOpfieldsSR(data, op1, reg_aa, op2);
  REGTOA(reg_aa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpAa_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr) {
  uint8_t op1, op2, reg_aa, s2, n, d;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s2, n, d);
  REGTOA(reg_aa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpDcDa_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t op1, op2, reg_da, s2, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOp_SR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr) {
  ITEXT(instr)  // no need to extract--just print instruction
  return true;
}

bool Instruction::TextOp_SYS(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr) {
  ITEXT(instr)  // no need to extract--just print instruction
  return true;
}

// TCv1.6 ISA only
bool Instruction::TextOpDa_SYS(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr) {
  uint8_t op1, op2, reg_da;
  char buf[32];

  Instruction::ExtractOpfieldsSYS(data, op1, op2, reg_da);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpDcDaDbPos_RRPW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, w, pos, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRPW(data, op1, op2, reg_da, reg_db, w, pos,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos);
  result.emplace_back(IntegerToken, buf, pos, sizeof(pos));

  return true;
}

bool Instruction::TextOpDcDaDbDd_RRRR(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRRR(data, op1, op2, reg_da, reg_db, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);

  return true;
}

bool Instruction::TextOpEcEdDb_RRR(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr) {
  uint8_t op1, op2, s1, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR(data, op1, op2, s1, reg_db, n, reg_ed,
                                  reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpEcDaDb_RR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpDcAaAb_RR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr) {
  uint8_t op1, op2, reg_aa, reg_ab, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n, reg_dc);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);

  return true;
}

bool Instruction::TextOpDcAa_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t op1, op2, reg_aa, s1, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s1, n, reg_dc);
  REGTOA(reg_aa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpDcDaPosWidth_RRPW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, s2, w, pos, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRPW(data, op1, op2, reg_da, s2, w, pos, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos);
  result.emplace_back(IntegerToken, buf, pos, sizeof(pos));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpDcDaEd_RRRR(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_da, s2, reg_ed, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRRR(data, op1, op2, reg_da, s2, reg_ed, reg_dc);
  REGTOE(reg_ed)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);

  return true;
}

bool Instruction::TextOpDcDaDdWidth_RRRW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, s2, w, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRRW(data, op1, op2, reg_da, s2, w, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpEcConst4PosWidth_RCPW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, s1, w, pos, reg_ec;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsRCPW(data, op1, op2, s1, const4, w, pos, reg_ec);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos);
  result.emplace_back(IntegerToken, buf, pos, sizeof(pos));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpEcConst4DdWidth_RCRW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, s1, w, reg_dd, reg_ec;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsRCRW(data, op1, op2, s1, const4, w, reg_dd,
                                   reg_ec);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpEcDbPosWidth_RRPW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, s1, reg_db, w, pos, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRPW(data, op1, op2, s1, reg_db, w, pos, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos);
  result.emplace_back(IntegerToken, buf, pos, sizeof(pos));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpEcDbDdWidth_RRRW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, s1, reg_db, w, reg_dd, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRRW(data, op1, op2, s1, reg_db, w, reg_dd,
                                   reg_ec);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpDcDaConst4PosWidth_RCPW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, reg_da, w, pos, reg_dc;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsRCPW(data, op1, op2, reg_da, const4, w, pos,
                                   reg_dc);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos);
  result.emplace_back(IntegerToken, buf, pos, sizeof(pos));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpDcDaConst4Ed_RCRR(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, reg_da, reg_ed, reg_dc;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsRCRR(data, op1, op2, reg_da, const4, reg_ed,
                                   reg_dc);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  REGTOE(reg_ed)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);

  return true;
}

bool Instruction::TextOpDcDaConst4DdWidth_RCRW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, reg_da, w, reg_dd, reg_dc;
  int8_t const4;
  char buf[32];

  Instruction::ExtractOpfieldsRCRW(data, op1, op2, reg_da, const4, w, reg_dd,
                                   reg_dc);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpDcDaDbPosWidth_RRPW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, w, pos, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRPW(data, op1, op2, reg_da, reg_db, w, pos,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", pos);
  result.emplace_back(IntegerToken, buf, pos, sizeof(pos));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpDcDaDbEd_RRRR(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRRR(data, op1, op2, reg_da, reg_db, reg_ed,
                                   reg_dc);
  REGTOE(reg_ed)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);

  return true;
}

bool Instruction::TextOpDcDaDbDdWidth_RRRW(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, w, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRRW(data, op1, op2, reg_da, reg_db, w, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", w);
  result.emplace_back(IntegerToken, buf, w, sizeof(w));

  return true;
}

bool Instruction::TextOpD15Const4Disp4_SBC(
    const uint8_t* data, uint64_t addr, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1;
  int8_t disp4, const4;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsSBC(data, op1, disp4, const4);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  ITEXT(instr)

  switch (op1) {
    case JEQ_D15_CONST4_DISP4:
    case JEQ_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
      target = Jeq::GetTarget(data, addr, len);
      break;
    case JNE_D15_CONST4_DISP4:
    case JNE_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
      target = Jne::GetTarget(data, addr, len);
      break;
    default:
      break;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpD15DbDisp4_SBR(
    const uint8_t* data, uint64_t addr, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, reg_db;
  int8_t disp4;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
  ITEXT(instr)

  switch (op1) {
    case JEQ_D15_DB_DISP4:
    case JEQ_D15_DB_DISP4_EX:  // TCv1.6 ISA only
      target = Jeq::GetTarget(data, addr, len);
    case JNE_D15_DB_DISP4:
    case JNE_D15_DB_DISP4_EX:  // TCv1.6 ISA only
      target = Jne::GetTarget(data, addr, len);
    default:
      break;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpDaConst4Disp15_BRC(
    const uint8_t* data, uint64_t addr, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, reg_da;
  int8_t const4;
  int16_t disp15;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4, disp15);
  if (is_unsigned) {
    const4 &= 0x0F;
  }
  ITEXT(instr)

  if (op1 == OP1_xDF_BRC && op2 == JEQ_DA_CONST4_DISP15) {
    target = Jeq::GetTarget(data, addr, len);
  } else if (op1 == OP1_xFF_BRC &&
             (op2 == JGE_DA_CONST4_DISP15 || op2 == JGEU_DA_CONST4_DISP15)) {
    target = Jge::GetTarget(data, addr, len);
  } else if (op1 == OP1_xBF_BRC &&
             (op2 == JLT_DA_CONST4_DISP15 || op2 == JLTU_DA_CONST4_DISP15)) {
    target = Jlt::GetTarget(data, addr, len);
  } else if (op1 == OP1_xDF_BRC && op2 == JNE_DA_CONST4_DISP15) {
    target = Jne::GetTarget(data, addr, len);
  } else if (op1 == OP1_x9F_BRC && op2 == JNED_DA_CONST4_DISP15) {
    target = Jned::GetTarget(data, addr, len);
  } else if (op1 == OP1_x9F_BRC && op2 == JNEI_DA_CONST4_DISP15) {
    target = Jnei::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const4)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const4 < 0 ? "-" : "",
                              (uint8_t)abs(const4));
  result.emplace_back(IntegerToken, buf, const4, sizeof(const4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpDaDbDisp15_BRR(
    const uint8_t* data, uint64_t addr, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db;
  int16_t disp15;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db, disp15);
  ITEXT(instr)

  if (op1 == OP1_x5F_BRR && op2 == JEQ_DA_DB_DISP15) {
    target = Jeq::GetTarget(data, addr, len);
  } else if (op1 == OP1_x7F_BRR &&
             (op2 == JGE_DA_DB_DISP15 || op2 == JGEU_DA_DB_DISP15)) {
    target = Jge::GetTarget(data, addr, len);
  } else if (op1 == OP1_x3F_BRR &&
             (op2 == JLT_DA_DB_DISP15 || op2 == JLTU_DA_DB_DISP15)) {
    target = Jlt::GetTarget(data, addr, len);
  } else if (op1 == OP1_x5F_BRR && op2 == JNE_DA_DB_DISP15) {
    target = Jne::GetTarget(data, addr, len);
  } else if (op1 == OP1_x1F_BRR && op2 == JNED_DA_DB_DISP15) {
    target = Jned::GetTarget(data, addr, len);
  } else if (op1 == OP1_x1F_BRR && op2 == JNEI_DA_DB_DISP15) {
    target = Jnei::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpAaAbDisp15_BRR(
    const uint8_t* data, uint64_t addr, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_aa, reg_ab;
  int16_t disp15;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_aa, reg_ab, disp15);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  if (op1 == OP1_x7D_BRR && op2 == JEQA_AA_AB_DISP15) {
    target = Jeqa::GetTarget(data, addr, len);
  }
  if (op1 == OP1_x7D_BRR && op2 == JNEA_AA_AB_DISP15) {
    target = Jnea::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "#%hi", disp15);
  result.emplace_back(PossibleAddressToken, buf, disp15, sizeof(disp15));

  return true;
}

bool Instruction::TextOpDbDisp4_SBR(const uint8_t* data, uint64_t addr,
                                    size_t len, size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, reg_db;
  int8_t disp4;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
  ITEXT(instr)

  switch (op1) {
    case JGEZ_DB_DISP4:
      target = Jgez::GetTarget(data, addr, len);
      break;
    case JGTZ_DB_DISP4:
      target = Jgtz::GetTarget(data, addr, len);
      break;
    case JLEZ_DB_DISP4:
      target = Jlez::GetTarget(data, addr, len);
      break;
    case JLTZ_DB_DISP4:
      target = Jltz::GetTarget(data, addr, len);
      break;
    case JNZ_DB_DISP4:
      target = Jnz::GetTarget(data, addr, len);
      break;
    case JZ_DB_DISP4:
      target = Jz::GetTarget(data, addr, len);
      break;
    default:
      break;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpD15Disp8_SB(const uint8_t* data, uint64_t addr,
                                    size_t len, size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1;
  int8_t disp8;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsSB(data, op1, disp8);
  ITEXT(instr)

  if (op1 == JNZ_D15_DISP8) {
    target = Jnz::GetTarget(data, addr, len);
  } else if (op1 == JZ_D15_DISP8) {
    target = Jz::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpAbDisp4_SBR(const uint8_t* data, uint64_t addr,
                                    size_t len, size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, reg_ab;
  int8_t disp4;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_ab);
  REGTOA(reg_ab)
  ITEXT(instr)

  if (op1 == JNZA_AB_DISP4) {
    target = Jnza::GetTarget(data, addr, len);
  } else if (op1 == JZ_DB_DISP4) {
    target = Jz::GetTarget(data, addr, len);
  } else if (op1 == JZA_AB_DISP4) {
    target = Jza::GetTarget(data, addr, len);
  } else if (op1 == LOOP_AB_DISP4) {
    target = Loop::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpAaDisp15_BRR(const uint8_t* data, uint64_t addr,
                                     size_t len, size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr) {
  uint8_t op1, op2, reg_aa, s2;
  int16_t disp15;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_aa, s2, disp15);
  REGTOA(reg_aa)
  ITEXT(instr)

  if (op1 == OP1_xBD_BRR && op2 == JNZA_AA_DISP15) {
    target = Jnza::GetTarget(data, addr, len);
  } else if (op1 == OP1_xBD_BRR && op2 == JZA_AA_DISP15) {
    target = Jza::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpD15NDisp4_SBRN(
    const uint8_t* data, uint64_t addr, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, n;
  int8_t disp4;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsSBRN(data, op1, disp4, n);
  ITEXT(instr)

  if (op1 == JNZT_D15_N_DISP4) {
    target = Jnzt::GetTarget(data, addr, len);
  } else if (op1 == JZT_D15_N_DISP4) {
    target = Jzt::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(TRICORE_REG_D15));
  result.emplace_back(RegisterToken, buf, TRICORE_REG_D15);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpDaNDisp15_BRN(const uint8_t* data, uint64_t addr,
                                      size_t len, size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, n, reg_da;
  int16_t disp15;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsBRN(data, op1, op2, n, reg_da, disp15);
  ITEXT(instr)

  if (op1 == OP1_x6F_BRN && op2 == JNZT_DA_N_DISP15) {
    target = Jnzt::GetTarget(data, addr, len);
  } else if (op1 == OP1_x6F_BRN && op2 == JZT_DA_N_DISP15) {
    target = Jzt::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpA15A10Const8_SC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_a15, reg_a10;
  int8_t const8;
  char buf[32];

  Instruction::ExtractOpfieldsSC(data, op1, const8);
  reg_a15 = TRICORE_REG_A15;
  reg_a10 = TRICORE_REG_A10;
  ITEXT(instr)

  if (op1 == LDA_A15_A10_CONST8) {
    const8 *= 4;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a10));
  result.emplace_back(RegisterToken, buf, reg_a10);
  result.emplace_back(TextToken, "]");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const8)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const8 < 0 ? "-" : "",
                              (uint8_t)abs(const8));
  result.emplace_back(IntegerToken, buf, const8, sizeof(const8));

  return true;
}

bool Instruction::TextOpAcAb_SLR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, uint8_t addressMode) {
  uint8_t op1, reg_ac, reg_ab;
  char buf[32];

  Instruction::ExtractOpfieldsSLR(data, op1, reg_ac, reg_ab);
  REGTOA(reg_ac)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");

  return true;
}

bool Instruction::TextOpAcA15Off4_SLRO(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, reg_ac, reg_a15;
  int8_t off4;
  char buf[32];

  Instruction::ExtractOpfieldsSLRO(data, op1, reg_ac, off4);
  reg_a15 = TRICORE_REG_A15;
  REGTOA(reg_ac)
  ITEXT(instr)

  if (op1 == LDA_AC_A15_OFF4) {
    off4 = (off4 & 0xF) * 4;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                (uint8_t)abs(off4));
  result.emplace_back(IntegerToken, buf, off4, sizeof(off4));

  return true;
}

bool Instruction::TextOpA15AbOff4_SRO(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, reg_ab, reg_a15;
  int8_t off4;
  char buf[32];

  Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
  reg_a15 = TRICORE_REG_A15;
  REGTOA(reg_ab)
  ITEXT(instr)

  if (op1 == LDA_A15_AB_OFF4) {
    off4 = (off4 & 0xF) * 4;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                (uint8_t)abs(off4));
  result.emplace_back(IntegerToken, buf, off4, sizeof(off4));

  return true;
}

bool Instruction::TextOpAaOff18_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_aa;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_aa, ea);
  REGTOA(reg_aa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));

  return true;
}

bool Instruction::TextOpAaAbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_aa, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpAaPb_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_aa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_pb, off10);
  REGTOA(reg_aa)
  REGTOP(reg_pb)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");

  return true;
}

bool Instruction::TextOpAaPbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_aa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_pb, off10);
  REGTOA(reg_aa)
  REGTOP(reg_pb)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpDcAb_SLR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, uint8_t addressMode) {
  uint8_t op1, reg_dc, reg_ab;
  char buf[32];

  Instruction::ExtractOpfieldsSLR(data, op1, reg_dc, reg_ab);
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");

  return true;
}

bool Instruction::TextOpDcA15Off4_SLRO(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, reg_dc, reg_a15;
  int8_t off4;
  char buf[32];

  Instruction::ExtractOpfieldsSLRO(data, op1, reg_dc, off4);
  reg_a15 = TRICORE_REG_A15;
  ITEXT(instr)

  if (op1 == LDBU_DC_A15_OFF4) {
    off4 &= 0xFu;
  } else if (op1 == LDH_DC_A15_OFF4) {
    off4 &= 0xFu;
    off4 *= 2;
  } else if (op1 == LDW_DC_A15_OFF4) {
    off4 &= 0xFu;
    off4 *= 4;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                (uint8_t)abs(off4));
  result.emplace_back(IntegerToken, buf, off4, sizeof(off4));

  return true;
}

bool Instruction::TextOpD15AbOff4_SRO(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, reg_ab, reg_d15;
  int8_t off4;
  char buf[32];

  Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
  reg_d15 = TRICORE_REG_D15;
  REGTOA(reg_ab)
  ITEXT(instr)

  if (op1 == LDBU_D15_AB_OFF4) {
    off4 &= 0xFu;
  } else if (op1 == LDH_D15_AB_OFF4) {
    off4 = (off4 & 0xF) * 2;
  } else if (op1 == LDW_D15_AB_OFF4) {
    off4 = (off4 & 0xF) * 4;
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_d15));
  result.emplace_back(RegisterToken, buf, reg_d15);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                (uint8_t)abs(off4));
  result.emplace_back(IntegerToken, buf, off4, sizeof(off4));

  return true;
}

bool Instruction::TextOpDaOff18_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_da;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));

  return true;
}

bool Instruction::TextOpDaAbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_da, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpDaPb_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_da, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_pb, off10);
  REGTOP(reg_pb)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");

  return true;
}

bool Instruction::TextOpDaPbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_da, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_pb, off10);
  REGTOP(reg_pb)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpEaOff18_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_ea;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_ea, ea);
  REGTOE(reg_ea)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));

  return true;
}

bool Instruction::TextOpEaAbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_ea, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
  REGTOE(reg_ea)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);
  result.emplace_back(OperandSeparatorToken, ", ");
  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpEaPb_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_ea, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_pb, off10);
  REGTOE(reg_ea)
  REGTOP(reg_pb)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");

  return true;
}

bool Instruction::TextOpEaPbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_ea, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_pb, off10);
  REGTOE(reg_ea)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpPaOff18_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_pa;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_pa, ea);
  REGTOP(reg_pa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));

  return true;
}

bool Instruction::TextOpPaAbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_pa, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
  REGTOP(reg_pa)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);
  result.emplace_back(OperandSeparatorToken, ", ");
  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpPaPb_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_pa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_pb, off10);
  REGTOP(reg_pa)
  REGTOP(reg_pb)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");

  return true;
}

bool Instruction::TextOpPaPbOff10_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_pa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_pb, off10);
  REGTOP(reg_pa)
  REGTOP(reg_pb)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));

  return true;
}

bool Instruction::TextOpD15A10Const8_SC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_d15, reg_a10;
  int8_t const8;
  uint32_t mem;
  char buf[32];

  Instruction::ExtractOpfieldsSC(data, op1, const8);
  reg_d15 = TRICORE_REG_D15;
  reg_a10 = TRICORE_REG_A10;
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_d15));
  result.emplace_back(RegisterToken, buf, reg_d15);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a10));
  result.emplace_back(RegisterToken, buf, reg_a10);
  result.emplace_back(TextToken, "]");
  if (op1 == LDW_D15_A10_CONST8) {
    mem = (uint32_t)const8 * 4;
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint32_t)mem)
                : std::snprintf(buf, sizeof(buf), "%s%#x", mem < 0 ? "-" : "",
                                abs((int32_t)mem));
    result.emplace_back(IntegerToken, buf, mem, sizeof(mem));
  } else {
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const8)
                : std::snprintf(buf, sizeof(buf), "%s%#x",
                                const8 < 0 ? "-" : "", (uint8_t)abs(const8));
    result.emplace_back(IntegerToken, buf, const8, sizeof(const8));
  }

  return true;
}

bool Instruction::TextOpDaAbOff16_BOL(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr, bool is_unsigned) {
  uint8_t op1, reg_da, reg_ab;
  int16_t off16;
  char buf[32];

  Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)abs(off16))
              : std::snprintf(buf, sizeof(buf), "%s%#x", off16 < 0 ? "-" : "",
                              (uint16_t)abs(off16));
  result.emplace_back(IntegerToken, buf, off16, sizeof(off16));

  return true;
}

bool Instruction::TextOpOff18_ABS(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr) {
  uint8_t op1, op2, s1d;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, s1d, ea);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));

  return true;
}

bool Instruction::TextOpOff18Ea_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_ea;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_ea, ea);
  REGTOE(reg_ea)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);

  return true;
}

bool Instruction::TextOpAbOff10Ea_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_ea, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
  REGTOE(reg_ea)
  REGTOA(reg_ab)
  ITEXT(instr)

  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);

  return true;
}

bool Instruction::TextOpPbEa_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_ea, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_pb, off10);
  REGTOE(reg_ea)
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);

  return true;
}

bool Instruction::TextOpPbOff10Ea_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_ea, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_pb, off10);
  REGTOE(reg_ea)
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);

  return true;
}

bool Instruction::TextOpAaAbOff16_BOL(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, reg_aa, reg_ab;
  int16_t off16;
  char buf[32];

  Instruction::ExtractOpfieldsBOL(data, op1, reg_aa, reg_ab, off16);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off16 < 0 ? "-" : "",
                (uint16_t)abs(off16));
  result.emplace_back(IntegerToken, buf, off16, sizeof(off16));

  return true;
}

bool Instruction::TextOpAbDisp15_BRR(const uint8_t* data, uint64_t addr,
                                     size_t len, size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr) {
  uint8_t op1, op2, s1, reg_ab;
  int16_t disp15;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsBRR(data, op1, op2, s1, reg_ab, disp15);
  REGTOA(reg_ab)
  ITEXT(instr)

  if (op1 == OP1_xFD_BRR && op2 == LOOP_AB_DISP15) {
    target = Loop::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", target);
  result.emplace_back(PossibleAddressToken, buf, target, sizeof(target));

  return true;
}

bool Instruction::TextOpDisp15_BRR(const uint8_t* data, uint64_t addr,
                                   size_t len, size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr) {
  uint8_t op1, op2, s1, s2;
  int16_t disp15;
  uint32_t target = 0;
  char buf[32];

  Instruction::ExtractOpfieldsBRR(data, op1, op2, s1, s2, disp15);
  ITEXT(instr)

  if (op1 == OP1_xFD_BRR && op2 == LOOPU_DISP15) {
    target = Loopu::GetTarget(data, addr, len);
  }

  std::snprintf(buf, sizeof(buf), "%s%#x", disp15 < 0 ? "-" : "",
                (uint16_t)abs(disp15));
  result.emplace_back(IntegerToken, buf, disp15, sizeof(disp15));

  return true;
}

bool Instruction::TextOpEcEdDaConst9_RCR(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, op2, reg_da, reg_ed, reg_ec;
  int16_t const9;
  char buf[32];

  Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9, reg_ed,
                                  reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  if (is_unsigned) {
    const9 &= 0x01FF;
  }
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const9)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const9 < 0 ? "-" : "",
                              (uint16_t)abs(const9));
  result.emplace_back(IntegerToken, buf, const9, sizeof(const9));

  return true;
}

bool Instruction::TextOpDcDdDaDb_RRR2(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpEcEdDaDb_RRR2(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpEcEdDaDbllN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDaDbluN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDaDbulN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDaDbuuN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDaDbN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDaDbN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDaDblN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDaDblN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDaDbuN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDaDbuN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDalDblN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDalDblN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDauDbuN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcEdDauDbuN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_ec);
  REGTOE(reg_ed)
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDaDbllN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDaDbluN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDaDbulN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcEdDaDbulN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ed, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_ed,
                                   reg_dc);
  REGTOE(reg_ed)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDdDaDbuuN_RRR1(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR1(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                   reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcConst16_RLC(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr, bool is_unsigned) {
  uint8_t op1, s1, reg_dc;
  int16_t const16;
  int32_t value;
  char buf[32];

  Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  if (op1 == MOVH_DC_CONST16) {
    value = const16;
    value <<= 16;
    value &= (int32_t)0xFFFF0000;
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint32_t)value)
                : std::snprintf(buf, sizeof(buf), "%s%#x", value < 0 ? "-" : "",
                                abs((int32_t)value));
    result.emplace_back(IntegerToken, buf, value, sizeof(value));
  } else {
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const16)
                : std::snprintf(buf, sizeof(buf), "%s%#x",
                                const16 < 0 ? "-" : "", (uint16_t)abs(const16));
    result.emplace_back(IntegerToken, buf, const16, sizeof(const16));
  }
  return true;
}

bool Instruction::TextOpEcConst16_RLC(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr, bool is_unsigned) {
  uint8_t op1, s1, reg_ec;
  int16_t const16;
  int32_t value;
  char buf[32];

  Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  if (op1 == MOVH_DC_CONST16) {
    value = const16;
    value <<= 16;
    value &= (int32_t)0xFFFF0000;
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint32_t)value)
                : std::snprintf(buf, sizeof(buf), "%s%#x", value < 0 ? "-" : "",
                                abs((int32_t)value));
    result.emplace_back(IntegerToken, buf, value, sizeof(value));
  } else {
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const16)
                : std::snprintf(buf, sizeof(buf), "%s%#x",
                                const16 < 0 ? "-" : "", (uint16_t)abs(const16));
    result.emplace_back(IntegerToken, buf, const16, sizeof(const16));
  }
  return true;
}

bool Instruction::TextOpAaDb_SRR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr) {
  uint8_t op1, reg_aa, reg_db;
  char buf[32];

  Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_db);
  REGTOA(reg_aa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpAcDb_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t op1, op2, s1, reg_db, n, reg_ac;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_ac);
  REGTOA(reg_ac)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpAcAb_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t op1, op2, s1, reg_ab, n, reg_ac;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_ab, n, reg_ac);
  REGTOA(reg_ab)
  REGTOA(reg_ac)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);

  return true;
}

bool Instruction::TextOpDaAb_SRR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr) {
  uint8_t op1, reg_da, reg_ab;
  char buf[32];

  Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_ab);
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);

  return true;
}

bool Instruction::TextOpDcAb_RR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr) {
  uint8_t op1, op2, s1, reg_ab, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_ab, n, reg_dc);
  REGTOA(reg_ab)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);

  return true;
}

bool Instruction::TextOpAcConst16_RLC(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr, bool is_unsigned) {
  uint8_t op1, s1, reg_ac;
  int16_t const16;
  int32_t value;
  char buf[32];

  Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_ac);
  REGTOA(reg_ac)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ac));
  result.emplace_back(RegisterToken, buf, reg_ac);
  result.emplace_back(OperandSeparatorToken, ", ");
  if (op1 == MOVHA_AC_CONST16) {
    value = const16;
    value <<= 16;
    value &= (int32_t)0xFFFF0000;
    // Always print this instruction as an unsigned int, because it specifies
    // the high bits of an address
    std::snprintf(buf, sizeof(buf), "%#x", (uint32_t)value);
    result.emplace_back(IntegerToken, buf, value, sizeof(value));
  } else {
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const16)
                : std::snprintf(buf, sizeof(buf), "%s%#x",
                                const16 < 0 ? "-" : "", (uint16_t)abs(const16));
    result.emplace_back(IntegerToken, buf, const16, sizeof(const16));
  }

  return true;
}

bool Instruction::TextOpConst16Da_RLC(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr, bool is_unsigned) {
  uint8_t op1, reg_da, d;
  int16_t const16;
  char buf[32];

  Instruction::ExtractOpfieldsRLC(data, op1, reg_da, const16, d);
  ITEXT(instr)

  // This is only used for the MTCR instruction, which I believe uses unsigned
  // values (i.e. no negative offsets)
  std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const16);
  result.emplace_back(IntegerToken, buf, const16, sizeof(const16));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpEcDaConst9_RC(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr, bool is_unsigned) {
  uint8_t op1, op2, reg_da, reg_ec;
  int16_t const9;
  char buf[32];

  Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_ec, const9);
  if (is_unsigned) {
    const9 &= 0x01FF;
  }
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint16_t)const9)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const9 < 0 ? "-" : "",
                              (uint16_t)abs(const9));
  result.emplace_back(IntegerToken, buf, const9, sizeof(const9));

  return true;
}

bool Instruction::TextOpDcDaDb_RR2(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR2(data, op1, op2, reg_da, reg_db, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpEcDaDb_RR2(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR2(data, op1, op2, reg_da, reg_db, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);

  return true;
}

bool Instruction::TextOpEcDaDbllN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcDaDbluN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcDaDbulN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcDaDbuuN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDaDbN_RR1(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcDaDbN_RR1(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDaDblN_RR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcDaDblN_RR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDaDbuN_RR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpEcDaDbuN_RR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_ec);
  REGTOE(reg_ec)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ec));
  result.emplace_back(RegisterToken, buf, reg_ec);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDalDblN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " L");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDauDbuN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " U");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDaDbllN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDaDbluN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " LU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDaDbulN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UL");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDcDaDbuuN_RR1(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, op2, reg_da, reg_db, n, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRR1(data, op1, op2, reg_da, reg_db, n, reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_db));
  result.emplace_back(RegisterToken, buf, reg_db);
  result.emplace_back(TextToken, " UU");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", n);
  result.emplace_back(IntegerToken, buf, n, sizeof(n));

  return true;
}

bool Instruction::TextOpDa_SR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr) {
  uint8_t op1, reg_da, op2;
  char buf[32];

  Instruction::ExtractOpfieldsSR(data, op1, reg_da, op2);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpDcEdDa_RRR(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr) {
  uint8_t op1, op2, reg_da, s2, n, reg_ed, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, s2, n, reg_ed,
                                  reg_dc);
  REGTOE(reg_ed)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ed));
  result.emplace_back(RegisterToken, buf, reg_ed);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpAbAa_SSR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, uint8_t addressMode) {
  uint8_t op1, reg_aa, reg_ab;
  char buf[32];

  Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_ab);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpA10Const8A15_SC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_a10, reg_a15;
  int8_t const8;
  uint32_t mem;
  char buf[32];

  Instruction::ExtractOpfieldsSC(data, op1, const8);
  reg_a10 = TRICORE_REG_A10;
  reg_a15 = TRICORE_REG_A15;
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a10));
  result.emplace_back(RegisterToken, buf, reg_a10);
  result.emplace_back(TextToken, "]");
  if (op1 == STA_A10_CONST8_A15) {
    mem = (uint32_t)const8 * 4;
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint32_t)mem)
                : std::snprintf(buf, sizeof(buf), "%s%#x", mem < 0 ? "-" : "",
                                abs((int32_t)mem));
    result.emplace_back(IntegerToken, buf, mem, sizeof(mem));
  } else {
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const8)
                : std::snprintf(buf, sizeof(buf), "%s%#x",
                                const8 < 0 ? "-" : "", (uint8_t)abs(const8));
    result.emplace_back(IntegerToken, buf, const8, sizeof(const8));
  }
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);

  return true;
}

bool Instruction::TextOpAbOff4A15_SRO(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, reg_ab, reg_a15;
  int8_t off4;
  uint32_t mem;
  char buf[32];

  Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
  REGTOA(reg_ab)
  reg_a15 = TRICORE_REG_A15;
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  if (op1 == STA_AB_OFF4_A15) {
    mem = (off4 & 0xF) * 4;
    std::snprintf(buf, sizeof(buf), "%s%#x", mem < 0 ? "-" : "",
                  abs((int32_t)mem));
    result.emplace_back(IntegerToken, buf, mem, sizeof(mem));
  } else {
    std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                  (uint8_t)abs(off4));
    result.emplace_back(IntegerToken, buf, off4, sizeof(off4));
  }
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);

  return true;
}

bool Instruction::TextOpA15Off4Aa_SSRO(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, reg_aa, reg_a15;
  int8_t off4;
  uint32_t mem;
  char buf[32];

  Instruction::ExtractOpfieldsSSRO(data, op1, reg_aa, off4);
  REGTOA(reg_aa)
  reg_a15 = TRICORE_REG_A15;
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);
  result.emplace_back(TextToken, "]");
  if (op1 == STA_A15_OFF4_AA) {
    mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
    std::snprintf(buf, sizeof(buf), "%s%#x", mem < 0 ? "-" : "",
                  abs((int32_t)mem));
    result.emplace_back(IntegerToken, buf, mem, sizeof(mem));
  } else {
    std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                  (uint8_t)abs(off4));
    result.emplace_back(IntegerToken, buf, off4, sizeof(off4));
  }
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpOff18Aa_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_aa;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_aa, ea);
  REGTOA(reg_aa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpAbOff10Aa_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_aa, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
  REGTOA(reg_aa)
  REGTOA(reg_ab)
  ITEXT(instr)

  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpPbAa_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_aa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_pb, off10);
  REGTOA(reg_aa)
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpPbOff10Aa_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_aa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_pb, off10);
  REGTOA(reg_aa)
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpAbDa_SSR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, uint8_t addressMode) {
  uint8_t op1, reg_da, reg_ab;
  char buf[32];

  Instruction::ExtractOpfieldsSSR(data, op1, reg_da, reg_ab);
  REGTOA(reg_ab)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpAbOff4D15_SRO(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, reg_ab, reg_d15;
  int8_t off4;
  char buf[32];

  Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
  REGTOA(reg_ab)
  reg_d15 = TRICORE_REG_D15;
  ITEXT(instr)

  if (op1 == STB_AB_OFF4_D15) {
    off4 &= 0xFu;  // clear sign extended bits
  } else if (op1 == STH_AB_OFF4_D15) {
    off4 = (off4 & 0xF) * 2;  // clear sign extended bits and multiply
  } else if (op1 == STW_AB_OFF4_D15) {
    off4 = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
  }

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                (uint8_t)abs(off4));
  result.emplace_back(IntegerToken, buf, off4, sizeof(off4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_d15));
  result.emplace_back(RegisterToken, buf, reg_d15);

  return true;
}

bool Instruction::TextOpA15Off4Da_SSRO(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, reg_da, reg_a15;
  int8_t off4;
  char buf[32];

  Instruction::ExtractOpfieldsSSRO(data, op1, reg_da, off4);
  reg_a15 = TRICORE_REG_A15;
  ITEXT(instr)

  if (op1 == STB_A15_OFF4_DA) {
    off4 &= 0xFu;
  } else if (op1 == STH_A15_OFF4_DA) {
    off4 = (off4 & 0xF) * 2;  // clear sign extended bits and multiply
  } else if (op1 == STW_A15_OFF4_DA) {
    off4 = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
  }

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a15));
  result.emplace_back(RegisterToken, buf, reg_a15);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off4 < 0 ? "-" : "",
                (uint8_t)abs(off4));
  result.emplace_back(IntegerToken, buf, off4, sizeof(off4));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpOff18Da_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_da;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpAbOff10Da_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_da, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
  REGTOA(reg_ab)
  ITEXT(instr)

  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpPbDa_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_da, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_pb, off10);
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpPbOff10Da_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_da, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_pb, off10);
  REGTOP(reg_pb)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpOff18Pa_ABS(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr) {
  uint8_t op1, op2, reg_pa;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABS(data, op1, op2, reg_pa, ea);
  REGTOP(reg_pa)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);

  return true;
}

bool Instruction::TextOpAbOff10Pa_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_pa, reg_ab;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
  REGTOP(reg_pa)
  REGTOA(reg_ab)
  ITEXT(instr)

  addressMode == ADDRMODE_PREINC ? result.emplace_back(TextToken, "[+")
                                 : result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  addressMode == ADDRMODE_POSTINC ? result.emplace_back(TextToken, "+]")
                                  : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);

  return true;
}

bool Instruction::TextOpPbPa_BO(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_pa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_pb, off10);
  REGTOP(reg_pb)
  REGTOP(reg_pa)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_BREV ? result.emplace_back(TextToken, "+r]")
                               : result.emplace_back(TextToken, "]");
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);

  return true;
}

bool Instruction::TextOpPbOff10Pa_BO(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, uint8_t addressMode) {
  uint8_t op1, op2, reg_pa, reg_pb;
  int16_t off10;
  char buf[32];

  Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_pb, off10);
  REGTOP(reg_pb)
  REGTOP(reg_pa)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pb));
  result.emplace_back(RegisterToken, buf, reg_pb);
  addressMode == ADDRMODE_CIRC ? result.emplace_back(TextToken, "+c]")
                               : result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off10 < 0 ? "-" : "",
                (uint16_t)abs(off10));
  result.emplace_back(IntegerToken, buf, off10, sizeof(off10));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_pa));
  result.emplace_back(RegisterToken, buf, reg_pa);

  return true;
}

bool Instruction::TextOpOff18Bpos3B_ABSB(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr) {
  uint8_t op1, op2, b, bpos3;
  uint32_t ea;
  char buf[32];

  Instruction::ExtractOpfieldsABSB(data, op1, op2, b, bpos3, ea);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%#x", ea);
  result.emplace_back(PossibleAddressToken, buf, ea, sizeof(ea));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", bpos3);
  result.emplace_back(IntegerToken, buf, bpos3, sizeof(bpos3));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%#x", b);
  result.emplace_back(IntegerToken, buf, b, sizeof(b));

  return true;
}

bool Instruction::TextOpA10Const8D15_SC(
    const uint8_t* data, size_t len, size_t width,
    std::vector<InstructionTextToken>& result, std::string instr,
    bool is_unsigned) {
  uint8_t op1, reg_a10, reg_d15;
  int8_t const8;
  uint32_t mem;
  char buf[32];

  Instruction::ExtractOpfieldsSC(data, op1, const8);
  reg_a10 = TRICORE_REG_A10;
  reg_d15 = TRICORE_REG_D15;
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a10));
  result.emplace_back(RegisterToken, buf, reg_a10);
  result.emplace_back(TextToken, "]");
  if (op1 == STW_A10_CONST8_D15) {
    mem = (uint32_t)const8 * 4;
    mem = (uint32_t)const8 * 4;
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint32_t)mem)
                : std::snprintf(buf, sizeof(buf), "%s%#x", mem < 0 ? "-" : "",
                                abs((int32_t)mem));
    result.emplace_back(IntegerToken, buf, mem, sizeof(mem));
  } else {
    is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const8)
                : std::snprintf(buf, sizeof(buf), "%s%#x",
                                const8 < 0 ? "-" : "", (uint8_t)abs(const8));
    result.emplace_back(IntegerToken, buf, const8, sizeof(const8));
  }
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_d15));
  result.emplace_back(RegisterToken, buf, reg_d15);

  return true;
}

bool Instruction::TextOpAbOff16Da_BOL(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, reg_da, reg_ab;
  int16_t off16;
  char buf[32];

  Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
  REGTOA(reg_ab)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off16 < 0 ? "-" : "",
                (uint16_t)abs(off16));
  result.emplace_back(IntegerToken, buf, off16, sizeof(off16));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpAbOff16Aa_BOL(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr) {
  uint8_t op1, reg_aa, reg_ab;
  int16_t off16;
  char buf[32];

  Instruction::ExtractOpfieldsBOL(data, op1, reg_aa, reg_ab, off16);
  REGTOA(reg_ab)
  REGTOA(reg_aa)
  ITEXT(instr)

  result.emplace_back(TextToken, "[");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ab));
  result.emplace_back(RegisterToken, buf, reg_ab);
  result.emplace_back(TextToken, "]");
  std::snprintf(buf, sizeof(buf), "%s%#x", off16 < 0 ? "-" : "",
                (uint16_t)abs(off16));
  result.emplace_back(IntegerToken, buf, off16, sizeof(off16));
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_aa));
  result.emplace_back(RegisterToken, buf, reg_aa);

  return true;
}

bool Instruction::TextOpA10Const8_SC(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr, bool is_unsigned) {
  uint8_t op1, reg_a10;
  int8_t const8;
  char buf[32];

  Instruction::ExtractOpfieldsSC(data, op1, const8);
  reg_a10 = TRICORE_REG_A10;
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_a10));
  result.emplace_back(RegisterToken, buf, reg_a10);
  result.emplace_back(OperandSeparatorToken, ", ");
  is_unsigned ? std::snprintf(buf, sizeof(buf), "%#x", (uint8_t)const8)
              : std::snprintf(buf, sizeof(buf), "%s%#x", const8 < 0 ? "-" : "",
                              (uint8_t)abs(const8));
  result.emplace_back(IntegerToken, buf, const8, sizeof(const8));

  return true;
}

bool Instruction::TextOpDcDdDa_RRR(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr) {
  uint8_t op1, op2, reg_da, s2, n, reg_dd, reg_dc;
  char buf[32];

  Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, s2, n, reg_dd,
                                  reg_dc);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dc));
  result.emplace_back(RegisterToken, buf, reg_dc);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_dd));
  result.emplace_back(RegisterToken, buf, reg_dd);
  result.emplace_back(OperandSeparatorToken, ", ");
  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOpDa_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr) {
  uint8_t op1, op2, reg_da, s2, n, d;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, d);
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_da));
  result.emplace_back(RegisterToken, buf, reg_da);

  return true;
}

bool Instruction::TextOp_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr) {
  ITEXT(instr)
  return true;
}

bool Instruction::TextOpEa_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr) {
  uint8_t op1, op2, reg_ea, s2, n, d;
  char buf[32];

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_ea, s2, n, d);
  REGTOE(reg_ea)
  ITEXT(instr)

  std::snprintf(buf, sizeof(buf), "%s", Instruction::RegToStr(reg_ea));
  result.emplace_back(RegisterToken, buf, reg_ea);

  return true;
}

uint32_t Call::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target;

  // NOTE: the instruction manual probably has a typo... It says that
  // PC = PC + sign_ext(disp * 2) and I'm pretty sure that you are supposed
  // to sign extend, then multiply, based on the other instructions that use
  // this method.

  if (len == 2) {
    uint8_t op1;
    int8_t disp8;
    int64_t offset;
    Instruction::ExtractOpfieldsSB(data, op1, disp8);

    offset = ((int64_t)disp8) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  } else if (len == 4) {
    uint8_t op1;
    int32_t disp24;
    int64_t offset;
    Instruction::ExtractOpfieldsB(data, op1, disp24);

    offset = ((int64_t)disp24) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  } else {
    target = 0;
  }
  return target;
}

uint32_t Calla::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;
  uint8_t op1;
  int32_t disp24;
  int64_t offset;
  Instruction::ExtractOpfieldsB(data, op1, disp24);

  // PC = {disp24[23:20], 7'b0, disp24[19:0], 1'b0};
  target |= ((disp24 & 0xF00000u) << 8);
  target |= ((disp24 & 0xFFFFFu) << 1);

  return target;
}

uint32_t Calli::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint8_t op1, op2, reg_aa, s2, n, d;

  Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s2, n, d);
  REGTOA(reg_aa)

  return reg_aa;
}

uint32_t J::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target;

  // PC = PC + sign_ext(disp) * 2;

  if (len == 2) {
    uint8_t op1;
    int8_t disp8;
    int64_t offset;
    Instruction::ExtractOpfieldsSB(data, op1, disp8);

    offset = ((int64_t)disp8) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  } else if (len == 4) {
    uint8_t op1;
    int32_t disp24;
    int64_t offset;
    Instruction::ExtractOpfieldsB(data, op1, disp24);

    offset = ((int64_t)disp24) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  } else {
    target = 0;
  }
  return target;
}

uint32_t Ja::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;
  uint8_t op1;
  int32_t disp24;
  int64_t offset;
  Instruction::ExtractOpfieldsB(data, op1, disp24);

  // PC = {disp24[23:20], 7'b0, disp24[19:0], 1'b0};
  target |= ((disp24 & 0xF00000u) << 8);
  target |= ((disp24 & 0xFFFFFu) << 1);

  return target;
}

uint32_t Jeq::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;

  if (len == 2) {
    // NOTE: there are two forms of jeq instruction that use disp15 (SBC and
    // SBR) however, we really only care about getting the displacement here so
    // we can use the SBC extractor no matter what.
    uint8_t op1;
    int8_t disp4, const4;
    int64_t offset = 0;
    Instruction::ExtractOpfieldsSBC(data, op1, disp4, const4);

    if (op1 == JEQ_D15_CONST4_DISP4_EX || op1 == JEQ_D15_DB_DISP4_EX ||
        op1 == JNE_D15_CONST4_DISP4_EX || op1 == JNE_D15_DB_DISP4_EX) {
      // TCv1.6 ISA only
      // PC = PC + zero_ext(disp + 16) * 2;
      offset = ((disp4 & 0x0F) + 16) * 2;  // zero-extend, add 16, then multiply
      offset += addr;
      target = (uint32_t)offset;
    } else {
      // PC = PC + zero_ext(disp) * 2;
      offset = (disp4 & 0x0F) * 2;  // zero-extend, then multiply
      offset += addr;
      target = (uint32_t)offset;
    }
  } else if (len == 4) {
    // NOTE: there are two forms of jeq instruction that use disp15 (BRC and
    // BRR) however, we really only care about getting the displacement here so
    // we can use the BRC extractor no matter what.
    uint8_t op1, op2, reg_da;
    int8_t const4;
    int16_t disp15;
    int64_t offset;
    Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4, disp15);

    offset = ((int64_t)disp15) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  }
  return target;
}

uint32_t Jeqa::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  // Use Jge because the BRC and BRR formats are very similar
  return Jge::GetTarget(data, addr, len);
}

uint32_t Jge::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target;
  uint8_t op1, op2, reg_da;
  int8_t const4;
  int16_t disp15;
  int64_t offset;
  Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4, disp15);

  offset = ((int64_t)disp15) * 2;  // sign-extend and multiply
  offset += addr;
  target = (uint32_t)offset;

  return target;
}

uint32_t Jgez::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target;
  uint8_t op1, reg_db;
  int8_t disp4;
  int64_t offset = 0;
  Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);

  offset = (disp4 & 0x0F) * 2;  // zero-extend and multiply
  offset += addr;
  target = (uint32_t)offset;

  return target;
}

uint32_t Jgtz::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jgez::GetTarget(data, addr, len);
}

uint32_t Jl::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;
  uint8_t op1;
  int32_t disp24;
  int64_t offset;
  Instruction::ExtractOpfieldsB(data, op1, disp24);

  offset = ((int64_t)disp24) * 2;  // sign-extend and multiply
  offset += addr;
  target = (uint32_t)offset;

  return target;
}

uint32_t Jla::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Ja::GetTarget(data, addr, len);
}

uint32_t Jlez::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jgez::GetTarget(data, addr, len);
}

uint32_t Jlt::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jge::GetTarget(data, addr, len);
}

uint32_t Jltz::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jgez::GetTarget(data, addr, len);
}

uint32_t Jne::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jeq::GetTarget(data, addr, len);
}

uint32_t Jnea::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jeqa::GetTarget(data, addr, len);
}

uint32_t Jned::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jge::GetTarget(data, addr, len);
}

uint32_t Jnei::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jge::GetTarget(data, addr, len);
}

uint32_t Jnz::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  if ((raw & 0xFFu) == JNZ_D15_DISP8) {
    uint8_t op1;
    int8_t disp8;
    int64_t offset;
    Instruction::ExtractOpfieldsSB(data, op1, disp8);

    offset = ((int64_t)disp8) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  } else if ((raw & 0xFFu) == JNZ_DB_DISP4) {
    uint8_t op1, reg_db;
    int8_t disp4;
    int64_t offset = 0;
    Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);

    offset = (disp4 & 0x0F) * 2;  // zero-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  }
  return target;
}

uint32_t Jnza::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jeq::GetTarget(data, addr, len);
}

uint32_t Jnzt::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;
  if (len == 2) {
    uint8_t op1, n;
    int8_t disp4;
    int64_t offset = 0;
    Instruction::ExtractOpfieldsSBRN(data, op1, disp4, n);

    offset = (disp4 & 0x0F) * 2;  // zero-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  } else if (len == 4) {
    uint8_t op1, op2, n, reg_da;
    int16_t disp15;
    int64_t offset;
    Instruction::ExtractOpfieldsBRN(data, op1, op2, n, reg_da, disp15);

    offset = ((int64_t)disp15) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  }
  return target;
}

uint32_t Jz::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;
  auto data16 = (const uint16_t*)data;
  uint16_t raw = *data16;
  if ((raw & 0xFFu) == JZ_D15_DISP8) {
    uint8_t op1;
    int8_t disp8;
    int64_t offset;
    Instruction::ExtractOpfieldsSB(data, op1, disp8);

    offset = ((int64_t)disp8) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  } else if ((raw & 0xFFu) == JZ_DB_DISP4) {
    uint8_t op1, reg_db;
    int8_t disp4;
    int64_t offset = 0;
    Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);

    offset = (disp4 & 0x0F) * 2;  // zero-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  }
  return target;
}

uint32_t Jza::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jeq::GetTarget(data, addr, len);
}

uint32_t Jzt::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jnzt::GetTarget(data, addr, len);
}

uint32_t Loop::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  uint32_t target = 0;
  if (len == 2) {
    uint8_t op1, reg_db;
    int8_t disp4;
    int64_t offset = 0;
    Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);

    offset |= 0xFFFFFFE0 | (disp4 << 1);
    offset += addr;
    target = (uint32_t)offset;
  } else if (len == 4) {
    uint8_t op1, op2, s1, reg_ab;
    int16_t disp15;
    int64_t offset;
    Instruction::ExtractOpfieldsBRR(data, op1, op2, s1, reg_ab, disp15);

    offset = ((int64_t)disp15) * 2;  // sign-extend and multiply
    offset += addr;
    target = (uint32_t)offset;
  }
  return target;
}

uint32_t Loopu::GetTarget(const uint8_t* data, uint64_t addr, size_t len) {
  return Jge::GetTarget(data, addr, len);
}
}  // namespace Tricore
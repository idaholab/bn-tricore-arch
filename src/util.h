// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef BINARYNINJA_API_TRICORE_UTIL_H
#define BINARYNINJA_API_TRICORE_UTIL_H

#include <binaryninjaapi.h>

#include <cstdint>

#define DWORD 8
#define WORD 4
#define HWORD 2
#define BYTE 1

// See TriCore Arch. Vol. 2 CACHEA.I for examples of addressing modes
#define ADDRMODE_REGULAR 0
#define ADDRMODE_PREINC 1
#define ADDRMODE_POSTINC 2
#define ADDRMODE_CIRC 3
#define ADDRMODE_BREV 4

using namespace BinaryNinja;

namespace Tricore {
class Instruction {
 public:
  // 16-bit instruction opcode field extractions (TriCore Arch, Vol. 2,
  // Section 2.2.1)
  static void ExtractOpfieldsSB(const uint8_t* data, uint8_t& op1,
                                int8_t& disp8);
  static void ExtractOpfieldsSBC(const uint8_t* data, uint8_t& op1,
                                 int8_t& disp4, int8_t& const4);
  static void ExtractOpfieldsSBR(const uint8_t* data, uint8_t& op1,
                                 int8_t& disp4, uint8_t& s2);
  static void ExtractOpfieldsSBRN(const uint8_t* data, uint8_t& op1,
                                  int8_t& disp4, uint8_t& n);
  static void ExtractOpfieldsSC(const uint8_t* data, uint8_t& op1,
                                int8_t& const8);
  static void ExtractOpfieldsSLR(const uint8_t* data, uint8_t& op1, uint8_t& d,
                                 uint8_t& s2);
  static void ExtractOpfieldsSLRO(const uint8_t* data, uint8_t& op1, uint8_t& d,
                                  int8_t& off4);
  static void ExtractOpfieldsSR(const uint8_t* data, uint8_t& op1, uint8_t& s1d,
                                uint8_t& op2);
  static void ExtractOpfieldsSRC(const uint8_t* data, uint8_t& op1,
                                 uint8_t& s1d, int8_t& const4);
  static void ExtractOpfieldsSRO(const uint8_t* data, uint8_t& op1,
                                 int8_t& off4, uint8_t& s2);
  static void ExtractOpfieldsSRR(const uint8_t* data, uint8_t& op1,
                                 uint8_t& s1d, uint8_t& s2);
  static void ExtractOpfieldsSRRS(const uint8_t* data, uint8_t& op1, uint8_t& n,
                                  uint8_t& s1d, uint8_t& s2);
  static void ExtractOpfieldsSSR(const uint8_t* data, uint8_t& op1, uint8_t& s1,
                                 uint8_t& s2);
  static void ExtractOpfieldsSSRO(const uint8_t* data, uint8_t& op1,
                                  uint8_t& s1, int8_t& off4);
  // 32-bit instruction opcode field extractions (TriCore Arch, Vol. 2,
  // Section 2.2.2)
  static void ExtractOpfieldsABS(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1d, uint32_t& ea);
  static void ExtractOpfieldsABSB(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& b, uint8_t& bpos3,
                                  uint32_t& ea);
  static void ExtractOpfieldsB(const uint8_t* data, uint8_t& op1,
                               int32_t& disp24);
  static void ExtractOpfieldsBIT(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                 uint8_t& pos1, uint8_t& pos2, uint8_t& d);
  static void ExtractOpfieldsBO(const uint8_t* data, uint8_t& op1, uint8_t& op2,
                                uint8_t& s1d, uint8_t& s2, int16_t& off10);
  static void ExtractOpfieldsBOL(const uint8_t* data, uint8_t& op1,
                                 uint8_t& s1d, uint8_t& s2, int16_t& off16);
  static void ExtractOpfieldsBRC(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1, int8_t& const4,
                                 int16_t& disp15);
  static void ExtractOpfieldsBRN(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& n, uint8_t& s1,
                                 int16_t& disp15);
  static void ExtractOpfieldsBRR(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                 int16_t& disp15);
  static void ExtractOpfieldsRC(const uint8_t* data, uint8_t& op1, uint8_t& op2,
                                uint8_t& s1, uint8_t& d, int16_t& const9);
  static void ExtractOpfieldsRCPW(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, int8_t& const4,
                                  uint8_t& width, uint8_t& pos, uint8_t& d);
  static void ExtractOpfieldsRCR(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1, int16_t& const9,
                                 uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsRCRR(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, int8_t& const4,
                                  uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsRCRW(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, int8_t& const4,
                                  uint8_t& width, uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsRLC(const uint8_t* data, uint8_t& op1, uint8_t& s1,
                                 int16_t& const16, uint8_t& d);
  static void ExtractOpfieldsRR(const uint8_t* data, uint8_t& op1, uint8_t& op2,
                                uint8_t& s1, uint8_t& s2, uint8_t& n,
                                uint8_t& d);
  static void ExtractOpfieldsRR1(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                 uint8_t& n, uint8_t& d);
  static void ExtractOpfieldsRR2(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                 uint8_t& d);
  static void ExtractOpfieldsRRPW(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                  uint8_t& width, uint8_t& pos, uint8_t& d);
  static void ExtractOpfieldsRRR(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                 uint8_t& n, uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsRRR1(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                  uint8_t& n, uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsRRR2(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                  uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsRRRR(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                  uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsRRRW(const uint8_t* data, uint8_t& op1,
                                  uint8_t& op2, uint8_t& s1, uint8_t& s2,
                                  uint8_t& width, uint8_t& s3, uint8_t& d);
  static void ExtractOpfieldsSYS(const uint8_t* data, uint8_t& op1,
                                 uint8_t& op2, uint8_t& s1d);

  static uint32_t ExtractSecondaryOpcode(uint8_t primaryOp,
                                         const uint8_t* data);
  static const char* RegToStr(uint32_t rid);
  static void LogFunctions(const char* funcname);

  // Text
  static bool TextOpDcDb_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOpEcDb_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOpDcDaConst9_RC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned = false);
  static bool TextOpDcDaDb_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr);
  static bool TextOpDcDbDa_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr);
  static bool TextOpDaConst4_SRC(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, bool is_unsigned = false);
  static bool TextOpEaConst4_SRC(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, bool is_unsigned = false);
  static bool TextOpDaD15Const4_SRC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpD15DaConst4_SRC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpDaDb_SRR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr);
  static bool TextOpDaD15Db_SRR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpD15DaDb_SRR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAaConst4_SRC(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, bool is_unsigned = false);
  static bool TextOpAaAb_SRR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr);
  static bool TextOpAcAaAb_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr);
  static bool TextOpDcDaConst16_RLC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpAcAaConst16_RLC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpAaAbD15N_SRRS(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpAcAbDaN_RR(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr);
  static bool TextOpAcAbDa_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr);
  static bool TextOpD15Const8_SC(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, bool is_unsigned = false);
  static bool TextOpDcDaPos1DbPos2_BIT(
      const uint8_t* data, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr);
  static bool TextOpConst8_SC(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr, bool is_unsigned = false);
  static bool TextOpConst9_RC(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr, bool is_unsigned = false);
  static bool TextOpEcDa_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOpAbOff10_BO(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr,
                               uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPb_BO(const uint8_t* data, size_t len, size_t width,
                          std::vector<InstructionTextToken>& result,
                          std::string instr,
                          uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbOff10_BO(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr,
                               uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpDcDdDaConst9_RCR(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr,
                                     bool is_unsigned = false);
  static bool TextOpDcDdDaDb_RRR(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr);
  static bool TextOpDisp8_SB(const uint8_t* data, uint64_t addr, size_t len,
                             size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr);
  static bool TextOpDisp24_B(const uint8_t* data, uint64_t addr, size_t len,
                             size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr);
  static bool TextOpAa_SR(const uint8_t* data, size_t len, size_t width,
                          std::vector<InstructionTextToken>& result,
                          std::string instr);
  static bool TextOpAa_RR(const uint8_t* data, size_t len, size_t width,
                          std::vector<InstructionTextToken>& result,
                          std::string instr);
  static bool TextOpDcDa_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOp_SR(const uint8_t* data, size_t len, size_t width,
                        std::vector<InstructionTextToken>& result,
                        std::string instr);
  static bool TextOp_SYS(const uint8_t* data, size_t len, size_t width,
                         std::vector<InstructionTextToken>& result,
                         std::string instr);
  static bool TextOpDa_SYS(const uint8_t* data, size_t len, size_t width,
                           std::vector<InstructionTextToken>& result,
                           std::string instr);
  static bool TextOpDcDaDbPos_RRPW(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpDcDaDbDd_RRRR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpEcEdDb_RRR(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr);
  static bool TextOpEcDaDb_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr);
  static bool TextOpDcAaAb_RR(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr);
  static bool TextOpDcAa_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOpDcDaPosWidth_RRPW(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr);
  static bool TextOpDcDaEd_RRRR(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpDcDaDdWidth_RRRW(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpEcConst4PosWidth_RCPW(
      const uint8_t* data, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr,
      bool is_unsigned = false);
  static bool TextOpEcConst4DdWidth_RCRW(
      const uint8_t* data, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr,
      bool is_unsigned = false);
  static bool TextOpEcDbPosWidth_RRPW(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr);
  static bool TextOpEcDbDdWidth_RRRW(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcDaConst4PosWidth_RCPW(
      const uint8_t* data, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr,
      bool is_unsigned = false);
  static bool TextOpDcDaConst4Ed_RCRR(const uint8_t* data, size_t len,
                                      size_t width,
                                      std::vector<InstructionTextToken>& result,
                                      std::string instr,
                                      bool is_unsigned = false);
  static bool TextOpDcDaConst4DdWidth_RCRW(
      const uint8_t* data, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr,
      bool is_unsigned = false);
  static bool TextOpDcDaDbPosWidth_RRPW(
      const uint8_t* data, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr);
  static bool TextOpDcDaDbEd_RRRR(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDcDaDbDdWidth_RRRW(
      const uint8_t* data, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr);
  static bool TextOpD15Const4Disp4_SBC(
      const uint8_t* data, uint64_t addr, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr,
      bool is_unsigned = false);
  static bool TextOpD15DbDisp4_SBR(const uint8_t* data, uint64_t addr,
                                   size_t len, size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpDaConst4Disp15_BRC(
      const uint8_t* data, uint64_t addr, size_t len, size_t width,
      std::vector<InstructionTextToken>& result, std::string instr,
      bool is_unsigned = false);
  static bool TextOpDaDbDisp15_BRR(const uint8_t* data, uint64_t addr,
                                   size_t len, size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpAaAbDisp15_BRR(const uint8_t* data, uint64_t addr,
                                   size_t len, size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpDbDisp4_SBR(const uint8_t* data, uint64_t addr, size_t len,
                                size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpD15Disp8_SB(const uint8_t* data, uint64_t addr, size_t len,
                                size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAbDisp4_SBR(const uint8_t* data, uint64_t addr, size_t len,
                                size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAaDisp15_BRR(const uint8_t* data, uint64_t addr, size_t len,
                                 size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr);
  static bool TextOpD15NDisp4_SBRN(const uint8_t* data, uint64_t addr,
                                   size_t len, size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpDaNDisp15_BRN(const uint8_t* data, uint64_t addr,
                                  size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpA15A10Const8_SC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpAcAb_SLR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr,
                             uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpAcA15Off4_SLRO(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpA15AbOff4_SRO(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpAaOff18_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAaAbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpAaPb_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpAaPbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpDcAb_SLR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr,
                             uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpDcA15Off4_SLRO(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpD15AbOff4_SRO(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDaOff18_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpDaAbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpDaPb_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpDaPbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpEaOff18_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpEaAbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpEaPb_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpEaPbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPaOff18_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpPaAbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPaPb_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPaPbOff10_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpD15A10Const8_SC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpDaAbOff16_BOL(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned = false);
  static bool TextOpOff18_ABS(const uint8_t* data, size_t len, size_t width,
                              std::vector<InstructionTextToken>& result,
                              std::string instr);
  static bool TextOpOff18Ea_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAbOff10Ea_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbEa_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbOff10Ea_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpAaAbOff16_BOL(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpAbDisp15_BRR(const uint8_t* data, uint64_t addr, size_t len,
                                 size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr);
  static bool TextOpDisp15_BRR(const uint8_t* data, uint64_t addr, size_t len,
                               size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr);
  static bool TextOpEcEdDaConst9_RCR(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr,
                                     bool is_unsigned = false);
  static bool TextOpDcDdDaDb_RRR2(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpEcEdDaDb_RRR2(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpEcEdDaDbllN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpEcEdDaDbluN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpEcEdDaDbulN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpEcEdDaDbuuN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcDdDaDbN_RRR1(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpEcEdDaDbN_RRR1(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpDcDdDaDblN_RRR1(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr);
  static bool TextOpEcEdDaDblN_RRR1(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr);
  static bool TextOpDcDdDaDbuN_RRR1(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr);
  static bool TextOpEcEdDaDbuN_RRR1(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr);
  static bool TextOpDcDdDalDblN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpEcEdDalDblN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcDdDauDbuN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpEcEdDauDbuN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcDdDaDbllN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcDdDaDbluN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcDdDaDbulN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcEdDaDbulN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcDdDaDbuuN_RRR1(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpDcConst16_RLC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned = false);
  static bool TextOpEcConst16_RLC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned = false);
  static bool TextOpAaDb_SRR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr);
  static bool TextOpAcDb_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOpAcAb_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOpDaAb_SRR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr);
  static bool TextOpDcAb_RR(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr);
  static bool TextOpAcConst16_RLC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned = false);
  static bool TextOpConst16Da_RLC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned = false);
  static bool TextOpEcDaConst9_RC(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr, bool is_unsigned = false);
  static bool TextOpDcDaDb_RR2(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr);
  static bool TextOpEcDaDb_RR2(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr);
  static bool TextOpEcDaDbllN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpEcDaDbluN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpEcDaDbulN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpEcDaDbuuN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDcDaDbN_RR1(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpEcDaDbN_RR1(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpDcDaDblN_RR1(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr);
  static bool TextOpEcDaDblN_RR1(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr);
  static bool TextOpDcDaDbuN_RR1(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr);
  static bool TextOpEcDaDbuN_RR1(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr);
  static bool TextOpDcDalDblN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDcDauDbuN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDcDaDbllN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDcDaDbluN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDcDaDbulN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDcDaDbuuN_RR1(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpDa_SR(const uint8_t* data, size_t len, size_t width,
                          std::vector<InstructionTextToken>& result,
                          std::string instr);
  static bool TextOpDcEdDa_RRR(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr);
  static bool TextOpAbAa_SSR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr,
                             uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpA10Const8A15_SC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpAbOff4A15_SRO(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpA15Off4Aa_SSRO(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpOff18Aa_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAbOff10Aa_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbAa_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbOff10Aa_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpAbDa_SSR(const uint8_t* data, size_t len, size_t width,
                             std::vector<InstructionTextToken>& result,
                             std::string instr,
                             uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpAbOff4D15_SRO(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpA15Off4Da_SSRO(const uint8_t* data, size_t len,
                                   size_t width,
                                   std::vector<InstructionTextToken>& result,
                                   std::string instr);
  static bool TextOpOff18Da_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAbOff10Da_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbDa_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbOff10Da_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpOff18Pa_ABS(const uint8_t* data, size_t len, size_t width,
                                std::vector<InstructionTextToken>& result,
                                std::string instr);
  static bool TextOpAbOff10Pa_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbPa_BO(const uint8_t* data, size_t len, size_t width,
                            std::vector<InstructionTextToken>& result,
                            std::string instr,
                            uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpPbOff10Pa_BO(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr,
                                 uint8_t addressMode = ADDRMODE_REGULAR);
  static bool TextOpOff18Bpos3B_ABSB(const uint8_t* data, size_t len,
                                     size_t width,
                                     std::vector<InstructionTextToken>& result,
                                     std::string instr);
  static bool TextOpA10Const8D15_SC(const uint8_t* data, size_t len,
                                    size_t width,
                                    std::vector<InstructionTextToken>& result,
                                    std::string instr,
                                    bool is_unsigned = false);
  static bool TextOpAbOff16Da_BOL(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpAbOff16Aa_BOL(const uint8_t* data, size_t len, size_t width,
                                  std::vector<InstructionTextToken>& result,
                                  std::string instr);
  static bool TextOpA10Const8_SC(const uint8_t* data, size_t len, size_t width,
                                 std::vector<InstructionTextToken>& result,
                                 std::string instr, bool is_unsigned = false);
  static bool TextOpDcDdDa_RRR(const uint8_t* data, size_t len, size_t width,
                               std::vector<InstructionTextToken>& result,
                               std::string instr);
  static bool TextOpDa_RR(const uint8_t* data, size_t len, size_t width,
                          std::vector<InstructionTextToken>& result,
                          std::string instr);
  static bool TextOp_RR(const uint8_t* data, size_t len, size_t width,
                        std::vector<InstructionTextToken>& result,
                        std::string instr);
  static bool TextOpEa_RR(const uint8_t* data, size_t len, size_t width,
                          std::vector<InstructionTextToken>& result,
                          std::string instr);
};
}  // namespace Tricore

#endif  // BINARYNINJA_API_TRICORE_UTIL_H

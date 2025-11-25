// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#ifndef BINARYNINJA_API_TRICORE_INSTRUCTIONS_H
#define BINARYNINJA_API_TRICORE_INSTRUCTIONS_H

#include <binaryninjaapi.h>

#include <cstdint>

#include "flags.h"

using namespace BinaryNinja;

namespace Tricore {
class Abs {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Absbh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Absdif {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Absdifbh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Absdifs {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Absdifsh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Abss {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Abssh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Add {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Adda {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addbh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addc {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addi {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addih {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addiha {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Adds {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addshhu {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addsu {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addscaat {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Addx {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class And {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class AndAndnNorOrT {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Andeq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class AndgeGeU {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class AndltLtU {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Andne {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Andt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Andn {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Andnt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Bisr {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Bmerge {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Bsplit {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cacheai {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cacheaw {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cacheawi {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

// TCv1.6 ISA only
class Cacheii {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cacheiw {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cacheiwi {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cadd {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Caddn {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Call {
 private:
  // NOTE/TODO: this instruction reads all flags
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Calla {
 private:
  // NOTE/TODO: this instruction reads all flags
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Calli {
 private:
  // NOTE/TODO: this instruction reads all flags
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Clo {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cloh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cls {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Clsh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Clz {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Clzh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cmov {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cmovn {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cmpswap {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Crc32 {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Csub {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Csubn {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Debug {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Dextr {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Disable {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

// TCv1.6 ISA only
class DisableDa {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Div {
 private:
  static const uint32_t flags = WRITE_VSVAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Dsync {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Dvadj {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Dvinit {
 private:
  static const uint32_t flags = WRITE_VSVAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Dvstep {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Enable {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Eq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Eqa {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Eqbhw {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Eqanybh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Eqza {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Extr {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Fcall {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Fcalla {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Fcalli {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Fret {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ge {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Gea {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Imask {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class InstNt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Insert {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Isync {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ixmax {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ixmin {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class J {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Ja {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jeq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jeqa {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jge {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jgez {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jgtz {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Ji {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Jl {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jla {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jlez {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jli {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Jlt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jltz {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jne {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jnea {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jned {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jnei {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jnz {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jnza {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jnzt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jz {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jza {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Jzt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il,
                   BinaryNinja::Architecture* arch);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Lda {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldb {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldd {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldda {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldhu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldw {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldlcx {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ldmst {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Lducx {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Lea {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Loop {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Loopu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
  static uint32_t GetTarget(const uint8_t* data, uint64_t addr, size_t len);
};

class Lt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Lta {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ltb {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Lth {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ltw {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Madd {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Maddh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Maddq {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Maddu {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MaddmhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MaddrhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Maddrq {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MaddsuhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MaddsumhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MaddsurhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Max {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Maxb {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Maxh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mfcr {
 private:
  // NOTE/TODO: this instruction reads all flags
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Min {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Minb {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Minh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mov {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mova {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Movaa {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Movd {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Movu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Movh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Movha {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Msub {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Msubh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Msubq {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Msubu {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MsubadhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MsubadmhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MsubadrhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MsubmhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class MsubrhSh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Msubrq {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mtcr {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mul {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mulh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mulq {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mulu {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mulmh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mulrh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mulrq {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Nand {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Nandt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ne {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Nea {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Neza {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Nop {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Nor {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Nort {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Not {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Or {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class OrAndtAndntNortOrt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Oreq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class OrgeGeu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class OrltLtu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Orne {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ort {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Orn {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ornt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Pack {
 private:
  // NOTE/TODO: C flag is read
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Parity {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

// TCv1.6 ISA only
class RestoreDa {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ret {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Rfe {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Rfm {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Rslcx {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Rstv {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Rsub {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Rsubs {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Satb {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Satbu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sath {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sathu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sel {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Seln {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sheq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class ShgeGeu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Shh {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class ShltLtu {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Shne {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class ShAndOrNorXorT {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sha {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Shah {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Shas {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sta {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Stb {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Std {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Stda {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sth {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Stq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Stt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Stw {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Stlcx {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Stucx {
 private:
  // TODO: all flags are read
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Sub {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Suba {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Subbh {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Subc {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Subsu {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class SubshHu {
 private:
  static const uint32_t flags = WRITE_VSVAVSAV;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Subx {
 private:
  static const uint32_t flags = WRITE_ALL;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Svlcx {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Swapw {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Syscall {
 private:
  // NOTE/TODO: all flags are read
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Trapsv {
 private:
  // NOTE/TODO: SV flag is read
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Trapv {
 private:
  // NOTE/TODO: V flag is read
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                   InstructionInfo& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Unpack {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xnor {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xnort {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xor {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xoreq {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xorge {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xorlt {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xorne {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Xort {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

/* Floating Point Instructions */
class Addf {
 private:
  static const uint32_t flags = WRITE_FSFIFVFUFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Cmpf {
 private:
  static const uint32_t flags = WRITE_FSFI;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Divf {
 private:
  static const uint32_t flags = WRITE_ALL_FP;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ftoi {
 private:
  static const uint32_t flags = WRITE_FSFIFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ftoiz {
 private:
  static const uint32_t flags = WRITE_FSFIFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ftoq31 {
 private:
  static const uint32_t flags = WRITE_FSFIFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ftoq31z {
 private:
  static const uint32_t flags = WRITE_FSFIFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ftou {
 private:
  static const uint32_t flags = WRITE_FSFIFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Ftouz {
 private:
  static const uint32_t flags = WRITE_FSFIFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Itof {
 private:
  static const uint32_t flags = WRITE_FSFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Maddf {
 private:
  static const uint32_t flags = WRITE_ALL_FP;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Msubf {
 private:
  static const uint32_t flags = WRITE_ALL_FP;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Mulf {
 private:
  static const uint32_t flags = WRITE_FSFIFVFUFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Q31tof {
 private:
  static const uint32_t flags = WRITE_FSFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Qseedf {
 private:
  static const uint32_t flags = WRITE_FSFI;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Subf {
 private:
  static const uint32_t flags = WRITE_ALL_FP;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Updfl {
 private:
  static const uint32_t flags = WRITE_ALL_FP;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Utof {
 private:
  static const uint32_t flags = WRITE_FSFX;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

/* MMU Instructions */
class Tlbdemap {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Tlbflush {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Tlbmap {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Tlbprobea {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Tlbprobei {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};

class Wait {
 private:
  static const uint32_t flags = WRITE_NONE;

 public:
  static bool Text(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, std::vector<InstructionTextToken>& result);
  static bool Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il);
};
}  // namespace Tricore

#endif  // BINARYNINJA_API_TRICORE_INSTRUCTIONS_H

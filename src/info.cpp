// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>

#include <cstdint>

#include "instructions.h"

using namespace std;
using namespace BinaryNinja;

namespace Tricore {
bool Call::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Call::GetTarget(data, addr, len);

  result.AddBranch(CallDestination, target);
  result.length = len;

  return true;
}

bool Calla::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                 InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Calla::GetTarget(data, addr, len);

  result.AddBranch(CallDestination, target);
  result.length = len;

  return true;
}

bool Calli::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                 InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;

  const uint32_t target_reg = Calli::GetTarget(data, addr, len);

  result.AddBranch(IndirectBranch, target_reg);
  result.length = len;

  return true;
}

bool Fcall::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                 InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Call::GetTarget(
      data, addr, len);  // FCALL gets the target the same way CALL does

  result.AddBranch(CallDestination, target);
  result.length = len;

  return true;
}

bool Fcalla::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                  InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Calla::GetTarget(
      data, addr, len);  // FCALLA gets the target the same way CALLA does

  result.AddBranch(CallDestination, target);
  result.length = len;

  return true;
}

bool Fcalli::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                  InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;

  const uint32_t target_reg = Calli::GetTarget(data, addr, len);

  result.AddBranch(IndirectBranch, target_reg);
  result.length = len;

  return true;
}

bool J::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
             InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = J::GetTarget(data, addr, len);

  result.AddBranch(UnconditionalBranch, target);
  result.length = len;

  return true;
}

bool Ja::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
              InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Ja::GetTarget(data, addr, len);

  result.AddBranch(UnconditionalBranch, target);
  result.length = len;

  return true;
}

bool Jeq::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jeq::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jeqa::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jeqa::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jge::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jge::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jgez::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jgez::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jgtz::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jgtz::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Ji::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
              InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;

  result.AddBranch(UnresolvedBranch);
  result.length = len;

  return true;
}

bool Jl::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
              InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jl::GetTarget(data, addr, len);
  // TODO: Should Jl have a call destination? I think so...
  result.AddBranch(CallDestination, target);
  result.length = len;

  return true;
}

bool Jla::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jla::GetTarget(data, addr, len);
  // TODO: Should Jla have a call destination? I think so...
  result.AddBranch(CallDestination, target);
  result.length = len;

  return true;
}

bool Jlez::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jlez::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jli::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;

  result.AddBranch(UnresolvedBranch);
  result.length = len;

  return true;
}

bool Jlt::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jlt::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jltz::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jltz::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jne::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jne::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jnea::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jnea::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jned::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jned::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jnei::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jnei::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jnz::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jnz::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jnza::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jnza::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jnzt::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jnzt::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jz::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
              InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jz::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jza::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jza::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Jzt::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
               InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Jzt::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Loop::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Loop::GetTarget(data, addr, len);

  result.AddBranch(TrueBranch, target);
  result.AddBranch(FalseBranch, addr + len);
  result.length = len;

  return true;
}

bool Loopu::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                 InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;
  const uint32_t target = Loopu::GetTarget(data, addr, len);

  result.AddBranch(UnconditionalBranch, target);
  result.length = len;

  return true;
}

bool Trapsv::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                  InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;

  result.AddBranch(UnresolvedBranch);
  result.length = len;

  return true;
}

bool Trapv::Info(const uint8_t* data, uint64_t addr, size_t maxLen,
                 InstructionInfo& result) {
  auto wdata = (const uint16_t*)data;
  uint16_t raw = *wdata;
  size_t len = (raw & 0x1) ? 4 : 2;

  result.AddBranch(UnresolvedBranch);
  result.length = len;

  return true;
}
}  // namespace Tricore
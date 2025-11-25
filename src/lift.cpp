// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

#include <binaryninjaapi.h>

#include <cmath>
#include <cstdint>

#include "instructions.h"
#include "opcodes.h"
#include "registers.h"
#include "util.h"

using namespace std;
using namespace BinaryNinja;

#define UNIMPLEMENTED(length)            \
  il.AddInstruction(il.Unimplemented()); \
  len = length;                          \
  return true;
#define REGTOA(reg) reg += 16;
#define REGTOE(reg) reg += 32;
#define REGTOP(reg) reg += 48;

namespace Tricore {
void ssov(ExprId x, ExprId y, uint32_t result_reg, LowLevelILFunction& il,
          size_t size) {
  ExprId max_pos, max_neg, condition;
  LowLevelILLabel trueLabel1, falseLabel1, trueLabel2, falseLabel2, doneLabel1;
  max_pos = il.Sub(
      size,
      il.ShiftLeft(size, il.Const(size, 1), il.Sub(size, y, il.Const(size, 1))),
      il.Const(size, 1));
  max_neg = il.Neg(size, il.ShiftLeft(size, il.Const(size, 1),
                                      il.Sub(size, y, il.Const(size, 1))));

  condition = il.CompareSignedGreaterThan(size, x, max_pos);
  il.AddInstruction(il.If(condition, trueLabel1, falseLabel1));
  il.MarkLabel(trueLabel1);
  il.AddInstruction(il.SetRegister(size, result_reg, max_pos));
  il.AddInstruction(il.Goto(doneLabel1));

  il.MarkLabel(falseLabel1);
  condition = il.CompareSignedLessThan(size, x, max_neg);
  il.AddInstruction(il.If(condition, trueLabel2, falseLabel2));
  il.MarkLabel(trueLabel2);
  il.AddInstruction(il.SetRegister(size, result_reg, max_neg));
  il.AddInstruction(il.Goto(doneLabel1));

  il.MarkLabel(falseLabel2);
  il.AddInstruction(il.SetRegister(size, result_reg, x));
  il.AddInstruction(il.Goto(doneLabel1));

  il.MarkLabel(doneLabel1);
  return;
}

void suov(ExprId x, ExprId y, uint32_t result_reg, LowLevelILFunction& il,
          size_t size) {
  ExprId max_pos, condition;
  LowLevelILLabel trueLabel1, falseLabel1, trueLabel2, falseLabel2, doneLabel1;
  max_pos =
      il.Sub(size, il.ShiftLeft(size, il.Const(size, 1), y), il.Const(size, 1));

  condition = il.CompareUnsignedGreaterThan(size, x, max_pos);
  il.AddInstruction(il.If(condition, trueLabel1, falseLabel1));
  il.MarkLabel(trueLabel1);
  il.AddInstruction(il.SetRegister(size, result_reg, max_pos));
  il.AddInstruction(il.Goto(doneLabel1));

  il.MarkLabel(falseLabel1);
  condition = il.CompareUnsignedLessThan(size, x, il.Const(size, 0));
  il.AddInstruction(il.If(condition, trueLabel2, falseLabel2));
  il.MarkLabel(trueLabel2);
  il.AddInstruction(il.SetRegister(size, result_reg, il.Const(size, 0)));
  il.AddInstruction(il.Goto(doneLabel1));

  il.MarkLabel(falseLabel2);
  il.AddInstruction(il.SetRegister(size, result_reg, x));
  il.AddInstruction(il.Goto(doneLabel1));

  il.MarkLabel(doneLabel1);
  return;
}

void abs(ExprId x, uint32_t result_reg, LowLevelILFunction& il) {
  ExprId absCondition;
  LowLevelILLabel absTrue, absFalse, absDone;

  absCondition = il.CompareSignedLessThan(WORD, x, il.Const(WORD, 0));
  il.AddInstruction(il.If(absCondition, absTrue, absFalse));

  il.MarkLabel(absTrue);
  il.AddInstruction(
      il.SetRegister(WORD, result_reg, il.Sub(WORD, il.Const(WORD, 0), x)));
  il.AddInstruction(il.Goto(absDone));

  il.MarkLabel(absFalse);
  il.AddInstruction(il.SetRegister(WORD, result_reg, x));
  il.AddInstruction(il.Goto(absDone));

  il.MarkLabel(absDone);
  return;
}

void float_is_nan(ExprId f, uint32_t result_reg, LowLevelILFunction& il) {
  uint32_t exp = 1000, mantissa = 1001;
  // exponent == 255
  il.AddInstruction(il.SetRegister(
      WORD, LLIL_TEMP(exp),
      il.BoolToInt(WORD, il.CompareEqual(
                             WORD, il.And(WORD, f, il.Const(WORD, 0x7F800000)),
                             il.Const(WORD, 0x7F800000)))));
  // mantissa != 0
  il.AddInstruction(il.SetRegister(
      WORD, LLIL_TEMP(mantissa),
      il.BoolToInt(WORD, il.CompareNotEqual(
                             WORD, il.And(WORD, f, il.Const(WORD, 0x007FFFFF)),
                             il.Const(WORD, 0)))));
  il.AddInstruction(
      il.SetRegister(WORD, result_reg,
                     il.And(WORD, il.Register(WORD, LLIL_TEMP(exp)),
                            il.Register(WORD, LLIL_TEMP(mantissa)))));
  return;
}

void float_is_denormal(ExprId f, uint32_t result_reg, LowLevelILFunction& il) {
  uint32_t exp = 1000, mantissa = 1001;
  // exponent == 0
  il.AddInstruction(il.SetRegister(
      WORD, LLIL_TEMP(exp),
      il.BoolToInt(WORD, il.CompareEqual(
                             WORD, il.And(WORD, f, il.Const(WORD, 0x7F800000)),
                             il.Const(WORD, 0)))));
  // mantissa != 0
  il.AddInstruction(il.SetRegister(
      WORD, LLIL_TEMP(mantissa),
      il.BoolToInt(WORD, il.CompareNotEqual(
                             WORD, il.And(WORD, f, il.Const(WORD, 0x007FFFFF)),
                             il.Const(WORD, 0)))));
  il.AddInstruction(
      il.SetRegister(WORD, result_reg,
                     il.And(WORD, il.Register(WORD, LLIL_TEMP(exp)),
                            il.Register(WORD, LLIL_TEMP(mantissa)))));
  return;
}

bool Abs::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR || op2 != ABS_DC_DB) {
    LogDebug("0x%lx: Abs::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId condition;
    LowLevelILLabel trueLabel, falseLabel, doneLabel;
    len = 4;
    uint8_t reg_dc, reg_db, op1, op2, s1, n;
    char buf[32];
    Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_dc);
    condition = il.CompareSignedGreaterEqual(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 0));
    il.AddInstruction(il.If(condition, trueLabel, falseLabel));

    il.MarkLabel(trueLabel);
    il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_db)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(falseLabel);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Sub(WORD, il.Const(WORD, 0), il.Register(WORD, reg_db))));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    return true;
  }
}

bool Absbh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Absbh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case ABSB_DC_DB: {
      ExprId result_byte0, result_byte1, result_byte2, result_byte3, result,
          condition;
      LowLevelILLabel trueLabel, falseLabel, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_db, op1, op2, s1, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_dc);
      result_byte0 =
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x000000FF));
      result_byte1 =
          il.And(WORD,
                 il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                      il.Const(WORD, 8)),
                 il.Const(WORD, 0x000000FF));
      result_byte2 =
          il.And(WORD,
                 il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                      il.Const(WORD, 16)),
                 il.Const(WORD, 0x000000FF));
      result_byte3 =
          il.And(WORD,
                 il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                      il.Const(WORD, 24)),
                 il.Const(WORD, 0x000000FF));

      il.AddInstruction(il.SetRegister(BYTE, LLIL_TEMP(0), result_byte0));
      il.AddInstruction(il.SetRegister(BYTE, LLIL_TEMP(1), result_byte1));
      il.AddInstruction(il.SetRegister(BYTE, LLIL_TEMP(2), result_byte2));
      il.AddInstruction(il.SetRegister(BYTE, LLIL_TEMP(3), result_byte3));

      condition = il.CompareSignedGreaterEqual(
          BYTE, il.Register(BYTE, LLIL_TEMP(0)), il.Const(BYTE, 0));
      il.AddInstruction(il.If(condition, trueLabel, falseLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.Goto(doneLabel));
      il.MarkLabel(falseLabel);
      il.AddInstruction(il.SetRegister(
          BYTE, LLIL_TEMP(0),
          il.Sub(BYTE, il.Const(BYTE, 0), il.Register(BYTE, LLIL_TEMP(0)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      condition = il.CompareSignedGreaterEqual(
          BYTE, il.Register(BYTE, LLIL_TEMP(1)), il.Const(BYTE, 0));
      il.AddInstruction(il.If(condition, trueLabel, falseLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.Goto(doneLabel));
      il.MarkLabel(falseLabel);
      il.AddInstruction(il.SetRegister(
          BYTE, LLIL_TEMP(1),
          il.Sub(BYTE, il.Const(BYTE, 0), il.Register(BYTE, LLIL_TEMP(1)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      condition = il.CompareSignedGreaterEqual(
          BYTE, il.Register(BYTE, LLIL_TEMP(2)), il.Const(BYTE, 0));
      il.AddInstruction(il.If(condition, trueLabel, falseLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.Goto(doneLabel));
      il.MarkLabel(falseLabel);
      il.AddInstruction(il.SetRegister(
          BYTE, LLIL_TEMP(2),
          il.Sub(BYTE, il.Const(BYTE, 0), il.Register(BYTE, LLIL_TEMP(2)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      condition = il.CompareSignedGreaterEqual(
          BYTE, il.Register(BYTE, LLIL_TEMP(3)), il.Const(BYTE, 0));
      il.AddInstruction(il.If(condition, trueLabel, falseLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.Goto(doneLabel));
      il.MarkLabel(falseLabel);
      il.AddInstruction(il.SetRegister(
          BYTE, LLIL_TEMP(3),
          il.Sub(BYTE, il.Const(BYTE, 0), il.Register(BYTE, LLIL_TEMP(3)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      result = il.Or(WORD,
                     il.Or(WORD,
                           il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                        il.Const(WORD, 24)),
                           il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                        il.Const(WORD, 16))),
                     il.Or(WORD,
                           il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                        il.Const(WORD, 8)),
                           il.Register(WORD, LLIL_TEMP(0))));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    case ABSH_DC_DB: {
      ExprId result_hword0, result_hword1, result, condition;
      LowLevelILLabel trueLabel, falseLabel, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_db, op1, op2, s1, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_dc);
      result_hword0 =
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x0000FFFF));
      result_hword1 =
          il.And(WORD,
                 il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                      il.Const(WORD, 16)),
                 il.Const(WORD, 0x0000FFFF));

      il.AddInstruction(il.SetRegister(HWORD, LLIL_TEMP(0), result_hword0));
      il.AddInstruction(il.SetRegister(HWORD, LLIL_TEMP(1), result_hword1));

      condition = il.CompareSignedGreaterEqual(HWORD, result_hword0,
                                               il.Const(HWORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, falseLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.Goto(doneLabel));
      il.MarkLabel(falseLabel);
      il.AddInstruction(il.SetRegister(
          HWORD, LLIL_TEMP(0),
          il.Sub(HWORD, il.Const(HWORD, 0), il.Register(HWORD, LLIL_TEMP(0)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      condition = il.CompareSignedGreaterEqual(HWORD, result_hword1,
                                               il.Const(HWORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, falseLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.Goto(doneLabel));
      il.MarkLabel(falseLabel);
      il.AddInstruction(il.SetRegister(
          HWORD, LLIL_TEMP(1),
          il.Sub(HWORD, il.Const(HWORD, 0), il.Register(HWORD, LLIL_TEMP(1)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      result = il.Or(WORD,
                     il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                  il.Const(WORD, 16)),
                     il.Register(WORD, LLIL_TEMP(0)));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    default:
      LogDebug("0x%lx: Absbh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Absdif::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ABSDIF_DC_DA_CONST9) {
        LogDebug("0x%lx: Absdif::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        condition = il.CompareSignedGreaterThan(
            WORD, il.Register(WORD, reg_da),
            il.SignExtend(WORD, il.Const(HWORD, const9)));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Sub(WORD, il.Register(WORD, reg_da),
                   il.SignExtend(WORD, il.Const(HWORD, const9)))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Sub(WORD, il.SignExtend(WORD, il.Const(HWORD, const9)),
                   il.Register(WORD, reg_da))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != ABSDIF_DC_DA_DB) {
        LogDebug("0x%lx: Absdif::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        condition = il.CompareSignedGreaterThan(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(WORD, reg_dc,
                                         il.Sub(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        il.AddInstruction(il.SetRegister(WORD, reg_dc,
                                         il.Sub(WORD, il.Register(WORD, reg_db),
                                                il.Register(WORD, reg_da))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        return true;
      }
    default:
      LogDebug("0x%lx: Absdif::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Absdifbh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Absdifs::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ABSDIFS_DC_DA_CONST9) {
        LogDebug("0x%lx: Absdifs::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition, result;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        condition = il.CompareSignedGreaterThan(
            WORD, il.Register(WORD, reg_da),
            il.SignExtend(WORD, il.Const(HWORD, const9)));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        result = il.Sub(WORD, il.Register(WORD, reg_da),
                        il.SignExtend(WORD, il.Const(HWORD, const9)));
        il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), result));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        result = il.Sub(WORD, il.SignExtend(WORD, il.Const(HWORD, const9)),
                        il.Register(WORD, reg_da));
        il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), result));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), LLIL_TEMP(1),
             il, WORD);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != ABSDIFS_DC_DA_DB) {
        LogDebug("0x%lx: Absdifs::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition, result;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        condition = il.CompareSignedGreaterThan(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        result =
            il.Sub(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db));
        il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), result));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        result =
            il.Sub(WORD, il.Register(WORD, reg_db), il.Register(WORD, reg_da));
        il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), result));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), LLIL_TEMP(1),
             il, WORD);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
        return true;
      }
    default:
      LogDebug("0x%lx: Absdifs::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Absdifsh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Abss::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR || op2 != ABSS_DC_DB) {
    LogDebug("0x%lx: Abss::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId condition;
    LowLevelILLabel trueLabel, falseLabel, doneLabel;
    len = 4;
    uint8_t reg_dc, reg_db, op1, op2, s1, n;
    char buf[32];
    Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_dc);
    condition = il.CompareSignedGreaterEqual(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 0));
    il.AddInstruction(il.If(condition, trueLabel, falseLabel));

    il.MarkLabel(trueLabel);
    il.AddInstruction(
        il.SetRegister(WORD, LLIL_TEMP(0), il.Register(WORD, reg_db)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(falseLabel);
    il.AddInstruction(il.SetRegister(
        WORD, LLIL_TEMP(0),
        il.Sub(WORD, il.Const(WORD, 0), il.Register(WORD, reg_db))));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), LLIL_TEMP(1), il,
         WORD);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
    return true;
  }
}

bool Abssh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Add::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    // 16-bit instructions
    case ADD_DA_CONST4: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Add(WORD, il.Register(WORD, reg_da),
                                il.Const(BYTE, const4), Add::flags)));
      return true;
    }
    case ADD_DA_D15_CONST4: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Add(WORD, il.Register(WORD, TRICORE_REG_D15),
                                il.Const(BYTE, const4), Add::flags)));
      return true;
    }
    case ADD_D15_DA_CONST4: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      il.AddInstruction(
          il.SetRegister(WORD, TRICORE_REG_D15,
                         il.Add(WORD, il.Register(WORD, reg_da),
                                il.Const(BYTE, const4), Add::flags)));
      return true;
    }
    case ADD_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Add(WORD, il.Register(WORD, reg_da),
                                il.Register(WORD, reg_db), Add::flags)));
      return true;
    }
    case ADD_DA_D15_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Add(WORD, il.Register(WORD, TRICORE_REG_D15),
                                il.Register(WORD, reg_db), Add::flags)));
      return true;
    }
    case ADD_D15_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(
          il.SetRegister(WORD, TRICORE_REG_D15,
                         il.Add(WORD, il.Register(WORD, reg_da),
                                il.Register(WORD, reg_db), Add::flags)));
      return true;
    }
    // 32-bit instructions
    case OP1_x8B_RC:
      if (op2 != ADD_DC_DA_CONST9) {
        LogDebug("0x%lx: Add::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {  // new scope to prevent redefinitions
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc,
                           il.Add(WORD, il.Register(WORD, reg_da),
                                  il.Const(HWORD, const9), Add::flags)));
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != ADD_DC_DA_DB) {
        LogDebug("0x%lx: Add::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc,
                           il.Add(WORD, il.Register(WORD, reg_da),
                                  il.Register(WORD, reg_db), Add::flags)));
        return true;
      }

    default:
      LogDebug("0x%lx: Add::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Adda::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    // 16-bit instructions
    case ADDA_AA_CONST4: {
      len = 2;
      uint8_t op1, reg_aa;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_aa, const4);
      REGTOA(reg_aa)
      il.AddInstruction(il.SetRegister(
          WORD, reg_aa,
          il.Add(WORD, il.Register(WORD, reg_aa),
                 il.SignExtend(WORD, il.Const(BYTE, const4)), Adda::flags)));
      return true;
    }
    case ADDA_AA_AB: {
      len = 2;
      uint8_t op1, reg_aa, reg_ab;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_ab);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(
          il.SetRegister(WORD, reg_aa,
                         il.Add(WORD, il.Register(WORD, reg_aa),
                                il.Register(WORD, reg_ab), Adda::flags)));
      return true;
    }
    // 32-bit instructions
    case OP1_x01_RR:
      if (op2 != ADDA_AC_AA_AB) {
        LogDebug("0x%lx: Adda::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_ac, reg_aa, reg_ab, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n,
                                       reg_ac);
        REGTOA(reg_aa)
        REGTOA(reg_ab)
        REGTOA(reg_ac)
        il.AddInstruction(
            il.SetRegister(WORD, reg_ac,
                           il.Add(WORD, il.Register(WORD, reg_aa),
                                  il.Register(WORD, reg_ab), Adda::flags)));
        return true;
      }
    default:
      LogDebug("0x%lx: Adda::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Addbh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Addbh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case ADDB_DC_DA_DB: {
      ExprId result_byte0, result_byte1, result_byte2, result_byte3, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_byte0 = il.Add(
          BYTE,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x000000FF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x000000FF)));
      result_byte1 =
          il.Add(BYTE,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 8)),
                        il.Const(WORD, 0x000000FF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 8)),
                        il.Const(WORD, 0x000000FF)));
      result_byte2 =
          il.Add(BYTE,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x000000FF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x000000FF)));
      result_byte3 =
          il.Add(BYTE,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 24)),
                        il.Const(WORD, 0x000000FF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 24)),
                        il.Const(WORD, 0x000000FF)));
      result = il.Or(
          WORD,
          il.Or(WORD, il.ShiftLeft(WORD, result_byte3, il.Const(WORD, 24)),
                il.ShiftLeft(WORD, result_byte2, il.Const(WORD, 16))),
          il.Or(WORD, il.ShiftLeft(WORD, result_byte1, il.Const(WORD, 8)),
                result_byte0));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    case ADDH_DC_DA_DB: {
      ExprId result_hword0, result_hword1, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_hword0 = il.Add(
          HWORD,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x0000FFFF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x0000FFFF)));
      result_hword1 =
          il.Add(HWORD,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)));
      result =
          il.Or(WORD, il.ShiftLeft(WORD, result_hword1, il.Const(WORD, 16)),
                result_hword0);
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    default:
      LogDebug("0x%lx: Addbh::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Addc::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Addi::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  // 32-bit, single opcode
  if (op1 != ADDI_DC_DA_CONST16) {
    LogDebug("0x%lx: Addi::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 4;
    uint8_t op1, reg_da, reg_dc;
    int16_t const16;
    Instruction::ExtractOpfieldsRLC(data, op1, reg_da, const16, reg_dc);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Add(WORD, il.Register(WORD, reg_da),
               il.SignExtend(WORD, il.Const(HWORD, const16)), Addi::flags)));
    return true;
  }
}

bool Addih::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  // 32-bit, single opcode
  if (op1 != ADDIH_DC_DA_CONST16) {
    LogDebug("0x%lx: Addih::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 4;
    uint8_t op1, reg_da, reg_dc;
    int16_t const16;
    int32_t result;
    Instruction::ExtractOpfieldsRLC(data, op1, reg_da, const16, reg_dc);
    result = const16 << 16;
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc,
                       il.Add(WORD, il.Register(WORD, reg_da),
                              il.Const(WORD, result), Addih::flags)));
    return true;
  }
}

bool Addiha::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  // 32-bit, single opcode
  if (op1 != ADDIHA_AC_AA_CONST16) {
    LogDebug("0x%lx: Addiha::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 4;
    uint8_t op1, reg_aa, reg_ac;
    int16_t const16;
    int32_t result;
    Instruction::ExtractOpfieldsRLC(data, op1, reg_aa, const16, reg_ac);
    REGTOA(reg_aa)
    REGTOA(reg_ac)
    result = const16 << 16;
    il.AddInstruction(
        il.SetRegister(WORD, reg_ac,
                       il.Add(WORD, il.Register(WORD, reg_aa),
                              il.Const(WORD, result), Addiha::flags)));
    return true;
  }
}

bool Adds::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case ADDS_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.Add(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
      ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), LLIL_TEMP(1),
           il, WORD);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da, il.Register(WORD, LLIL_TEMP(1))));
      return true;
    }
    case OP1_x8B_RC:
      if (op2 != ADDS_DC_DA_CONST9) {
        LogDebug("0x%lx: Adds::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        il.AddInstruction(il.SetRegister(
            WORD, LLIL_TEMP(0),
            il.Add(WORD, il.Register(WORD, reg_da),
                   il.SignExtend(WORD, il.Const(HWORD, const9)))));
        ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), LLIL_TEMP(1),
             il, WORD);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != ADDS_DC_DA_DB) {
        LogDebug("0x%lx: Adds::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0),
                                         il.Add(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db))));
        ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), LLIL_TEMP(1),
             il, WORD);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
        return true;
      }
    default:
      LogDebug("0x%lx: Adds::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Addshhu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Addshhu::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case ADDSH_DC_DA_DB: {
      ExprId result_hword0, result_hword1, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_hword0 = il.Add(
          HWORD,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x0000FFFF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x0000FFFF)));
      result_hword1 =
          il.Add(HWORD,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)));
      ssov(result_hword0, il.Const(HWORD, 16), LLIL_TEMP(0), il, HWORD);
      ssov(result_hword1, il.Const(HWORD, 16), LLIL_TEMP(1), il, HWORD);
      result = il.Or(WORD,
                     il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                  il.Const(WORD, 16)),
                     il.Register(WORD, LLIL_TEMP(0)));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    case ADDSHU_DC_DA_DB: {
      ExprId result_hword0, result_hword1, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_hword0 = il.Add(
          HWORD,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x0000FFFF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x0000FFFF)));
      result_hword1 =
          il.Add(HWORD,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)));
      suov(result_hword0, il.Const(HWORD, 16), LLIL_TEMP(0), il, HWORD);
      suov(result_hword1, il.Const(HWORD, 16), LLIL_TEMP(1), il, HWORD);
      result = il.Or(WORD,
                     il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                  il.Const(WORD, 16)),
                     il.Register(WORD, LLIL_TEMP(0)));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    default:
      LogDebug("0x%lx: Addshhu::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Addsu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  // NOTE/TODO: this instruction calls for an unsigned Add... not sure how to do
  // that in the binja IL
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ADDSU_DC_DA_CONST9) {
        LogDebug("0x%lx: Addsu::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        il.AddInstruction(il.SetRegister(
            WORD, LLIL_TEMP(0),
            il.Add(WORD, il.Register(WORD, reg_da),
                   il.SignExtend(WORD, il.Const(HWORD, const9)))));
        ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), reg_dc, il,
             WORD);
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != ADDSU_DC_DA_DB) {
        LogDebug("0x%lx: Addsu::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0),
                                         il.Add(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db))));
        suov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), reg_dc, il,
             WORD);
        return true;
      }
    default:
      LogDebug("0x%lx: Addsu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Addscaat::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case ADDSCA_AA_AB_D15_N: {
      len = 2;
      uint8_t op1, n, reg_aa, reg_ab;
      Instruction::ExtractOpfieldsSRRS(data, op1, n, reg_aa, reg_ab);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_aa,
          il.Add(WORD, il.Register(WORD, reg_ab),
                 il.ShiftLeft(WORD, il.Register(WORD, TRICORE_REG_D15),
                              il.Const(BYTE, n)),
                 Addscaat::flags)));
      return true;
    }
    case OP1_x01_RR:
      switch (op2) {
        case ADDSCA_AC_AB_DA_N: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab, n, reg_ac;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_ab, n,
                                         reg_ac);
          REGTOA(reg_ab)
          REGTOA(reg_ac)
          il.AddInstruction(il.SetRegister(
              WORD, reg_ac,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.ShiftLeft(WORD, il.Register(WORD, reg_da),
                                  il.Const(BYTE, n)),
                     Addscaat::flags)));
          return true;
        }
        case ADDSCAT_AC_AB_DA: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab, n, reg_ac;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_ab, n,
                                         reg_ac);
          REGTOA(reg_ab)
          REGTOA(reg_ac)
          il.AddInstruction(il.SetRegister(
              WORD, reg_ac,
              il.And(WORD,
                     il.Add(WORD, il.Register(WORD, reg_ab),
                            il.ArithShiftRight(WORD, il.Register(WORD, reg_da),
                                               il.Const(WORD, 3)),
                            Addscaat::flags),
                     il.Const(WORD, 0xFFFFFFFC))));
          return true;
        }
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

bool Addx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ADDX_DC_DA_CONST9) {
        LogDebug("0x%lx: Addx::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Add(WORD, il.Register(WORD, reg_da),
                   il.SignExtend(WORD, il.Const(HWORD, const9)))));
        return true;
        // TODO: handle carry
      }
    case OP1_x0B_RR:
      if (op2 != ADDX_DC_DA_DB) {
        LogDebug("0x%lx: Addx::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        il.AddInstruction(il.SetRegister(WORD, reg_dc,
                                         il.Add(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db))));
        return true;
        // TODO: handle carry
      }
    default:
      LogDebug("0x%lx: Addx::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool And::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case AND_D15_CONST8: {
      len = 2;
      uint8_t op1;
      int8_t const8;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.And(WORD, il.Register(WORD, TRICORE_REG_D15),
                 il.ZeroExtend(WORD, il.Const(BYTE, const8)), And::flags)));
      return true;
    }
    case AND_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.And(WORD, il.Register(WORD, reg_da),
                                il.Register(WORD, reg_db), And::flags)));
      return true;
    }
    case OP1_x8F_RC:
      if (op2 != AND_DC_DA_CONST9) {
        LogDebug("0x%lx: And::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        const9 = const9 & 0x01FF;  // clear sign-extended bits
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.And(WORD, il.Register(WORD, reg_da),
                   il.ZeroExtend(WORD, il.Const(HWORD, const9)), And::flags)));
        return true;
      }
    case OP1_x0F_RR:
      if (op2 != AND_DC_DA_DB) {
        LogDebug("0x%lx: And::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc,
                           il.And(WORD, il.Register(WORD, reg_da),
                                  il.Register(WORD, reg_db), And::flags)));
        return true;
      }
    default:
      LogDebug("0x%lx: And::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool AndAndnNorOrT::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                         uint64_t addr, size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x47_BIT) {
    LogDebug("0x%lx: AndAndnNorOrT::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case ANDANDT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, bp1)),
                                   il.Const(WORD, 0)));
      db = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_db),
                                          il.Const(WORD, bp2)),
                                   il.Const(WORD, 0)));
      result = il.And(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    case ANDANDNT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, bp1)),
                                   il.Const(WORD, 0)));
      db = il.BoolToInt(WORD,
                        il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_db),
                                               il.Const(WORD, bp2)),
                                        il.Const(WORD, 0)));
      result = il.And(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    case ANDNORT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(WORD,
                        il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_da),
                                               il.Const(WORD, bp1)),
                                        il.Const(WORD, 0)));
      db = il.BoolToInt(WORD,
                        il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_db),
                                               il.Const(WORD, bp2)),
                                        il.Const(WORD, 0)));
      result = il.And(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    case ANDORT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, bp1)),
                                   il.Const(WORD, 0)));
      db = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_db),
                                          il.Const(WORD, bp2)),
                                   il.Const(WORD, 0)));
      result = il.Or(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    default:
      LogDebug("0x%lx: AndAndnNorOrT::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Andeq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ANDEQ_DC_DA_CONST9) {
        LogDebug("0x%lx: Andeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId eq;
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        eq = il.BoolToInt(
            WORD,
            il.CompareEqual(WORD, il.Register(WORD, reg_da),
                            il.SignExtend(WORD, il.Const(HWORD, const9))));
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), eq)));
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != ANDEQ_DC_DA_DB) {
        LogDebug("0x%lx: Andeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId eq;
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        eq = il.BoolToInt(WORD, il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db)));
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), eq)));
        return true;
      }
    default:
      LogDebug("0x%lx: Andeq::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool AndgeGeU::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ANDGE_DC_DA_CONST9: {
          ExprId ge;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          ge = il.BoolToInt(WORD,
                            il.CompareSignedGreaterEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        case ANDGEU_DC_DA_CONST9: {
          ExprId ge;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          ge = il.BoolToInt(WORD,
                            il.CompareUnsignedGreaterEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.ZeroExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        default:
          LogDebug("0x%lx: AndgeGeU::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ANDGE_DC_DA_DB: {
          ExprId ge;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          ge = il.BoolToInt(WORD, il.CompareSignedGreaterEqual(
                                      WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        case ANDGEU_DC_DA_DB: {
          ExprId ge;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          ge = il.BoolToInt(WORD, il.CompareUnsignedGreaterEqual(
                                      WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
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

bool AndltLtU::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ANDLT_DC_DA_CONST9: {
          ExprId ge;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          ge = il.BoolToInt(WORD,
                            il.CompareSignedLessThan(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        case ANDLTU_DC_DA_CONST9: {
          ExprId ge;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          ge = il.BoolToInt(WORD,
                            il.CompareUnsignedLessThan(
                                WORD, il.Register(WORD, reg_da),
                                il.ZeroExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        default:
          LogDebug("0x%lx: AndltLtU::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ANDLT_DC_DA_DB: {
          ExprId ge;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          ge = il.BoolToInt(
              WORD, il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        case ANDLTU_DC_DA_DB: {
          ExprId ge;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          ge = il.BoolToInt(
              WORD, il.CompareUnsignedLessThan(WORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
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

bool Andne::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      if (op2 != ANDNE_DC_DA_CONST9) {
        LogDebug("0x%lx: Andne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId ge;
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        ge = il.BoolToInt(
            WORD,
            il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(HWORD, const9))));
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != ANDNE_DC_DA_DB) {
        LogDebug("0x%lx: Andne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId ge;
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        ge = il.BoolToInt(WORD,
                          il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)));
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc, il.And(WORD, il.Register(WORD, reg_dc), ge)));
        return true;
      }
    default:
      LogDebug("0x%lx: Andne::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Andt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x87_BIT || op2 != ANDT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Andt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId da, db, condition;
    LowLevelILLabel trueLabel1, trueLabel2, doneLabel;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.CompareNotEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
        il.Const(WORD, 0));
    db = il.CompareNotEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
        il.Const(WORD, 0));

    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 0)));
    il.AddInstruction(il.If(da, trueLabel1, doneLabel));
    il.MarkLabel(trueLabel1);
    il.AddInstruction(il.If(db, trueLabel2, doneLabel));
    il.MarkLabel(trueLabel2);
    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 1)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(0))));
    return true;
  }
}

bool Andn::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8F_RC:
      if (op2 != ANDN_DC_DA_CONST9) {
        LogDebug("0x%lx: Andn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        const9 &= 0x01FF;  // clear sign-extended bits
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.And(
                WORD, il.Register(WORD, reg_da),
                il.Not(WORD, il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
        return true;
      }
    case OP1_x0F_RR:
      if (op2 != ANDN_DC_DA_DB) {
        LogDebug("0x%lx: Andn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc,
                           il.And(WORD, il.Register(WORD, reg_da),
                                  il.Not(WORD, il.Register(WORD, reg_db)))));
        return true;
      }
    default:
      LogDebug("0x%lx: Andn::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Andnt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x87_BIT || op2 != ANDNT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Andnt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId da, db, condition;
    LowLevelILLabel trueLabel1, trueLabel2, doneLabel;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.CompareNotEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
        il.Const(WORD, 0));
    db = il.CompareEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
        il.Const(WORD, 0));

    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 0)));
    il.AddInstruction(il.If(da, trueLabel1, doneLabel));
    il.MarkLabel(trueLabel1);
    il.AddInstruction(il.If(db, trueLabel2, doneLabel));
    il.MarkLabel(trueLabel2);
    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 1)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(0))));
    return true;
  }
}

bool Bisr::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Bmerge::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Bsplit::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cacheai::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cacheaw::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cacheawi::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

// TCv1.6 ISA only
bool Cacheii::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cacheiw::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cacheiwi::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cadd::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case CADD_DA_D15_CONST4: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      condition = il.CompareNotEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                     il.Const(WORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Add(WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(BYTE, const4)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case OP1_xAB_RCR:
      if (op2 != CADD_DC_DD_DA_CONST9) {
        LogDebug("0x%lx: Cadd::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t op1, op2, reg_da, reg_dd, reg_dc;
        int16_t const9;
        Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9, reg_dd,
                                        reg_dc);
        condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_dd),
                                       il.Const(WORD, 0));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Add(WORD, il.Register(WORD, reg_da),
                   il.SignExtend(WORD, il.Const(HWORD, const9)))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        return true;
      }
    case OP1_x2B_RRR:
      if (op2 != CADD_DC_DD_DA_DB) {
        LogDebug("0x%lx: Cadd::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
        Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n,
                                        reg_dd, reg_dc);
        condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_dd),
                                       il.Const(WORD, 0));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(WORD, reg_dc,
                                         il.Add(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        return true;
      }
    default:
      LogDebug("0x%lx: Cadd::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Caddn::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case CADDN_DA_D15_CONST4: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      condition = il.CompareEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                  il.Const(WORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Add(WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(BYTE, const4)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case OP1_xAB_RCR:
      if (op2 != CADDN_DC_DD_DA_CONST9) {
        LogDebug("0x%lx: Caddn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t op1, op2, reg_da, reg_dd, reg_dc;
        int16_t const9;
        Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9, reg_dd,
                                        reg_dc);
        condition =
            il.CompareEqual(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Add(WORD, il.Register(WORD, reg_da),
                   il.SignExtend(WORD, il.Const(HWORD, const9)))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        return true;
      }
    case OP1_x2B_RRR:
      if (op2 != CADDN_DC_DD_DA_DB) {
        LogDebug("0x%lx: Caddn::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId condition;
        LowLevelILLabel trueLabel, falseLabel, doneLabel;
        len = 4;
        uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
        Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n,
                                        reg_dd, reg_dc);
        condition =
            il.CompareEqual(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0));
        il.AddInstruction(il.If(condition, trueLabel, falseLabel));

        il.MarkLabel(trueLabel);
        il.AddInstruction(il.SetRegister(WORD, reg_dc,
                                         il.Add(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db))));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(falseLabel);
        il.AddInstruction(
            il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
        il.AddInstruction(il.Goto(doneLabel));

        il.MarkLabel(doneLabel);
        return true;
      }
    default:
      LogDebug("0x%lx: Caddn::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Call::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  // TODO: add LLIL to save state
  switch (op1) {
    case CALL_DISP8:
      len = 2;
      il.AddInstruction(
          il.Call(il.ConstPointer(WORD, Call::GetTarget(data, addr, len))));
      return true;
    case CALL_DISP24:
      len = 4;
      il.AddInstruction(
          il.Call(il.ConstPointer(WORD, Call::GetTarget(data, addr, len))));
      return true;
    default:
      LogDebug("0x%lx: Call::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Calla::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  // TODO: add LLIL to save state
  if (op1 != CALLA_DISP24) {
    LogDebug("0x%lx: Calla::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  il.AddInstruction(
      il.Call(il.ConstPointer(WORD, Calla::GetTarget(data, addr, len))));
  return true;
}

bool Calli::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x2D_RR || op2 != CALLI_AA) {
    LogDebug("0x%lx: Calli::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, s2, n, d;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s2, n, d);
    REGTOA(reg_aa)
    il.AddInstruction(il.Call(il.Register(WORD, reg_aa)));
    return true;
  }
}

bool Clo::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cloh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cls::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Clsh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Clz::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Clzh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Cmov::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case CMOV_DA_D15_CONST4: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      condition = il.CompareNotEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                     il.Const(WORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da, il.SignExtend(WORD, il.Const(BYTE, const4))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case CMOV_DA_D15_DB: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      condition = il.CompareNotEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                     il.Const(WORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da, il.Register(WORD, reg_db)));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    default:
      LogDebug("0x%lx: Cmov::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Cmovn::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case CMOVN_DA_D15_CONST4: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      condition = il.CompareEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                  il.Const(WORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da, il.SignExtend(WORD, il.Const(BYTE, const4))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case CMOVN_DA_D15_DB: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      condition = il.CompareEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                  il.Const(WORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da, il.Register(WORD, reg_db)));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    default:
      LogDebug("0x%lx: Cmovn::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

// TCv1.6 ISA only
bool Cmpswap::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

// TCv1.6 ISA only
bool Crc32::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Csub::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x2B_RRR || op2 != CSUB_DC_DD_DA_DB) {
    LogDebug("0x%lx: Csub::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId condition;
    LowLevelILLabel trueLabel, falseLabel, doneLabel;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
    Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                    reg_dc);
    condition =
        il.CompareNotEqual(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0));
    il.AddInstruction(il.If(condition, trueLabel, falseLabel));

    il.MarkLabel(trueLabel);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Sub(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(falseLabel);
    il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    return true;
  }
}

bool Csubn::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x2B_RRR || op2 != CSUBN_DC_DD_DA_DB) {
    LogDebug("0x%lx: Csubn::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId condition;
    LowLevelILLabel trueLabel, falseLabel, doneLabel;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
    Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                    reg_dc);
    condition =
        il.CompareEqual(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0));
    il.AddInstruction(il.If(condition, trueLabel, falseLabel));

    il.MarkLabel(trueLabel);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Sub(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(falseLabel);
    il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    return true;
  }
}

bool Debug::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Dextr::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  // This instruction requires shifting between registers, and they are not
  // necessarily adjacent (e.g. e0)
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Disable::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

// TCv1.6 ISA only
bool DisableDa::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

// TCv1.6 ISA only
bool Div::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Dsync::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Dvadj::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x6B_RRR || op2 != DVADJ_EC_ED_DB) {
    LogDebug("0x%lx: Dvadj::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId cond_absequal, cond_edhighbit, cond_edlow_highbit;
    LowLevelILLabel trueLabel1, falseLabel1, trueLabel2, trueLabel3,
        falseLabel3, trueLabel4, falseLabel4, doneLabel;
    len = 4;
    uint8_t op1, op2, s1, reg_db, n, reg_ed, reg_ec, reg_edlow, reg_edhigh,
        reg_eclow, reg_echigh;
    Instruction::ExtractOpfieldsRRR(data, op1, op2, s1, reg_db, n, reg_ed,
                                    reg_ec);
    // Get individual registers (E[2][63:0] == d2/d3 == {D3[31:0], D2[31:0]})
    reg_edlow = reg_ed;
    reg_edhigh = reg_ed + 1;
    reg_eclow = reg_ec;
    reg_echigh = reg_ec + 1;
    REGTOE(reg_ed)
    REGTOE(reg_ec)

    abs(il.Register(WORD, reg_edhigh), LLIL_TEMP(0), il);
    abs(il.Register(WORD, reg_db), LLIL_TEMP(1), il);
    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(2),
                                     il.And(WORD, il.Register(WORD, reg_edhigh),
                                            il.Const(WORD, 0x80000000))));
    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(3),
                                     il.And(WORD, il.Register(WORD, reg_edlow),
                                            il.Const(WORD, 0x80000000))));

    cond_absequal = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Register(WORD, LLIL_TEMP(1)));
    cond_edhighbit = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                     il.Const(WORD, 0x80000000));
    cond_edlow_highbit = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                         il.Const(WORD, 0x80000000));

    // If
    il.AddInstruction(il.If(cond_absequal, trueLabel1, falseLabel1));
    il.MarkLabel(trueLabel1);
    il.AddInstruction(il.If(cond_edhighbit, trueLabel2, falseLabel1));

    // Then
    il.MarkLabel(trueLabel2);
    il.AddInstruction(il.If(cond_edlow_highbit, trueLabel3, falseLabel3));

    il.MarkLabel(trueLabel3);
    il.AddInstruction(
        il.SetRegister(WORD, reg_eclow, il.Register(WORD, reg_edlow)));
    il.AddInstruction(il.SetRegister(WORD, reg_echigh, il.Const(WORD, 0)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(falseLabel3);
    il.AddInstruction(il.SetRegister(
        WORD, reg_eclow,
        il.Add(WORD, il.Register(WORD, reg_edlow), il.Const(WORD, 1))));
    il.AddInstruction(il.SetRegister(WORD, reg_echigh, il.Const(WORD, 0)));
    il.AddInstruction(il.Goto(doneLabel));

    // Else
    il.MarkLabel(falseLabel1);
    il.AddInstruction(il.If(cond_edlow_highbit, trueLabel4, falseLabel4));

    il.MarkLabel(trueLabel4);
    il.AddInstruction(il.SetRegister(
        WORD, reg_eclow,
        il.Add(WORD, il.Register(WORD, reg_edlow), il.Const(WORD, 1))));
    il.AddInstruction(
        il.SetRegister(WORD, reg_echigh, il.Register(WORD, reg_edhigh)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(falseLabel4);
    il.AddInstruction(
        il.SetRegister(WORD, reg_eclow, il.Register(WORD, reg_edlow)));
    il.AddInstruction(
        il.SetRegister(WORD, reg_echigh, il.Register(WORD, reg_edhigh)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    return true;
  }
}

bool Dvinit::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  // TODO: handle flags
  if (op1 != OP1_x4B_RR) {
    LogDebug("0x%lx: Dvinit::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case DVINITB_EC_DA_DB: {
      ExprId cond_quotient_sign;
      LowLevelILLabel trueLabel, doneLabel;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_ec);
      REGTOE(reg_ec)
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x80000000))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x80000000))));
      cond_quotient_sign =
          il.CompareNotEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                             il.Register(WORD, LLIL_TEMP(1)));

      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec,
          il.ShiftLeft(DWORD, il.SignExtend(DWORD, il.Register(WORD, reg_da)),
                       il.Const(WORD, 24))));

      il.AddInstruction(il.If(cond_quotient_sign, trueLabel, doneLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec,
          il.Or(DWORD, il.Register(DWORD, reg_ec), il.Const(DWORD, 0xFFFFFF))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case DVINITBU_EC_DA_DB: {
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_ec);
      REGTOE(reg_ec)

      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec,
          il.ShiftLeft(DWORD, il.ZeroExtend(DWORD, il.Register(WORD, reg_da)),
                       il.Const(WORD, 24))));
      return true;
    }
    case DVINITH_EC_DA_DB: {
      ExprId cond_quotient_sign;
      LowLevelILLabel trueLabel, doneLabel;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_ec);
      REGTOE(reg_ec)

      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec,
          il.ShiftLeft(DWORD, il.SignExtend(DWORD, il.Register(WORD, reg_da)),
                       il.Const(WORD, 16))));

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x80000000))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x80000000))));
      cond_quotient_sign =
          il.CompareNotEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                             il.Register(WORD, LLIL_TEMP(1)));

      il.AddInstruction(il.If(cond_quotient_sign, trueLabel, doneLabel));
      il.MarkLabel(trueLabel);
      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec,
          il.Or(DWORD, il.Register(DWORD, reg_ec), il.Const(DWORD, 0xFFFF))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case DVINITHU_EC_DA_DB: {
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_ec);
      REGTOE(reg_ec)

      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec,
          il.ShiftLeft(DWORD, il.ZeroExtend(DWORD, il.Register(WORD, reg_da)),
                       il.Const(WORD, 16))));
      return true;
    }
    case DVINIT_EC_DA_DB: {
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_ec);
      REGTOE(reg_ec)

      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec, il.SignExtend(DWORD, il.Register(WORD, reg_da))));
      return true;
    }
    case DVINITU_EC_DA_DB: {
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, n, reg_ec;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_ec);
      REGTOE(reg_ec)

      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec, il.ZeroExtend(DWORD, il.Register(WORD, reg_da))));
      return true;
    }
    default:
      LogDebug("0x%lx: Dvinit::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Dvstep::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x6B_RRR) {
    LogDebug("0x%lx: Dvstep::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case DVSTEP_EC_ED_DB: {
      ExprId c1, c2, c3, loopc;
      uint8_t dividend_sign = 0, divisor_sign = 1, quotient_sign = 2,
              dividend_quotient = 3;
      uint8_t remainder = 4, addend = 5, ireg = 6, temp = 7;
      LowLevelILLabel t1, f1, loopstart, t2, d2, t3, f3, d3, loopend;
      len = 4;
      uint8_t op1, op2, s1, reg_db, n, reg_ed, reg_edlow, reg_edhigh, reg_ec,
          reg_eclow, reg_echigh;
      Instruction::ExtractOpfieldsRRR(data, op1, op2, s1, reg_db, n, reg_ed,
                                      reg_ec);
      reg_edlow = reg_ed;
      reg_edhigh = reg_ed + 1;
      reg_eclow = reg_ec;
      reg_echigh = reg_ec + 1;
      REGTOE(reg_ed)
      REGTOE(reg_ec)

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(dividend_sign),
          il.BoolToInt(WORD, il.CompareNotEqual(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_edhigh),
                                        il.Const(WORD, 0x80000000)),
                                 il.Const(WORD, 0)))));

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(divisor_sign),
          il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 0x80000000)),
                                       il.Const(WORD, 0)))));

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(quotient_sign),
          il.BoolToInt(WORD,
                       il.CompareNotEqual(
                           WORD, il.Register(WORD, LLIL_TEMP(dividend_sign)),
                           il.Register(WORD, LLIL_TEMP(divisor_sign))))));

      il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(dividend_quotient),
                                       il.Register(WORD, reg_edlow)));

      il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(remainder),
                                       il.Register(WORD, reg_edhigh)));

      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(ireg), il.Const(WORD, 0)));

      // set addend
      c1 = il.CompareNotEqual(WORD, il.Register(WORD, LLIL_TEMP(quotient_sign)),
                              il.Const(WORD, 0));
      il.AddInstruction(il.If(c1, t1, f1));
      il.MarkLabel(t1);
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(addend), il.Register(WORD, reg_db)));
      il.AddInstruction(il.Goto(loopstart));
      il.MarkLabel(f1);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(addend), il.Neg(WORD, il.Register(WORD, reg_db))));
      il.AddInstruction(il.Goto(loopstart));

      // Loop for i = 0 to 7:
      il.MarkLabel(loopstart);
      // remainder = (remainder << 1) | dividend_quotient[31];
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(remainder),
          il.Or(WORD,
                il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(remainder)),
                             il.Const(WORD, 1)),
                il.And(WORD, il.Register(WORD, LLIL_TEMP(dividend_quotient)),
                       il.Const(WORD, 0x80000000)))));
      // dividend_quotiend <<= 1;
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(dividend_quotient),
          il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(dividend_quotient)),
                       il.Const(WORD, 1))));
      // temp = remainder + addend;
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(temp),
                         il.Add(WORD, il.Register(WORD, LLIL_TEMP(remainder)),
                                il.Register(WORD, LLIL_TEMP(addend)))));

      // remainder = ((temp < 0) == dividend_sign) ? temp : remainder;
      c2 = il.CompareEqual(
          WORD,
          il.CompareSignedLessThan(WORD, il.Register(WORD, LLIL_TEMP(temp)),
                                   il.Const(WORD, 0)),
          il.Register(WORD, LLIL_TEMP(dividend_sign)));
      il.AddInstruction(il.If(c2, t2, d2));
      il.MarkLabel(t2);
      il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(remainder),
                                       il.Register(WORD, LLIL_TEMP(temp))));
      il.AddInstruction(il.Goto(d2));

      // dividend_quotient = dividend_quotient | (((temp < 0) == dividend_sign)
      // ? !quotient_sign : quotient_sign);
      il.MarkLabel(d2);
      c3 = il.CompareEqual(
          WORD,
          il.CompareSignedLessThan(WORD, il.Register(WORD, LLIL_TEMP(temp)),
                                   il.Const(WORD, 0)),
          il.Register(WORD, LLIL_TEMP(dividend_sign)));
      il.AddInstruction(il.If(c3, t3, f3));

      il.MarkLabel(t3);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(temp),
          il.BoolToInt(
              WORD,
              il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(quotient_sign)),
                              il.Const(WORD, 0)))));
      il.AddInstruction(il.Goto(d3));

      il.MarkLabel(f3);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(temp), il.Register(WORD, LLIL_TEMP(quotient_sign))));
      il.AddInstruction(il.Goto(d3));

      il.MarkLabel(d3);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(dividend_quotient),
          il.Or(WORD, il.Register(WORD, LLIL_TEMP(dividend_quotient)),
                il.Register(WORD, LLIL_TEMP(temp)))));

      // Loop check
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(ireg),
          il.Add(WORD, il.Register(WORD, LLIL_TEMP(ireg)), il.Const(WORD, 1))));
      loopc = il.CompareSignedLessThan(WORD, il.Register(WORD, LLIL_TEMP(ireg)),
                                       il.Const(WORD, 8));
      il.AddInstruction(il.If(loopc, loopstart, loopend));

      il.MarkLabel(loopend);
      il.AddInstruction(il.SetRegister(
          WORD, reg_echigh, il.Register(WORD, LLIL_TEMP(remainder))));
      il.AddInstruction(il.SetRegister(
          WORD, reg_eclow, il.Register(WORD, LLIL_TEMP(dividend_quotient))));
      return true;
    }
    case DVSTEPU_EC_ED_DB: {
      ExprId c1, c2, loopc;
      uint8_t divisor = 0, dividend_quotient = 1, remainder = 2, ireg = 3,
              temp = 4;
      LowLevelILLabel t1, f1, loopstart, t2, d2, loopend;
      len = 4;
      uint8_t op1, op2, s1, reg_db, n, reg_ed, reg_edlow, reg_edhigh, reg_ec,
          reg_eclow, reg_echigh;
      Instruction::ExtractOpfieldsRRR(data, op1, op2, s1, reg_db, n, reg_ed,
                                      reg_ec);
      reg_edlow = reg_ed;
      reg_edhigh = reg_ed + 1;
      reg_eclow = reg_ec;
      reg_echigh = reg_ec + 1;
      REGTOE(reg_ed)
      REGTOE(reg_ec)

      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(divisor), il.Register(WORD, reg_db)));
      il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(dividend_quotient),
                                       il.Register(WORD, reg_edlow)));
      il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(remainder),
                                       il.Register(WORD, reg_edhigh)));
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(ireg), il.Const(WORD, 0)));

      // Loop for i = 0 to 7:
      il.MarkLabel(loopstart);
      // remainder = (remainder << 1) | dividend_quotient[31];
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(remainder),
          il.Or(WORD,
                il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(remainder)),
                             il.Const(WORD, 1)),
                il.And(WORD, il.Register(WORD, LLIL_TEMP(dividend_quotient)),
                       il.Const(WORD, 0x80000000)))));
      // dividend_quotiend <<= 1;
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(dividend_quotient),
          il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(dividend_quotient)),
                       il.Const(WORD, 1))));
      // temp = remainder - divisor;
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(temp),
                         il.Sub(WORD, il.Register(WORD, LLIL_TEMP(remainder)),
                                il.Register(WORD, LLIL_TEMP(divisor)))));

      // remainder = (temp < 0) ? remainder : temp;
      c2 = il.CompareUnsignedGreaterEqual(
          WORD, il.Register(WORD, LLIL_TEMP(temp)), il.Const(WORD, 0));
      il.AddInstruction(il.If(c2, t2, d2));
      il.MarkLabel(t2);
      il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(remainder),
                                       il.Register(WORD, LLIL_TEMP(temp))));
      il.AddInstruction(il.Goto(d2));

      // dividend_quotient = dividend_quotient | !(temp < 0);
      il.MarkLabel(d2);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(dividend_quotient),
          il.Or(WORD, il.Register(WORD, LLIL_TEMP(dividend_quotient)),
                il.BoolToInt(WORD, il.CompareUnsignedGreaterEqual(
                                       WORD, il.Register(WORD, LLIL_TEMP(temp)),
                                       il.Const(WORD, 0))))));

      // Loop check
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(ireg),
          il.Add(WORD, il.Register(WORD, LLIL_TEMP(ireg)), il.Const(WORD, 1))));
      loopc = il.CompareSignedLessThan(WORD, il.Register(WORD, LLIL_TEMP(ireg)),
                                       il.Const(WORD, 8));
      il.AddInstruction(il.If(loopc, loopstart, loopend));

      il.MarkLabel(loopend);
      il.AddInstruction(il.SetRegister(
          WORD, reg_echigh, il.Register(WORD, LLIL_TEMP(remainder))));
      il.AddInstruction(il.SetRegister(
          WORD, reg_eclow, il.Register(WORD, LLIL_TEMP(dividend_quotient))));
      return true;
    }
    default:
      LogDebug("0x%lx: Dvstep::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Enable::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Eq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case EQ_D15_DA_CONST4: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.ZeroExtend(
              WORD,
              il.BoolToInt(WORD,
                           il.CompareEqual(
                               WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(BYTE, const4)))))));
      return true;
    }
    case EQ_D15_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.ZeroExtend(
              WORD, il.BoolToInt(
                        WORD, il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                              il.Register(WORD, reg_db))))));
      return true;
    }
    case OP1_x8B_RC:
      if (op2 != EQ_DC_DA_CONST9) {
        LogDebug("0x%lx: Eq::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_da, reg_dc, op1, op2;
        int16_t const9;
        Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.ZeroExtend(
                WORD,
                il.BoolToInt(
                    WORD, il.CompareEqual(
                              WORD, il.Register(WORD, reg_da),
                              il.SignExtend(WORD, il.Const(HWORD, const9)))))));
        return true;
      }
    case OP1_x0B_RR:
      if (op2 != EQ_DC_DA_DB) {
        LogDebug("0x%lx: Eq::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      {
        len = 4;
        uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                       reg_dc);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.ZeroExtend(
                WORD, il.BoolToInt(
                          WORD, il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db))))));
        return true;
      }
    default:
      LogDebug("0x%lx: Eq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Eqa::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x01_RR || op2 != EQA_DC_AA_AB) {
    LogDebug("0x%lx: Eqa::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, reg_ab, n, reg_dc;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n, reg_dc);
    REGTOA(reg_aa)
    REGTOA(reg_ab)
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.ZeroExtend(
            WORD,
            il.BoolToInt(WORD, il.CompareEqual(WORD, il.Register(WORD, reg_aa),
                                               il.Register(WORD, reg_ab))))));
    return true;
  }
}

bool Eqbhw::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Eqbhw::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case EQB_DC_DA_DB: {
      ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(3),
          il.BoolToInt(
              WORD,
              il.CompareEqual(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(2),
          il.BoolToInt(
              WORD,
              il.CompareEqual(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareEqual(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD,
                       il.CompareEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 0xFF)),
                                       il.And(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 0xFF))))));

      cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                   il.Const(WORD, 1));
      cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                   il.Const(WORD, 1));
      cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                   il.Const(WORD, 1));
      cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                   il.Const(WORD, 1));

      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

      il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF00))));
      il.AddInstruction(il.Goto(falseLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
      il.MarkLabel(trueLabel2);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF0000))));
      il.AddInstruction(il.Goto(falseLabel2));

      il.MarkLabel(falseLabel2);
      il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
      il.MarkLabel(trueLabel3);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF000000))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case EQH_DC_DA_DB: {
      ExprId cond_hword0, cond_hword1;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareEqual(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD,
                       il.CompareEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 0xFFFF)),
                                       il.And(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 0xFFFF))))));

      cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Const(WORD, 1));
      cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                    il.Const(WORD, 1));

      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

      il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFFFF))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFFFF0000))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case EQW_DC_DA_DB: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      condition = il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                  il.Register(WORD, reg_db));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Const(WORD, 0xFFFFFFFF)));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    default:
      LogDebug("0x%lx: Eqbhw::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Eqanybh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case EQANYB_DC_DA_CONST9: {
          ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3, constant;
          LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
              falseLabel0, falseLabel1, falseLabel2, doneLabel;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          constant = il.SignExtend(WORD, il.Const(HWORD, const9));

          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(3),
              il.BoolToInt(WORD,
                           il.CompareEqual(
                               WORD,
                               il.And(WORD,
                                      il.LogicalShiftRight(
                                          WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, 24)),
                                      il.Const(WORD, 0xFF)),
                               il.And(WORD,
                                      il.LogicalShiftRight(WORD, constant,
                                                           il.Const(WORD, 24)),
                                      il.Const(WORD, 0xFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(2),
              il.BoolToInt(WORD,
                           il.CompareEqual(
                               WORD,
                               il.And(WORD,
                                      il.LogicalShiftRight(
                                          WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, 16)),
                                      il.Const(WORD, 0xFF)),
                               il.And(WORD,
                                      il.LogicalShiftRight(WORD, constant,
                                                           il.Const(WORD, 16)),
                                      il.Const(WORD, 0xFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(1),
              il.BoolToInt(
                  WORD,
                  il.CompareEqual(WORD,
                                  il.And(WORD,
                                         il.LogicalShiftRight(
                                             WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 8)),
                                         il.Const(WORD, 0xFF)),
                                  il.And(WORD,
                                         il.LogicalShiftRight(
                                             WORD, constant, il.Const(WORD, 8)),
                                         il.Const(WORD, 0xFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.BoolToInt(WORD,
                           il.CompareEqual(
                               WORD,
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF)),
                               il.And(WORD, constant, il.Const(WORD, 0xFF))))));

          cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                       il.Const(WORD, 1));
          cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                       il.Const(WORD, 1));
          cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                       il.Const(WORD, 1));
          cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                       il.Const(WORD, 1));

          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

          il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
          il.MarkLabel(trueLabel0);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel0);
          il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
          il.MarkLabel(trueLabel1);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
          il.MarkLabel(trueLabel2);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
          il.MarkLabel(trueLabel3);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        case EQANYH_DC_DA_CONST9: {
          ExprId cond_hword0, cond_hword1, constant;
          LowLevelILLabel trueLabel0, trueLabel1, falseLabel0, doneLabel;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          constant = il.SignExtend(WORD, il.Const(HWORD, const9));

          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(1),
              il.BoolToInt(WORD,
                           il.CompareEqual(
                               WORD,
                               il.And(WORD,
                                      il.LogicalShiftRight(
                                          WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, 16)),
                                      il.Const(WORD, 0xFFFF)),
                               il.And(WORD,
                                      il.LogicalShiftRight(WORD, constant,
                                                           il.Const(WORD, 16)),
                                      il.Const(WORD, 0xFFFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.BoolToInt(
                  WORD, il.CompareEqual(
                            WORD,
                            il.And(WORD, il.Register(WORD, reg_da),
                                   il.Const(WORD, 0xFFFF)),
                            il.And(WORD, constant, il.Const(WORD, 0xFFFF))))));

          cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                        il.Const(WORD, 1));
          cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                        il.Const(WORD, 1));

          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

          il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
          il.MarkLabel(trueLabel0);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel0);
          il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
          il.MarkLabel(trueLabel1);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        default:
          LogDebug("0x%lx: Eqanybh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case EQANYB_DC_DA_DB: {
          ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
          LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
              falseLabel0, falseLabel1, falseLabel2, doneLabel;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);

          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(3),
              il.BoolToInt(WORD, il.CompareEqual(
                                     WORD,
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, 24)),
                                            il.Const(WORD, 0xFF)),
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_db),
                                                il.Const(WORD, 24)),
                                            il.Const(WORD, 0xFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(2),
              il.BoolToInt(WORD, il.CompareEqual(
                                     WORD,
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, 16)),
                                            il.Const(WORD, 0xFF)),
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_db),
                                                il.Const(WORD, 16)),
                                            il.Const(WORD, 0xFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(1),
              il.BoolToInt(WORD, il.CompareEqual(
                                     WORD,
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, 8)),
                                            il.Const(WORD, 0xFF)),
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_db),
                                                il.Const(WORD, 8)),
                                            il.Const(WORD, 0xFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.BoolToInt(
                  WORD, il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_da),
                                               il.Const(WORD, 0xFF)),
                                        il.And(WORD, il.Register(WORD, reg_db),
                                               il.Const(WORD, 0xFF))))));

          cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                       il.Const(WORD, 1));
          cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                       il.Const(WORD, 1));
          cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                       il.Const(WORD, 1));
          cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                       il.Const(WORD, 1));

          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

          il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
          il.MarkLabel(trueLabel0);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel0);
          il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
          il.MarkLabel(trueLabel1);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
          il.MarkLabel(trueLabel2);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
          il.MarkLabel(trueLabel3);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        case EQANYH_DC_DA_DB: {
          ExprId cond_hword0, cond_hword1;
          LowLevelILLabel trueLabel0, trueLabel1, falseLabel0, doneLabel;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);

          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(1),
              il.BoolToInt(WORD, il.CompareEqual(
                                     WORD,
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, 16)),
                                            il.Const(WORD, 0xFFFF)),
                                     il.And(WORD,
                                            il.LogicalShiftRight(
                                                WORD, il.Register(WORD, reg_db),
                                                il.Const(WORD, 16)),
                                            il.Const(WORD, 0xFFFF))))));
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.BoolToInt(
                  WORD, il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_da),
                                               il.Const(WORD, 0xFFFF)),
                                        il.And(WORD, il.Register(WORD, reg_db),
                                               il.Const(WORD, 0xFFFF))))));

          cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                        il.Const(WORD, 1));
          cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                        il.Const(WORD, 1));

          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

          il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
          il.MarkLabel(trueLabel0);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel0);
          il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
          il.MarkLabel(trueLabel1);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 1)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
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

bool Eqza::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x01_RR || op2 != EQZA_DC_AA) {
    LogDebug("0x%lx: Eqza::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId condition;
    len = 4;
    uint8_t op1, op2, reg_aa, s1, n, reg_dc;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s1, n, reg_dc);
    REGTOA(reg_aa)

    condition =
        il.CompareEqual(WORD, il.Register(WORD, reg_aa), il.Const(WORD, 0));
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.BoolToInt(WORD, condition)));
    return true;
  }
}

bool Extr::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x37_RRPW:
      switch (op2) {
        case EXTR_DC_DA_POS_WIDTH: {
          ExprId exp1, exp2, exp3, exp4;
          len = 4;
          uint8_t op1, op2, reg_da, s2, width, pos, reg_dc;
          Instruction::ExtractOpfieldsRRPW(data, op1, op2, reg_da, s2, width,
                                           pos, reg_dc);

          // NOTE: the following complex expression is needed because the result
          // is supposed to be sign-extended, but the width of the result is
          // variable. In other words, we can't just use il.SignExtend() because
          // we may be extracting only 3 bits, or 23 bits, or something that is
          // not an even byte, hword, or word.

          // D[a] >> pos
          exp1 = il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                      il.Const(BYTE, pos));
          // 32 - w
          exp2 = il.Sub(WORD, il.Const(WORD, 32), il.Const(WORD, width));
          // exp3 = (D[a] >> pos)[w-1:0] == (D[a] >> pos) & (0xFFFFFFFF >> 32 -
          // w)
          exp3 = il.And(
              WORD, exp1,
              il.LogicalShiftRight(WORD, il.Const(WORD, 0xFFFFFFFF), exp2));
          // Sign-extend by shifting left to get MSB of the desired extracted
          // data to the highest bit of the word, then shift back right to fill
          // with 1s.
          exp4 = il.ArithShiftRight(WORD, il.ShiftLeft(WORD, exp3, exp2), exp2);

          il.AddInstruction(il.SetRegister(WORD, reg_dc, exp4));
          return true;
        }
        case EXTRU_DC_DA_POS_WIDTH: {
          len = 4;
          uint8_t op1, op2, reg_da, s2, width, pos, reg_dc;
          Instruction::ExtractOpfieldsRRPW(data, op1, op2, reg_da, s2, width,
                                           pos, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.ZeroExtend(WORD, il.And(WORD,
                                         il.LogicalShiftRight(
                                             WORD, il.Register(WORD, reg_da),
                                             il.Const(BYTE, pos)),
                                         il.LogicalShiftRight(
                                             WORD, il.Const(WORD, 0xFFFFFFFF),
                                             il.Sub(WORD, il.Const(WORD, 32),
                                                    il.Const(WORD, width)))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Extr::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x17_RRRR:
      switch (op2) {
        case EXTR_DC_DA_ED: {
          ExprId exp1, exp2, exp3, exp4, pos, width;
          len = 4;
          uint8_t op1, op2, reg_da, s2, reg_ed, reg_dc, reg_edlow, reg_edhigh;
          Instruction::ExtractOpfieldsRRRR(data, op1, op2, reg_da, s2, reg_ed,
                                           reg_dc);
          reg_edlow = reg_ed;
          reg_edhigh = reg_ed + 1;
          REGTOE(reg_edlow)
          REGTOE(reg_edhigh)
          REGTOE(reg_ed)

          pos =
              il.And(WORD, il.Register(WORD, reg_edlow), il.Const(WORD, 0x0F));
          width =
              il.And(WORD, il.Register(WORD, reg_edhigh), il.Const(WORD, 0x0F));

          // D[a] >> pos
          exp1 = il.LogicalShiftRight(WORD, il.Register(WORD, reg_da), pos);
          // 32 - w
          exp2 = il.Sub(WORD, il.Const(WORD, 32), width);
          // exp3 = (D[a] >> pos)[w-1:0] == (D[a] >> pos) & (0xFFFFFFFF >> 32 -
          // w)
          exp3 = il.And(
              WORD, exp1,
              il.LogicalShiftRight(WORD, il.Const(WORD, 0xFFFFFFFF), exp2));
          // Sign-extend by shifting left to get MSB of the desired extracted
          // data to the highest bit of the word, then shift back right to fill
          // with 1s.
          exp4 = il.ArithShiftRight(WORD, il.ShiftLeft(WORD, exp3, exp2), exp2);

          il.AddInstruction(il.SetRegister(WORD, reg_dc, exp4));
          return true;
        }
        case EXTRU_DC_DA_ED: {
          ExprId width, pos;
          len = 4;
          uint8_t op1, op2, reg_da, s2, reg_ed, reg_dc, reg_edlow, reg_edhigh;
          Instruction::ExtractOpfieldsRRRR(data, op1, op2, reg_da, s2, reg_ed,
                                           reg_dc);
          reg_edlow = reg_ed;
          reg_edhigh = reg_ed + 1;
          REGTOE(reg_edlow)
          REGTOE(reg_edhigh)
          REGTOE(reg_ed)

          pos =
              il.And(WORD, il.Register(WORD, reg_edlow), il.Const(WORD, 0x0F));
          width =
              il.And(WORD, il.Register(WORD, reg_edhigh), il.Const(WORD, 0x0F));

          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.ZeroExtend(
                  WORD, il.And(WORD,
                               il.LogicalShiftRight(
                                   WORD, il.Register(WORD, reg_da), pos),
                               il.LogicalShiftRight(
                                   WORD, il.Const(WORD, 0xFFFFFFFF),
                                   il.Sub(WORD, il.Const(WORD, 32), width))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Extr::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x57_RRRW:
      switch (op2) {
        case EXTR_DC_DA_DD_WIDTH: {
          ExprId exp1, exp2, exp3, exp4, pos;
          len = 4;
          uint8_t op1, op2, reg_da, s2, width, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRRW(data, op1, op2, reg_da, s2, width,
                                           reg_dd, reg_dc);

          pos = il.And(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0x0F));

          // D[a] >> pos
          exp1 = il.LogicalShiftRight(WORD, il.Register(WORD, reg_da), pos);
          // 32 - w
          exp2 = il.Sub(WORD, il.Const(WORD, 32), il.Const(WORD, width));
          // exp3 = (D[a] >> pos)[w-1:0] == (D[a] >> pos) & (0xFFFFFFFF >> 32 -
          // w)
          exp3 = il.And(
              WORD, exp1,
              il.LogicalShiftRight(WORD, il.Const(WORD, 0xFFFFFFFF), exp2));
          // Sign-extend by shifting left to get MSB of the desired extracted
          // data to the highest bit of the word, then shift back right to fill
          // with 1s.
          exp4 = il.ArithShiftRight(WORD, il.ShiftLeft(WORD, exp3, exp2), exp2);

          il.AddInstruction(il.SetRegister(WORD, reg_dc, exp4));
          return true;
        }
        case EXTRU_DC_DA_DD_WIDTH: {
          ExprId pos;
          len = 4;
          uint8_t op1, op2, reg_da, s2, width, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRRW(data, op1, op2, reg_da, s2, width,
                                           reg_dd, reg_dc);

          pos = il.And(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0x0F));

          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.ZeroExtend(WORD,
                            il.And(WORD,
                                   il.LogicalShiftRight(
                                       WORD, il.Register(WORD, reg_da), pos),
                                   il.LogicalShiftRight(
                                       WORD, il.Const(WORD, 0xFFFFFFFF),
                                       il.Sub(WORD, il.Const(WORD, 32),
                                              il.Const(WORD, width)))))));
          return true;
        }
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

bool Fcall::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  // TODO: add LLIL to save state
  switch (op1) {
    case FCALL_DISP24:
      len = 4;
      // FCALL gets the target the same way CALL does
      il.AddInstruction(
          il.Call(il.ConstPointer(WORD, Call::GetTarget(data, addr, len))));
      return true;
    default:
      LogDebug("0x%lx: Fcall::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Fcalla::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  // TODO: add LLIL to save state
  if (op1 != FCALLA_DISP24) {
    LogDebug("0x%lx: Fcalla::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  // FCALLA gets the target the same way CALLA does
  il.AddInstruction(
      il.Call(il.ConstPointer(WORD, Calla::GetTarget(data, addr, len))));
  return true;
}

bool Fcalli::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x2D_RR || op2 != FCALLI_AA) {
    LogDebug("0x%lx: Fcalli::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, s2, n, d;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s2, n, d);
    REGTOA(reg_aa)
    il.AddInstruction(il.Call(il.Register(WORD, reg_aa)));
    return true;
  }
}

bool Fret::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  len = *((const uint16_t*)data) & 0x1 ? 4 : 2;
  // TODO: make sure A11 is even
  il.AddInstruction(il.Return(il.Register(WORD, TRICORE_REG_A11)));
  return true;
}

bool Ge::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case GE_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD,
                           il.CompareSignedGreaterEqual(
                               WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        case GEU_DC_DA_CONST9: {
          // NOTE/TODO: the TriCore Arch. Vol. 2 manual says to use a
          // sign_ext(const9) here... Could that be a typo?
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD,
                           il.CompareUnsignedGreaterEqual(
                               WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case GE_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD, il.CompareSignedGreaterEqual(
                                     WORD, il.Register(WORD, reg_da),
                                     il.Register(WORD, reg_db)))));
          return true;
        }
        case GEU_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD, il.CompareUnsignedGreaterEqual(
                                     WORD, il.Register(WORD, reg_da),
                                     il.Register(WORD, reg_db)))));
          return true;
        }
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

bool Gea::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x01_RR || op2 != GEA_DC_AA_AB) {
    LogDebug("0x%lx: Gea::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, reg_ab, n, reg_dc;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n, reg_dc);
    REGTOA(reg_aa)
    REGTOA(reg_ab)
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc,
                       il.BoolToInt(WORD, il.CompareUnsignedGreaterEqual(
                                              WORD, il.Register(WORD, reg_aa),
                                              il.Register(WORD, reg_ab)))));
    return true;
  }
}

bool Imask::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool InstNt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Insert::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_xB7_RCPW:
      if (op2 != INSERT_DC_DA_CONST4_POS_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId mask;
        len = 4;
        uint8_t op1, op2, reg_da, w, pos, reg_dc;
        int8_t const4;
        Instruction::ExtractOpfieldsRCPW(data, op1, op2, reg_da, const4, w, pos,
                                         reg_dc);
        const4 &= 0xF;  // zero extend
        mask = il.ShiftLeft(WORD, il.Const(WORD, pow(2, w) - 1),
                            il.Const(BYTE, pos));
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Or(WORD,
                  il.And(WORD, il.Register(WORD, reg_da), il.Not(WORD, mask)),
                  il.And(WORD,
                         il.ShiftLeft(
                             WORD, il.ZeroExtend(WORD, il.Const(BYTE, const4)),
                             il.Const(BYTE, pos)),
                         mask))));
        return true;
      }
    case OP1_x97_RCRR:
      // TODO: I don't know how to do an exponent operation with the LLIL, which
      // is necessary for this instruction
      if (op2 != INSERT_DC_DA_CONST4_ED) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
    case OP1_xD7_RCRW:
      if (op2 != INSERT_DC_DA_CONST4_DD_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId mask, pos;
        len = 4;
        uint8_t op1, op2, reg_da, w, reg_dd, reg_dc;
        int8_t const4;
        Instruction::ExtractOpfieldsRCRW(data, op1, op2, reg_da, const4, w,
                                         reg_dd, reg_dc);
        const4 &= 0xF;  // zero extend
        pos = il.And(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0x1F));
        mask = il.ShiftLeft(WORD, il.Const(WORD, pow(2, w) - 1), pos);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Or(WORD,
                  il.And(WORD, il.Register(WORD, reg_da), il.Not(WORD, mask)),
                  il.And(WORD,
                         il.ShiftLeft(
                             WORD, il.ZeroExtend(WORD, il.Const(BYTE, const4)),
                             pos),
                         mask))));
        return true;
      }
    case OP1_x37_RRPW:
      if (op2 != INSERT_DC_DA_DB_POS_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId mask;
        len = 4;
        uint8_t op1, op2, reg_da, reg_db, w, pos, reg_dc;
        Instruction::ExtractOpfieldsRRPW(data, op1, op2, reg_da, reg_db, w, pos,
                                         reg_dc);
        mask = il.ShiftLeft(WORD, il.Const(WORD, pow(2, w) - 1),
                            il.Const(BYTE, pos));
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Or(WORD,
                  il.And(WORD, il.Register(WORD, reg_da), il.Not(WORD, mask)),
                  il.And(WORD,
                         il.ShiftLeft(WORD, il.Register(WORD, reg_db),
                                      il.Const(BYTE, pos)),
                         mask))));
        return true;
      }
    case OP1_x17_RRRR:
      // TODO: I don't know how to do an exponent operation with the LLIL, which
      // is necessary for this instruction
      if (op2 != INSERT_DC_DA_DB_ED) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      len = 4;
      UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
    case OP1_x57_RRRW:
      if (op2 != INSERT_DC_DA_DB_DD_WIDTH) {
        LogDebug("0x%lx: Insert::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        ExprId mask, pos;
        len = 4;
        uint8_t op1, op2, reg_da, reg_db, w, reg_dd, reg_dc;
        Instruction::ExtractOpfieldsRRRW(data, op1, op2, reg_da, reg_db, w,
                                         reg_dd, reg_dc);
        pos = il.And(WORD, il.Register(WORD, reg_dd), il.Const(WORD, 0x1F));
        mask = il.ShiftLeft(WORD, il.Const(WORD, pow(2, w) - 1), pos);
        il.AddInstruction(il.SetRegister(
            WORD, reg_dc,
            il.Or(
                WORD,
                il.And(WORD, il.Register(WORD, reg_da), il.Not(WORD, mask)),
                il.And(WORD, il.ShiftLeft(WORD, il.Register(WORD, reg_db), pos),
                       mask))));
        return true;
      }
    default:
      LogDebug("0x%lx: Insert::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Isync::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ixmax::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ixmin::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool J::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
             size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case J_DISP8:
      len = 2;
      il.AddInstruction(
          il.Jump(il.ConstPointer(WORD, J::GetTarget(data, addr, len))));
      return true;
    case J_DISP24:
      len = 4;
      il.AddInstruction(
          il.Jump(il.ConstPointer(WORD, J::GetTarget(data, addr, len))));
      return true;
    default:
      LogDebug("0x%lx: J::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Ja::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  if (op1 != JA_DISP24) {
    LogDebug("0x%lx: Ja::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  il.AddInstruction(
      il.Jump(il.ConstPointer(WORD, Ja::GetTarget(data, addr, len))));
  return true;
}

bool Jeq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il,
               BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case JEQ_D15_CONST4_DISP4:
    case JEQ_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
    {  // NOTE: scopes are needed for each case to preserve the condition (it
       // depends on extraction of operands, which are per-scope)
      len = 2;
      uint8_t op1;
      int8_t disp4, const4;
      Instruction::ExtractOpfieldsSBC(data, op1, disp4, const4);
      condition = il.CompareEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                  il.SignExtend(WORD, il.Const(BYTE, const4)));
      target = Jeq::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jeq::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case JEQ_D15_DB_DISP4:
    case JEQ_D15_DB_DISP4_EX:  // TCv1.6 ISA only
    {
      len = 2;
      uint8_t op1, reg_db;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
      condition = il.CompareEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                  il.Register(WORD, reg_db));
      target = Jeq::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jeq::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case OP1_xDF_BRC:
      if (op2 != JEQ_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da;
        int8_t const4;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4, disp15);
        condition =
            il.CompareEqual(WORD, il.Register(WORD, reg_da),
                            il.SignExtend(WORD, il.Const(BYTE, const4)));
        target = Jeq::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jeq::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    case OP1_x5F_BRR:
      if (op2 != JEQ_DA_DB_DISP15) {
        LogDebug("0x%lx: Jeq::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da, reg_db;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db, disp15);
        condition = il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                    il.Register(WORD, reg_db));
        target = Jeq::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jeq::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    default:
      LogDebug("0x%lx: Jeq::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jeqa::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  if (op1 != OP1_x7D_BRR || op2 != JEQA_AA_AB_DISP15) {
    LogDebug("0x%lx: Jeqa::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, reg_ab;
    int16_t disp15;
    Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_aa, reg_ab, disp15);
    REGTOA(reg_aa)
    REGTOA(reg_ab)
    condition = il.CompareEqual(WORD, il.Register(WORD, reg_aa),
                                il.Register(WORD, reg_ab));
    target = Jeqa::GetTarget(data, addr, len);
    t = il.GetLabelForAddress(arch, target);
    f = il.GetLabelForAddress(arch, addr + len);
    if (!t || !f) {
      LogDebug("0x%lx: Jeq::%s Failed to find true/false labels!", addr,
               __func__);
      return false;
    }
    il.AddInstruction(il.If(condition, *t, *f));
    return true;
  }
}

bool Jge::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il,
               BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case OP1_xFF_BRC:
      switch (op2) {
        case JGE_DA_CONST4_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da;
          int8_t const4;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4,
                                          disp15);
          condition = il.CompareSignedGreaterEqual(
              WORD, il.Register(WORD, reg_da),
              il.SignExtend(WORD, il.Const(BYTE, const4)));
          target = Jge::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jge::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
        case JGEU_DA_CONST4_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da;
          int8_t const4;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4,
                                          disp15);
          const4 &= 0x0F;  // clear sign-extended bits
          condition = il.CompareUnsignedGreaterEqual(
              WORD, il.Register(WORD, reg_da),
              il.SignExtend(WORD, il.Const(BYTE, const4)));
          target = Jge::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jge::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
        default:
          LogDebug("0x%lx: Jge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x7F_BRR:
      switch (op2) {
        case JGE_DA_DB_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db,
                                          disp15);
          condition = il.CompareSignedGreaterEqual(
              WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db));
          target = Jge::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jge::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
        case JGEU_DA_DB_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db,
                                          disp15);
          condition = il.CompareUnsignedGreaterEqual(
              WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db));
          target = Jge::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jge::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
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

bool Jgez::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  if (op1 != JGEZ_DB_DISP4) {
    LogDebug("0x%lx: Jgez::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 2;
    uint8_t op1, reg_db;
    int8_t disp4;
    Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
    condition = il.CompareSignedGreaterEqual(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 0));
    target = Jgez::GetTarget(data, addr, len);
    t = il.GetLabelForAddress(arch, target);
    f = il.GetLabelForAddress(arch, addr + len);
    if (!t || !f) {
      LogDebug("0x%lx: Jgez::%s Failed to find true/false labels!", addr,
               __func__);
      return false;
    }
    il.AddInstruction(il.If(condition, *t, *f));
    return true;
  }
}

bool Jgtz::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  if (op1 != JGTZ_DB_DISP4) {
    LogDebug("0x%lx: Jgtz::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 2;
    uint8_t op1, reg_db;
    int8_t disp4;
    Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
    condition = il.CompareSignedGreaterThan(WORD, il.Register(WORD, reg_db),
                                            il.Const(WORD, 0));
    target = Jgez::GetTarget(data, addr, len);
    t = il.GetLabelForAddress(arch, target);
    f = il.GetLabelForAddress(arch, addr + len);
    if (!t || !f) {
      LogDebug("0x%lx: Jgtz::%s Failed to find true/false labels!", addr,
               __func__);
      return false;
    }
    il.AddInstruction(il.If(condition, *t, *f));
    return true;
  }
}

bool Ji::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_xDC_SR:
      if (op2 != JI_AA_16) {
        LogDebug("0x%lx: Ji::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      {
        len = 2;
        uint8_t op1, reg_aa, op2;
        Instruction::ExtractOpfieldsSR(data, op1, reg_aa, op2);
        REGTOA(reg_aa)
        // TODO: This instruction really does: PC = {A[a][31:1], 1'b0} (i.e.
        // can't jump to odd address)
        il.AddInstruction(il.Jump(il.Register(WORD, reg_aa)));
        return true;
      }
    case OP1_x2D_RR:
      if (op2 != JI_AA_32) {
        LogDebug("0x%lx: Ji::%s received invalid opcode 2 0x%x", addr, __func__,
                 op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_aa, s2, n, d;
        Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s2, n, d);
        REGTOA(reg_aa)
        // TODO: This instruction really does: PC = {A[a][31:1], 1'b0} (i.e.
        // can't jump to odd address)
        il.AddInstruction(il.Jump(il.Register(WORD, reg_aa)));
        return true;
      }
    default:
      LogDebug("0x%lx: Ji::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jl::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  if (op1 != JL_DISP24) {
    LogDebug("0x%lx: Jl::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  il.AddInstruction(il.Store(WORD, il.Register(WORD, TRICORE_REG_A11),
                             il.Const(WORD, addr + 4)));
  il.AddInstruction(
      il.Jump(il.ConstPointer(WORD, Jl::GetTarget(data, addr, len))));
  return true;
}

bool Jla::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != JLA_DISP24) {
    LogDebug("0x%lx: Jla::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  len = 4;
  il.AddInstruction(il.Store(WORD, il.Register(WORD, TRICORE_REG_A11),
                             il.Const(WORD, addr + 4)));
  il.AddInstruction(
      il.Jump(il.ConstPointer(WORD, Jla::GetTarget(data, addr, len))));
  return true;
}

bool Jlez::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  if (op1 != JLEZ_DB_DISP4) {
    LogDebug("0x%lx: Jlez::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 2;
    uint8_t op1, reg_db;
    int8_t disp4;
    Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
    condition = il.CompareSignedLessEqual(WORD, il.Register(WORD, reg_db),
                                          il.Const(WORD, 0));
    target = Jlez::GetTarget(data, addr, len);
    t = il.GetLabelForAddress(arch, target);
    f = il.GetLabelForAddress(arch, addr + len);
    if (!t || !f) {
      LogDebug("0x%lx: Jlez::%s Failed to find true/false labels!", addr,
               __func__);
      return false;
    }
    il.AddInstruction(il.If(condition, *t, *f));
    return true;
  }
}

bool Jli::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x2D_RR || op2 != JLI_AA) {
    LogDebug("0x%lx: Jli::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, s2, n, d;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s2, n, d);
    REGTOA(reg_aa)
    il.AddInstruction(il.Store(WORD, il.Register(WORD, TRICORE_REG_A11),
                               il.Const(WORD, addr + 4)));
    // TODO: This instruction really does: PC = {A[a][31:1], 1'b0} (i.e. can't
    // jump to odd address)
    il.AddInstruction(il.Jump(il.Register(WORD, reg_aa)));
    return true;
  }
}

bool Jlt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il,
               BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case OP1_xBF_BRC:
      switch (op2) {
        case JLT_DA_CONST4_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da;
          int8_t const4;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4,
                                          disp15);
          condition = il.CompareSignedLessEqual(
              WORD, il.Register(WORD, reg_da),
              il.SignExtend(WORD, il.Const(BYTE, const4)));
          target = Jlt::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jlt::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
        case JLTU_DA_CONST4_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da;
          int8_t const4;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4,
                                          disp15);
          const4 &= 0x0F;
          condition = il.CompareUnsignedLessEqual(
              WORD, il.Register(WORD, reg_da),
              il.SignExtend(WORD, il.Const(BYTE, const4)));
          target = Jlt::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jlt::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
        default:
          LogDebug("0x%lx: Jlt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x3F_BRR:
      switch (op2) {
        case JLT_DA_DB_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db,
                                          disp15);
          condition = il.CompareSignedLessEqual(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db));
          target = Jlt::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jlt::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
        case JLTU_DA_DB_DISP15: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db,
                                          disp15);
          condition = il.CompareUnsignedLessEqual(
              WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db));
          target = Jlt::GetTarget(data, addr, len);
          t = il.GetLabelForAddress(arch, target);
          f = il.GetLabelForAddress(arch, addr + len);
          if (!t || !f) {
            LogDebug("0x%lx: Jlt::%s Failed to find true/false labels!", addr,
                     __func__);
            return false;
          }
          il.AddInstruction(il.If(condition, *t, *f));
          return true;
        }
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

bool Jltz::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  if (op1 != JLTZ_DB_DISP4) {
    LogDebug("0x%lx: Jltz::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 2;
    uint8_t op1, reg_db;
    int8_t disp4;
    Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
    condition = il.CompareSignedLessThan(WORD, il.Register(WORD, reg_db),
                                         il.Const(WORD, 0));
    target = Jltz::GetTarget(data, addr, len);
    t = il.GetLabelForAddress(arch, target);
    f = il.GetLabelForAddress(arch, addr + len);
    if (!t || !f) {
      LogDebug("0x%lx: Jltz::%s Failed to find true/false labels!", addr,
               __func__);
      return false;
    }
    il.AddInstruction(il.If(condition, *t, *f));
    return true;
  }
}

bool Jne::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il,
               BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case OP1_xDF_BRC:
      if (op2 != JNE_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da;
        int8_t const4;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4, disp15);
        condition =
            il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(BYTE, const4)));
        target = Jne::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jne::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    case OP1_x5F_BRR:
      if (op2 != JNE_DA_DB_DISP15) {
        LogDebug("0x%lx: Jne::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da, reg_db;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db, disp15);
        condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                       il.Register(WORD, reg_db));
        target = Jne::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jne::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    case JNE_D15_CONST4_DISP4:
    case JNE_D15_CONST4_DISP4_EX:  // TCv1.6 ISA only
    {
      len = 2;
      uint8_t op1;
      int8_t disp4, const4;
      Instruction::ExtractOpfieldsSBC(data, op1, disp4, const4);
      condition =
          il.CompareNotEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                             il.SignExtend(WORD, il.Const(BYTE, const4)));
      target = Jne::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jne::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case JNE_D15_DB_DISP4:
    case JNE_D15_DB_DISP4_EX:  // TCv1.6 ISA only
    {
      len = 2;
      uint8_t op1, reg_db;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
      condition = il.CompareNotEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                     il.Register(WORD, reg_db));
      target = Jne::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jne::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    default:
      LogDebug("0x%lx: Jne::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnea::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  if (op1 != OP1_x7D_BRR || op2 != JNEA_AA_AB_DISP15) {
    LogDebug("0x%lx: Jnea::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, reg_ab;
    int16_t disp15;
    Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_aa, reg_ab, disp15);
    REGTOA(reg_aa)
    REGTOA(reg_ab)
    condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_aa),
                                   il.Register(WORD, reg_ab));
    target = Jnea::GetTarget(data, addr, len);
    t = il.GetLabelForAddress(arch, target);
    f = il.GetLabelForAddress(arch, addr + len);
    if (!t || !f) {
      LogDebug("0x%lx: Jnea::%s Failed to find true/false labels!", addr,
               __func__);
      return false;
    }
    il.AddInstruction(il.If(condition, *t, *f));
    return true;
  }
}

bool Jned::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case OP1_x9F_BRC:
      if (op2 != JNED_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jned::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da;
        int8_t const4;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4, disp15);
        condition =
            il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(BYTE, const4)));
        target = Jned::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jned::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        il.AddInstruction(il.SetRegister(
            WORD, reg_da,
            il.Sub(WORD, il.Register(WORD, reg_da), il.Const(WORD, 1))));
        return true;
      }
    case OP1_x1F_BRR:
      if (op2 != JNED_DA_DB_DISP15) {
        LogDebug("0x%lx: Jned::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da, reg_db;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db, disp15);
        condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                       il.Register(WORD, reg_db));
        target = Jned::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jned::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        il.AddInstruction(il.SetRegister(
            WORD, reg_da,
            il.Sub(WORD, il.Register(WORD, reg_da), il.Const(WORD, 1))));
        return true;
      }
    default:
      LogDebug("0x%lx: Jned::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnei::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case OP1_x9F_BRC:
      if (op2 != JNEI_DA_CONST4_DISP15) {
        LogDebug("0x%lx: Jnei::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da;
        int8_t const4;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRC(data, op1, op2, reg_da, const4, disp15);
        condition =
            il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(BYTE, const4)));
        target = Jnei::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jnei::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        il.AddInstruction(il.SetRegister(
            WORD, reg_da,
            il.Add(WORD, il.Register(WORD, reg_da), il.Const(WORD, 1))));
        return true;
      }
    case OP1_x1F_BRR:
      if (op2 != JNEI_DA_DB_DISP15) {
        LogDebug("0x%lx: Jnei::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_da, reg_db;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_da, reg_db, disp15);
        condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                       il.Register(WORD, reg_db));
        target = Jnei::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jnei::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        il.AddInstruction(il.SetRegister(
            WORD, reg_da,
            il.Add(WORD, il.Register(WORD, reg_da), il.Const(WORD, 1))));
        return true;
      }
    default:
      LogDebug("0x%lx: Jnei::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnz::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il,
               BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case JNZ_D15_DISP8:
      len = 2;
      condition = il.CompareNotEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                     il.Const(WORD, 0));
      target = Jnz::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jnz::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    case JNZ_DB_DISP4: {
      len = 2;
      uint8_t op1, reg_db;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
      condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_db),
                                     il.Const(WORD, 0));
      target = Jnz::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jnz::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    default:
      LogDebug("0x%lx: Jnz::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnza::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case JNZA_AB_DISP4: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_ab);
      REGTOA(reg_ab)
      condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_ab),
                                     il.Const(WORD, 0));
      target = Jnza::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jnza::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case OP1_xBD_BRR:
      if (op2 != JNZA_AA_DISP15) {
        LogDebug("0x%lx: Jnza::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_aa, s2;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_aa, s2, disp15);
        REGTOA(reg_aa)
        condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_aa),
                                       il.Const(WORD, 0));
        target = Jnza::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jnza::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    default:
      LogDebug("0x%lx: Jnza::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jnzt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il,
                BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition, bit;
  switch (op1) {
    case JNZT_D15_N_DISP4: {
      len = 2;
      uint8_t op1, n;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBRN(data, op1, disp4, n);
      bit = il.ShiftLeft(WORD, il.Const(WORD, 0x1), il.Const(WORD, n));
      condition = il.CompareNotEqual(
          WORD, il.And(WORD, il.Register(WORD, TRICORE_REG_D15), bit),
          il.Const(WORD, 0));
      target = Jnzt::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jnzt::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case OP1_x6F_BRN:
      if (op2 != JNZT_DA_N_DISP15) {
        LogDebug("0x%lx: Jnzt::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, n, reg_da;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRN(data, op1, op2, n, reg_da, disp15);
        bit = il.ShiftLeft(WORD, il.Const(WORD, 0x1), il.Const(WORD, n));
        condition = il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_da), bit),
            il.Const(WORD, 0));
        target = Jnzt::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jnzt::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    default:
      LogDebug("0x%lx: Jnzt::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jz::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il,
              BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case JZ_D15_DISP8: {
      len = 2;
      uint8_t op1;
      int8_t disp8;
      Instruction::ExtractOpfieldsSB(data, op1, disp8);
      condition = il.CompareEqual(WORD, il.Register(WORD, TRICORE_REG_D15),
                                  il.Const(WORD, 0));
      target = Jz::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jz::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case JZ_DB_DISP4: {
      len = 2;
      uint8_t op1, reg_db;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_db);
      condition =
          il.CompareEqual(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0));
      target = Jz::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jz::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    default:
      LogDebug("0x%lx: Jz::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jza::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il,
               BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition;

  switch (op1) {
    case JZA_AB_DISP4: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_ab);
      REGTOA(reg_ab)
      condition =
          il.CompareEqual(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 0));
      target = Jza::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jza::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case OP1_xBD_BRR:
      if (op2 != JZA_AA_DISP15) {
        LogDebug("0x%lx: Jza::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_aa, s2;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRR(data, op1, op2, reg_aa, s2, disp15);
        REGTOA(reg_aa)
        condition =
            il.CompareEqual(WORD, il.Register(WORD, reg_aa), il.Const(WORD, 0));
        target = Jza::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jza::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    default:
      LogDebug("0x%lx: Jza::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Jzt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il,
               BinaryNinja::Architecture* arch) {
  uint32_t target = 0;
  BNLowLevelILLabel *t, *f;
  ExprId condition, bit;
  switch (op1) {
    case JZT_D15_N_DISP4: {
      len = 2;
      uint8_t op1, n;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBRN(data, op1, disp4, n);
      bit = il.ShiftLeft(WORD, il.Const(WORD, 0x1), il.Const(WORD, n));
      condition = il.CompareEqual(
          WORD, il.And(WORD, il.Register(WORD, TRICORE_REG_D15), bit),
          il.Const(WORD, 0));
      target = Jzt::GetTarget(data, addr, len);
      t = il.GetLabelForAddress(arch, target);
      f = il.GetLabelForAddress(arch, addr + len);
      if (!t || !f) {
        LogDebug("0x%lx: Jzt::%s Failed to find true/false labels!", addr,
                 __func__);
        return false;
      }
      il.AddInstruction(il.If(condition, *t, *f));
      return true;
    }
    case OP1_x6F_BRN:
      if (op2 != JZT_DA_N_DISP15) {
        LogDebug("0x%lx: Jzt::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, n, reg_da;
        int16_t disp15;
        Instruction::ExtractOpfieldsBRN(data, op1, op2, n, reg_da, disp15);
        bit = il.ShiftLeft(WORD, il.Const(WORD, 0x1), il.Const(WORD, n));
        condition =
            il.CompareEqual(WORD, il.And(WORD, il.Register(WORD, reg_da), bit),
                            il.Const(WORD, 0));
        target = Jzt::GetTarget(data, addr, len);
        t = il.GetLabelForAddress(arch, target);
        f = il.GetLabelForAddress(arch, addr + len);
        if (!t || !f) {
          LogDebug("0x%lx: Jnzt::%s Failed to find true/false labels!", addr,
                   __func__);
          return false;
        }
        il.AddInstruction(il.If(condition, *t, *f));
        return true;
      }
    default:
      LogDebug("0x%lx: Jzt::%s received invalid opcode 1 0x%x", addr, __func__,
               op1);
      return false;
  }
}

bool Lda::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LDA_A15_A10_CONST8: {
      len = 2;
      uint8_t op1;
      int8_t const8;
      uint32_t mem;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      mem = (uint32_t)const8 * 4;
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_A15,
          il.Load(WORD, il.Add(WORD, il.Register(WORD, TRICORE_REG_A10),
                               il.Const(WORD, mem)))));
      return true;
    }
    case LDA_AC_AB: {
      len = 2;
      uint8_t op1, reg_ac, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_ac, reg_ab);
      REGTOA(reg_ac)
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_ac, il.Load(WORD, il.Register(WORD, reg_ab))));
      return true;
    }
    case LDA_AC_AB_POSTINC: {
      len = 2;
      uint8_t op1, reg_ac, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_ac, reg_ab);
      REGTOA(reg_ac)
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_ac, il.Load(WORD, il.Register(WORD, reg_ab))));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 4))));
      return true;
    }
    case LDA_AC_A15_OFF4: {
      len = 2;
      uint8_t op1, reg_ac;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSLRO(data, op1, reg_ac, off4);
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      REGTOA(reg_ac)
      il.AddInstruction(il.SetRegister(
          WORD, reg_ac,
          il.Load(WORD, il.Add(WORD, il.Register(WORD, TRICORE_REG_A15),
                               il.Const(WORD, mem)))));
      return true;
    }
    case LDA_A15_AB_OFF4: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      REGTOA(reg_ab)
      il.AddInstruction(
          il.SetRegister(WORD, TRICORE_REG_A15,
                         il.Load(WORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                              il.Const(WORD, mem)))));
      return true;
    }
    case LDA_AA_AB_OFF16: {
      len = 4;
      uint8_t op1, reg_aa, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_aa, reg_ab, off16);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(
          il.SetRegister(WORD, reg_aa,
                         il.Load(WORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                              il.Const(WORD, off16)))));
      return true;
    }
    case OP1_x85_ABS:
      if (op2 != LDA_AA_OFF18) {
        LogDebug("0x%lx: Lda::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_aa;
        uint32_t ea;
        Instruction::ExtractOpfieldsABS(data, op1, op2, reg_aa, ea);
        REGTOA(reg_aa)
        il.AddInstruction(il.SetRegister(
            WORD, reg_aa, il.Load(WORD, il.ConstPointer(WORD, ea))));
        return true;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDA_AA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_aa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
          REGTOA(reg_aa)
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_aa,
              il.Load(WORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          return true;
        }
        case LDA_AA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_aa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
          REGTOA(reg_ab)
          REGTOA(reg_aa)
          il.AddInstruction(il.SetRegister(
              WORD, reg_aa, il.Load(WORD, il.Register(WORD, reg_ab))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }

        case LDA_AA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_aa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_aa,
              il.Load(WORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Lda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDA_AA_PB:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDA_AA_PB_OFF10:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Ldb::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LDBU_DC_AB: {
      len = 2;
      uint8_t op1, reg_dc, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_dc, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.ZeroExtend(WORD, il.Load(BYTE, il.Register(WORD, reg_ab)))));
      return true;
    }
    case LDBU_DC_AB_POSTINC: {
      len = 2;
      uint8_t op1, reg_dc, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_dc, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.ZeroExtend(WORD, il.Load(BYTE, il.Register(WORD, reg_ab)))));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 1))));
      return true;
    }
    case LDBU_DC_A15_OFF4: {
      len = 2;
      uint8_t op1, reg_dc;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSLRO(data, op1, reg_dc, off4);
      mem = (off4 & 0xFu);  // clear sign extended bits
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.ZeroExtend(
              WORD,
              il.Load(BYTE, il.Add(WORD, il.Register(WORD, TRICORE_REG_A15),
                                   il.Const(WORD, mem))))));
      return true;
    }
    case LDBU_D15_AB_OFF4: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      REGTOA(reg_ab)
      mem = (off4 & 0xF);  // clear sign extended bits
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.ZeroExtend(WORD,
                        il.Load(BYTE, il.Add(WORD, il.Register(WORD, reg_ab),
                                             il.Const(WORD, mem))))));
      return true;
    }
    case LDB_DA_AB_OFF16:  // TCv1.6 ISA only
    {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.SignExtend(
              WORD, il.Load(BYTE, il.Add(WORD, il.Register(WORD, reg_ab),
                                         il.SignExtend(
                                             WORD, il.Const(HWORD, off16)))))));
      return true;
    }
    case LDBU_DA_AB_OFF16:  // TCv1.6 ISA only
    {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.ZeroExtend(
              WORD, il.Load(BYTE, il.Add(WORD, il.Register(WORD, reg_ab),
                                         il.SignExtend(
                                             WORD, il.Const(HWORD, off16)))))));
      return true;
    }
    case OP1_x05_ABS:
      switch (op2) {
        case LDB_DA_OFF18: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(WORD, il.Load(BYTE, il.ConstPointer(WORD, ea)))));
          return true;
        }
        case LDBU_DA_OFF18: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(WORD, il.Load(BYTE, il.ConstPointer(WORD, ea)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDB_DA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(
                  WORD,
                  il.Load(BYTE, il.Add(WORD, il.Register(WORD, reg_ab),
                                       il.SignExtend(
                                           WORD, il.Const(HWORD, off10)))))));
          return true;
        }
        case LDB_DA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(WORD, il.Load(BYTE, il.Register(WORD, reg_ab)))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDB_DA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(
                  WORD,
                  il.Load(BYTE, il.Add(WORD, il.Register(WORD, reg_ab),
                                       il.SignExtend(
                                           WORD, il.Const(HWORD, off10)))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDBU_DA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(
                  WORD,
                  il.Load(BYTE, il.Add(WORD, il.Register(WORD, reg_ab),
                                       il.SignExtend(
                                           WORD, il.Const(HWORD, off10)))))));
          return true;
        }
        case LDBU_DA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(WORD, il.Load(BYTE, il.Register(WORD, reg_ab)))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDBU_DA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(
                  WORD,
                  il.Load(BYTE, il.Add(WORD, il.Register(WORD, reg_ab),
                                       il.SignExtend(
                                           WORD, il.Const(HWORD, off10)))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDB_DA_PB:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDB_DA_PB_OFF10:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDBU_DA_PB:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDBU_DA_PB_OFF10:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Ldd::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x85_ABS:
      if (op2 != LDD_EA_OFF18) {
        LogDebug("0x%lx: Ldd::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_ea;
        uint32_t ea;
        Instruction::ExtractOpfieldsABS(data, op1, op2, reg_ea, ea);
        REGTOE(reg_ea)
        il.AddInstruction(il.SetRegister(
            DWORD, reg_ea, il.Load(DWORD, il.ConstPointer(WORD, ea))));
        return true;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDD_EA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_ea, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
          REGTOA(reg_ab)
          REGTOE(reg_ea)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ea,
              il.Load(DWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          return true;
        }
        case LDD_EA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_ea, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
          REGTOA(reg_ab)
          REGTOE(reg_ea)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ea, il.Load(DWORD, il.Register(WORD, reg_ab))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDD_EA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_ea, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
          REGTOA(reg_ab)
          REGTOE(reg_ea)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ea,
              il.Load(DWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldd::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDD_EA_PB:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDD_EA_PB_OFF10:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Ldda::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x85_ABS:
      if (op2 != LDDA_PA_OFF18) {
        LogDebug("0x%lx: Ldda::%s received invalid opcode 2 0x%x", addr,
                 __func__, op2);
        return false;
      }
      {
        len = 4;
        uint8_t op1, op2, reg_pa;
        uint32_t ea;
        Instruction::ExtractOpfieldsABS(data, op1, op2, reg_pa, ea);
        REGTOP(reg_pa)
        il.AddInstruction(il.SetRegister(
            DWORD, reg_pa, il.Load(DWORD, il.ConstPointer(WORD, ea))));
        return true;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDDA_PA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_pa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
          REGTOA(reg_ab)
          REGTOP(reg_pa)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_pa,
              il.Load(DWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          return true;
        }
        case LDDA_PA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_pa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
          REGTOA(reg_ab)
          REGTOP(reg_pa)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_pa, il.Load(DWORD, il.Register(WORD, reg_ab))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDDA_PA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_pa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
          REGTOA(reg_ab)
          REGTOP(reg_pa)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_pa,
              il.Load(DWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDDA_PA_PB:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDDA_PA_PB_OFF10:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Ldh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LDH_DC_AB: {
      len = 2;
      uint8_t op1, reg_dc, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_dc, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.SignExtend(WORD, il.Load(HWORD, il.Register(WORD, reg_ab)))));
      return true;
    }
    case LDH_DC_AB_POSTINC: {
      len = 2;
      uint8_t op1, reg_dc, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_dc, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.SignExtend(WORD, il.Load(HWORD, il.Register(WORD, reg_ab)))));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 2))));
      return true;
    }
    case LDH_DC_A15_OFF4: {
      len = 2;
      uint8_t op1, reg_dc;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSLRO(data, op1, reg_dc, off4);
      mem = (off4 & 0xF) * 2;  // clear sign extended bits & multiply
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.SignExtend(
              WORD,
              il.Load(HWORD, il.Add(WORD, il.Register(WORD, TRICORE_REG_A15),
                                    il.Const(WORD, mem))))));
      return true;
    }
    case LDH_D15_AB_OFF4: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      REGTOA(reg_ab)
      mem = (off4 & 0xF) * 2;  // clear sign extended bits & multiply
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.SignExtend(WORD,
                        il.Load(HWORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                              il.Const(WORD, mem))))));
      return true;
    }
    case LDH_DA_AB_OFF16:  // TCv1.6 ISA only
    {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.SignExtend(
              WORD,
              il.Load(HWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off16)))))));
      return true;
    }
    case OP1_x05_ABS:
      switch (op2) {
        case LDH_DA_OFF18: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(WORD, il.Load(HWORD, il.ConstPointer(WORD, ea)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDH_DA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(
                  WORD,
                  il.Load(HWORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                        il.SignExtend(
                                            WORD, il.Const(HWORD, off10)))))));
          return true;
        }
        case LDH_DA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(WORD, il.Load(HWORD, il.Register(WORD, reg_ab)))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDH_DA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.SignExtend(
                  WORD,
                  il.Load(HWORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                        il.SignExtend(
                                            WORD, il.Const(HWORD, off10)))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDH_DA_PB:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDH_DA_PB_OFF10:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Ldhu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LDHU_DA_AB_OFF16:  // TCv1.6 ISA only
    {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.ZeroExtend(
              WORD,
              il.Load(HWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off16)))))));
      return true;
    }
    case OP1_x05_ABS:
      switch (op2) {
        case LDHU_DA_OFF18: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(WORD, il.Load(HWORD, il.ConstPointer(WORD, ea)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldhu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDHU_DA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(
                  WORD,
                  il.Load(HWORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                        il.SignExtend(
                                            WORD, il.Const(HWORD, off10)))))));
          return true;
        }
        case LDHU_DA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(WORD, il.Load(HWORD, il.Register(WORD, reg_ab)))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDHU_DA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ZeroExtend(
                  WORD,
                  il.Load(HWORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                        il.SignExtend(
                                            WORD, il.Const(HWORD, off10)))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldhu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDHU_DA_PB:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDHU_DA_PB_OFF10:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Ldq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x45_ABS:
      switch (op2) {
        case LDQ_DA_OFF18: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0), il.Load(HWORD, il.ConstPointer(WORD, ea))));
          il.AddInstruction(
              il.SetRegister(WORD, reg_da,
                             il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                          il.Const(WORD, 16))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDQ_DA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)

          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Load(HWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          il.AddInstruction(
              il.SetRegister(WORD, reg_da,
                             il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                          il.Const(WORD, 16))));
          return true;
        }
        case LDQ_DA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)

          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0), il.Load(HWORD, il.Register(WORD, reg_ab))));
          il.AddInstruction(
              il.SetRegister(WORD, reg_da,
                             il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                          il.Const(WORD, 16))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, LLIL_TEMP(0)),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDQ_DA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Load(HWORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          il.AddInstruction(
              il.SetRegister(WORD, reg_da,
                             il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                          il.Const(WORD, 16))));
          il.AddInstruction(
              il.SetRegister(WORD, reg_ab, il.Register(WORD, LLIL_TEMP(0))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDQ_DA_PB:
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDQ_DA_PB_OFF10: {
          ExprId ea, condition;
          LowLevelILLabel trueLabel, falseLabel, doneLabel;
          len = 4;
          uint8_t op1, op2, reg_da, reg_pb, reg_pblow, reg_pbhigh;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_pb, off10);
          reg_pblow = reg_pb;
          reg_pbhigh = reg_pb + 1;
          REGTOA(reg_pblow)
          REGTOA(reg_pbhigh)
          REGTOP(reg_pb)

          // index
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0),
                             il.And(WORD, il.Register(WORD, reg_pbhigh),
                                    il.Const(WORD, 0x0000FFFF))));
          // length
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(1),
              il.LogicalShiftRight(WORD,
                                   il.And(WORD, il.Register(WORD, reg_pbhigh),
                                          il.Const(WORD, 0xFFFF0000)),
                                   il.Const(WORD, 16))));
          ea = il.Add(WORD, il.Register(WORD, reg_pblow),
                      il.Register(WORD, LLIL_TEMP(0)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.ShiftLeft(WORD, il.Load(HWORD, ea), il.Const(WORD, 16))));
          // new index
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Add(WORD, il.Register(WORD, LLIL_TEMP(0)),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));

          // not sure if this should be signed or unsigned <
          condition = il.CompareSignedLessThan(
              WORD, il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 0));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0),
                             il.Add(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Register(WORD, LLIL_TEMP(1)))));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          // not sure if this should be signed or unsigned %
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0),
                             il.ModSigned(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                          il.Register(WORD, LLIL_TEMP(1)))));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          il.AddInstruction(il.SetRegister(
              WORD, reg_pbhigh,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                 il.Const(WORD, 16)),
                    il.And(WORD, il.Register(WORD, LLIL_TEMP(0)),
                           il.Const(WORD, 0x0000FFFF)))));
          return true;
        }
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

bool Ldw::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LDW_DC_AB: {
      len = 2;
      uint8_t op1, reg_dc, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_dc, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.Load(WORD, il.Register(WORD, reg_ab))));
      return true;
    }
    case LDW_DC_AB_POSTINC: {
      len = 2;
      uint8_t op1, reg_dc, reg_ab;
      Instruction::ExtractOpfieldsSLR(data, op1, reg_dc, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.Load(WORD, il.Register(WORD, reg_ab))));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 4))));
      return true;
    }
    case LDW_D15_A10_CONST8: {
      len = 2;
      uint8_t op1, reg_d15;
      int8_t const8;
      uint32_t mem;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      mem = (uint32_t)const8 * 4;
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.Load(WORD, il.Add(WORD, il.Register(WORD, TRICORE_REG_A10),
                               il.Const(WORD, mem)))));
      return true;
    }
    case LDW_DC_A15_OFF4: {
      len = 2;
      uint8_t op1, reg_dc;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSLRO(data, op1, reg_dc, off4);
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Load(WORD, il.Add(WORD, il.Register(WORD, TRICORE_REG_A15),
                               il.Const(WORD, mem)))));
      return true;
    }
    case LDW_D15_AB_OFF4: {
      len = 2;
      uint8_t op1, reg_ab, reg_d15;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      REGTOA(reg_ab)
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      il.AddInstruction(
          il.SetRegister(WORD, TRICORE_REG_D15,
                         il.Load(WORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                              il.Const(WORD, mem)))));
      return true;
    }
    case LDW_DA_AB_OFF16: {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Load(WORD, il.Add(WORD, il.Register(WORD, reg_ab),
                                              il.Const(WORD, off16)))));
      return true;
    }
    case OP1_x85_ABS:
      switch (op2) {
        case LDW_DA_OFF18: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.SetRegister(
              WORD, reg_da, il.Load(WORD, il.ConstPointer(WORD, ea))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x09_BO:
      switch (op2) {
        case LDW_DA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.Load(WORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          return true;
        }
        case LDW_DA_AB_OFF10_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da, il.Load(WORD, il.Register(WORD, reg_ab))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case LDW_DA_AB_OFF10_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.Load(WORD,
                      il.Add(WORD, il.Register(WORD, reg_ab),
                             il.SignExtend(WORD, il.Const(HWORD, off10))))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ldw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x29_BO:
      switch (op2) {
        case LDW_DA_PB:
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case LDW_DA_PB_OFF10:
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Ldlcx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ldmst::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Lducx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Lea::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LEA_AA_AB_OFF16: {
      len = 4;
      uint8_t op1, reg_aa, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_aa, reg_ab, off16);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(
          il.SetRegister(WORD, reg_aa,
                         il.Add(WORD, il.Register(WORD, reg_ab),
                                il.SignExtend(WORD, il.Const(HWORD, off16)))));
      return true;
    }
    case OP1_xC5_ABS:
      switch (op2) {
        case LEA_AA_OFF18: {
          len = 4;
          uint8_t op1, op2, reg_aa;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_aa, ea);
          REGTOA(reg_aa)
          il.AddInstruction(il.SetRegister(WORD, reg_aa, il.Const(WORD, ea)));
          return true;
        }
        default:
          LogDebug("0x%lx: Lea::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x49_BO:
      switch (op2) {
        case LEA_AA_AB_OFF10: {
          len = 4;
          uint8_t op1, op2, reg_aa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
          REGTOA(reg_aa)
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_aa,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
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

bool Loop::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LOOP_AB_DISP4: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 2;
      uint8_t op1, reg_ab;
      int8_t disp4;
      Instruction::ExtractOpfieldsSBR(data, op1, disp4, reg_ab);
      REGTOA(reg_ab)
      condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_ab),
                                     il.Const(WORD, 0));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Sub(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 1))));
      il.AddInstruction(
          il.Jump(il.ConstPointer(WORD, Loop::GetTarget(data, addr, len))));

      il.MarkLabel(doneLabel);
      return true;
    }
    case OP1_xFD_BRR:
      switch (op2) {
        case LOOP_AB_DISP15: {
          ExprId condition;
          LowLevelILLabel trueLabel, doneLabel;
          len = 4;
          uint8_t op1, op2, s1, reg_ab;
          int16_t disp15;
          Instruction::ExtractOpfieldsBRR(data, op1, op2, s1, reg_ab, disp15);
          REGTOA(reg_ab)

          condition = il.CompareNotEqual(WORD, il.Register(WORD, reg_ab),
                                         il.Const(WORD, 0));
          il.AddInstruction(il.If(condition, trueLabel, doneLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Sub(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 1))));
          il.AddInstruction(
              il.Jump(il.ConstPointer(WORD, Loop::GetTarget(data, addr, len))));

          il.MarkLabel(doneLabel);
          return true;
        }
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

bool Loopu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_xFD_BRR:
      switch (op2) {
        case LOOPU_DISP15:
          len = 4;
          il.AddInstruction(il.Jump(
              il.ConstPointer(WORD, Loopu::GetTarget(data, addr, len))));
          return true;
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

bool Lt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case LT_D15_DA_CONST4: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.BoolToInt(WORD,
                       il.CompareSignedLessThan(
                           WORD, il.Register(WORD, reg_da),
                           il.SignExtend(WORD, il.Const(BYTE, const4))))));
      return true;
    }
    case LT_D15_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.BoolToInt(WORD,
                       il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db)))));
      return true;
    }
    case OP1_x8B_RC:
      switch (op2) {
        case LT_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD,
                           il.CompareSignedLessThan(
                               WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        case LTU_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD,
                           il.CompareUnsignedLessThan(
                               WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Lt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case LT_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD, il.CompareSignedLessThan(
                                     WORD, il.Register(WORD, reg_da),
                                     il.Register(WORD, reg_db)))));
          return true;
        }
        case LTU_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD, il.CompareUnsignedLessThan(
                                     WORD, il.Register(WORD, reg_da),
                                     il.Register(WORD, reg_db)))));
          return true;
        }
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

bool Lta::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x01_RR || op2 != LTA_DC_AA_AB) {
    LogDebug("0x%lx: Lta::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, reg_ab, n, reg_dc;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n, reg_dc);
    REGTOA(reg_aa)
    REGTOA(reg_ab)
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.BoolToInt(WORD,
                     il.CompareUnsignedLessThan(WORD, il.Register(WORD, reg_aa),
                                                il.Register(WORD, reg_ab)))));
    return true;
  }
}

bool Ltb::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Ltb::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case LTB_DC_DA_DB: {
      ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(3),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(2),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareSignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFF))))));

      cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                   il.Const(WORD, 1));
      cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                   il.Const(WORD, 1));
      cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                   il.Const(WORD, 1));
      cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                   il.Const(WORD, 1));

      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

      il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF00))));
      il.AddInstruction(il.Goto(falseLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
      il.MarkLabel(trueLabel2);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF0000))));
      il.AddInstruction(il.Goto(falseLabel2));

      il.MarkLabel(falseLabel2);
      il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
      il.MarkLabel(trueLabel3);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF000000))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case LTBU_DC_DA_DB: {
      ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(3),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(2),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareUnsignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFF))))));

      cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                   il.Const(WORD, 1));
      cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                   il.Const(WORD, 1));
      cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                   il.Const(WORD, 1));
      cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                   il.Const(WORD, 1));

      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

      il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF00))));
      il.AddInstruction(il.Goto(falseLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
      il.MarkLabel(trueLabel2);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF0000))));
      il.AddInstruction(il.Goto(falseLabel2));

      il.MarkLabel(falseLabel2);
      il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
      il.MarkLabel(trueLabel3);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFF000000))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    default:
      LogDebug("0x%lx: Ltb::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Lth::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Lth::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case LTH_DC_DA_DB: {
      ExprId cond_hword0, cond_hword1;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareSignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFFFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFFFF))))));

      cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Const(WORD, 1));
      cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                    il.Const(WORD, 1));

      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

      il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFFFF))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFFFF0000))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case LTHU_DC_DA_DB: {
      ExprId cond_hword0, cond_hword1;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareUnsignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFFFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFFFF))))));

      cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Const(WORD, 1));
      cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                    il.Const(WORD, 1));

      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));

      il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFFFF))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_dc,
          il.Or(WORD, il.Register(WORD, reg_dc), il.Const(WORD, 0xFFFF0000))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    default:
      LogDebug("0x%lx: Lth::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Ltw::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Ltw::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case LTW_DC_DA_DB: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      condition = il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                           il.Register(WORD, reg_db));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Const(WORD, 0xFFFFFFFF)));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    case LTWU_DC_DA_DB: {
      ExprId condition;
      LowLevelILLabel trueLabel, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      condition = il.CompareUnsignedLessThan(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));
      il.AddInstruction(il.If(condition, trueLabel, doneLabel));

      il.MarkLabel(trueLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Const(WORD, 0xFFFFFFFF)));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      return true;
    }
    default:
      LogDebug("0x%lx: Ltw::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Madd::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x13_RCR:
      switch (op2) {
        case MADD_DC_DD_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_dd, reg_dc;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Add(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(
                         WORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        case MADD_EC_ED_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ed, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.Add(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(
                         DWORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        case MADDS_DC_DD_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_dd, reg_dc;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Add(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(
                         WORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        case MADDS_EC_ED_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ed, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, LLIL_TEMP(0),
              il.Add(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(
                         DWORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          ssov(il.Register(DWORD, LLIL_TEMP(0)), il.Const(WORD, 64),
               LLIL_TEMP(1), il, DWORD);
          il.AddInstruction(
              il.SetRegister(DWORD, reg_ec, il.Register(DWORD, LLIL_TEMP(1))));
          return true;
        }
        default:
          LogDebug("0x%lx: Madd::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x03_RRR2:
      switch (op2) {
        case MADD_DC_DD_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Add(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          return true;
        }
        case MADD_EC_ED_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_ec;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.Add(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(DWORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          return true;
        }
        case MADDS_DC_DD_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Add(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        case MADDS_EC_ED_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_ec;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, LLIL_TEMP(0),
              il.Add(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(DWORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          ssov(il.Register(DWORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, DWORD);
          il.AddInstruction(
              il.SetRegister(DWORD, reg_ec, il.Register(DWORD, LLIL_TEMP(1))));
          return true;
        }
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

bool Maddh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Maddq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Maddu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MaddmhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MaddrhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Maddrq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MaddsuhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MaddsumhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MaddsurhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Max::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  ExprId condition;
  LowLevelILLabel trueLabel, falseLabel, doneLabel;
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case MAX_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          condition = il.CompareSignedGreaterThan(
              WORD, il.Register(WORD, reg_da),
              il.SignExtend(WORD, il.Const(HWORD, const9)));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        case MAXU_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          condition = il.CompareUnsignedGreaterThan(
              WORD, il.Register(WORD, reg_da),
              il.ZeroExtend(WORD, il.Const(HWORD, const9)));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.ZeroExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        default:
          LogDebug("0x%lx: Max::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case MAX_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          condition = il.CompareSignedGreaterThan(
              WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_db)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        case MAXU_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          condition = il.CompareUnsignedGreaterThan(
              WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_db)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
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

bool Maxb::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Maxb::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MAXB_DC_DA_DB: {
      ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(3),
          il.BoolToInt(
              WORD,
              il.CompareSignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(2),
          il.BoolToInt(
              WORD,
              il.CompareSignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareSignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareSignedGreaterThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFF))))));

      cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                   il.Const(WORD, 1));
      cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                   il.Const(WORD, 1));
      cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                   il.Const(WORD, 1));
      cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                   il.Const(WORD, 1));

      // Set to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFFFF00))));
      // Set byte to true case
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(4),
          il.Or(
              WORD, il.Register(WORD, LLIL_TEMP(4)),
              il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF00FF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF00)))));
      il.AddInstruction(il.Goto(falseLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
      il.MarkLabel(trueLabel2);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFF00FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF0000)))));
      il.AddInstruction(il.Goto(falseLabel2));

      il.MarkLabel(falseLabel2);
      il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
      il.MarkLabel(trueLabel3);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x00FFFFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF000000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    case MAXBU_DC_DA_DB: {
      ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(3),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(2),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareUnsignedGreaterThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFF))))));

      cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                   il.Const(WORD, 1));
      cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                   il.Const(WORD, 1));
      cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                   il.Const(WORD, 1));
      cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                   il.Const(WORD, 1));

      // Set to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFFFF00))));
      // Set byte to true case
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(4),
          il.Or(
              WORD, il.Register(WORD, LLIL_TEMP(4)),
              il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF00FF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF00)))));
      il.AddInstruction(il.Goto(falseLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
      il.MarkLabel(trueLabel2);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFF00FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF0000)))));
      il.AddInstruction(il.Goto(falseLabel2));

      il.MarkLabel(falseLabel2);
      il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
      il.MarkLabel(trueLabel3);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x00FFFFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF000000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    default:
      LogDebug("0x%lx: Maxb::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Maxh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Maxh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MAXH_DC_DA_DB: {
      ExprId cond_hword0, cond_hword1;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareSignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareSignedGreaterThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFFFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFFFF))))));

      cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Const(WORD, 1));
      cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                    il.Const(WORD, 1));

      // Set byte to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF0000))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x0000FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF0000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    case MAXHU_DC_DA_DB: {
      ExprId cond_hword0, cond_hword1;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedGreaterThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareUnsignedGreaterThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFFFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFFFF))))));

      cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Const(WORD, 1));
      cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                    il.Const(WORD, 1));

      // Set byte to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF0000))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x0000FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF0000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    default:
      LogDebug("0x%lx: Maxh::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Mfcr::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != MFCR_DC_CONST16) {
    LogDebug("0x%lx: Mfcr::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 4;
    uint8_t op1, s1, reg_dc;
    int16_t const16;
    uint32_t core_reg;
    Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_dc);
    core_reg = (uint32_t)const16 & 0xFFFF;
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.Register(WORD, core_reg)));
    return true;
  }
}

bool Min::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  ExprId condition;
  LowLevelILLabel trueLabel, falseLabel, doneLabel;
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case MIN_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          condition = il.CompareSignedLessThan(
              WORD, il.Register(WORD, reg_da),
              il.SignExtend(WORD, il.Const(HWORD, const9)));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        case MINU_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          condition = il.CompareUnsignedLessThan(
              WORD, il.Register(WORD, reg_da),
              il.ZeroExtend(WORD, il.Const(HWORD, const9)));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.ZeroExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        default:
          LogDebug("0x%lx: Min::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case MIN_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          condition = il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_db)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
        case MINU_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          condition = il.CompareUnsignedLessThan(
              WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db));
          il.AddInstruction(il.If(condition, trueLabel, falseLabel));

          il.MarkLabel(trueLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(falseLabel);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_db)));
          il.AddInstruction(il.Goto(doneLabel));

          il.MarkLabel(doneLabel);
          return true;
        }
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

bool Minb::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Minb::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MINB_DC_DA_DB: {
      ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(3),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(2),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareSignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFF))))));

      cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                   il.Const(WORD, 1));
      cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                   il.Const(WORD, 1));
      cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                   il.Const(WORD, 1));
      cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                   il.Const(WORD, 1));

      // Set to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFFFF00))));
      // Set byte to true case
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(4),
          il.Or(
              WORD, il.Register(WORD, LLIL_TEMP(4)),
              il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF00FF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF00)))));
      il.AddInstruction(il.Goto(falseLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
      il.MarkLabel(trueLabel2);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFF00FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF0000)))));
      il.AddInstruction(il.Goto(falseLabel2));

      il.MarkLabel(falseLabel2);
      il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
      il.MarkLabel(trueLabel3);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x00FFFFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF000000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    case MINBU_DC_DA_DB: {
      ExprId cond_byte0, cond_byte1, cond_byte2, cond_byte3;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(3),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 24)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(2),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 8)),
                         il.Const(WORD, 0xFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareUnsignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFF))))));

      cond_byte0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                   il.Const(WORD, 1));
      cond_byte1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                   il.Const(WORD, 1));
      cond_byte2 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(2)),
                                   il.Const(WORD, 1));
      cond_byte3 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(3)),
                                   il.Const(WORD, 1));

      // Set to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_byte0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFFFF00))));
      // Set byte to true case
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(4),
          il.Or(
              WORD, il.Register(WORD, LLIL_TEMP(4)),
              il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_byte1, trueLabel1, falseLabel1));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF00FF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF00)))));
      il.AddInstruction(il.Goto(falseLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(il.If(cond_byte2, trueLabel2, falseLabel2));
      il.MarkLabel(trueLabel2);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFF00FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF0000)))));
      il.AddInstruction(il.Goto(falseLabel2));

      il.MarkLabel(falseLabel2);
      il.AddInstruction(il.If(cond_byte3, trueLabel3, doneLabel));
      il.MarkLabel(trueLabel3);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x00FFFFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFF000000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    default:
      LogDebug("0x%lx: Minb::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Minh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Minh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case MINH_DC_DA_DB: {
      ExprId cond_hword0, cond_hword1;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareSignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareSignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFFFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFFFF))))));

      cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Const(WORD, 1));
      cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                    il.Const(WORD, 1));

      // Set byte to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF0000))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x0000FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF0000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    case MINHU_DC_DA_DB: {
      ExprId cond_hword0, cond_hword1;
      LowLevelILLabel trueLabel0, trueLabel1, trueLabel2, trueLabel3,
          falseLabel0, falseLabel1, falseLabel2, doneLabel;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(1),
          il.BoolToInt(
              WORD,
              il.CompareUnsignedLessThan(
                  WORD,
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF)),
                  il.And(WORD,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, 16)),
                         il.Const(WORD, 0xFFFF))))));
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.BoolToInt(WORD, il.CompareUnsignedLessThan(
                                 WORD,
                                 il.And(WORD, il.Register(WORD, reg_da),
                                        il.Const(WORD, 0xFFFF)),
                                 il.And(WORD, il.Register(WORD, reg_db),
                                        il.Const(WORD, 0xFFFF))))));

      cond_hword0 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(0)),
                                    il.Const(WORD, 1));
      cond_hword1 = il.CompareEqual(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                    il.Const(WORD, 1));

      // Set byte to false case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4), il.Register(WORD, reg_db)));

      il.AddInstruction(il.If(cond_hword0, trueLabel0, falseLabel0));
      il.MarkLabel(trueLabel0);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0xFFFF0000))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF)))));
      il.AddInstruction(il.Goto(falseLabel0));

      il.MarkLabel(falseLabel0);
      il.AddInstruction(il.If(cond_hword1, trueLabel1, doneLabel));
      il.MarkLabel(trueLabel1);
      // Clear byte
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.And(WORD, il.Register(WORD, LLIL_TEMP(4)),
                                il.Const(WORD, 0x0000FFFF))));
      // Set byte to true case
      il.AddInstruction(
          il.SetRegister(WORD, LLIL_TEMP(4),
                         il.Or(WORD, il.Register(WORD, LLIL_TEMP(4)),
                               il.And(WORD, il.Register(WORD, reg_da),
                                      il.Const(WORD, 0xFFFF0000)))));
      il.AddInstruction(il.Goto(doneLabel));

      il.MarkLabel(doneLabel);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(4))));
      return true;
    }
    default:
      LogDebug("0x%lx: Minh::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Mov::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case MOV_D15_CONST8: {
      uint8_t op1;
      int8_t const8;
      len = 2;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      il.AddInstruction(il.SetRegister(WORD, TRICORE_REG_D15,
                                       il.Const(BYTE, const8), Mov::flags));
      return true;
    }
    case MOV_DA_CONST4: {
      uint8_t op1, reg_da;
      int8_t const4;
      len = 2;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da, il.Const(BYTE, const4), Mov::flags));
      return true;
    }
    case MOV_EA_CONST4:  // TCv1.6 ISA only
    {
      uint8_t op1, reg_ea;
      int8_t const4;
      len = 2;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_ea, const4);
      REGTOE(reg_ea)
      il.AddInstruction(il.SetRegister(
          DWORD, reg_ea,
          il.SignExtend(DWORD, il.Const(BYTE, const4), Mov::flags)));
      return true;
    }
    case MOV_DA_DB: {
      uint8_t op1, reg_da, reg_db;
      len = 2;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da, il.Register(WORD, reg_db), Mov::flags));
      return true;
    }
    case MOV_DC_CONST16: {
      uint8_t op1, s1, reg_dc;
      int16_t const16;
      len = 4;
      Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_dc);
      il.AddInstruction(
          il.SetRegister(WORD, reg_dc, il.Const(HWORD, const16), Mov::flags));
      return true;
    }
    case MOV_EC_CONST16:  // TCv1.6 ISA only
    {
      uint8_t op1, s1, reg_ec;
      int16_t const16;
      len = 4;
      Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_ec);
      REGTOE(reg_ec)
      il.AddInstruction(il.SetRegister(
          DWORD, reg_ec,
          il.SignExtend(DWORD, il.Const(HWORD, const16), Mov::flags)));
      return true;
    }
    case OP1_x0B_RR:
      switch (op2) {
        case MOV_DC_DB: {
          uint8_t reg_dc, reg_db, op1, op2, s1, n;
          len = 4;
          Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Register(WORD, reg_db), Mov::flags));
          return true;
        }
        case MOV_EC_DB:  // TCv1.6 ISA only
        {
          uint8_t reg_ec, reg_db, op1, op2, s1, n;
          len = 4;
          Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_ec);
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.SignExtend(DWORD, il.Register(WORD, reg_db), Mov::flags)));
          return true;
        }
        case MOV_EC_DA_DB:  // TCv1.6 ISA only
        {
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        }
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

bool Mova::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case MOVA_AA_CONST4: {
      len = 2;
      uint8_t op1, reg_aa;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_aa, const4);
      const4 &= 0x0F;  // zero extend
      REGTOA(reg_aa)
      il.AddInstruction(
          il.SetRegister(WORD, reg_aa, il.Const(BYTE, const4), Mova::flags));
      return true;
    }
    case MOVA_AA_DB: {
      len = 2;
      uint8_t op1, reg_aa, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_db);
      REGTOA(reg_aa)
      il.AddInstruction(
          il.SetRegister(WORD, reg_aa, il.Register(WORD, reg_db), Mova::flags));
      return true;
    }
    case OP1_x01_RR:
      switch (op2) {
        case MOVA_AC_DB: {
          len = 4;
          uint8_t op1, op2, s1, reg_db, n, reg_ac;
          Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_db, n, reg_ac);
          REGTOA(reg_ac)
          il.AddInstruction(il.SetRegister(
              WORD, reg_ac, il.Register(WORD, reg_db), Mova::flags));
          return true;
        }
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

bool Movaa::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case MOVAA_AA_AB: {
      len = 2;
      uint8_t op1, reg_aa, reg_ab;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_ab);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(il.SetRegister(WORD, reg_aa, il.Register(WORD, reg_ab),
                                       Movaa::flags));
      return true;
    }
    case OP1_x01_RR:
      switch (op2) {
        case MOVAA_AC_AB: {
          len = 4;
          uint8_t op1, op2, s1, reg_ab, n, reg_ac;
          Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_ab, n, reg_ac);
          REGTOA(reg_ab)
          REGTOA(reg_ac)
          il.AddInstruction(il.SetRegister(
              WORD, reg_ac, il.Register(WORD, reg_ab), Movaa::flags));
          return true;
        }
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

bool Movd::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case MOVD_DA_AB: {
      len = 2;
      uint8_t op1, reg_da, reg_ab;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.SetRegister(WORD, reg_da, il.Register(WORD, reg_ab), Movd::flags));
      return true;
    }
    case OP1_x01_RR:
      switch (op2) {
        case MOVD_DC_AB: {
          len = 4;
          uint8_t op1, op2, s1, reg_ab, n, reg_dc;
          Instruction::ExtractOpfieldsRR(data, op1, op2, s1, reg_ab, n, reg_dc);
          REGTOA(reg_ab)
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Register(WORD, reg_ab), Movd::flags));
          return true;
        }
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

bool Movu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != MOVU_DC_CONST16) {
    LogDebug("0x%lx: Movu::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {  // new scope because op1 exists as a parameter to this function
    len = 4;
    uint8_t op1, s1, reg_dc;
    int16_t const16;
    Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_dc);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc, il.Const(HWORD, (uint16_t)const16), Movu::flags));
  }
  return true;
}

bool Movh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != MOVH_DC_CONST16) {
    LogDebug("0x%lx: Movh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {  // new scope because op1 exists as a parameter to this function
    len = 4;
    uint8_t op1, s1, reg_dc;
    int16_t const16;
    int32_t value;
    len = 4;
    Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_dc);

    value = (int32_t)(const16 << 16) & (int32_t)0xFFFF0000;
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.Const(WORD, value), Movh::flags));
    return true;
  }
}

bool Movha::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != MOVHA_AC_CONST16) {
    LogDebug("0x%lx: Movha::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {  // new scope because op1 exists as a parameter to this function
    len = 4;
    uint8_t op1, s1, reg_ac;
    int16_t const16;
    uint32_t value;
    len = 4;
    Instruction::ExtractOpfieldsRLC(data, op1, s1, const16, reg_ac);
    REGTOA(reg_ac)

    value = (const16 << 16) & 0xFFFF0000;
    il.AddInstruction(
        il.SetRegister(WORD, reg_ac, il.Const(WORD, value), Movha::flags));
    return true;
  }
}

bool Msub::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x33_RCR:
      switch (op2) {
        case MSUB_DC_DD_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_dd, reg_dc;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Sub(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(
                         WORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        case MSUB_EC_ED_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ed, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(
                         DWORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        case MSUBS_DC_DD_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_dd, reg_dc;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Sub(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(
                         WORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        case MSUBS_EC_ED_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ed, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, LLIL_TEMP(0),
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(
                         DWORD, il.Register(WORD, reg_da),
                         il.SignExtend(WORD, il.Const(HWORD, const9))))));
          ssov(il.Register(DWORD, LLIL_TEMP(0)), il.Const(WORD, 64),
               LLIL_TEMP(1), il, DWORD);
          il.AddInstruction(
              il.SetRegister(DWORD, reg_ec, il.Register(DWORD, LLIL_TEMP(1))));
          return true;
        }
        default:
          LogDebug("0x%lx: Msub::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x23_RRR2:
      switch (op2) {
        case MSUB_DC_DD_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Sub(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          return true;
        }
        case MSUB_EC_ED_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_ec;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(DWORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          return true;
        }
        case MSUBS_DC_DD_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Sub(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecSigned(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        case MSUBS_EC_ED_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_ec;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, LLIL_TEMP(0),
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecSigned(DWORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));
          ssov(il.Register(DWORD, LLIL_TEMP(0)), il.Const(WORD, 64),
               LLIL_TEMP(1), il, DWORD);
          il.AddInstruction(
              il.SetRegister(DWORD, reg_ec, il.Register(DWORD, LLIL_TEMP(1))));
          return true;
        }
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

bool Msubh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Msubq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Msubu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x33_RCR:
      switch (op2) {
        case MSUBU_EC_ED_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ed, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecUnsigned(
                         DWORD, il.Register(WORD, reg_da),
                         il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        case MSUBSU_DC_DD_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_dd, reg_dc;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_dd, reg_dc);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Sub(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecUnsigned(
                         WORD, il.Register(WORD, reg_da),
                         il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
          suov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        case MSUBSU_EC_ED_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ed, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              DWORD, LLIL_TEMP(0),
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecUnsigned(
                         DWORD, il.Register(WORD, reg_da),
                         il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
          suov(il.Register(DWORD, LLIL_TEMP(0)), il.Const(WORD, 64),
               LLIL_TEMP(1), il, DWORD);
          il.AddInstruction(
              il.SetRegister(DWORD, reg_ec, il.Register(DWORD, LLIL_TEMP(1))));
          return true;
        }
        default:
          LogDebug("0x%lx: Msubu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x23_RRR2:
      switch (op2) {
        case MSUBU_EC_ED_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_ec;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecUnsigned(DWORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db)))));
          return true;
        }
        case MSUBSU_DC_DD_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_dd, reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.Sub(WORD, il.Register(WORD, reg_dd),
                     il.MultDoublePrecUnsigned(WORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db)))));
          suov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        case MSUBSU_EC_ED_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ed, reg_ec;
          Instruction::ExtractOpfieldsRRR2(data, op1, op2, reg_da, reg_db,
                                           reg_ed, reg_ec);
          REGTOE(reg_ed)
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, LLIL_TEMP(0),
              il.Sub(DWORD, il.Register(DWORD, reg_ed),
                     il.MultDoublePrecUnsigned(DWORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db)))));
          suov(il.Register(DWORD, LLIL_TEMP(0)), il.Const(WORD, 64),
               LLIL_TEMP(1), il, DWORD);
          il.AddInstruction(
              il.SetRegister(DWORD, reg_ec, il.Register(DWORD, LLIL_TEMP(1))));
          return true;
        }
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

bool MsubadhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MsubadmhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MsubadrhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                      uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MsubmhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool MsubrhSh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Msubrq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Mtcr::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != MTCR_CONST16_DA) {
    LogDebug("0x%lx: Mtcr::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  {
    len = 4;
    uint8_t op1, reg_da, d;
    int16_t const16;
    uint32_t core_reg;
    Instruction::ExtractOpfieldsRLC(data, op1, reg_da, const16, d);
    core_reg = (uint32_t)const16 & 0xFFFF;
    il.AddInstruction(
        il.SetRegister(WORD, core_reg, il.Register(WORD, reg_da)));
    return true;
  }
}

bool Mul::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case MUL_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.MultDoublePrecSigned(WORD, il.Register(WORD, reg_da),
                                  il.Register(WORD, reg_db))));
      return true;
    }
    case OP1_x53_RC:
      switch (op2) {
        case MUL_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.MultDoublePrecSigned(
                  WORD, il.Register(WORD, reg_da),
                  il.SignExtend(WORD, il.Const(HWORD, const9)))));
          return true;
        }
        case MUL_EC_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_ec,
                                         const9);
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.MultDoublePrecSigned(
                  DWORD, il.Register(WORD, reg_da),
                  il.SignExtend(WORD, il.Const(HWORD, const9)))));
          return true;
        }
        case MULS_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.MultDoublePrecSigned(
                  WORD, il.Register(WORD, reg_da),
                  il.SignExtend(WORD, il.Const(HWORD, const9)))));
          ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        default:
          LogDebug("0x%lx: Mul::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x73_RR2:
      switch (op2) {
        case MUL_DC_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dc;
          Instruction::ExtractOpfieldsRR2(data, op1, op2, reg_da, reg_db,
                                          reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.MultDoublePrecSigned(WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db))));
          return true;
        }
        case MUL_EC_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ec;
          Instruction::ExtractOpfieldsRR2(data, op1, op2, reg_da, reg_db,
                                          reg_ec);
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.MultDoublePrecSigned(DWORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db))));
          return true;
        }
        case MULS_DC_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dc;
          Instruction::ExtractOpfieldsRR2(data, op1, op2, reg_da, reg_db,
                                          reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.MultDoublePrecSigned(WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db))));
          ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
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

bool Mulh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Mulq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Mulu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x53_RC:
      switch (op2) {
        case MULU_EC_DA_CONST9: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ec;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_ec,
                                         const9);
          REGTOE(reg_ec)
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.MultDoublePrecUnsigned(
                  DWORD, il.Register(WORD, reg_da),
                  il.ZeroExtend(WORD, il.Const(HWORD, const9)))));
          return true;
        }
        case MULSU_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.MultDoublePrecUnsigned(
                  WORD, il.Register(WORD, reg_da),
                  il.ZeroExtend(WORD, il.Const(HWORD, const9)))));
          suov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
        default:
          LogDebug("0x%lx: Mulu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x73_RR2:
      switch (op2) {
        case MULU_EC_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_ec;
          Instruction::ExtractOpfieldsRR2(data, op1, op2, reg_da, reg_db,
                                          reg_ec);
          REGTOE(reg_ec)
          il.AddInstruction(il.SetRegister(
              DWORD, reg_ec,
              il.MultDoublePrecUnsigned(DWORD, il.Register(WORD, reg_da),
                                        il.Register(WORD, reg_db))));
          return true;
        }
        case MULSU_DC_DA_DB: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, reg_dc;
          Instruction::ExtractOpfieldsRR2(data, op1, op2, reg_da, reg_db,
                                          reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, LLIL_TEMP(0),
              il.MultDoublePrecUnsigned(WORD, il.Register(WORD, reg_da),
                                        il.Register(WORD, reg_db))));
          suov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32),
               LLIL_TEMP(1), il, WORD);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(1))));
          return true;
        }
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

bool Mulmh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Mulrh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Mulrq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Nand::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case NAND_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Not(WORD,
                     il.And(WORD, il.Register(WORD, reg_da),
                            il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Nand::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case NAND_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Not(WORD, il.And(WORD, il.Register(WORD, reg_da),
                                  il.Register(WORD, reg_db)))));
          return true;
        }
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

bool Nandt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x07_BIT || op2 != NANDT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Nandt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId da, db, condition;
    LowLevelILLabel trueLabel1, trueLabel2, doneLabel;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.CompareNotEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
        il.Const(WORD, 0));
    db = il.CompareNotEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
        il.Const(WORD, 0));

    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 1)));
    il.AddInstruction(il.If(da, trueLabel1, doneLabel));
    il.MarkLabel(trueLabel1);
    il.AddInstruction(il.If(db, trueLabel2, doneLabel));
    il.MarkLabel(trueLabel2);
    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 0)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(0))));
    return true;
  }
}

bool Ne::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case NE_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD,
                           il.CompareNotEqual(
                               WORD, il.Register(WORD, reg_da),
                               il.SignExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Ne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case NE_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.BoolToInt(WORD,
                           il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                              il.Register(WORD, reg_db)))));
          return true;
        }
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

bool Nea::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x01_RR || op2 != NEA_DC_AA_AB) {
    LogDebug("0x%lx: Nea::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, reg_ab, n, reg_dc;
    char buf[32];

    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n, reg_dc);
    REGTOA(reg_aa)
    REGTOA(reg_ab)
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.BoolToInt(WORD, il.CompareNotEqual(WORD, il.Register(WORD, reg_aa),
                                              il.Register(WORD, reg_ab)))));
    return true;
  }
}

bool Neza::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x01_RR || op2 != NEZA_DC_AA) {
    LogDebug("0x%lx: Neza::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_aa, s1, n, reg_dc;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, s1, n, reg_dc);
    REGTOA(reg_aa)
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.BoolToInt(WORD, il.CompareNotEqual(WORD, il.Register(WORD, reg_aa),
                                              il.Const(WORD, 0)))));
    return true;
  }
}

bool Nop::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  len = *((const uint16_t*)data) & 0x1 ? 4 : 2;
  il.AddInstruction(il.Nop());
  return true;
}

bool Nor::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case NOR_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Not(WORD,
                     il.Or(WORD, il.Register(WORD, reg_da),
                           il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Nor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case NOR_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Not(WORD, il.Or(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db)))));
          return true;
        }
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

bool Nort::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x87_BIT || op2 != NORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Nort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    ExprId da, db, condition;
    LowLevelILLabel trueLabel1, trueLabel2, falseLabel, doneLabel;
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.CompareNotEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
        il.Const(WORD, 0));
    db = il.CompareNotEqual(
        WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
        il.Const(WORD, 0));

    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 1)));

    il.AddInstruction(il.If(da, trueLabel1, falseLabel));
    il.MarkLabel(trueLabel1);
    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 0)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(falseLabel);
    il.AddInstruction(il.If(db, trueLabel2, doneLabel));
    il.MarkLabel(trueLabel2);
    il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, 0)));
    il.AddInstruction(il.Goto(doneLabel));

    il.MarkLabel(doneLabel);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(0))));
    return true;
  }
}

bool Not::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x46_SR || op2 != NOT_DA) {
    LogDebug("0x%lx: Not::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 2;
    uint8_t op1, reg_da, op2;
    Instruction::ExtractOpfieldsSR(data, op1, reg_da, op2);
    il.AddInstruction(
        il.SetRegister(WORD, reg_da, il.Not(WORD, il.Register(WORD, reg_da))));
    return true;
  }
}

bool Or::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OR_D15_CONST8: {
      len = 2;
      uint8_t op1;
      int8_t const8;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      il.AddInstruction(
          il.SetRegister(WORD, TRICORE_REG_D15,
                         il.Or(WORD, il.Register(WORD, TRICORE_REG_D15),
                               il.ZeroExtend(WORD, il.Const(BYTE, const8)))));
      return true;
    }
    case OR_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.Or(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
      return true;
    }
    case OP1_x8F_RC:
      switch (op2) {
        case OR_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD, il.Register(WORD, reg_da),
                    il.ZeroExtend(WORD, il.Const(HWORD, const9)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Or::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case OR_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD, il.Register(WORD, reg_da),
                                   il.Register(WORD, reg_db))));
          return true;
        }
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

bool OrAndtAndntNortOrt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                              uint64_t addr, size_t& len,
                              LowLevelILFunction& il) {
  if (op1 != OP1_xC7_BIT) {
    LogDebug("0x%lx: OrAndtAndntNortOrt::%s received invalid opcode 1 0x%x",
             addr, __func__, op1);
    return false;
  }
  switch (op2) {
    case ORANDT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, bp1)),
                                   il.Const(WORD, 0)));
      db = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_db),
                                          il.Const(WORD, bp2)),
                                   il.Const(WORD, 0)));
      result = il.And(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    case ORANDNT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, bp1)),
                                   il.Const(WORD, 0)));
      db = il.BoolToInt(WORD,
                        il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_db),
                                               il.Const(WORD, bp2)),
                                        il.Const(WORD, 0)));
      result = il.And(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    case ORNORT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(WORD,
                        il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_da),
                                               il.Const(WORD, bp1)),
                                        il.Const(WORD, 0)));
      db = il.BoolToInt(WORD,
                        il.CompareEqual(WORD,
                                        il.And(WORD, il.Register(WORD, reg_db),
                                               il.Const(WORD, bp2)),
                                        il.Const(WORD, 0)));
      result = il.And(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    case ORORT_DC_DA_POS1_DB_POS2: {
      ExprId da, db, result, condition;
      len = 4;
      uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
      Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                      pos2, reg_dc);
      uint32_t bp1, bp2;
      bp1 = 1 << pos1;
      bp2 = 1 << pos2;
      da = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_da),
                                          il.Const(WORD, bp1)),
                                   il.Const(WORD, 0)));
      db = il.BoolToInt(
          WORD, il.CompareNotEqual(WORD,
                                   il.And(WORD, il.Register(WORD, reg_db),
                                          il.Const(WORD, bp2)),
                                   il.Const(WORD, 0)));
      result = il.Or(WORD, da, db);

      il.AddInstruction(il.SetRegister(
          WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), result)));
      return true;
    }
    default:
      LogDebug("0x%lx: OrAndtAndntNortOrt::%s received invalid opcode 2 0x%x",
               addr, __func__, op2);
      return false;
  }
}

bool Oreq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case OREQ_DC_DA_CONST9: {
          ExprId eq;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          eq = il.BoolToInt(
              WORD,
              il.CompareEqual(WORD, il.Register(WORD, reg_da),
                              il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        default:
          LogDebug("0x%lx: Oreq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case OREQ_DC_DA_DB: {
          ExprId eq;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          eq = il.BoolToInt(WORD,
                            il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                            il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
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

bool OrgeGeu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ORGE_DC_DA_CONST9: {
          ExprId ge;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          ge = il.BoolToInt(WORD,
                            il.CompareSignedGreaterEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        case ORGEU_DC_DA_CONST9: {
          ExprId ge;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          ge = il.BoolToInt(WORD,
                            il.CompareUnsignedGreaterEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.ZeroExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        default:
          LogDebug("0x%lx: OrgeGeu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ORGE_DC_DA_DB: {
          ExprId ge;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          ge = il.BoolToInt(WORD, il.CompareSignedGreaterEqual(
                                      WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
        case ORGEU_DC_DA_DB: {
          ExprId ge;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          ge = il.BoolToInt(WORD, il.CompareUnsignedGreaterEqual(
                                      WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), ge)));
          return true;
        }
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

bool OrltLtu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ORLT_DC_DA_CONST9: {
          ExprId lt;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          lt = il.BoolToInt(WORD,
                            il.CompareSignedLessEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), lt)));
          return true;
        }
        case ORLTU_DC_DA_CONST9: {
          ExprId lt;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          lt = il.BoolToInt(WORD,
                            il.CompareUnsignedLessEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.ZeroExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), lt)));
          return true;
        }
        default:
          LogDebug("0x%lx: OrltLtu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ORLT_DC_DA_DB: {
          ExprId lt;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          lt = il.BoolToInt(
              WORD, il.CompareSignedLessEqual(WORD, il.Register(WORD, reg_da),
                                              il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), lt)));
          return true;
        }
        case ORLTU_DC_DA_DB: {
          ExprId lt;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          lt = il.BoolToInt(
              WORD, il.CompareUnsignedLessEqual(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), lt)));
          return true;
        }
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

bool Orne::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case ORNE_DC_DA_CONST9: {
          ExprId ne;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          ne = il.BoolToInt(
              WORD,
              il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                 il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), ne)));
          return true;
        }
        default:
          LogDebug("0x%lx: Orne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case ORNE_DC_DA_DB: {
          ExprId ne;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          ne = il.BoolToInt(WORD,
                            il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Or(WORD, il.Register(WORD, reg_dc), ne)));
          return true;
        }
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

bool Ort::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x87_BIT || op2 != ORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Ort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId da, db, result, condition;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.BoolToInt(
        WORD,
        il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
            il.Const(WORD, 0)));
    db = il.BoolToInt(
        WORD,
        il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
            il.Const(WORD, 0)));
    result = il.Or(WORD, da, db);
    il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
    return true;
  }
}

bool Orn::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case ORN_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(
                  WORD, il.Register(WORD, reg_da),
                  il.Not(WORD, il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Orn::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case ORN_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD, il.Register(WORD, reg_da),
                                   il.Not(WORD, il.Register(WORD, reg_db)))));
          return true;
        }
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

bool Ornt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x87_BIT || op2 != ORNT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Ornt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId da, db, result;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.BoolToInt(
        WORD,
        il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
            il.Const(WORD, 0)));
    db = il.BoolToInt(
        WORD,
        il.CompareEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
            il.Const(WORD, 0)));
    result = il.Or(WORD, da, db);
    il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
    return true;
  }
}

bool Pack::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Parity::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

// TCv1.6 ISA only
bool RestoreDa::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ret::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  len = *((const uint16_t*)data) & 0x1 ? 4 : 2;
  // TODO: make sure A11 is even
  il.AddInstruction(il.Return(il.Register(WORD, TRICORE_REG_A11)));
  return true;
}

bool Rfe::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  len = *((const uint16_t*)data) & 0x1 ? 4 : 2;
  il.AddInstruction(il.Unimplemented());  // there is a lot more to this
                                          // instruction than just returning
  il.AddInstruction(il.Return(
      il.Register(WORD, TRICORE_REG_A11)));  // TODO: make sure A11 is even
  return true;
}

bool Rfm::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  len = *((const uint16_t*)data) & 0x1 ? 4 : 2;
  il.AddInstruction(il.Unimplemented());  // there is a lot more to this
                                          // instruction than just returning
  il.AddInstruction(il.Return(
      il.Register(WORD, TRICORE_REG_A11)));  // TODO: make sure A11 is even
  return true;
}

bool Rslcx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Rstv::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Rsub::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case RSUB_DA: {
          len = 2;
          uint8_t op1, reg_da, op2;
          Instruction::ExtractOpfieldsSR(data, op1, reg_da, op2);
          il.AddInstruction(il.SetRegister(
              WORD, reg_da,
              il.Sub(WORD, il.Const(WORD, 0), il.Register(WORD, reg_da))));
          return true;
        }
        default:
          LogDebug("0x%lx: Rsub::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x8B_RC:
      switch (op2) {
        case RSUB_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Sub(WORD, il.SignExtend(WORD, il.Const(HWORD, const9)),
                     il.Register(WORD, reg_da))));
          return true;
        }
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

bool Rsubs::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x8B_RC) {
    LogDebug("0x%lx: Rsubs::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case RSUBS_DC_DA_CONST9: {
      len = 4;
      uint8_t reg_da, reg_dc, op1, op2;
      int16_t const9;
      Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.Sub(WORD, il.SignExtend(WORD, il.Const(HWORD, const9)),
                 il.Register(WORD, reg_da))));
      ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), reg_dc, il,
           WORD);
      return true;
    }
    case RSUBSU_DC_DA_CONST9: {
      // TODO: what about this is unsigned? The manual still says to sign extend
      // const9, and there is no unsigned sub in the IL...
      len = 4;
      uint8_t reg_da, reg_dc, op1, op2;
      int16_t const9;
      Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc, const9);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.Sub(WORD, il.SignExtend(WORD, il.Const(HWORD, const9)),
                 il.Register(WORD, reg_da))));
      suov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), reg_dc, il,
           WORD);
      return true;
    }
    default:
      LogDebug("0x%lx: Rsubs::%s received invalid opcode 2 0x%x", addr,
               __func__, op2);
      return false;
  }
}

bool Satb::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATB_DA: {
          ExprId condition1, condition2;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1, trueLabel2,
              falseLabel2, doneLabel2;
          len = 2;
          uint8_t op1, reg_da, op2;
          Instruction::ExtractOpfieldsSR(data, op1, reg_da, op2);

          condition1 = il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, -0x80));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, -0x80)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          condition2 = il.CompareSignedGreaterThan(
              WORD, il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 0x7F));
          il.AddInstruction(il.If(condition2, trueLabel2, falseLabel2));

          il.MarkLabel(trueLabel2);
          il.AddInstruction(il.SetRegister(WORD, reg_da, il.Const(WORD, 0x7F)));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_da, il.Register(WORD, LLIL_TEMP(0))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(doneLabel2);
          return true;
        }
        default:
          LogDebug("0x%lx: Satb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATB_DC_DA: {
          ExprId condition1, condition2;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1, trueLabel2,
              falseLabel2, doneLabel2;
          len = 4;
          uint8_t op1, op2, reg_da, s2, n, reg_dc;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_dc);

          condition1 = il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, -0x80));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, -0x80)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          condition2 = il.CompareSignedGreaterThan(
              WORD, il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 0x7F));
          il.AddInstruction(il.If(condition2, trueLabel2, falseLabel2));

          il.MarkLabel(trueLabel2);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0x7F)));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(0))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(doneLabel2);
          return true;
        }
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

bool Satbu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATBU_DA: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, doneLabel1;
          len = 2;
          uint8_t op1, reg_da, op2;
          Instruction::ExtractOpfieldsSR(data, op1, reg_da, op2);

          condition1 = il.CompareUnsignedGreaterThan(
              WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFF));
          il.AddInstruction(il.If(condition1, trueLabel1, doneLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(il.SetRegister(WORD, reg_da, il.Const(WORD, 0xFF)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
        default:
          LogDebug("0x%lx: Satbu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATBU_DC_DA: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t op1, op2, reg_da, s2, n, reg_dc;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_dc);

          condition1 = il.CompareUnsignedGreaterThan(
              WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFF));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0xFF)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
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

bool Sath::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATH_DA: {
          ExprId condition1, condition2;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1, trueLabel2,
              falseLabel2, doneLabel2;
          len = 2;
          uint8_t op1, reg_da, op2;
          Instruction::ExtractOpfieldsSR(data, op1, reg_da, op2);

          condition1 = il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, -0x8000));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, -0x8000)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          condition2 = il.CompareSignedGreaterThan(
              WORD, il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 0x7FFF));
          il.AddInstruction(il.If(condition2, trueLabel2, falseLabel2));

          il.MarkLabel(trueLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_da, il.Const(WORD, 0x7FFF)));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_da, il.Register(WORD, LLIL_TEMP(0))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(doneLabel2);
          return true;
        }
        default:
          LogDebug("0x%lx: Sath::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATH_DC_DA: {
          ExprId condition1, condition2;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1, trueLabel2,
              falseLabel2, doneLabel2;
          len = 4;
          uint8_t op1, op2, reg_da, s2, n, reg_dc;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_dc);

          condition1 = il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                                il.Const(WORD, -0x8000));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Const(WORD, -0x8000)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0), il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          condition2 = il.CompareSignedGreaterThan(
              WORD, il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 0x7FFF));
          il.AddInstruction(il.If(condition2, trueLabel2, falseLabel2));

          il.MarkLabel(trueLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Const(WORD, 0x7FFF)));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, LLIL_TEMP(0))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(doneLabel2);
          return true;
        }
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

bool Sathu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x32_SR:
      switch (op2) {
        case SATHU_DA: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, doneLabel1;
          len = 2;
          uint8_t op1, reg_da, op2;
          Instruction::ExtractOpfieldsSR(data, op1, reg_da, op2);

          condition1 = il.CompareUnsignedGreaterThan(
              WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFFFF));
          il.AddInstruction(il.If(condition1, trueLabel1, doneLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_da, il.Const(WORD, 0xFFFF)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
        default:
          LogDebug("0x%lx: Sathu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SATHU_DC_DA: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t op1, op2, reg_da, s2, n, reg_dc;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_dc);

          condition1 = il.CompareUnsignedGreaterThan(
              WORD, il.Register(WORD, reg_da), il.Const(WORD, 0xFFFF));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Const(WORD, 0xFFFF)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
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

bool Sel::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_xAB_RCR:
      switch (op2) {
        case SEL_DC_DD_DA_CONST9: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t op1, op2, reg_da, reg_dd, reg_dc;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_dd, reg_dc);

          condition1 = il.CompareNotEqual(WORD, il.Register(WORD, reg_dd),
                                          il.Const(WORD, 0));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
        default:
          LogDebug("0x%lx: Sel::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x2B_RRR:
      switch (op2) {
        case SEL_DC_DD_DA_DB: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n,
                                          reg_dd, reg_dc);

          condition1 = il.CompareNotEqual(WORD, il.Register(WORD, reg_dd),
                                          il.Const(WORD, 0));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_db)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
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

bool Seln::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_xAB_RCR:
      switch (op2) {
        case SELN_DC_DD_DA_CONST9: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t op1, op2, reg_da, reg_dd, reg_dc;
          int16_t const9;
          Instruction::ExtractOpfieldsRCR(data, op1, op2, reg_da, const9,
                                          reg_dd, reg_dc);

          condition1 = il.CompareEqual(WORD, il.Register(WORD, reg_dd),
                                       il.Const(WORD, 0));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
        default:
          LogDebug("0x%lx: Seln::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x2B_RRR:
      switch (op2) {
        case SELN_DC_DD_DA_DB: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
          Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n,
                                          reg_dd, reg_dc);

          condition1 = il.CompareEqual(WORD, il.Register(WORD, reg_dd),
                                       il.Const(WORD, 0));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_da)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc, il.Register(WORD, reg_db)));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
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

bool Sh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
              size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case SH_DA_CONST4: {
      ExprId condition1, shift;
      LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      shift = il.Const(BYTE, const4);
      condition1 = il.CompareSignedGreaterEqual(WORD, shift, il.Const(BYTE, 0));
      il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da, il.ShiftLeft(WORD, il.Register(WORD, reg_da), shift)));
      il.AddInstruction(il.Goto(doneLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                              il.Neg(BYTE, shift))));
      il.AddInstruction(il.Goto(doneLabel1));

      il.MarkLabel(doneLabel1);
      return true;
    }
    case OP1_x8F_RC:
      switch (op2) {
        case SH_DC_DA_CONST9: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          // The shifting constant is 6 bits and must be properly sign extended
          const9 &= 0x3F;
          if (const9 & 0x20) {
            const9 |= 0xFFC0u;
          }

          condition1 = il.CompareSignedGreaterEqual(
              WORD, il.Const(HWORD, const9), il.Const(HWORD, 0));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.ShiftLeft(WORD, il.Register(WORD, reg_da),
                                          il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                   il.Neg(WORD, il.Const(HWORD, const9)))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
        default:
          LogDebug("0x%lx: Sh::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case SH_DC_DA_DB: {
          ExprId condition1, condition2, shift;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1, trueLabel2,
              falseLabel2, doneLabel2;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          shift = il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x3F));
          condition1 = il.And(WORD, shift, il.Const(WORD, 0x20));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0),
                             il.Or(WORD, shift, il.Const(WORD, 0xFFFFFFC0))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), shift));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          condition2 = il.CompareSignedGreaterEqual(
              WORD, il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 0));
          il.AddInstruction(il.If(condition2, trueLabel2, falseLabel2));

          il.MarkLabel(trueLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.ShiftLeft(WORD, il.Register(WORD, reg_da),
                                          il.Register(WORD, LLIL_TEMP(0)))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.LogicalShiftRight(
                  WORD, il.Register(WORD, reg_da),
                  il.Neg(WORD, il.Register(WORD, LLIL_TEMP(0))))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(doneLabel2);
          return true;
        }
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

bool Sheq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHEQ_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(
                        WORD,
                        il.CompareEqual(
                            WORD, il.Register(WORD, reg_da),
                            il.SignExtend(WORD, il.Const(HWORD, const9)))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Sheq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHEQ_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(
                        WORD, il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                              il.Register(WORD, reg_db))))));
          return true;
        }
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

bool ShgeGeu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHGE_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(
                        WORD,
                        il.CompareSignedGreaterEqual(
                            WORD, il.Register(WORD, reg_da),
                            il.SignExtend(WORD, il.Const(HWORD, const9)))))));
          return true;
        }
        case SHGEU_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(
                        WORD,
                        il.CompareUnsignedGreaterEqual(
                            WORD, il.Register(WORD, reg_da),
                            il.ZeroExtend(WORD, il.Const(HWORD, const9)))))));
          return true;
        }
        default:
          LogDebug("0x%lx: ShgeGeu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHGE_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(WORD, il.CompareSignedGreaterEqual(
                                           WORD, il.Register(WORD, reg_da),
                                           il.Register(WORD, reg_db))))));
          return true;
        }
        case SHGEU_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(WORD, il.CompareUnsignedGreaterEqual(
                                           WORD, il.Register(WORD, reg_da),
                                           il.Register(WORD, reg_db))))));
          return true;
        }
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

bool Shh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool ShltLtu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHLT_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(
                        WORD,
                        il.CompareSignedLessThan(
                            WORD, il.Register(WORD, reg_da),
                            il.SignExtend(WORD, il.Const(HWORD, const9)))))));
          return true;
        }
        case SHLTU_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x1FF;
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(
                        WORD,
                        il.CompareUnsignedLessThan(
                            WORD, il.Register(WORD, reg_da),
                            il.ZeroExtend(WORD, il.Const(HWORD, const9)))))));
          return true;
        }
        default:
          LogDebug("0x%lx: ShltLtu::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHLT_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(WORD, il.CompareSignedLessThan(
                                           WORD, il.Register(WORD, reg_da),
                                           il.Register(WORD, reg_db))))));
          return true;
        }
        case SHLTU_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(WORD, il.CompareUnsignedLessThan(
                                           WORD, il.Register(WORD, reg_da),
                                           il.Register(WORD, reg_db))))));
          return true;
        }
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

bool Shne::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case SHNE_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(
                        WORD,
                        il.CompareNotEqual(
                            WORD, il.Register(WORD, reg_da),
                            il.SignExtend(WORD, il.Const(HWORD, const9)))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Shne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case SHNE_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Or(WORD,
                    il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                 il.Const(WORD, 1)),
                    il.BoolToInt(WORD, il.CompareNotEqual(
                                           WORD, il.Register(WORD, reg_da),
                                           il.Register(WORD, reg_db))))));
          return true;
        }
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

bool ShAndOrNorXorT::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                          uint64_t addr, size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x27_BIT:
      switch (op2) {
        case SHANDT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, bp1)),
                                       il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, bp2)),
                                       il.Const(WORD, 0)));
          result = il.And(WORD, da, db);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
        case SHANDNT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, bp1)),
                                       il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareEqual(WORD,
                                    il.And(WORD, il.Register(WORD, reg_db),
                                           il.Const(WORD, bp2)),
                                    il.Const(WORD, 0)));
          result = il.And(WORD, da, db);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
        case SHNORT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareEqual(WORD,
                                    il.And(WORD, il.Register(WORD, reg_da),
                                           il.Const(WORD, bp1)),
                                    il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareEqual(WORD,
                                    il.And(WORD, il.Register(WORD, reg_db),
                                           il.Const(WORD, bp2)),
                                    il.Const(WORD, 0)));
          result = il.And(WORD, da, db);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
        case SHORT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, bp1)),
                                       il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, bp2)),
                                       il.Const(WORD, 0)));
          result = il.Or(WORD, da, db);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
        default:
          LogDebug("0x%lx: ShAndOrNorXorT::%s received invalid opcode 2 0x%x",
                   addr, __func__, op2);
          return false;
      }
    case OP1_xA7_BIT:
      switch (op2) {
        case SHNANDT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareEqual(WORD,
                                    il.And(WORD, il.Register(WORD, reg_da),
                                           il.Const(WORD, bp1)),
                                    il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareEqual(WORD,
                                    il.And(WORD, il.Register(WORD, reg_db),
                                           il.Const(WORD, bp2)),
                                    il.Const(WORD, 0)));
          result = il.Or(WORD, da, db);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
        case SHORNT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, bp1)),
                                       il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareEqual(WORD,
                                    il.And(WORD, il.Register(WORD, reg_db),
                                           il.Const(WORD, bp2)),
                                    il.Const(WORD, 0)));
          result = il.Or(WORD, da, db);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
        case SHXNORT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, bp1)),
                                       il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, bp2)),
                                       il.Const(WORD, 0)));
          result = il.Xor(WORD, da, db);
          result = il.BoolToInt(
              WORD, il.CompareEqual(WORD, result,
                                    il.Const(WORD, 0)));  // essentially a
                                                          // logical not
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
        case SHXORT_DC_DA_POS1_DB_POS2: {
          ExprId da, db, result;
          len = 4;
          uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
          Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1,
                                          pos2, reg_dc);
          uint32_t bp1, bp2;
          bp1 = 1 << pos1;
          bp2 = 1 << pos2;
          da = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_da),
                                              il.Const(WORD, bp1)),
                                       il.Const(WORD, 0)));
          db = il.BoolToInt(
              WORD, il.CompareNotEqual(WORD,
                                       il.And(WORD, il.Register(WORD, reg_db),
                                              il.Const(WORD, bp2)),
                                       il.Const(WORD, 0)));
          result = il.Xor(WORD, da, db);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Or(WORD,
                                   il.ShiftLeft(WORD, il.Register(WORD, reg_dc),
                                                il.Const(WORD, 1)),
                                   result)));
          return true;
        }
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

bool Sha::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  // TODO: handle carry_out
  switch (op1) {
    case SHA_DA_CONST4: {
      ExprId condition1, shift;
      LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
      len = 2;
      uint8_t op1, reg_da;
      int8_t const4;
      Instruction::ExtractOpfieldsSRC(data, op1, reg_da, const4);
      shift = il.Const(BYTE, const4);
      condition1 = il.CompareSignedGreaterEqual(WORD, shift, il.Const(BYTE, 0));
      il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

      il.MarkLabel(trueLabel1);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da, il.ShiftLeft(WORD, il.Register(WORD, reg_da), shift)));
      il.AddInstruction(il.Goto(doneLabel1));

      il.MarkLabel(falseLabel1);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.ArithShiftRight(WORD, il.Register(WORD, reg_da),
                                            il.Neg(BYTE, shift))));
      il.AddInstruction(il.Goto(doneLabel1));

      il.MarkLabel(doneLabel1);
      return true;
    }
    case OP1_x8F_RC:
      switch (op2) {
        case SHA_DC_DA_CONST9: {
          ExprId condition1;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          // The shifting constant is 6 bits and must be properly sign extended
          const9 &= 0x3F;
          if (const9 & 0x20) {
            const9 |= 0xFFC0u;
          }

          condition1 = il.CompareSignedGreaterEqual(
              WORD, il.Const(HWORD, const9), il.Const(HWORD, 0));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.ShiftLeft(WORD, il.Register(WORD, reg_da),
                                          il.Const(HWORD, const9))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.ArithShiftRight(WORD, il.Register(WORD, reg_da),
                                 il.Neg(WORD, il.Const(HWORD, const9)))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          return true;
        }
        default:
          LogDebug("0x%lx: Sha::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case SHA_DC_DA_DB: {
          ExprId condition1, condition2, shift;
          LowLevelILLabel trueLabel1, falseLabel1, doneLabel1, trueLabel2,
              falseLabel2, doneLabel2;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          shift = il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x3F));
          condition1 = il.And(WORD, shift, il.Const(WORD, 0x20));
          il.AddInstruction(il.If(condition1, trueLabel1, falseLabel1));

          il.MarkLabel(trueLabel1);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0),
                             il.Or(WORD, shift, il.Const(WORD, 0xFFFFFFC0))));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(falseLabel1);
          il.AddInstruction(il.SetRegister(WORD, LLIL_TEMP(0), shift));
          il.AddInstruction(il.Goto(doneLabel1));

          il.MarkLabel(doneLabel1);
          condition2 = il.CompareSignedGreaterEqual(
              WORD, il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 0));
          il.AddInstruction(il.If(condition2, trueLabel2, falseLabel2));

          il.MarkLabel(trueLabel2);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.ShiftLeft(WORD, il.Register(WORD, reg_da),
                                          il.Register(WORD, LLIL_TEMP(0)))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(falseLabel2);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.ArithShiftRight(
                  WORD, il.Register(WORD, reg_da),
                  il.Neg(WORD, il.Register(WORD, LLIL_TEMP(0))))));
          il.AddInstruction(il.Goto(doneLabel2));

          il.MarkLabel(doneLabel2);
          return true;
        }
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

bool Shah::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Shas::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Sta::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case STA_AB_AA: {
      len = 2;
      uint8_t op1, reg_aa, reg_ab;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_ab);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(WORD, il.Register(WORD, reg_ab), il.Register(WORD, reg_aa)));
      return true;
    }
    case STA_AB_AA_POSTINC: {
      len = 2;
      uint8_t op1, reg_aa, reg_ab;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_aa, reg_ab);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(WORD, il.Register(WORD, reg_ab), il.Register(WORD, reg_aa)));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 4))));
      return true;
    }
    case STA_A10_CONST8_A15: {
      len = 2;
      uint8_t op1;
      int8_t const8;
      uint32_t mem;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      mem = (uint32_t)const8 * 4;
      il.AddInstruction(il.Store(
          WORD,
          il.Add(WORD, il.Register(WORD, TRICORE_REG_A10), il.Const(WORD, mem)),
          il.Register(WORD, TRICORE_REG_A15)));
      return true;
    }
    case STA_AB_OFF4_A15: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      REGTOA(reg_ab)
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      il.AddInstruction(il.Store(
          WORD, il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, mem)),
          il.Register(WORD, TRICORE_REG_A15)));
      return true;
    }
    case STA_A15_OFF4_AA: {
      len = 2;
      uint8_t op1, reg_aa;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSSRO(data, op1, reg_aa, off4);
      REGTOA(reg_aa)
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      il.AddInstruction(il.Store(
          WORD,
          il.Add(WORD, il.Register(WORD, TRICORE_REG_A15), il.Const(WORD, mem)),
          il.Register(WORD, reg_aa)));
      return true;
    }
    case STA_AB_OFF16_AA:  // TCv1.6 ISA only
    {
      len = 4;
      uint8_t op1, reg_aa, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_aa, reg_ab, off16);
      REGTOA(reg_aa)
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(WORD,
                   il.Add(WORD, il.Register(WORD, reg_ab),
                          il.SignExtend(WORD, il.Const(HWORD, off16))),
                   il.Register(WORD, reg_aa)));
      return true;
    }
    case OP1_xA5_ABS:
      switch (op2) {
        case STA_OFF18_AA: {
          len = 4;
          uint8_t op1, op2, reg_aa;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_aa, ea);
          REGTOA(reg_aa)
          il.AddInstruction(il.Store(WORD, il.ConstPointer(WORD, ea),
                                     il.Register(WORD, reg_aa)));
          return true;
        }
        default:
          LogDebug("0x%lx: Sta::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STA_AB_OFF10_AA: {
          len = 4;
          uint8_t op1, op2, reg_aa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
          REGTOA(reg_aa)
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(WORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(WORD, reg_aa)));
          return true;
        }
        case STA_AB_OFF10_AA_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_aa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
          REGTOA(reg_aa)
          REGTOA(reg_ab)
          il.AddInstruction(il.Store(WORD, il.Register(WORD, reg_ab),
                                     il.Register(WORD, reg_aa)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case STA_AB_OFF10_AA_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_aa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_aa, reg_ab, off10);
          REGTOA(reg_aa)
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(WORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(WORD, reg_aa)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Sta::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STA_PB_AA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case STA_PB_OFF10_AA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Stb::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case STB_AB_DA: {
      len = 2;
      uint8_t op1, reg_da, reg_ab;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(BYTE, il.Register(WORD, reg_ab), il.Register(BYTE, reg_da)));
      return true;
    }
    case STB_AB_DA_POSTINC: {
      len = 2;
      uint8_t op1, reg_da, reg_ab;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(BYTE, il.Register(WORD, reg_ab), il.Register(BYTE, reg_da)));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 1))));
      return true;
    }
    case STB_AB_OFF4_D15: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      REGTOA(reg_ab)
      mem = (off4 & 0xF);  // clear sign extended bits
      il.AddInstruction(il.Store(
          BYTE, il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, mem)),
          il.Register(BYTE, TRICORE_REG_D15)));
      return true;
    }
    case STB_A15_OFF4_DA: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSSRO(data, op1, reg_da, off4);
      mem = (off4 & 0xF);  // clear sign extended bits
      il.AddInstruction(il.Store(
          BYTE,
          il.Add(WORD, il.Register(WORD, TRICORE_REG_A15), il.Const(WORD, mem)),
          il.Register(BYTE, reg_da)));
      return true;
    }
    case STB_AB_OFF16_DA:  // TCv1.6 ISA only
    {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(BYTE,
                   il.Add(WORD, il.Register(WORD, reg_ab),
                          il.SignExtend(WORD, il.Const(HWORD, off16))),
                   il.Register(BYTE, reg_da)));
      return true;
    }
    case OP1_x25_ABS:
      switch (op2) {
        case STB_OFF18_DA: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.Store(BYTE, il.ConstPointer(WORD, ea),
                                     il.Register(BYTE, reg_da)));
          return true;
        }
        default:
          LogDebug("0x%lx: Stb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STB_AB_OFF10_DA: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(BYTE,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(BYTE, reg_da)));
          return true;
        }
        case STB_AB_OFF10_DA_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.Store(BYTE, il.Register(WORD, reg_ab),
                                     il.Register(BYTE, reg_da)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case STB_AB_OFF10_DA_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(BYTE,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(BYTE, reg_da)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Stb::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STB_PB_DA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case STB_PB_OFF10_DA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Std::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_xA5_ABS:
      switch (op2) {
        case STD_OFF18_EA: {
          len = 4;
          uint8_t op1, op2, reg_ea;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_ea, ea);
          REGTOE(reg_ea)
          il.AddInstruction(il.Store(DWORD, il.ConstPointer(WORD, ea),
                                     il.Register(DWORD, reg_ea)));
          return true;
        }
        default:
          LogDebug("0x%lx: Std::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STD_AB_OFF10_EA: {
          len = 4;
          uint8_t op1, op2, reg_ea, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
          REGTOE(reg_ea)
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(DWORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(DWORD, reg_ea)));
          return true;
        }
        case STD_AB_OFF10_EA_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_ea, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
          REGTOE(reg_ea)
          REGTOA(reg_ab)
          il.AddInstruction(il.Store(DWORD, il.Register(WORD, reg_ab),
                                     il.Register(DWORD, reg_ea)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case STD_AB_OFF10_EA_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_ea, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_ea, reg_ab, off10);
          REGTOE(reg_ea)
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(DWORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(DWORD, reg_ea)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Std::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STD_PB_EA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case STD_PB_OFF10_EA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Stda::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_xA5_ABS:
      switch (op2) {
        case STDA_OFF18_PA: {
          len = 4;
          uint8_t op1, op2, reg_pa;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_pa, ea);
          REGTOP(reg_pa)
          il.AddInstruction(il.Store(DWORD, il.ConstPointer(WORD, ea),
                                     il.Register(DWORD, reg_pa)));
          return true;
        }
        default:
          LogDebug("0x%lx: Stda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STDA_AB_OFF10_PA: {
          len = 4;
          uint8_t op1, op2, reg_pa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
          REGTOP(reg_pa)
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(DWORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(DWORD, reg_pa)));
          return true;
        }
        case STDA_AB_OFF10_PA_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_pa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
          REGTOP(reg_pa)
          REGTOA(reg_ab)
          il.AddInstruction(il.Store(DWORD, il.Register(WORD, reg_ab),
                                     il.Register(DWORD, reg_pa)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case STDA_AB_OFF10_PA_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_pa, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_pa, reg_ab, off10);
          REGTOP(reg_pa)
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(DWORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(DWORD, reg_pa)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Stda::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STDA_PB_PA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case STDA_PB_OFF10_PA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Sth::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case STH_AB_DA: {
      len = 2;
      uint8_t op1, reg_da, reg_ab;
      Instruction::ExtractOpfieldsSSR(data, op1, reg_da, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.Store(HWORD, il.Register(WORD, reg_ab),
                                 il.Register(HWORD, reg_da)));
      return true;
    }
    case STH_AB_DA_POSTINC: {
      len = 2;
      uint8_t op1, reg_da, reg_ab;
      Instruction::ExtractOpfieldsSSR(data, op1, reg_da, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(il.Store(HWORD, il.Register(WORD, reg_ab),
                                 il.Register(HWORD, reg_da)));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 2))));
      return true;
    }
    case STH_AB_OFF4_D15: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      REGTOA(reg_ab)
      mem = (off4 & 0xF) * 2;  // clear sign extended bits and multiply
      il.AddInstruction(il.Store(
          HWORD, il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, mem)),
          il.Register(HWORD, TRICORE_REG_D15)));
      return true;
    }
    case STH_A15_OFF4_DA: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSSRO(data, op1, reg_da, off4);
      mem = (off4 & 0xF) * 2;  // clear sign extended bits and multiply
      il.AddInstruction(il.Store(
          HWORD,
          il.Add(WORD, il.Register(WORD, TRICORE_REG_A15), il.Const(WORD, mem)),
          il.Register(HWORD, reg_da)));
      return true;
    }
    case STH_AB_OFF16_DA:  // TCv1.6 ISA only
    {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(HWORD,
                   il.Add(WORD, il.Register(WORD, reg_ab),
                          il.SignExtend(WORD, il.Const(HWORD, off16))),
                   il.Register(HWORD, reg_da)));
      return true;
    }
    case OP1_x25_ABS:
      switch (op2) {
        case STH_OFF18_DA: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.Store(HWORD, il.ConstPointer(WORD, ea),
                                     il.Register(HWORD, reg_da)));
          return true;
        }
        default:
          LogDebug("0x%lx: Sth::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STH_AB_OFF10_DA: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(HWORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(HWORD, reg_da)));
          return true;
        }
        case STH_AB_OFF10_DA_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.Store(HWORD, il.Register(WORD, reg_ab),
                                     il.Register(HWORD, reg_da)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case STH_AB_OFF10_DA_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(HWORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(HWORD, reg_da)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Sth::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STH_PB_DA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case STH_PB_OFF10_DA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Stq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x65_ABS:
      switch (op2) {
        case STQ_OFF18_DA: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(
              il.Store(HWORD, il.ConstPointer(WORD, ea),
                       il.LogicalShiftRight(HWORD, il.Register(WORD, reg_da),
                                            il.Const(WORD, 16))));
          return true;
        }
        default:
          LogDebug("0x%lx: Stq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STQ_AB_OFF10_DA: {
          ExprId target;
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          target = il.Add(WORD, il.Register(WORD, reg_ab),
                          il.SignExtend(WORD, il.Const(HWORD, off10)));
          il.AddInstruction(
              il.Store(HWORD, target,
                       il.LogicalShiftRight(HWORD, il.Register(WORD, reg_da),
                                            il.Const(WORD, 16))));
          return true;
        }
        case STQ_AB_OFF10_DA_POSTINC: {
          ExprId target;
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          target = il.Add(WORD, il.Register(WORD, reg_ab),
                          il.SignExtend(WORD, il.Const(HWORD, off10)));
          il.AddInstruction(
              il.Store(HWORD, il.Register(WORD, reg_ab),
                       il.LogicalShiftRight(HWORD, il.Register(WORD, reg_da),
                                            il.Const(WORD, 16))));
          il.AddInstruction(il.SetRegister(WORD, reg_ab, target));
          return true;
        }
        case STQ_AB_OFF10_DA_PREINC: {
          ExprId target;
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          target = il.Add(WORD, il.Register(WORD, reg_ab),
                          il.SignExtend(WORD, il.Const(HWORD, off10)));
          il.AddInstruction(
              il.Store(HWORD, target,
                       il.LogicalShiftRight(HWORD, il.Register(WORD, reg_da),
                                            il.Const(WORD, 16))));
          il.AddInstruction(il.SetRegister(WORD, reg_ab, target));
          return true;
        }
        default:
          LogDebug("0x%lx: Stq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STQ_PB_DA:
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case STQ_PB_OFF10_DA:
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Stt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_xD5_ABSB || op2 != STT_OFF18_BPOS3_B) {
    LogDebug("0x%lx: Stt::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, b, bpos3;
    uint32_t ea;
    Instruction::ExtractOpfieldsABSB(data, op1, op2, b, bpos3, ea);
    il.AddInstruction(il.SetRegister(BYTE, LLIL_TEMP(0),
                                     il.Load(BYTE, il.ConstPointer(WORD, ea))));
    il.AddInstruction(il.Store(
        BYTE, il.ConstPointer(WORD, ea),
        il.Or(BYTE,
              il.And(BYTE, il.Register(BYTE, LLIL_TEMP(0)),
                     il.Not(BYTE, il.ShiftLeft(BYTE, il.Const(BYTE, 1),
                                               il.Const(BYTE, bpos3)))),
              il.ShiftLeft(BYTE, il.Const(BYTE, b), il.Const(BYTE, bpos3)))));
    return true;
  }
}

bool Stw::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case STW_AB_DA: {
      len = 2;
      uint8_t op1, reg_da, reg_ab;
      Instruction::ExtractOpfieldsSSR(data, op1, reg_da, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(WORD, il.Register(WORD, reg_ab), il.Register(WORD, reg_da)));
      return true;
    }
    case STW_AB_DA_POSTINC: {
      len = 2;
      uint8_t op1, reg_da, reg_ab;
      Instruction::ExtractOpfieldsSSR(data, op1, reg_da, reg_ab);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(WORD, il.Register(WORD, reg_ab), il.Register(WORD, reg_da)));
      il.AddInstruction(il.SetRegister(
          WORD, reg_ab,
          il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, 4))));
      return true;
    }
    case STW_A10_CONST8_D15: {
      len = 2;
      uint8_t op1;
      int8_t const8;
      uint32_t mem;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      mem = (uint32_t)const8 * 4;
      il.AddInstruction(il.Store(
          WORD,
          il.Add(WORD, il.Register(WORD, TRICORE_REG_A10), il.Const(WORD, mem)),
          il.Register(WORD, TRICORE_REG_D15)));
      return true;
    }
    case STW_AB_OFF4_D15: {
      len = 2;
      uint8_t op1, reg_ab;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSRO(data, op1, off4, reg_ab);
      REGTOA(reg_ab)
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      il.AddInstruction(il.Store(
          WORD, il.Add(WORD, il.Register(WORD, reg_ab), il.Const(WORD, mem)),
          il.Register(WORD, TRICORE_REG_D15)));
      return true;
    }
    case STW_A15_OFF4_DA: {
      len = 2;
      uint8_t op1, reg_da;
      int8_t off4;
      uint32_t mem;
      Instruction::ExtractOpfieldsSSRO(data, op1, reg_da, off4);
      mem = (off4 & 0xF) * 4;  // clear sign extended bits and multiply
      il.AddInstruction(il.Store(
          WORD,
          il.Add(WORD, il.Register(WORD, TRICORE_REG_A15), il.Const(WORD, mem)),
          il.Register(WORD, reg_da)));
      return true;
    }
    case STW_AB_OFF16_DA: {
      len = 4;
      uint8_t op1, reg_da, reg_ab;
      int16_t off16;
      Instruction::ExtractOpfieldsBOL(data, op1, reg_da, reg_ab, off16);
      REGTOA(reg_ab)
      il.AddInstruction(
          il.Store(WORD,
                   il.Add(WORD, il.Register(WORD, reg_ab),
                          il.SignExtend(WORD, il.Const(HWORD, off16))),
                   il.Register(WORD, reg_da)));
      return true;
    }
    case OP1_xA5_ABS:
      switch (op2) {
        case STW_OFF18_DA: {
          len = 4;
          uint8_t op1, op2, reg_da;
          uint32_t ea;
          Instruction::ExtractOpfieldsABS(data, op1, op2, reg_da, ea);
          il.AddInstruction(il.Store(WORD, il.ConstPointer(WORD, ea),
                                     il.Register(WORD, reg_da)));
          return true;
        }
        default:
          LogDebug("0x%lx: Stw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x89_BO:
      switch (op2) {
        case STW_AB_OFF10_DA: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(WORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(WORD, reg_da)));
          return true;
        }
        case STW_AB_OFF10_DA_POSTINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(il.Store(WORD, il.Register(WORD, reg_ab),
                                     il.Register(WORD, reg_da)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        case STW_AB_OFF10_DA_PREINC: {
          len = 4;
          uint8_t op1, op2, reg_da, reg_ab;
          int16_t off10;
          Instruction::ExtractOpfieldsBO(data, op1, op2, reg_da, reg_ab, off10);
          REGTOA(reg_ab)
          il.AddInstruction(
              il.Store(WORD,
                       il.Add(WORD, il.Register(WORD, reg_ab),
                              il.SignExtend(WORD, il.Const(HWORD, off10))),
                       il.Register(WORD, reg_da)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_ab,
              il.Add(WORD, il.Register(WORD, reg_ab),
                     il.SignExtend(WORD, il.Const(HWORD, off10)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Stw::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_xA9_BO:
      switch (op2) {
        case STW_PB_DA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
        case STW_PB_OFF10_DA:
          len = 4;
          UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
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

bool Stlcx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Stucx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Sub::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case SUB_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.Sub(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
      return true;
    }
    case SUB_DA_D15_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(
          il.SetRegister(WORD, reg_da,
                         il.Sub(WORD, il.Register(WORD, TRICORE_REG_D15),
                                il.Register(WORD, reg_db))));
      return true;
    }
    case SUB_D15_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, TRICORE_REG_D15,
          il.Sub(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
      return true;
    }
    case OP1_x0B_RR:
      switch (op2) {
        case SUB_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Sub(WORD, il.Register(WORD, reg_da),
                                    il.Register(WORD, reg_db))));
          return true;
        }
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

bool Suba::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case SUBA_A10_CONST8: {
      len = 2;
      uint8_t op1;
      int8_t const8;
      Instruction::ExtractOpfieldsSC(data, op1, const8);
      il.AddInstruction(
          il.SetRegister(WORD, TRICORE_REG_A10,
                         il.Sub(WORD, il.Register(WORD, TRICORE_REG_A10),
                                il.ZeroExtend(WORD, il.Const(BYTE, const8)))));
      return true;
    }
    case OP1_x01_RR:
      switch (op2) {
        case SUBA_AC_AA_AB: {
          len = 4;
          uint8_t reg_ac, reg_aa, reg_ab, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_aa, reg_ab, n,
                                         reg_ac);
          REGTOA(reg_aa)
          REGTOA(reg_ab)
          REGTOA(reg_ac)
          il.AddInstruction(
              il.SetRegister(WORD, reg_ac,
                             il.Sub(WORD, il.Register(WORD, reg_aa),
                                    il.Register(WORD, reg_ab))));
          return true;
        }
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

bool Subbh::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: Subbh::%s received invalid opcode 1 0x%x", addr, __func__,
             op1);
    return false;
  }
  switch (op2) {
    case SUBB_DC_DA_DB: {
      ExprId result_byte0, result_byte1, result_byte2, result_byte3, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_byte0 = il.Sub(
          BYTE,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x000000FF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x000000FF)));
      result_byte1 =
          il.Sub(BYTE,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 8)),
                        il.Const(WORD, 0x000000FF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 8)),
                        il.Const(WORD, 0x000000FF)));
      result_byte2 =
          il.Sub(BYTE,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x000000FF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x000000FF)));
      result_byte3 =
          il.Sub(BYTE,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 24)),
                        il.Const(WORD, 0x000000FF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 24)),
                        il.Const(WORD, 0x000000FF)));
      result = il.Or(
          WORD,
          il.Or(WORD, il.ShiftLeft(WORD, result_byte3, il.Const(WORD, 24)),
                il.ShiftLeft(WORD, result_byte2, il.Const(WORD, 16))),
          il.Or(WORD, il.ShiftLeft(WORD, result_byte1, il.Const(WORD, 8)),
                result_byte0));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    case SUBH_DC_DA_DB: {
      ExprId result_hword0, result_hword1, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_hword0 = il.Sub(
          HWORD,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x0000FFFF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x0000FFFF)));
      result_hword1 =
          il.Sub(HWORD,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)));
      result =
          il.Or(WORD, il.ShiftLeft(WORD, result_hword1, il.Const(WORD, 16)),
                result_hword0);
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    default:
      LogDebug("0x%lx: Subh::%s received invalid opcode 2 0x%x", addr, __func__,
               op2);
      return false;
  }
}

bool Subc::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Subsu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  // NOTE/TODO: this instruction calls for an unsigned Sub... not sure how to do
  // that in the binja IL
  switch (op1) {
    case SUBS_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, LLIL_TEMP(0),
          il.Sub(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
      ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), reg_da, il,
           WORD);
      return true;
    }
    case OP1_x0B_RR:
      switch (op2) {
        case SUBS_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0),
                             il.Sub(WORD, il.Register(WORD, reg_da),
                                    il.Register(WORD, reg_db))));
          ssov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), reg_dc, il,
               WORD);
          return true;
        }
        case SUBSU_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(
              il.SetRegister(WORD, LLIL_TEMP(0),
                             il.Sub(WORD, il.Register(WORD, reg_da),
                                    il.Register(WORD, reg_db))));
          suov(il.Register(WORD, LLIL_TEMP(0)), il.Const(WORD, 32), reg_dc, il,
               WORD);
          return true;
        }
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

bool SubshHu::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR) {
    LogDebug("0x%lx: SubshHu::%s received invalid opcode 1 0x%x", addr,
             __func__, op1);
    return false;
  }
  switch (op2) {
    case SUBSH_DC_DA_DB: {
      ExprId result_hword0, result_hword1, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_hword0 = il.Sub(
          HWORD,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x0000FFFF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x0000FFFF)));
      result_hword1 =
          il.Sub(HWORD,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)));
      ssov(result_hword0, il.Const(HWORD, 16), LLIL_TEMP(0), il, HWORD);
      ssov(result_hword1, il.Const(HWORD, 16), LLIL_TEMP(1), il, HWORD);
      result = il.Or(WORD,
                     il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                  il.Const(WORD, 16)),
                     il.Register(WORD, LLIL_TEMP(0)));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    case SUBSHU_DC_DA_DB: {
      ExprId result_hword0, result_hword1, result;
      len = 4;
      uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
      Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
      result_hword0 = il.Sub(
          HWORD,
          il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, 0x0000FFFF)),
          il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, 0x0000FFFF)));
      result_hword1 =
          il.Sub(HWORD,
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_da),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)),
                 il.And(WORD,
                        il.LogicalShiftRight(WORD, il.Register(WORD, reg_db),
                                             il.Const(WORD, 16)),
                        il.Const(WORD, 0x0000FFFF)));
      suov(result_hword0, il.Const(HWORD, 16), LLIL_TEMP(0), il, HWORD);
      suov(result_hword1, il.Const(HWORD, 16), LLIL_TEMP(1), il, HWORD);
      result = il.Or(WORD,
                     il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(1)),
                                  il.Const(WORD, 16)),
                     il.Register(WORD, LLIL_TEMP(0)));
      il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
      return true;
    }
    default:
      LogDebug("0x%lx: SubshHu::%s received invalid opcode 1 0x%x", addr,
               __func__, op1);
      return false;
  }
}

bool Subx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x0B_RR || op2 != SUBX_DC_DA_DB) {
    LogDebug("0x%lx: Subx::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Sub(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
    return true;
    // TODO: handle carry
  }
}

bool Svlcx::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Swapw::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Syscall::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  len = *((const uint16_t*)data) & 0x1 ? 4 : 2;
  il.AddInstruction(il.SystemCall());
  return true;
}

bool Trapsv::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Trapv::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Unpack::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Xnor::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8F_RC:
      switch (op2) {
        case XNOR_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x01FF;  // clear sign-extended bits
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Not(WORD,
                     il.Xor(WORD, il.Register(WORD, reg_da),
                            il.ZeroExtend(WORD, il.Const(HWORD, const9))))));
          return true;
        }
        default:
          LogDebug("0x%lx: Xnor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case XNOR_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Not(WORD, il.Xor(WORD, il.Register(WORD, reg_da),
                                  il.Register(WORD, reg_db)))));
          return true;
        }
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

bool Xnort::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x07_BIT || op2 != XNORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Xnort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId da, db, result;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.BoolToInt(
        WORD,
        il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
            il.Const(WORD, 0)));
    db = il.BoolToInt(
        WORD,
        il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
            il.Const(WORD, 0)));
    result = il.Xor(WORD, da, db);
    result =
        il.BoolToInt(WORD, il.CompareEqual(WORD, result, il.Const(WORD, 0)));
    il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
    return true;
  }
}

bool Xor::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
               size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case XOR_DA_DB: {
      len = 2;
      uint8_t op1, reg_da, reg_db;
      Instruction::ExtractOpfieldsSRR(data, op1, reg_da, reg_db);
      il.AddInstruction(il.SetRegister(
          WORD, reg_da,
          il.Xor(WORD, il.Register(WORD, reg_da), il.Register(WORD, reg_db))));
      return true;
    }
    case OP1_x8F_RC:
      switch (op2) {
        case XOR_DC_DA_CONST9: {
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          const9 &= 0x01FF;  // clear sign-extended bits
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc,
              il.Xor(WORD, il.Register(WORD, reg_da),
                     il.ZeroExtend(WORD, il.Const(HWORD, const9)))));
          return true;
        }
        default:
          LogDebug("0x%lx: Xor::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0F_RR:
      switch (op2) {
        case XOR_DC_DA_DB: {
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          il.AddInstruction(
              il.SetRegister(WORD, reg_dc,
                             il.Xor(WORD, il.Register(WORD, reg_da),
                                    il.Register(WORD, reg_db))));
          return true;
        }
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

bool Xoreq::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XOREQ_DC_DA_CONST9: {
          ExprId eq;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          eq = il.BoolToInt(
              WORD,
              il.CompareEqual(WORD, il.Register(WORD, reg_da),
                              il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        default:
          LogDebug("0x%lx: Xoreq::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XOREQ_DC_DA_DB: {
          ExprId eq;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          eq = il.BoolToInt(WORD,
                            il.CompareEqual(WORD, il.Register(WORD, reg_da),
                                            il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
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

bool Xorge::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XORGE_DC_DA_CONST9: {
          ExprId eq;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          eq = il.BoolToInt(WORD,
                            il.CompareSignedGreaterEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        case XORGEU_DC_DA_CONST9: {
          ExprId eq;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          eq = il.BoolToInt(WORD,
                            il.CompareUnsignedGreaterEqual(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        default:
          LogDebug("0x%lx: Xorge::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XORGE_DC_DA_DB: {
          ExprId eq;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          eq = il.BoolToInt(WORD, il.CompareSignedGreaterEqual(
                                      WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        case XORGEU_DC_DA_DB: {
          ExprId eq;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          eq = il.BoolToInt(WORD, il.CompareUnsignedGreaterEqual(
                                      WORD, il.Register(WORD, reg_da),
                                      il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
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

bool Xorlt::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XORLT_DC_DA_CONST9: {
          ExprId eq;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          eq = il.BoolToInt(WORD,
                            il.CompareSignedLessThan(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        case XORLTU_DC_DA_CONST9: {
          ExprId eq;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          eq = il.BoolToInt(WORD,
                            il.CompareUnsignedLessThan(
                                WORD, il.Register(WORD, reg_da),
                                il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        default:
          LogDebug("0x%lx: Xorlt::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XORLT_DC_DA_DB: {
          ExprId eq;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          eq = il.BoolToInt(
              WORD, il.CompareSignedLessThan(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        case XORLTU_DC_DA_DB: {
          ExprId eq;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          eq = il.BoolToInt(
              WORD, il.CompareUnsignedLessThan(WORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
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

bool Xorne::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  switch (op1) {
    case OP1_x8B_RC:
      switch (op2) {
        case XORNE_DC_DA_CONST9: {
          ExprId eq;
          len = 4;
          uint8_t reg_da, reg_dc, op1, op2;
          int16_t const9;
          Instruction::ExtractOpfieldsRC(data, op1, op2, reg_da, reg_dc,
                                         const9);
          eq = il.BoolToInt(
              WORD,
              il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                 il.SignExtend(WORD, il.Const(HWORD, const9))));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
        default:
          LogDebug("0x%lx: Xorne::%s received invalid opcode 2 0x%x", addr,
                   __func__, op2);
          return false;
      }
    case OP1_x0B_RR:
      switch (op2) {
        case XORNE_DC_DA_DB: {
          ExprId eq;
          len = 4;
          uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
          Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n,
                                         reg_dc);
          eq = il.BoolToInt(WORD,
                            il.CompareNotEqual(WORD, il.Register(WORD, reg_da),
                                               il.Register(WORD, reg_db)));
          il.AddInstruction(il.SetRegister(
              WORD, reg_dc, il.Xor(WORD, il.Register(WORD, reg_dc), eq)));
          return true;
        }
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

bool Xort::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x07_BIT || op2 != XORT_DC_DA_POS1_DB_POS2) {
    LogDebug("0x%lx: Xort::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    ExprId da, db, result;
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, pos1, pos2, reg_dc;
    Instruction::ExtractOpfieldsBIT(data, op1, op2, reg_da, reg_db, pos1, pos2,
                                    reg_dc);
    uint32_t bp1, bp2;
    bp1 = 1 << pos1;
    bp2 = 1 << pos2;
    da = il.BoolToInt(
        WORD,
        il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_da), il.Const(WORD, bp1)),
            il.Const(WORD, 0)));
    db = il.BoolToInt(
        WORD,
        il.CompareNotEqual(
            WORD, il.And(WORD, il.Register(WORD, reg_db), il.Const(WORD, bp2)),
            il.Const(WORD, 0)));
    result = il.Xor(WORD, da, db);
    il.AddInstruction(il.SetRegister(WORD, reg_dc, result));
    return true;
  }
}

/* Floating Point Instructions */
// TODO: Add error-checking cases to float operations

bool Addf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x6B_RRR || op2 != ADDF_DC_DD_DA) {
    LogDebug("0x%lx: Addf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_da, s2, n, reg_dd, reg_dc;
    Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, s2, n, reg_dd,
                                    reg_dc);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc,
                       il.FloatAdd(WORD, il.Register(WORD, reg_dd),
                                   il.Register(WORD, reg_da))));
    return true;
  }
}

bool Cmpf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  // bit [0] D[a] < D[b]
  // bit [1] D[a] == D[b]
  // bit [2] D[a] > D[b]
  // bit [3] Unordered (is_nan(D[a]) || is_nan(D[b]))
  // bit [4] D[a] is denormal
  // bit [5] D[b] is denormal
  // bits[31:06] are cleared

  if (op1 != OP1_x4B_RR || op2 != CMPF_DC_DA_DB) {
    LogDebug("0x%lx: Cmpf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    uint8_t lt = 0, eq = 1, gt = 2, nan = 3, denorma = 4, denormb = 5;
    len = 4;
    uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);

    // D[a] < D[b]
    il.AddInstruction(il.SetRegister(
        WORD, LLIL_TEMP(lt),
        il.BoolToInt(WORD,
                     il.FloatCompareLessThan(WORD, il.Register(WORD, reg_da),
                                             il.Register(WORD, reg_db)))));

    // D[a] == D[b]
    il.AddInstruction(il.SetRegister(
        WORD, LLIL_TEMP(eq),
        il.BoolToInt(WORD, il.FloatCompareEqual(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db)))));

    // D[a] > D[b]
    il.AddInstruction(il.SetRegister(
        WORD, LLIL_TEMP(gt),
        il.BoolToInt(WORD,
                     il.FloatCompareGreaterThan(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db)))));

    // D[a] || D[b] is NaN
    float_is_nan(il.Register(WORD, reg_da), 6, il);
    float_is_nan(il.Register(WORD, reg_db), 7, il);
    il.AddInstruction(
        il.SetRegister(WORD, LLIL_TEMP(nan),
                       il.Or(WORD, il.Register(WORD, LLIL_TEMP(6)),
                             il.Register(WORD, LLIL_TEMP(7)))));

    // D[a] is denormal
    float_is_denormal(il.Register(WORD, reg_da), LLIL_TEMP(denorma), il);

    // D[b] is denormal
    float_is_denormal(il.Register(WORD, reg_db), LLIL_TEMP(denormb), il);

    il.AddInstruction(il.SetRegister(WORD, reg_dc, il.Const(WORD, 0)));
    il.AddInstruction(il.SetRegister(WORD, reg_dc,
                                     il.Or(WORD, il.Register(WORD, reg_dc),
                                           il.Register(WORD, LLIL_TEMP(lt)))));
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Or(WORD, il.Register(WORD, reg_dc),
              il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(eq)),
                           il.Const(WORD, 1)))));
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Or(WORD, il.Register(WORD, reg_dc),
              il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(gt)),
                           il.Const(WORD, 2)))));
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Or(WORD, il.Register(WORD, reg_dc),
              il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(nan)),
                           il.Const(WORD, 3)))));
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Or(WORD, il.Register(WORD, reg_dc),
              il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(denorma)),
                           il.Const(WORD, 4)))));
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc,
        il.Or(WORD, il.Register(WORD, reg_dc),
              il.ShiftLeft(WORD, il.Register(WORD, LLIL_TEMP(denormb)),
                           il.Const(WORD, 5)))));
    return true;
  }
}

bool Divf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x4B_RR || op2 != DIVF_DC_DA_DB) {
    LogDebug("0x%lx: Divf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc,
                       il.FloatDiv(WORD, il.Register(WORD, reg_da),
                                   il.Register(WORD, reg_db))));
    return true;
  }
}

bool Ftoi::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x4B_RR || op2 != FTOI_DC_DA) {
    LogDebug("0x%lx: Ftoi::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_da, s2, n, reg_dc;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_dc);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc, il.FloatToInt(WORD, il.Register(WORD, reg_da))));
    return true;
  }
}

bool Ftoiz::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ftoq31::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ftoq31z::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                   size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ftou::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Ftouz::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Itof::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x4B_RR || op2 != ITOF_DC_DA) {
    LogDebug("0x%lx: Itof::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_da, s2, n, reg_dc;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, s2, n, reg_dc);
    il.AddInstruction(il.SetRegister(
        WORD, reg_dc, il.IntToFloat(WORD, il.Register(WORD, reg_da))));
    return true;
  }
}

bool Maddf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x6B_RRR || op2 != MADDF_DC_DD_DA_DB) {
    LogDebug("0x%lx: Maddf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_da, reg_db, n, reg_dd, reg_dc;
    Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, reg_db, n, reg_dd,
                                    reg_dc);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc,
                       il.FloatAdd(WORD, il.Register(WORD, reg_dd),
                                   il.FloatMult(WORD, il.Register(WORD, reg_da),
                                                il.Register(WORD, reg_db)))));
    return true;
  }
}

bool Msubf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Mulf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x4B_RR || op2 != MULF_DC_DA_DB) {
    LogDebug("0x%lx: Mulf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t reg_dc, reg_da, reg_db, op1, op2, n;
    Instruction::ExtractOpfieldsRR(data, op1, op2, reg_da, reg_db, n, reg_dc);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc,
                       il.FloatMult(WORD, il.Register(WORD, reg_da),
                                    il.Register(WORD, reg_db))));
    return true;
  }
}

bool Q31tof::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Qseedf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Subf::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  if (op1 != OP1_x6B_RRR || op2 != SUBF_DC_DD_DA) {
    LogDebug("0x%lx: Subf::%s received invalid opcode(s) 0x%x, 0x%x", addr,
             __func__, op1, op2);
    return false;
  }
  {
    len = 4;
    uint8_t op1, op2, reg_da, s2, n, reg_dd, reg_dc;
    Instruction::ExtractOpfieldsRRR(data, op1, op2, reg_da, s2, n, reg_dd,
                                    reg_dc);
    il.AddInstruction(
        il.SetRegister(WORD, reg_dc,
                       il.FloatSub(WORD, il.Register(WORD, reg_dd),
                                   il.Register(WORD, reg_da))));
    return true;
  }
}

bool Updfl::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                 size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Utof::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

/* MMU Instructions */
bool Tlbdemap::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Tlbflush::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                    uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Tlbmap::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                  size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Tlbprobea::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

bool Tlbprobei::Lift(uint8_t op1, uint8_t op2, const uint8_t* data,
                     uint64_t addr, size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}

// TCv1.6 ISA only
bool Wait::Lift(uint8_t op1, uint8_t op2, const uint8_t* data, uint64_t addr,
                size_t& len, LowLevelILFunction& il) {
  UNIMPLEMENTED(*((const uint16_t*)data) & 0x1 ? 4 : 2)
}
}  // namespace Tricore
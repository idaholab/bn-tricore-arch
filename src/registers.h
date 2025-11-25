// Copyright (c) 2025. Battelle Energy Alliance, LLC
// ALL RIGHTS RESERVED

/*
 * See TriCore Architecture Volume 1 -- s. 3.1 for register info
 * Vol. 2 s. 2.2 indicates that registers are ID'd with 4 bits.
 */

#ifndef BINARYNINJA_API_TRICORE_REGISTERS_H
#define BINARYNINJA_API_TRICORE_REGISTERS_H

/* Full-Width General Purpose Data Registers
 * Data registers start at base address 0xFF00, but instructions
 * only ID registers using 4 bits, therefore they've been defined
 * using only 4 bits. TODO: is this okay?
 * See TriCore Arch Vol. 1 - s. 3.1.1
 */
#define TRICORE_REG_D0 0
#define TRICORE_REG_D1 1
#define TRICORE_REG_D2 2
#define TRICORE_REG_D3 3
#define TRICORE_REG_D4 4
#define TRICORE_REG_D5 5
#define TRICORE_REG_D6 6
#define TRICORE_REG_D7 7
#define TRICORE_REG_D8 8
#define TRICORE_REG_D9 9
#define TRICORE_REG_D10 10
#define TRICORE_REG_D11 11
#define TRICORE_REG_D12 12
#define TRICORE_REG_D13 13
#define TRICORE_REG_D14 14
#define TRICORE_REG_D15 15  // Implicit Data Register

/* Full-Width General Purpose Address Registers
 * Address registers start at base address 0xFF80, but instructions
 * only ID registers using 4 bits, therefore they've been defined
 * See TriCore Arch Vol. 1 - s. 3.1.2
 * using only 4 bits. TODO: is this okay?
 */
#define TRICORE_REG_A0 16  // Global Address Register
#define TRICORE_REG_A1 17  // Global Address Register
#define TRICORE_REG_A2 18
#define TRICORE_REG_A3 19
#define TRICORE_REG_A4 20
#define TRICORE_REG_A5 21
#define TRICORE_REG_A6 22
#define TRICORE_REG_A7 23
#define TRICORE_REG_A8 24   // Global Address Register
#define TRICORE_REG_A9 25   // Global Address Register
#define TRICORE_REG_A10 26  // Stack Pointer Register
#define TRICORE_REG_A11 27  // Return Address Register
#define TRICORE_REG_A12 28
#define TRICORE_REG_A13 29
#define TRICORE_REG_A14 30
#define TRICORE_REG_A15 31  // Implicit Address Register

/* 64-bit Data Registers
 * These are constructed using two 32-bit registers.
 * For example, E[0] has the least significant bit in D[0]
 * and the most significant bit in D[1].
 */
#define TRICORE_REG_E0 32
#define TRICORE_REG_E2 34
#define TRICORE_REG_E4 36
#define TRICORE_REG_E6 38
#define TRICORE_REG_E8 40
#define TRICORE_REG_E10 42
#define TRICORE_REG_E12 44
#define TRICORE_REG_E14 46

/* 64-bit Address Registers
 * These are constructed using two 32-bit registers.
 * For example, P[0] has the least significant bit in D[0]
 * and the most significant bit in D[1].
 */
#define TRICORE_REG_P0 48
#define TRICORE_REG_P2 50
#define TRICORE_REG_P4 52
#define TRICORE_REG_P6 54
#define TRICORE_REG_P8 56
#define TRICORE_REG_P10 58
#define TRICORE_REG_P12 60
#define TRICORE_REG_P14 62

/* Core Special Function Registers (not exhaustive, TODO: add the rest)
 * See section 14 of the TriCore Architecture Vol. 1
 * (section 6.5 for interrupt/trap regs)
 */
#define TRICORE_REG_PCXI 0xFE00    // Previous Context Information
#define TRICORE_REG_PSW 0xFE04     // Program Status Word
#define TRICORE_REG_PC 0xFE08      // Program Counter
#define TRICORE_REG_SYSCON 0xFE14  // System config
#define TRICORE_REG_CPUID 0xFE18   // CPU ID info
#define TRICORE_REG_BIV 0xFE20  // Base Interrupt Vector Table Pointer Register
#define TRICORE_REG_BTV 0xFE24  // Base Trap Vector Table Pointer Register
#define TRICORE_REG_ISP 0xFE28  // Interrupt Stack Pointer
#define TRICORE_REG_ICR 0xFE2C  // Interrupt Control Register
#define TRICORE_REG_FCX 0xFE38  // Free CSA List Head Pointer Register
#define TRICORE_REG_LCX 0xFE3C  // Free CSA List Limit Pointer Register
#define TRICORE_REG_DIEAR 0x9020     // Data Integrity Error Address Register
#define TRICORE_REG_DIETR 0x9024     // Data Integrity Error Trap Register
#define TRICORE_REG_CCDIER 0x9028    // Count Corrected Data Integrity Errors
#define TRICORE_REG_MIECON 0x9044    // Memory Integrity Error Control Register
#define TRICORE_REG_PIEAR 0x9210     // Program Integrity Error Address Register
#define TRICORE_REG_PIETR 0x9214     // Program Integrity Error Trap Register
#define TIRCORE_REG_CCPIER 0x9218    // Count Corrected Program Integrity Errors
#define TRICORE_REG_DBGSR 0xFD00     // Debug Status Register
#define TRICORE_REG_EXEVT 0xFD08     // External Event Register
#define TRICORE_REG_CREVT 0xFD0C     // Core Register Access Event
#define TRICORE_REG_SWEVT 0xFD10     // Software Debug Event
#define TRICORE_REG_TR0EVT 0xFD20    // Trigger Event 0
#define TRICORE_REG_TR1EVT 0xFD24    // Trigger Event 1
#define TRICORE_REG_DMS 0xFD40       // Debug Monitor Start Address Register
#define TRICORE_REG_DCX 0xFD44       // Debug Context Save Area Pointer Register
#define TRICORE_REG_DBGTCR 0xFD48    // Debug Trap Control
#define TRICORE_REG_CCTRL 0xFC00     // Counter Control Register
#define TRICORE_REG_CCNT 0xFC04      // CPU Clock Count Register
#define TRICORE_REG_ICNT 0xFC08      // Instruction Count Register
#define TRICORE_REG_M1CNT 0xFC0C     // Multi Count Register 1
#define TRICORE_REG_M2CNT 0xFC10     // Multi Count Register 2
#define TRICORE_REG_M3CNT 0xFC14     // Multi Count Register 3
#define TRICIRE_REG_COMPAT 0x9400    // Compatibility Mode
#define TRICORE_REG_BMACON 0x9004    // Built-in Self Test
#define TRICORE_REG_SMACON 0x900C    // Software in System Test
#define TRICORE_REG_FPUTC 0xA000     // FPU Trap Control
#define TRICORE_REG_FPUTPC 0xA004    // FPU Trapping Instruction Program Counter
#define TRICORE_REG_FPUTOPC 0xA008   // FPU Trapping Instruction Opcode Register
#define TRICORE_REG_FPUTSRC1 0xA010  // FPU Trapping Instruction Operand SRC1
#define TRICORE_REG_FPUTSRC2 0xA014  // FPU Trapping Instruction Operand SRC1
#define TRICORE_REG_FPUTSRC3 0xA018  // FPU Trapping Instruction Operand SRC1
#define TRICORE_REG_FPUIDR 0xA020    // FPU ID register

// TODO: TriCore 1.3 software breakpoint svc request regs may be implementation
// specific
// TODO: There are a ton of memory protection registers (see vol. 1 s. 14)... do
// we need them? There are also other, probably unnecessary registers I'm
// ignoring for now.

#endif  // BINARYNINJA_API_TRICORE_REGISTERS_H

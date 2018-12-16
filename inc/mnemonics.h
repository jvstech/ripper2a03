#if !defined(JVS_RIPPER2A03_MNEMONICS_H_)
#define JVS_RIPPER2A03_MNEMONICS_H_

#include <string>
#include "convert_cast.h"

namespace jvs
{

// MOS 6502 instruction mnemonics
enum class MOSM
{
  ADC,
  AND,
  ASL,
  BCC,
  BCS,
  BEQ,
  BIT,
  BMI,
  BNE,
  BPL,
  BRK,
  BVC,
  BVS,
  CLC,
  CLD,
  CLI,
  CLV,
  CMP,
  CPX,
  CPY,
  DEC,
  DEX,
  DEY,
  EOR,
  INC,
  INX,
  INY,
  JMP,
  JSR,
  LDA,
  LDX,
  LDY,
  LSR,
  NOP,
  ORA,
  PHA,
  PHP,
  PLA,
  PLP,
  ROL,
  ROR,
  RTI,
  RTS,
  SBC,
  SEC,
  SED,
  SEI,
  STA,
  STX,
  STY,
  TAX,
  TAY,
  TSX,
  TXA,
  TXS,
  TYA
};

template <>
struct ConvertCast<MOSM, std::string>
{
  const std::string& operator()(MOSM mnemonic) const;
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_MNEMONICS_H_
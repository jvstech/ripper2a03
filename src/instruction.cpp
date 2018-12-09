#include "instruction.h"

#include <algorithm>
#include <array>
#include <cstring>

#include "addressing_mode.h"
#include "mnemonics.h"

jvs::Instruction::Instruction(std::uint8_t opCode, const char* mnemonic, 
  AddressingMode addressingMode, unsigned int byteCount, bool isCFI, 
  bool isTerminator)
  : opcode_(opCode),
  mnemonic_(mnemonic),
  addressing_mode_(addressingMode),
  byte_count_(byteCount),
  is_cfi_(isCFI),
  is_terminator_(isTerminator)
{
}

std::array<jvs::Instruction, jvs::InstructionCount>& GetInstructionSet()
{
  static std::array<jvs::Instruction, jvs::InstructionCount>* instructionSet =
    new std::array<jvs::Instruction, jvs::InstructionCount>{};
  return *instructionSet;
}

jvs::Instruction& jvs::GetInstruction(std::uint8_t opCode)
{
  return GetInstructionSet()[opCode];
}

jvs::Instruction& jvs::SetInstruction(std::uint8_t opCode, 
  const char* mnemonic, AddressingMode addressingMode, 
  unsigned int byteCount, bool isCFI /*= false*/, bool isTerminator /*= false*/)
{
  Instruction inst{opCode, mnemonic, addressingMode, byteCount, isCFI, 
    isTerminator};
  GetInstructionSet()[opCode] = inst;
  return GetInstruction(opCode);
}

void jvs::InitializeInstructionSet()
{
  using AM = jvs::AddressingMode;

  SetInstruction(0x00, MOS::BRK, AM::Interrupt, 1);
  SetInstruction(0x01, MOS::ORA, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x05, MOS::ORA, AM::DirectPage, 2);
  SetInstruction(0x06, MOS::ASL, AM::DirectPage, 2);
  
  SetInstruction(0x08, MOS::PHP, AM::Stack, 1);
  SetInstruction(0x09, MOS::ORA, AM::Immediate, 2);
  SetInstruction(0x0A, MOS::ASL, AM::Accumulator, 1);
  
  SetInstruction(0x0D, MOS::ORA, AM::Absolute, 3);
  SetInstruction(0x0E, MOS::ASL, AM::Absolute, 3);
  
  SetInstruction(0x10, MOS::BPL, AM::PCRelative, 2, true);
  SetInstruction(0x11, MOS::ORA, AM::DPIndexedIndirectY, 2);
  
  SetInstruction(0x15, MOS::ORA, AM::DPIndexedX, 2);
  SetInstruction(0x16, MOS::ASL, AM::DPIndexedX, 2);
  
  SetInstruction(0x18, MOS::CLC, AM::Implied, 1);
  SetInstruction(0x19, MOS::ORA, AM::AbsoluteIndexedY, 3);
  
  SetInstruction(0x1D, MOS::ORA, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x1E, MOS::ASL, AM::AbsoluteIndexedX, 3);
  
  SetInstruction(0x20, MOS::JSR, AM::Absolute, 3, true);
  SetInstruction(0x21, MOS::AND, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x24, MOS::BIT, AM::DirectPage, 2);
  SetInstruction(0x25, MOS::AND, AM::DirectPage, 2);
  SetInstruction(0x26, MOS::ROL, AM::DirectPage, 2);
  
  SetInstruction(0x28, MOS::PLP, AM::Stack, 1);
  SetInstruction(0x29, MOS::AND, AM::Immediate, 2);
  SetInstruction(0x2A, MOS::ROL, AM::Accumulator, 1);
  
  SetInstruction(0x2C, MOS::BIT, AM::Absolute, 3);
  SetInstruction(0x2D, MOS::AND, AM::Absolute, 3);
  SetInstruction(0x2E, MOS::ROL, AM::Absolute, 3);
  
  SetInstruction(0x30, MOS::BMI, AM::PCRelative, 2, true);
  SetInstruction(0x31, MOS::AND, AM::DPIndexedIndirectY, 2);
  
  SetInstruction(0x35, MOS::AND, AM::DPIndexedIndirectX, 2);
  SetInstruction(0x36, MOS::ROL, AM::DPIndexedX, 2);
  
  SetInstruction(0x3D, MOS::AND, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x3E, MOS::ROL, AM::AbsoluteIndexedX, 3);
  
  SetInstruction(0x40, MOS::RTI, AM::StackRTI, 1, false, true);
  SetInstruction(0x41, MOS::EOR, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x45, MOS::EOR, AM::DirectPage, 2);
  SetInstruction(0x46, MOS::LSR, AM::DirectPage, 2);
  
  SetInstruction(0x48, MOS::PHA, AM::Stack, 1);
  SetInstruction(0x49, MOS::EOR, AM::Immediate, 2);
  SetInstruction(0x4A, MOS::LSR, AM::Accumulator, 1);
  
  SetInstruction(0x4C, MOS::JMP, AM::Absolute, 3, true, true);
  SetInstruction(0x4D, MOS::EOR, AM::Absolute, 3);
  SetInstruction(0x4E, MOS::LSR, AM::Absolute, 3);
  
  SetInstruction(0x50, MOS::BVC, AM::PCRelative, 2, true);
  SetInstruction(0x51, MOS::EOR, AM::DPIndexedIndirectY, 2);
  
  SetInstruction(0x55, MOS::EOR, AM::DPIndexedX, 2);
  SetInstruction(0x56, MOS::LSR, AM::DPIndexedX, 2);
  
  SetInstruction(0x58, MOS::CLI, AM::Implied, 1);
  SetInstruction(0x59, MOS::EOR, AM::AbsoluteIndexedY, 3);
  
  SetInstruction(0x5D, MOS::EOR, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x5E, MOS::LSR, AM::AbsoluteIndexedX, 3);
  
  SetInstruction(0x60, MOS::RTS, AM::Stack, 1);
  SetInstruction(0x61, MOS::ADC, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x65, MOS::ADC, AM::DirectPage, 2);
  SetInstruction(0x66, MOS::ROR, AM::DirectPage, 2);
  
  SetInstruction(0x68, MOS::PLA, AM::Stack, 1);
  SetInstruction(0x69, MOS::ADC, AM::Immediate, 2);
  SetInstruction(0x6A, MOS::ROR, AM::Accumulator, 1);
  
  SetInstruction(0x6C, MOS::JMP, AM::AbsoluteIndirect, 3, true, true);
  SetInstruction(0x6D, MOS::ADC, AM::Absolute, 3);
  SetInstruction(0x6E, MOS::ROR, AM::Absolute, 3);

  SetInstruction(0x70, MOS::BVS, AM::PCRelative, 2, true);
  SetInstruction(0x71, MOS::ADC, AM::DPIndexedIndirectY, 2);

  SetInstruction(0x75, MOS::ADC, AM::DPIndexedX, 2);
  SetInstruction(0x76, MOS::ROR, AM::DPIndexedX, 2);

  SetInstruction(0x78, MOS::SEI, AM::Implied, 1);
  SetInstruction(0x79, MOS::ADC, AM::AbsoluteIndexedY, 3);

  SetInstruction(0x7D, MOS::ADC, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x7E, MOS::ROR, AM::AbsoluteIndexedX, 3);

  SetInstruction(0x81, MOS::STA, AM::DPIndexedIndirectX, 2);

  SetInstruction(0x84, MOS::STY, AM::DirectPage, 2);
  SetInstruction(0x85, MOS::STA, AM::DirectPage, 2);
  SetInstruction(0x86, MOS::STX, AM::DirectPage, 2);

  SetInstruction(0x88, MOS::DEY, AM::Implied, 1);
  
  SetInstruction(0x8A, MOS::TXA, AM::Implied, 1);

  SetInstruction(0x8C, MOS::STY, AM::Absolute, 3);
  SetInstruction(0x8D, MOS::STA, AM::Absolute, 3);
  SetInstruction(0x8E, MOS::STX, AM::Absolute, 3);

  SetInstruction(0x90, MOS::BCC, AM::PCRelative, 2, true);
  SetInstruction(0x91, MOS::STA, AM::DPIndexedIndirectY, 2);

  SetInstruction(0x94, MOS::STY, AM::DPIndexedX, 2);
  SetInstruction(0x95, MOS::STA, AM::DPIndexedX, 2);
  SetInstruction(0x96, MOS::STX, AM::DPIndexedY, 2);

  SetInstruction(0x98, MOS::TYA, AM::Implied, 1);
  SetInstruction(0x99, MOS::STA, AM::AbsoluteIndexedY, 3);
  SetInstruction(0x9A, MOS::TXS, AM::Implied, 1);

  SetInstruction(0x9D, MOS::STA, AM::AbsoluteIndexedX, 3);

  SetInstruction(0xA0, MOS::LDY, AM::Immediate, 2);
  SetInstruction(0xA1, MOS::LDA, AM::DPIndexedIndirectX, 2);
  SetInstruction(0xA2, MOS::LDX, AM::Immediate, 2);

  SetInstruction(0xA4, MOS::LDY, AM::DirectPage, 2);
  SetInstruction(0xA5, MOS::LDA, AM::DirectPage, 2);
  SetInstruction(0xA6, MOS::LDX, AM::DirectPage, 2);

  SetInstruction(0xA8, MOS::TAY, AM::Implied, 1);
  SetInstruction(0xA9, MOS::LDA, AM::Immediate, 2);
  SetInstruction(0xAA, MOS::TAX, AM::Implied, 1);

  SetInstruction(0xAC, MOS::LDY, AM::Absolute, 3);
  SetInstruction(0xAD, MOS::LDA, AM::Absolute, 3);
  SetInstruction(0xAE, MOS::LDX, AM::Absolute, 3);

  SetInstruction(0xB0, MOS::BCS, AM::PCRelative, 2, true);
  SetInstruction(0xB1, MOS::LDA, AM::DPIndexedIndirectY, 2);

  SetInstruction(0xB4, MOS::LDY, AM::DPIndexedX, 2);
  SetInstruction(0xB5, MOS::LDA, AM::DPIndexedX, 2);
  SetInstruction(0xB6, MOS::LDX, AM::DPIndexedY, 2);

  SetInstruction(0xB8, MOS::CLV, AM::Implied, 1);
  SetInstruction(0xB9, MOS::LDA, AM::AbsoluteIndexedY, 3);
  SetInstruction(0xBA, MOS::TSX, AM::Implied, 1);
  
  SetInstruction(0xBC, MOS::LDY, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xBD, MOS::LDA, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xBE, MOS::LDX, AM::AbsoluteIndexedY, 3);

  SetInstruction(0xC0, MOS::CPY, AM::Immediate, 2);
  SetInstruction(0xC1, MOS::CMP, AM::DPIndexedIndirectX, 2);

  SetInstruction(0xC4, MOS::CPY, AM::DirectPage, 2);
  SetInstruction(0xC5, MOS::CMP, AM::DirectPage, 2);
  SetInstruction(0xC6, MOS::DEC, AM::DirectPage, 2);

  SetInstruction(0xC8, MOS::INY, AM::Implied, 1);
  SetInstruction(0xC9, MOS::CMP, AM::Immediate, 2);
  SetInstruction(0xCA, MOS::DEX, AM::Implied, 1);

  SetInstruction(0xCC, MOS::CPY, AM::Absolute, 3);
  SetInstruction(0xCD, MOS::CMP, AM::Absolute, 3);
  SetInstruction(0xCE, MOS::DEC, AM::Absolute, 3);

  SetInstruction(0xD0, MOS::BNE, AM::PCRelative, 2, true);
  SetInstruction(0xD1, MOS::CMP, AM::DPIndexedIndirectY, 2);

  SetInstruction(0xD5, MOS::CMP, AM::DPIndexedX, 2);
  SetInstruction(0xD6, MOS::DEC, AM::DPIndexedX, 2);

  SetInstruction(0xD8, MOS::CLD, AM::Implied, 1);
  SetInstruction(0xD9, MOS::CMP, AM::AbsoluteIndexedY, 3);

  SetInstruction(0xDD, MOS::CMP, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xDE, MOS::DEC, AM::AbsoluteIndexedX, 3);

  SetInstruction(0xE0, MOS::CPX, AM::Immediate, 2);
  SetInstruction(0xE1, MOS::SBC, AM::DPIndexedIndirectX, 2);

  SetInstruction(0xE4, MOS::CPX, AM::DirectPage, 2);
  SetInstruction(0xE5, MOS::SBC, AM::DirectPage, 2);
  SetInstruction(0xE6, MOS::INC, AM::DirectPage, 2);

  SetInstruction(0xE8, MOS::INX, AM::Implied, 1);
  SetInstruction(0xE9, MOS::SBC, AM::Immediate, 2);
  SetInstruction(0xEA, MOS::NOP, AM::Implied, 1);

  SetInstruction(0xEC, MOS::CPX, AM::Absolute, 3);
  SetInstruction(0xED, MOS::SBC, AM::Absolute, 3);
  SetInstruction(0xEE, MOS::INC, AM::Absolute, 3);

  SetInstruction(0xF0, MOS::BEQ, AM::PCRelative, 2, true);
  SetInstruction(0xF1, MOS::SBC, AM::DPIndexedIndirectY, 2);

  SetInstruction(0xF5, MOS::SBC, AM::DPIndexedX, 2);
  SetInstruction(0xF6, MOS::INC, AM::DPIndexedX, 2);

  SetInstruction(0xF8, MOS::SED, AM::Implied, 1);
  SetInstruction(0xF9, MOS::SBC, AM::AbsoluteIndexedY, 3);

  SetInstruction(0xFD, MOS::SBC, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xFE, MOS::INC, AM::AbsoluteIndexedX, 3);
}

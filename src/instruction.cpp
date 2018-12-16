#include "instruction.h"

#include <algorithm>
#include <array>
#include <cstring>

#include "addressing_mode.h"
#include "mnemonics.h"

jvs::Instruction::Instruction(std::uint8_t opCode, MOSM mnemonic, 
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

std::array<jvs::Instruction, jvs::InstructionCount>& jvs::GetInstructionSet()
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
  MOSM mnemonic, AddressingMode addressingMode, 
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

  SetInstruction(0x00, MOSM::BRK, AM::Interrupt, 1);
  SetInstruction(0x01, MOSM::ORA, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x05, MOSM::ORA, AM::DirectPage, 2);
  SetInstruction(0x06, MOSM::ASL, AM::DirectPage, 2);
  
  SetInstruction(0x08, MOSM::PHP, AM::Stack, 1);
  SetInstruction(0x09, MOSM::ORA, AM::Immediate, 2);
  SetInstruction(0x0A, MOSM::ASL, AM::Accumulator, 1);
  
  SetInstruction(0x0D, MOSM::ORA, AM::Absolute, 3);
  SetInstruction(0x0E, MOSM::ASL, AM::Absolute, 3);
  
  SetInstruction(0x10, MOSM::BPL, AM::PCRelative, 2, true);
  SetInstruction(0x11, MOSM::ORA, AM::DPIndexedIndirectY, 2);
  
  SetInstruction(0x15, MOSM::ORA, AM::DPIndexedX, 2);
  SetInstruction(0x16, MOSM::ASL, AM::DPIndexedX, 2);
  
  SetInstruction(0x18, MOSM::CLC, AM::Implied, 1);
  SetInstruction(0x19, MOSM::ORA, AM::AbsoluteIndexedY, 3);
  
  SetInstruction(0x1D, MOSM::ORA, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x1E, MOSM::ASL, AM::AbsoluteIndexedX, 3);
  
  SetInstruction(0x20, MOSM::JSR, AM::Absolute, 3, true);
  SetInstruction(0x21, MOSM::AND, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x24, MOSM::BIT, AM::DirectPage, 2);
  SetInstruction(0x25, MOSM::AND, AM::DirectPage, 2);
  SetInstruction(0x26, MOSM::ROL, AM::DirectPage, 2);
  
  SetInstruction(0x28, MOSM::PLP, AM::Stack, 1);
  SetInstruction(0x29, MOSM::AND, AM::Immediate, 2);
  SetInstruction(0x2A, MOSM::ROL, AM::Accumulator, 1);
  
  SetInstruction(0x2C, MOSM::BIT, AM::Absolute, 3);
  SetInstruction(0x2D, MOSM::AND, AM::Absolute, 3);
  SetInstruction(0x2E, MOSM::ROL, AM::Absolute, 3);
  
  SetInstruction(0x30, MOSM::BMI, AM::PCRelative, 2, true);
  SetInstruction(0x31, MOSM::AND, AM::DPIndexedIndirectY, 2);
  
  SetInstruction(0x35, MOSM::AND, AM::DPIndexedIndirectX, 2);
  SetInstruction(0x36, MOSM::ROL, AM::DPIndexedX, 2);
  
  SetInstruction(0x3D, MOSM::AND, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x3E, MOSM::ROL, AM::AbsoluteIndexedX, 3);
  
  SetInstruction(0x40, MOSM::RTI, AM::StackRTI, 1, false, true);
  SetInstruction(0x41, MOSM::EOR, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x45, MOSM::EOR, AM::DirectPage, 2);
  SetInstruction(0x46, MOSM::LSR, AM::DirectPage, 2);
  
  SetInstruction(0x48, MOSM::PHA, AM::Stack, 1);
  SetInstruction(0x49, MOSM::EOR, AM::Immediate, 2);
  SetInstruction(0x4A, MOSM::LSR, AM::Accumulator, 1);
  
  SetInstruction(0x4C, MOSM::JMP, AM::Absolute, 3, true, true);
  SetInstruction(0x4D, MOSM::EOR, AM::Absolute, 3);
  SetInstruction(0x4E, MOSM::LSR, AM::Absolute, 3);
  
  SetInstruction(0x50, MOSM::BVC, AM::PCRelative, 2, true);
  SetInstruction(0x51, MOSM::EOR, AM::DPIndexedIndirectY, 2);
  
  SetInstruction(0x55, MOSM::EOR, AM::DPIndexedX, 2);
  SetInstruction(0x56, MOSM::LSR, AM::DPIndexedX, 2);
  
  SetInstruction(0x58, MOSM::CLI, AM::Implied, 1);
  SetInstruction(0x59, MOSM::EOR, AM::AbsoluteIndexedY, 3);
  
  SetInstruction(0x5D, MOSM::EOR, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x5E, MOSM::LSR, AM::AbsoluteIndexedX, 3);
  
  SetInstruction(0x60, MOSM::RTS, AM::Stack, 1, false, true);
  SetInstruction(0x61, MOSM::ADC, AM::DPIndexedIndirectX, 2);
  
  SetInstruction(0x65, MOSM::ADC, AM::DirectPage, 2);
  SetInstruction(0x66, MOSM::ROR, AM::DirectPage, 2);
  
  SetInstruction(0x68, MOSM::PLA, AM::Stack, 1);
  SetInstruction(0x69, MOSM::ADC, AM::Immediate, 2);
  SetInstruction(0x6A, MOSM::ROR, AM::Accumulator, 1);
  
  SetInstruction(0x6C, MOSM::JMP, AM::AbsoluteIndirect, 3, true, true);
  SetInstruction(0x6D, MOSM::ADC, AM::Absolute, 3);
  SetInstruction(0x6E, MOSM::ROR, AM::Absolute, 3);

  SetInstruction(0x70, MOSM::BVS, AM::PCRelative, 2, true);
  SetInstruction(0x71, MOSM::ADC, AM::DPIndexedIndirectY, 2);

  SetInstruction(0x75, MOSM::ADC, AM::DPIndexedX, 2);
  SetInstruction(0x76, MOSM::ROR, AM::DPIndexedX, 2);

  SetInstruction(0x78, MOSM::SEI, AM::Implied, 1);
  SetInstruction(0x79, MOSM::ADC, AM::AbsoluteIndexedY, 3);

  SetInstruction(0x7D, MOSM::ADC, AM::AbsoluteIndexedX, 3);
  SetInstruction(0x7E, MOSM::ROR, AM::AbsoluteIndexedX, 3);

  SetInstruction(0x81, MOSM::STA, AM::DPIndexedIndirectX, 2);

  SetInstruction(0x84, MOSM::STY, AM::DirectPage, 2);
  SetInstruction(0x85, MOSM::STA, AM::DirectPage, 2);
  SetInstruction(0x86, MOSM::STX, AM::DirectPage, 2);

  SetInstruction(0x88, MOSM::DEY, AM::Implied, 1);
  
  SetInstruction(0x8A, MOSM::TXA, AM::Implied, 1);

  SetInstruction(0x8C, MOSM::STY, AM::Absolute, 3);
  SetInstruction(0x8D, MOSM::STA, AM::Absolute, 3);
  SetInstruction(0x8E, MOSM::STX, AM::Absolute, 3);

  SetInstruction(0x90, MOSM::BCC, AM::PCRelative, 2, true);
  SetInstruction(0x91, MOSM::STA, AM::DPIndexedIndirectY, 2);

  SetInstruction(0x94, MOSM::STY, AM::DPIndexedX, 2);
  SetInstruction(0x95, MOSM::STA, AM::DPIndexedX, 2);
  SetInstruction(0x96, MOSM::STX, AM::DPIndexedY, 2);

  SetInstruction(0x98, MOSM::TYA, AM::Implied, 1);
  SetInstruction(0x99, MOSM::STA, AM::AbsoluteIndexedY, 3);
  SetInstruction(0x9A, MOSM::TXS, AM::Implied, 1);

  SetInstruction(0x9D, MOSM::STA, AM::AbsoluteIndexedX, 3);

  SetInstruction(0xA0, MOSM::LDY, AM::Immediate, 2);
  SetInstruction(0xA1, MOSM::LDA, AM::DPIndexedIndirectX, 2);
  SetInstruction(0xA2, MOSM::LDX, AM::Immediate, 2);

  SetInstruction(0xA4, MOSM::LDY, AM::DirectPage, 2);
  SetInstruction(0xA5, MOSM::LDA, AM::DirectPage, 2);
  SetInstruction(0xA6, MOSM::LDX, AM::DirectPage, 2);

  SetInstruction(0xA8, MOSM::TAY, AM::Implied, 1);
  SetInstruction(0xA9, MOSM::LDA, AM::Immediate, 2);
  SetInstruction(0xAA, MOSM::TAX, AM::Implied, 1);

  SetInstruction(0xAC, MOSM::LDY, AM::Absolute, 3);
  SetInstruction(0xAD, MOSM::LDA, AM::Absolute, 3);
  SetInstruction(0xAE, MOSM::LDX, AM::Absolute, 3);

  SetInstruction(0xB0, MOSM::BCS, AM::PCRelative, 2, true);
  SetInstruction(0xB1, MOSM::LDA, AM::DPIndexedIndirectY, 2);

  SetInstruction(0xB4, MOSM::LDY, AM::DPIndexedX, 2);
  SetInstruction(0xB5, MOSM::LDA, AM::DPIndexedX, 2);
  SetInstruction(0xB6, MOSM::LDX, AM::DPIndexedY, 2);

  SetInstruction(0xB8, MOSM::CLV, AM::Implied, 1);
  SetInstruction(0xB9, MOSM::LDA, AM::AbsoluteIndexedY, 3);
  SetInstruction(0xBA, MOSM::TSX, AM::Implied, 1);
  
  SetInstruction(0xBC, MOSM::LDY, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xBD, MOSM::LDA, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xBE, MOSM::LDX, AM::AbsoluteIndexedY, 3);

  SetInstruction(0xC0, MOSM::CPY, AM::Immediate, 2);
  SetInstruction(0xC1, MOSM::CMP, AM::DPIndexedIndirectX, 2);

  SetInstruction(0xC4, MOSM::CPY, AM::DirectPage, 2);
  SetInstruction(0xC5, MOSM::CMP, AM::DirectPage, 2);
  SetInstruction(0xC6, MOSM::DEC, AM::DirectPage, 2);

  SetInstruction(0xC8, MOSM::INY, AM::Implied, 1);
  SetInstruction(0xC9, MOSM::CMP, AM::Immediate, 2);
  SetInstruction(0xCA, MOSM::DEX, AM::Implied, 1);

  SetInstruction(0xCC, MOSM::CPY, AM::Absolute, 3);
  SetInstruction(0xCD, MOSM::CMP, AM::Absolute, 3);
  SetInstruction(0xCE, MOSM::DEC, AM::Absolute, 3);

  SetInstruction(0xD0, MOSM::BNE, AM::PCRelative, 2, true);
  SetInstruction(0xD1, MOSM::CMP, AM::DPIndexedIndirectY, 2);

  SetInstruction(0xD5, MOSM::CMP, AM::DPIndexedX, 2);
  SetInstruction(0xD6, MOSM::DEC, AM::DPIndexedX, 2);

  SetInstruction(0xD8, MOSM::CLD, AM::Implied, 1);
  SetInstruction(0xD9, MOSM::CMP, AM::AbsoluteIndexedY, 3);

  SetInstruction(0xDD, MOSM::CMP, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xDE, MOSM::DEC, AM::AbsoluteIndexedX, 3);

  SetInstruction(0xE0, MOSM::CPX, AM::Immediate, 2);
  SetInstruction(0xE1, MOSM::SBC, AM::DPIndexedIndirectX, 2);

  SetInstruction(0xE4, MOSM::CPX, AM::DirectPage, 2);
  SetInstruction(0xE5, MOSM::SBC, AM::DirectPage, 2);
  SetInstruction(0xE6, MOSM::INC, AM::DirectPage, 2);

  SetInstruction(0xE8, MOSM::INX, AM::Implied, 1);
  SetInstruction(0xE9, MOSM::SBC, AM::Immediate, 2);
  SetInstruction(0xEA, MOSM::NOP, AM::Implied, 1);

  SetInstruction(0xEC, MOSM::CPX, AM::Absolute, 3);
  SetInstruction(0xED, MOSM::SBC, AM::Absolute, 3);
  SetInstruction(0xEE, MOSM::INC, AM::Absolute, 3);

  SetInstruction(0xF0, MOSM::BEQ, AM::PCRelative, 2, true);
  SetInstruction(0xF1, MOSM::SBC, AM::DPIndexedIndirectY, 2);

  SetInstruction(0xF5, MOSM::SBC, AM::DPIndexedX, 2);
  SetInstruction(0xF6, MOSM::INC, AM::DPIndexedX, 2);

  SetInstruction(0xF8, MOSM::SED, AM::Implied, 1);
  SetInstruction(0xF9, MOSM::SBC, AM::AbsoluteIndexedY, 3);

  SetInstruction(0xFD, MOSM::SBC, AM::AbsoluteIndexedX, 3);
  SetInstruction(0xFE, MOSM::INC, AM::AbsoluteIndexedX, 3);
}

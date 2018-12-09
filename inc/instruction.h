#if !defined(JVS_RIPPER2A03_INSTRUCTION_H_)
#define JVS_RIPPER2A03_INSTRUCTION_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "addressing_mode.h"

namespace jvs
{

struct Instruction
{
  Instruction() = default;
  Instruction(const Instruction&) = default;
  Instruction(Instruction&&) = default;
  Instruction& operator=(const Instruction&) = default;
  Instruction& operator=(Instruction&&) = default;

  Instruction(std::uint8_t opCode, const char* mnemonic,
    AddressingMode addressingMode, unsigned int byteCount, bool isCFI,
    bool isTerminator);

  std::uint8_t opcode() const
  {
    return this->opcode_;
  }

  const char* mnemonic() const
  {
    return this->mnemonic_;
  }

  unsigned int byte_count() const
  {
    return this->byte_count_;
  }

  bool is_cfi() const
  {
    return this->is_cfi_;
  }

  bool is_terminator() const
  {
    return this->is_terminator_;
  }

  bool IsEmpty() const
  {
    return (this->byte_count_ == 0);
  }


private:
  std::uint8_t opcode_{0};
  const char* mnemonic_{nullptr};
  AddressingMode addressing_mode_{AddressingMode::Absolute};
  unsigned int byte_count_{0};
  bool is_cfi_{false};
  bool is_terminator_{false};
};

constexpr unsigned int InstructionCount = 0xff;

std::array<Instruction, InstructionCount>& GetInstructionSet();
Instruction& GetInstruction(std::uint8_t opCode);
Instruction& SetInstruction(std::uint8_t opCode, const char* mnemonic,
  AddressingMode addressingMode, unsigned int byteCount, bool isCFI = false,
  bool isTerminator = false);
void InitializeInstructionSet();

} // namespace jvs

#endif // !JVS_RIPPER2A03_INSTRUCTION_H_

#if !defined(JVS_RIPPER2A03_INSTRUCTION_INFO_H_)
#define JVS_RIPPER2A03_INSTRUCTION_INFO_H_

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "addressing_mode.h"
#include "mnemonics.h"

namespace jvs
{

struct InstructionInfo
{
  InstructionInfo() = default;
  InstructionInfo(const InstructionInfo&) = default;
  InstructionInfo(InstructionInfo&&) = default;
  InstructionInfo& operator=(const InstructionInfo&) = default;
  InstructionInfo& operator=(InstructionInfo&&) = default;
  virtual ~InstructionInfo() = default;

  InstructionInfo(std::uint8_t opCode, MOSM mnemonic,
    AddressingMode addressingMode, unsigned int byteCount, bool isCFI,
    bool isTerminator);

  std::uint8_t opcode() const
  {
    return this->opcode_;
  }

  MOSM mnemonic() const
  {
    return this->mnemonic_;
  }

  AddressingMode addressing_mode() const
  {
    return this->addressing_mode_;
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

  explicit operator bool() const
  {
    return (!(this->IsEmpty()));
  }

private:
  std::uint8_t opcode_{0};
  MOSM mnemonic_{};
  AddressingMode addressing_mode_{AddressingMode::Absolute};
  unsigned int byte_count_{0};
  bool is_cfi_{false};
  bool is_terminator_{false};
};

constexpr unsigned int InstructionCount = 0xff;

std::array<InstructionInfo, InstructionCount>& GetInstructionInfoSet();
InstructionInfo& GetInstructionInfo(std::uint8_t opCode);
InstructionInfo& SetInstructionInfo(std::uint8_t opCode, MOSM mnemonic,
  AddressingMode addressingMode, unsigned int byteCount, bool isCFI = false,
  bool isTerminator = false);

} // namespace jvs

#endif // !JVS_RIPPER2A03_INSTRUCTION_INFO_H_

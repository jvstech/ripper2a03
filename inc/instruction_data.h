#if !defined(JVS_RIPPER2A03_INSTRUCTION_DATA_H_)
#define JVS_RIPPER2A03_INSTRUCTION_DATA_H_

#include <cstdint>
#include <istream>
#include <string>

#include "convert_cast.h"
#include "instruction_assembly_text.h"
#include "instruction_info.h"

namespace jvs
{

struct InstructionData
{
  InstructionData(const InstructionInfo& instInfo, unsigned int addr,
    std::uint16_t rawOperand)
    : info_(instInfo),
    address_(addr),
    raw_operand_(rawOperand)
  {
  }

  InstructionData(InstructionInfo&& instInfo, unsigned int addr,
    std::uint16_t rawOperand)
    : info_(std::move(instInfo)),
    address_(addr),
    raw_operand_(rawOperand)
  {
  }

  InstructionData() = default;

  const InstructionInfo& info() const
  {
    return this->info_;
  }

  unsigned int address() const
  {
    return this->address_;
  }

  std::uint16_t raw_operand() const
  {
    return this->raw_operand_;
  }

  bool has_raw_operand() const
  {
    return (this->info_.byte_count() > 1);
  }

  static InstructionData FromBytes(const std::uint8_t* bytes, 
    unsigned int addr);
  static InstructionData FromStream(std::istream& is, unsigned int addr);

private:
  InstructionInfo info_{};
  unsigned int address_{static_cast<unsigned int>(-1)};
  std::uint16_t raw_operand_{0};
};

template <>
struct ConvertCast<InstructionData, InstructionAssemblyText>
{
  InstructionAssemblyText operator()(InstructionData instData) const noexcept;
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_INSTRUCTION_DATA_H_

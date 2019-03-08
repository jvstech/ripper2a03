#include "instruction_data.h"

#include <cstdint>
#include <cstring>
#include <iomanip>
#include <istream>
#include <sstream>
#include <string>

#include "instruction_assembly_text.h"
#include "instruction_info.h"
#include "mnemonics.h"

jvs::InstructionData jvs::InstructionData::FromBytes(const std::uint8_t* bytes, 
  unsigned int addr)
{
  std::uint16_t operandValue{0};
  auto opCode = *bytes;
  auto& instInfo = jvs::GetInstructionInfo(opCode);
  if (!instInfo.IsEmpty())
  {
    if (instInfo.byte_count() > 1)
    {
      std::memcpy(&operandValue, bytes + 1, instInfo.byte_count() - 1);
    }
  }

  InstructionData instData{instInfo, addr, operandValue};
  return instData;
}

jvs::InstructionData jvs::InstructionData::FromStream(std::istream& is, 
  unsigned int addr)
{
  std::uint16_t operandValue{0};
  std::uint8_t opCode{0};
  is.read(reinterpret_cast<char*>(&opCode), 1);
  if (!is.good())
  {
    InstructionData badResult{{}, addr, operandValue};
    return badResult;
  }

  auto& instInfo = jvs::GetInstructionInfo(opCode);
  if (instInfo)
  {
    if (instInfo.byte_count() > 1)
    {
      is.read(reinterpret_cast<char*>(operandValue), instInfo.byte_count() - 1);
      if (!is.good())
      {
        InstructionData badResult{{}, addr, operandValue};
        return badResult;
      }
    }
  }

  InstructionData instData{instInfo, addr, operandValue};
  return instData;
}


auto jvs::ConvertCast<jvs::InstructionData, jvs::InstructionAssemblyText>::operator()(
  jvs::InstructionData instData) const noexcept
  -> jvs::InstructionAssemblyText
{
  std::stringstream ss{};
  ss << jvs::ConvertTo<std::string>(instData.info().mnemonic());
  if (instData.info().byte_count() == 1)
  {
    switch (instData.info().addressing_mode())
    {
    default:
      ss << " ; bad addressing mode for size (size = 1, AM = "
        << static_cast<int>(instData.info().addressing_mode()) << ")";
      break;
    case jvs::AddressingMode::Accumulator:
      ss << " A";
      break;
    case jvs::AddressingMode::Implied:
    case jvs::AddressingMode::Stack:
      break;
    }
  }
  else if (instData.info().byte_count() > 1)
  {
    switch (instData.info().addressing_mode())
    {
    default:
      ss << " #$" << std::hex << instData.raw_operand() << std::dec
        << " ; unhandled addressing mode ("
        << static_cast<int>(instData.info().addressing_mode()) << ")";
      break;
    case jvs::AddressingMode::Absolute:
      ss << " $" << std::hex << instData.raw_operand() << std::dec;
      break;
    case jvs::AddressingMode::Immediate:
      ss << " #$" << std::hex << instData.raw_operand() << std::dec;
      break;
    case jvs::AddressingMode::AbsoluteIndexedX:
    case jvs::AddressingMode::DPIndexedX:
      ss << " $" << std::hex << instData.raw_operand() << std::dec << ",X";
      break;
    case jvs::AddressingMode::DPIndirectIndexedX:
      ss << " (" << instData.raw_operand() << ",X)";
      break;
    case jvs::AddressingMode::AbsoluteIndexedY:
    case jvs::AddressingMode::DPIndexedY:
      ss << " $" << std::hex << instData.raw_operand() << std::dec << ",Y";
      break;
    case jvs::AddressingMode::DPIndexedIndirectY:
      ss << " (" << instData.raw_operand() << "),Y";
      break;
    case jvs::AddressingMode::PCRelative:
      ss << " $" << std::hex << (instData.address() + 
        instData.info().byte_count() + 
        static_cast<std::int8_t>(instData.raw_operand())) << std::dec;
      break;
    }
  }

  InstructionAssemblyText asmText{ss.str()};
  return asmText;
}

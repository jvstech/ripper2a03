#include "instruction_info.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <memory>

#include "addressing_mode.h"
#include "mnemonics.h"

namespace 
{

using InstructionInfoArray = 
  std::array<jvs::InstructionInfo, jvs::InstructionCount>;

static InstructionInfoArray* newDefaultInstructionInfoArray()
{
  InstructionInfoArray* instInfos = new InstructionInfoArray();
#define INSTRUCTION_INFO(m, o, a, s, c, t) \
  (*instInfos)[(o)] = jvs::InstructionInfo((o), jvs::MOSM:: ## m, \
  jvs::AddressingMode:: ## a, (s), (c), (t));
#include "instruction_info.h"
  return instInfos;
}

} // namespace 

jvs::InstructionInfo::InstructionInfo(std::uint8_t opCode, MOSM mnemonic, 
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

InstructionInfoArray& jvs::GetInstructionInfoSet()
{
  static auto* instInfoSet = newDefaultInstructionInfoArray();
  return *instInfoSet;
}

jvs::InstructionInfo& jvs::GetInstructionInfo(std::uint8_t opCode)
{
  return GetInstructionInfoSet()[opCode];
}

jvs::InstructionInfo& jvs::SetInstructionInfo(std::uint8_t opCode, 
  MOSM mnemonic, AddressingMode addressingMode, 
  unsigned int byteCount, bool isCFI /*= false*/, bool isTerminator /*= false*/)
{
  InstructionInfo inst{opCode, mnemonic, addressingMode, byteCount, isCFI, 
    isTerminator};
  GetInstructionInfoSet()[opCode] = inst;
  return GetInstructionInfo(opCode);
}

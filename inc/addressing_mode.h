#if !defined(JVS_RIPPER2A03_ADDRESSING_MODE_H_)
#define JVS_RIPPER2A03_ADDRESSING_MODE_H_

namespace jvs
{

enum class AddressingMode
{
  Absolute,
  AbsoluteIndexedX,
  AbsoluteIndexedY,
  AbsoluteIndirect,
  Accumulator,
  DirectPage,
  DPIndexedX,
  DPIndirectIndexedX,
  DPIndexedY,
  DPIndexedIndirectY,
  Immediate,
  Implied,
  Interrupt,
  PCRelative,
  Stack,
  StackRTI
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_ADDRESSING_MODE_H_
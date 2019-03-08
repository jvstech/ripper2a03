#if !defined(JVS_RIPPER2A03_INSTRUCTION_ASSEMBLY_TEXT_H_)
#define JVS_RIPPER2A03_INSTRUCTION_ASSEMBLY_TEXT_H_

#include <string>

#include "convert_cast.h"

namespace jvs
{

struct InstructionAssemblyText
{
  InstructionAssemblyText(const std::string& asmText, bool indentText = false)
    : text_(asmText),
    indent_(indentText)
  {
  }

  const std::string& text() const
  {
    return text_;
  }

  bool indent() const
  {
    return indent_;
  }

private:
  std::string text_{};
  bool indent_{false};
};

template <>
struct ConvertCast<InstructionAssemblyText, std::string>
{
  std::string operator()(InstructionAssemblyText asmText) const noexcept
  {
    if (asmText.indent())
    {
      return ("  " + asmText.text());
    }

    return asmText.text();
  }
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_INSTRUCTION_ASSEMBLY_TEXT_H_
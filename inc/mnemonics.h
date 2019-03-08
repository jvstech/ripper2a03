#if !defined(JVS_RIPPER2A03_MNEMONICS_H_)
#define JVS_RIPPER2A03_MNEMONICS_H_

#include <string>
#include "convert_cast.h"

namespace jvs
{

// MOS 6502 instruction mnemonics
enum class MOSM
{
#define INSTRUCTION_MNEMONIC(m) m ,
#include "mnemonics.def"
};

template <>
struct ConvertCast<MOSM, std::string>
{
  const std::string& operator()(MOSM mnemonic) const;
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_MNEMONICS_H_
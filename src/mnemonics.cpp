#include "mnemonics.h"

#include <map>
#include <string>

namespace jvs
{

static const std::map<MOSM, std::string>& getMnemonics()
{
#define INSTRUCTION_MNEMONIC(m) \
  { MOSM::m , #m },

  static const std::map<MOSM, std::string>* mnemonicsMap =
    new const std::map<MOSM, std::string>
  {
#include "mnemonics.def"
  };

  return *mnemonicsMap;
}

} // namespace jvs

const std::string& jvs::ConvertCast<jvs::MOSM, std::string>::operator()(
  jvs::MOSM mnemonic) const
{
  return jvs::getMnemonics().at(mnemonic);
}

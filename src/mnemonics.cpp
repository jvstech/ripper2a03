#include "mnemonics.h"

#include <map>
#include <string>

namespace jvs
{

static const std::map<MOSM, std::string>& getMnemonics()
{
  static const std::map<MOSM, std::string>* mnemonicsMap =
    new const std::map<MOSM, std::string>
  {
    { MOSM::ADC, "ADC" },
    { MOSM::AND, "AND" },
    { MOSM::ASL, "ASL" },
    { MOSM::BCC, "BCC" },
    { MOSM::BCS, "BCS" },
    { MOSM::BEQ, "BEQ" },
    { MOSM::BIT, "BIT" },
    { MOSM::BMI, "BMI" },
    { MOSM::BNE, "BNE" },
    { MOSM::BPL, "BPL" },
    { MOSM::BRK, "BRK" },
    { MOSM::BVC, "BVC" },
    { MOSM::BVS, "BVS" },
    { MOSM::CLC, "CLC" },
    { MOSM::CLD, "CLD" },
    { MOSM::CLI, "CLI" },
    { MOSM::CLV, "CLV" },
    { MOSM::CMP, "CMP" },
    { MOSM::CPX, "CPX" },
    { MOSM::CPY, "CPY" },
    { MOSM::DEC, "DEC" },
    { MOSM::DEX, "DEX" },
    { MOSM::DEY, "DEY" },
    { MOSM::EOR, "EOR" },
    { MOSM::INC, "INC" },
    { MOSM::INX, "INX" },
    { MOSM::INY, "INY" },
    { MOSM::JMP, "JMP" },
    { MOSM::JSR, "JSR" },
    { MOSM::LDA, "LDA" },
    { MOSM::LDX, "LDX" },
    { MOSM::LDY, "LDY" },
    { MOSM::LSR, "LSR" },
    { MOSM::NOP, "NOP" },
    { MOSM::ORA, "ORA" },
    { MOSM::PHA, "PHA" },
    { MOSM::PHP, "PHP" },
    { MOSM::PLA, "PLA" },
    { MOSM::PLP, "PLP" },
    { MOSM::ROL, "ROL" },
    { MOSM::ROR, "ROR" },
    { MOSM::RTI, "RTI" },
    { MOSM::RTS, "RTS" },
    { MOSM::SBC, "SBC" },
    { MOSM::SEC, "SEC" },
    { MOSM::SED, "SED" },
    { MOSM::SEI, "SEI" },
    { MOSM::STA, "STA" },
    { MOSM::STX, "STX" },
    { MOSM::STY, "STY" },
    { MOSM::TAX, "TAX" },
    { MOSM::TAY, "TAY" },
    { MOSM::TSX, "TSX" },
    { MOSM::TXA, "TXA" },
    { MOSM::TXS, "TXS" },
    { MOSM::TYA, "TYA" }
  };

  return *mnemonicsMap;
}

} // namespace jvs

const std::string& jvs::ConvertCast<jvs::MOSM, std::string>::operator()(
  jvs::MOSM mnemonic) const
{
  return jvs::getMnemonics().at(mnemonic);
}

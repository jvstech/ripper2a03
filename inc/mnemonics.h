#if !defined(JVS_RIPPER2A03_MNEMONICS_H_)
#define JVS_RIPPER2A03_MNEMONICS_H_

namespace jvs
{

// MOS 6502 instruction mnemonics
struct MOS
{
  static constexpr char ADC[] = "ADC";
  static constexpr char AND[] = "AND";
  static constexpr char ASL[] = "ASL";
  static constexpr char BCC[] = "BCC";
  static constexpr char BCS[] = "BCS";
  static constexpr char BEQ[] = "BEQ";
  static constexpr char BIT[] = "BIT";
  static constexpr char BMI[] = "BMI";
  static constexpr char BNE[] = "BNE";
  static constexpr char BPL[] = "BPL";
  static constexpr char BRK[] = "BRK";
  static constexpr char BVC[] = "BVC";
  static constexpr char BVS[] = "BVS";
  static constexpr char CLC[] = "CLC";
  static constexpr char CLD[] = "CLD";
  static constexpr char CLI[] = "CLI";
  static constexpr char CLV[] = "CLV";
  static constexpr char CMP[] = "CMP";
  static constexpr char CPX[] = "CPX";
  static constexpr char CPY[] = "CPY";
  static constexpr char DEC[] = "DEC";
  static constexpr char DEX[] = "DEX";
  static constexpr char DEY[] = "DEY";
  static constexpr char EOR[] = "EOR";
  static constexpr char INC[] = "INC";
  static constexpr char INX[] = "INX";
  static constexpr char INY[] = "INY";
  static constexpr char JMP[] = "JMP";
  static constexpr char JSR[] = "JSR";
  static constexpr char LDA[] = "LDA";
  static constexpr char LDX[] = "LDX";
  static constexpr char LDY[] = "LDY";
  static constexpr char LSR[] = "LSR";
  static constexpr char NOP[] = "NOP";
  static constexpr char ORA[] = "ORA";
  static constexpr char PHA[] = "PHA";
  static constexpr char PHP[] = "PHP";
  static constexpr char PLA[] = "PLA";
  static constexpr char PLP[] = "PLP";
  static constexpr char ROL[] = "ROL";
  static constexpr char ROR[] = "ROR";
  static constexpr char RTI[] = "RTI";
  static constexpr char RTS[] = "RTS";
  static constexpr char SBC[] = "SBC";
  static constexpr char SEC[] = "SEC";
  static constexpr char SED[] = "SED";
  static constexpr char SEI[] = "SEI";
  static constexpr char STA[] = "STA";
  static constexpr char STX[] = "STX";
  static constexpr char STY[] = "STY";
  static constexpr char TAX[] = "TAX";
  static constexpr char TAY[] = "TAY";
  static constexpr char TSX[] = "TSX";
  static constexpr char TXA[] = "TXA";
  static constexpr char TXS[] = "TXS";
  static constexpr char TYA[] = "TYA";
};

} // namespace jvs

#endif // !JVS_RIPPER2A03_MNEMONICS_H_
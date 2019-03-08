#include "instruction_visitor.h"

#include "instruction_data.h"

// X-macro for visitor function overrides, and callback accessors and mutators
#define INSTRUCTION_MNEMONIC(m) \
void jvs::CallbackInstructionVisitor::Visit##m( \
  const jvs::InstructionData& instData) \
{ \
  visit_##m##_(instData, *this); \
} \
const jvs::CallbackInstructionVisitor::CallbackVisitorFunction& jvs::CallbackInstructionVisitor::visit_##m() const \
{ \
  return visit_##m##_; \
} \
jvs::CallbackInstructionVisitor::CallbackVisitorFunction& jvs::CallbackInstructionVisitor::visit_##m() \
{ \
  return visit_##m##_;\
} \
jvs::CallbackInstructionVisitor& jvs::CallbackInstructionVisitor::set_visit_##m( \
  jvs::CallbackInstructionVisitor::CallbackVisitorFunction cb) \
{ \
  visit_##m##_ = cb; \
  return *this; \
}
#include "mnemonics.def"

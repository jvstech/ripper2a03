// This is meant to be a llvm::cl style wrapper around cxxopts. It needs to be
// improved in the future (I don't like the way option storage is handled), but
// for now it gets the job done.

#include "cl_options.h"

#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "cxxopts.hpp"

namespace
{
using OptionMap = std::map<std::string, jvs::cl::detail::BasicOption*>;
using OptionKeyList = std::vector<std::string>;

static OptionKeyList& getRegisteredOptKeys()
{
  static OptionKeyList* registeredOptKeys = new OptionKeyList();
  return *registeredOptKeys;
}

static OptionMap& getRegisteredOpts()
{
  static OptionMap* registeredOpts = new OptionMap();
  return *registeredOpts;
}

static auto addRegisteredOpt(const jvs::cl::detail::BasicOption& option)
{
  auto [insertIter, wasInserted] = getRegisteredOpts().emplace(option.name(),
    std::addressof(const_cast<jvs::cl::detail::BasicOption&>(option)));
  if (wasInserted)
  {
    getRegisteredOptKeys().push_back(insertIter->first);
  }

  return std::make_pair(insertIter, wasInserted);
}

} // namespace `anonymous-namespace'


bool jvs::cl::detail::OptionRegistrar::RegisterOption(BasicOption& option)
{
  auto [ignored, wasRegistered] = addRegisteredOpt(option);
  return wasRegistered;
}

auto jvs::cl::detail::OptionRegistrar::ParseArguments(
  const std::string& program, const std::string& description, int argc, 
  char** argv)
  -> std::optional<std::pair<std::string, bool>>
{
  std::string posHelp{};
  std::vector<std::string> posOpts{};
  std::set<std::string> groupNames{};
  auto& registeredKeys = getRegisteredOptKeys();
  std::set<std::string> remainingOptKeys(registeredKeys.begin(),
    registeredKeys.end());
  
  cxxopts::Options options(program, description);
  options.add_options()
    ("h,help", "Display help")
    ;

  const std::string posSpace{" "};
  const std::string negSpace{};
  const std::string* spacer{std::addressof(negSpace)};

  auto& registeredOpts = getRegisteredOpts();
  for (const std::string& name : registeredKeys)
  {
    auto optPtr = registeredOpts[name];
    if (optPtr->positional())
    {
      posHelp += *spacer + (optPtr->value_description().empty()
        ? name
        : optPtr->value_description());
      spacer = std::addressof(posSpace);
      posOpts.push_back(name);
    }

    
    groupNames.insert(optPtr->group());
    cxxopts::OptionAdder optAdder = options.add_options(optPtr->group());
    optPtr->AddOption(optAdder);
  }

  if (!posHelp.empty())
  {
    options.positional_help(posHelp)
      .show_positional_help();
  }

  if (!posOpts.empty())
  {
    options.parse_positional(posOpts);
  }

  try
  {
    auto results = options.parse(argc, argv);
    if (results.count("h"))
    {
      std::vector<std::string> groupNamesVec(groupNames.begin(),
        groupNames.end());
      return std::make_pair(options.help(groupNamesVec), false);
    }

    for (auto& argResult : results.arguments())
    {
      remainingOptKeys.erase(argResult.key());
    }

    for (const std::string& remainingKey : remainingOptKeys)
    {
      registeredOpts[remainingKey]->SetDefaultValue();
    }
  }
  catch (cxxopts::OptionParseException& ex)
  {
    return std::make_pair(ex.what(), true);
  }
  
  return {};
}

std::optional<std::pair<std::string, bool>> jvs::cl::ParseArguments(
  const std::string& program, const std::string& description, int argc, 
  char** argv)
{
  return detail::OptionRegistrar::ParseArguments(program, description, argc,
    argv);
}

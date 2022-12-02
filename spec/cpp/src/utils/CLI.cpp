#include <sstream>
#include <utility>
#include <string>

#include "CLI.h"

using namespace libbft;

Argument::Argument(char _shortName, bool _itHasValue, std::string _longName, std::string _defaultValue)
   : shortName(_shortName), itHasValue(_itHasValue), longName(std::move(_longName)),
     defaultValue(std::move(_defaultValue)) {
}

char Argument::getShortName() const {
   return shortName;
}

bool Argument::hasValue() const {
   return itHasValue;
}

std::string Argument::getLongName() const {
   return longName;
}

bool Argument::hasLongName() const {
   return !getLongName().empty();
}

std::string Argument::getDefaultValue() const {
   return defaultValue;
}

bool Argument::hasDefaultValue() const {
   return !getDefaultValue().empty();
}

std::string Argument::getShortNameParam() const {
   std::stringstream ss;
   ss << "-" << getShortName();
   return ss.str();
}

std::string Argument::getLongNameParam() const {
   return "--" + getLongName();
}

ArgumentParser::ArgumentParser(int argc, char **argv) {
   for (auto i = 0; i < argc; ++i) {
      args.emplace_back(std::string(argv[i]));
   }
}

ArgumentParser::ArgumentParser(std::vector<std::string> &argv) {
   for (auto &arg: argv) {
      args.emplace_back(arg);
   }
}

void ArgumentParser::addArguments(const std::vector<Argument> &arguments) {
   for (auto &arg: arguments) {
      addArgument(arg);
   }
}

void ArgumentParser::addArgument(const Argument &arg) {
   shortArgument[arg.getShortNameParam()] = arg;
   if (arg.hasLongName()) {
      longArgument[arg.getLongNameParam()] = arg;
   }
}

void ArgumentParser::parse() {
   for (auto &kv: shortArgument) {
      if (kv.second.hasDefaultValue()) {
         argumentValue[kv.first] = kv.second.getDefaultValue();
      }
   }

   auto size = args.size();
   for (auto i = 0ul; i < size; ++i) {
      if (shortArgument.find(args[i]) != shortArgument.end()) {
         auto arg = shortArgument.find(args[i]);
         if (arg->second.hasValue()) {
            if (i < size - 1) {
               argumentValue[arg->second.getShortNameParam()] = args[i++ + 1];
            } else {
               // It should treat the error
            }
         } else {
            argumentValue[arg->second.getShortNameParam()] = "";
         }
      } else if (longArgument.find(args[i]) != longArgument.end()) {
         auto arg = longArgument.find(args[i]);
         if (arg->second.hasValue()) {
            if (i < size - 1) {
               argumentValue[arg->second.getShortNameParam()] = args[i++ + 1];
            } else {
               // It should treat the error
            }
         } else {
            argumentValue[arg->second.getShortNameParam()] = "";
         }
      } else {
         // It was not found
      }
   }
}

std::string ArgumentParser::getValue(const Argument &arg) const {
   const auto &iterator = argumentValue.find(arg.getShortNameParam());
   return iterator->second;
}

bool ArgumentParser::isPresent(const Argument &arg) const {
   const auto &iterator = argumentValue.find(arg.getShortNameParam());
   return iterator != argumentValue.end();
}

#ifndef LIBBFT_UTILS_CLI
#define LIBBFT_UTILS_CLI

#include <vector>
#include <unordered_map>
#include <string>

namespace libbft {
   class Argument {
   private:
      char shortName;
      bool itHasValue;
      std::string longName;
      std::string defaultValue;
   public:
      explicit Argument(
         char shortName = '\0', bool itHasValue = false, std::string longName = "", std::string defaultValue = "");

      char getShortName() const;
      bool hasValue() const;
      std::string getLongName() const;
      bool hasLongName() const;
      std::string getDefaultValue() const;
      bool hasDefaultValue() const;

      std::string getShortNameParam() const;
      std::string getLongNameParam() const;
   };

   class ArgumentParser {
   private:
      std::vector<std::string> args;
      std::unordered_map<std::string, Argument> shortArgument;
      std::unordered_map<std::string, Argument> longArgument;
      std::unordered_map<std::string, std::string> argumentValue;
   public:
      explicit ArgumentParser(int argc, char **argv);
      explicit ArgumentParser(std::vector<std::string> &argv);

      void addArguments(const std::vector<Argument> &arguments);
      void addArgument(const Argument &arg);
      void parse();
      std::string getValue(const Argument &arg) const;
      bool isPresent(const Argument &arg) const;
   };
}

#endif

#include "Selcal.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <regex>
#include <string>
#include <unordered_set>

namespace Selcal {
namespace {

// Match any four alphanumeric SEL/ token so invalid codes can still be shown and edited.
const std::regex kSelToken(R"((\s+|^)SEL/([A-Za-z0-9]{4})(\s+|$))", std::regex::icase);

// SELCAL32 tone designators in ascending alphanumeric order (I, N, O omitted; digits after Z).
constexpr const char* kAlphabet = "ABCDEFGHJKLMPQRSTUVWXYZ123456789";

bool IsAllowedDesignator(char c)
{
  return std::strchr(kAlphabet, c) != nullptr;
}

// Index in kAlphabet, or -1 if not a SELCAL32 designator.
int ToneIndex(char c)
{
  const char* found = std::strchr(kAlphabet, c);
  if (!found) {
    return -1;
  }
  return static_cast<int>(found - kAlphabet);
}

bool HasDuplicateDesignator(const std::string& code)
{
  std::unordered_set<char> seen;
  for (char c : code) {
    if (!seen.insert(c).second) {
      return true;
    }
  }
  return false;
}

bool HasOutOfOrderPairs(const std::string& code)
{
  const int a = ToneIndex(code[0]);
  const int b = ToneIndex(code[1]);
  const int c = ToneIndex(code[2]);
  const int d = ToneIndex(code[3]);
  if (a < 0 || b < 0 || c < 0 || d < 0) {
    return true;
  }
  return a > b || c > d;
}

std::string Trim(const std::string& value)
{
  const auto start = value.find_first_not_of(" \t");
  if (start == std::string::npos) {
    return {};
  }
  const auto end = value.find_last_not_of(" \t");
  return value.substr(start, end - start + 1);
}

std::string ToUpperAlphanumeric(const std::string& input)
{
  std::string code;
  code.reserve(input.size());
  for (unsigned char c : input) {
    if (c == '-' || c == ' ' || c == '\t') {
      continue;
    }
    code.push_back(static_cast<char>(std::toupper(c)));
  }
  return code;
}

bool IsAlphanumeric(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

} // namespace

bool IsValidCode(const std::string& code)
{
  if (code.size() != 4) {
    return false;
  }
  for (char c : code) {
    if (!IsAllowedDesignator(c)) {
      return false;
    }
  }
  if (HasDuplicateDesignator(code) || HasOutOfOrderPairs(code)) {
    return false;
  }
  return true;
}

std::optional<std::string> ParseFromRemarks(const std::string& remarks)
{
  std::smatch matches;
  if (!std::regex_search(remarks, matches, kSelToken) || matches.size() < 3) {
    return std::nullopt;
  }

  std::string code = matches[2].str();
  std::transform(code.begin(), code.end(), code.begin(),
                 [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
  return code;
}

std::optional<std::string> NormalizeCode(const std::string& input)
{
  const std::string code = ToUpperAlphanumeric(input);

  if (code.empty()) {
    return std::string{};
  }
  if (code.size() != 4) {
    return std::nullopt;
  }
  for (char c : code) {
    if (!IsAlphanumeric(c)) {
      return std::nullopt;
    }
  }
  return code;
}

std::string UpdateRemarks(const std::string& remarks, const std::string& code)
{
  std::smatch matches;
  const bool hasToken = std::regex_search(remarks, matches, kSelToken);

  if (code.empty()) {
    if (!hasToken) {
      return Trim(remarks);
    }

    std::string result = remarks;
    const auto pos = static_cast<std::string::size_type>(matches.position(0));
    const auto len = static_cast<std::string::size_type>(matches.length(0));
    const bool leadingSpace = matches[1].matched && matches[1].length() > 0;
    const bool trailingSpace = matches[3].matched && matches[3].length() > 0;

    if (leadingSpace && trailingSpace) {
      result.replace(pos, len, " ");
    } else {
      result.erase(pos, len);
    }
    return Trim(result);
  }

  const std::string token = "SEL/" + code;
  if (!hasToken) {
    if (Trim(remarks).empty()) {
      return token;
    }
    return Trim(remarks) + " " + token;
  }

  std::string result = remarks;
  const auto codePos = static_cast<std::string::size_type>(matches.position(2));
  const auto codeLen = static_cast<std::string::size_type>(matches.length(2));
  const auto selPos = codePos - 4; // length of "SEL/"
  result.replace(selPos, 4 + codeLen, token);
  return result;
}

} // namespace Selcal

#pragma once

#include <optional>
#include <string>

namespace Selcal {

// Parse SEL/XXXX from remarks (any four letters). Returns nullopt if no token.
std::optional<std::string> ParseFromRemarks(const std::string& remarks);

// Normalize user input (uppercase, strip spaces/hyphens). Empty input stays empty.
// Returns nullopt if non-empty input is not exactly four letters A-Z.
std::optional<std::string> NormalizeCode(const std::string& input);

// True if code is four valid SELCAL letters with no duplicates and ordered pairs.
bool IsValidCode(const std::string& code);

// Insert or replace SEL/ABCD in remarks. Empty code removes any existing SEL/ token.
std::string UpdateRemarks(const std::string& remarks, const std::string& code);

} // namespace Selcal

#pragma once

#include <optional>
#include <string>

namespace Selcal {

// Parse SEL/XXXX from remarks (any four alphanumeric chars). Returns nullopt if no token.
std::optional<std::string> ParseFromRemarks(const std::string& remarks);

// Normalize user input (uppercase letters, strip spaces/hyphens). Empty input stays empty.
// Returns nullopt if non-empty input is not exactly four alphanumeric characters (A-Z, 0-9).
std::optional<std::string> NormalizeCode(const std::string& input);

// True if code is four valid SELCAL32 designators (A-H, J-M, P-S, T-Z, 1-9)
// with no duplicates and pairs ascending in tone order.
bool IsValidCode(const std::string& code);

// Insert or replace SEL/XXXX in remarks. Empty code removes any existing SEL/ token.
std::string UpdateRemarks(const std::string& remarks, const std::string& code);

} // namespace Selcal

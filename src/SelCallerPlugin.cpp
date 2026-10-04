#include "SelCallerPlugin.h"

#include "Selcal.h"
#include "Version.h"

#include <cstring>
#include <string>

namespace {
constexpr int TAG_ITEM_SELCAL = 1;
constexpr int TAG_FUNC_EDIT_SELCAL = 100;
constexpr int TAG_FUNC_SELCAL_SUBMIT = 101;
} // namespace

SelCallerPlugin::SelCallerPlugin()
    : CPlugIn(EuroScopePlugIn::COMPATIBILITY_CODE,
              PLUGIN_NAME,
              PLUGIN_VERSION,
              PLUGIN_AUTHOR,
              PLUGIN_LICENSE)
{
  RegisterTagItemType("SELCAL", TAG_ITEM_SELCAL);
  RegisterTagItemFunction("Edit SELCAL", TAG_FUNC_EDIT_SELCAL);

  DisplayMessage(std::string("Version ") + PLUGIN_VERSION + " loaded", "Initialisation");
}

void SelCallerPlugin::DisplayMessage(const std::string& message, const std::string& sender)
{
  DisplayUserMessage(PLUGIN_NAME, sender.c_str(), message.c_str(), true, true, false, false, false);
}

void SelCallerPlugin::OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
                                   EuroScopePlugIn::CRadarTarget /*RadarTarget*/,
                                   int ItemCode,
                                   int /*TagData*/,
                                   char sItemString[16],
                                   int* pColorCode,
                                   COLORREF* pRGB,
                                   double* /*pFontSize*/)
{
  if (ItemCode != TAG_ITEM_SELCAL || !FlightPlan.IsValid()) {
    return;
  }

  const char* remarks = FlightPlan.GetFlightPlanData().GetRemarks();
  const auto code = Selcal::ParseFromRemarks(remarks ? remarks : "");
  if (!code) {
    sItemString[0] = '\0';
    return;
  }

  strncpy_s(sItemString, 16, code->c_str(), _TRUNCATE);

  // Warn on ICAO-invalid codes (wrong letters, duplicates, out-of-order pairs).
  if (!Selcal::IsValidCode(*code) && pColorCode && pRGB) {
    *pColorCode = EuroScopePlugIn::TAG_COLOR_RGB_DEFINED;
    *pRGB = RGB(255, 165, 0);
  }
}

void SelCallerPlugin::OnFunctionCall(int FunctionId,
                                     const char* sItemString,
                                     POINT /*Pt*/,
                                     RECT Area)
{
  EuroScopePlugIn::CFlightPlan fp = FlightPlanSelectASEL();
  if (!fp.IsValid()) {
    return;
  }

  EuroScopePlugIn::CFlightPlanData data = fp.GetFlightPlanData();
  const char* remarksRaw = data.GetRemarks();
  const std::string remarks = remarksRaw ? remarksRaw : "";

  if (FunctionId == TAG_FUNC_EDIT_SELCAL) {
    const auto current = Selcal::ParseFromRemarks(remarks);
    OpenPopupEdit(Area, TAG_FUNC_SELCAL_SUBMIT, current ? current->c_str() : "");
    return;
  }

  if (FunctionId != TAG_FUNC_SELCAL_SUBMIT) {
    return;
  }

  const std::string input = sItemString ? sItemString : "";
  const auto normalized = Selcal::NormalizeCode(input);
  if (!normalized) {
    DisplayMessage("SELCAL must be four letters (e.g. ABCD or AB-CD), or empty to clear.",
                   "Edit SELCAL");
    return;
  }

  const std::string updated = Selcal::UpdateRemarks(remarks, *normalized);
  if (!data.SetRemarks(updated.c_str())) {
    DisplayMessage("Failed to update flight plan remarks.", "Edit SELCAL");
    return;
  }
  if (!data.AmendFlightPlan()) {
    DisplayMessage("Failed to amend flight plan.", "Edit SELCAL");
    return;
  }
}

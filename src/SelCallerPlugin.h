#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#pragma warning(push, 0)
#include "EuroScopePlugIn.h"
#pragma warning(pop)

#include <string>

class SelCallerPlugin : public EuroScopePlugIn::CPlugIn
{
public:
  SelCallerPlugin();
  ~SelCallerPlugin() override = default;

  void OnGetTagItem(EuroScopePlugIn::CFlightPlan FlightPlan,
                    EuroScopePlugIn::CRadarTarget RadarTarget,
                    int ItemCode,
                    int TagData,
                    char sItemString[16],
                    int* pColorCode,
                    COLORREF* pRGB,
                    double* pFontSize) override;

  void OnFunctionCall(int FunctionId,
                      const char* sItemString,
                      POINT Pt,
                      RECT Area) override;

private:
  void DisplayMessage(const std::string& message, const std::string& sender = "Selcaller");
};

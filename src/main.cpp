#include <memory>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#pragma warning(push, 0)
#include "EuroScopePlugIn.h"
#pragma warning(pop)

#include "SelCallerPlugin.h"

namespace {
std::unique_ptr<SelCallerPlugin> g_plugin;
}

void __declspec(dllexport) EuroScopePlugInInit(EuroScopePlugIn::CPlugIn** ppPlugInInstance)
{
  g_plugin = std::make_unique<SelCallerPlugin>();
  *ppPlugInInstance = g_plugin.get();
}

void __declspec(dllexport) EuroScopePlugInExit(void)
{
  g_plugin.reset();
}

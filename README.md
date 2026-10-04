# Selcaller

EuroScope plugin to display and edit a pilot’s SELCAL code stored in flight-plan remarks as `SEL/ABCD`.

## Features

- **SELCAL** list/TAG item — shows the current code (`ABCD`), or blank if none
- **Edit SELCAL** list/TAG action — popup edit to add, change, or clear the code

Accepted input formats: `ABCD`, `AB-CD`, lowercase. Clearing the edit box removes `SEL/...` from remarks.

ICAO-invalid codes (wrong letters, duplicates, out-of-order pairs) are still allowed and shown in **orange** as a warning.

## Requirements

- EuroScope 3.2 (32-bit)
- Visual Studio 2019+ with **Desktop development with C++** (MSVC)
- CMake 3.14+
- `EuroScopePlugInDll.lib` from the EuroScope plugin SDK (`%APPDATA%\EuroScope\PlugIn`)

## Setup

1. Copy `EuroScopePlugInDll.lib` (and optionally the matching `EuroScopePlugIn.h`) from `%APPDATA%\EuroScope\PlugIn` into `external/lib/` / `external/include/`. This repo already vendors those files when available.
2. Configure and build as **Win32** (x86) with MSVC:

```powershell
cmake -S . -B build -A Win32
cmake --build build --config Release
```

The DLL is written to `build/Release/selcaller.dll` (or `build/Debug/selcaller.dll`).

## Load in EuroScope

1. Other Settings → Plug-ins → Load → select `selcaller.dll`
2. Open the list (or TAG) editor for the list you use
3. Add item type **SELCAL** as a column
4. Assign function **Edit SELCAL** to the click action on that column (or another item)

After editing, the remarks field contains `SEL/ABCD` and the flight plan is amended so other controllers see the update.

## Releases

GitHub Actions uses [release-please](https://github.com/googleapis/release-please) on pushes to `main`.

- Use [Conventional Commits](https://www.conventionalcommits.org/) (`feat:`, `fix:`, `feat!:`, etc.) so release-please can open a release PR
- Merging that PR creates a GitHub Release and tags the version
- CI then builds the Win32 `selcaller.dll` and attaches it to the release

Published builds are also available from the Actions **ci** workflow artifacts on every push/PR.

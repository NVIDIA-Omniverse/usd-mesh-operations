# Changelog

## 2026-06-25

- Updated all four vendored packages to their latest published builds:
  - `omnimesh_ops_usd` → `1.1.0-r4` (headers reorganized: added `Cleanup.h` and `UsdIO.h`; USD headers flattened from `OmniMeshOps/usd/` into `OmniMeshOps/`).
  - `mesh_tools` → `mesh_tools_lib` `1.0.0.1ed0a585` (now a shared library `libmesh_tools_lib` / `mesh_tools_lib.dll`, replacing the previous static `libmesh_tools.a`).
  - `shrinkwrap_openvdb` → `13.0.1+mr4.4.d98d25a3.gl`.
  - `autouv-core` → `1.0.2+main.39.ee30cdcc.gl`.
- Added libraries for both USD 25.05 and USD 25.11 builds. USD-dependent packages now nest binaries under `lib/usd25.05/` and `lib/usd25.11/`. `mesh_tools` remains USD-agnostic under `lib/<platform>`.

## Initial release — 2026-05-08

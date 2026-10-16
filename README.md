# USD Mesh Operations — Vendored Dependencies

Prebuilt C++ libraries and headers consumed by [Omniverse Scene Optimizer](https://docs.omniverse.nvidia.com/extensions/latest/ext_scene-optimizer.html) (`omni.scene.optimizer.core` and related extensions) and other NVIDIA USD tooling. This repository pins known-good binary and header layouts so consuming projects can build without pulling dependencies from multiple sources at configure time.

This project is currently not accepting contributions.

## Links

| Topic | Where |
| --- | --- |
| **Repository** | [GitHub — `usd-mesh-operations`](https://github.com/NVIDIA-Omniverse/usd-mesh-operations) |
| **Related product docs** | [Scene Optimizer extension](https://docs.omniverse.nvidia.com/extensions/latest/ext_scene-optimizer.html) · [User manual](https://docs.omniverse.nvidia.com/scene-optimizer/latest/user-manual.html) |
| **Security** | [SECURITY.md](SECURITY.md) |
| **Governance** | [Code of Conduct](CODE_OF_CONDUCT.md) |

## Packages

| Package | Description |
| --- | --- |
| `vendor/omnimesh_ops_usd` | USD-facing mesh operations — remesh, subdivide, triangulate, decimate, normals, slicing, attribute transfer, and more. |
| `vendor/mesh_tools` | Mesh utilities — decimation, reconstruction, USD loading, visibility analysis, and related helpers. |
| `vendor/shrinkwrap_openvdb` | Shrinkwrap tooling built on OpenVDB for watertight mesh generation from level-set volumes. |
| `vendor/autouv-core` | AutoUV core API for automatic UV unwrapping with low distortion. |

Each package provides:

- `include/` — C++ headers to add to your compiler's include path (identical across USD versions and platforms).
- `lib/` — prebuilt shared/static libraries to link against, organized by USD build and platform (see layout below).

## Vendored versions

These prebuilts were published to the internal packman registry and are pinned here. Only `release` builds are vendored. The exact, fully-qualified packman package identifiers — together with archive sizes and the UTC publish time of each build — are recorded per platform and USD version in [`vendor/VERSIONS.yaml`](vendor/VERSIONS.yaml).

| Package | Packman package | Version | USD builds |
| --- | --- | --- | --- |
| `omnimesh_ops_usd` | `omnimesh_ops_usd` | `1.1.0-r4` | 25.05, 25.11 |
| `mesh_tools` | `mesh_tools_lib` | `1.0.0.1ed0a585` | USD-agnostic (single build) |
| `shrinkwrap_openvdb` | `shrinkwrap_openvdb` | `13.0.1+mr4.4.d98d25a3.gl` | 25.05, 25.11 |
| `autouv-core` | `autouv-core` | `1.0.2+main.39.ee30cdcc.gl` | 25.05, 25.11 |

## Library layout

USD-dependent packages carry a separate set of binaries per OpenUSD release, under a `usd25.05` / `usd25.11` subfolder, then by platform:

```
vendor/<package>/lib/usd25.05/<platform>/...
vendor/<package>/lib/usd25.11/<platform>/...
```

`mesh_tools` is USD-agnostic (a single build serves all USD versions), so it omits the USD level:

```
vendor/mesh_tools/lib/<platform>/...
```

Supported `<platform>` values:

| Platform | Architecture | Folder |
| --- | --- | --- |
| Linux | x86_64 | `manylinux_2_35_x86_64` |
| Linux | aarch64 | `manylinux_2_35_aarch64` |
| Windows | x86_64 | `windows-x86_64` |

## Usage

1. Clone this repository (or add it as a submodule) next to or under your main project.
2. Point your build system's include and link search paths at `vendor/<package>/include` and the matching `vendor/<package>/lib/usd<version>/<platform>` directory for the USD release you are building against (`mesh_tools` uses `vendor/mesh_tools/lib/<platform>`).
3. Keep this repository in sync with the Scene Optimizer release you are building against.

## License

This repository is distributed under the terms in [LICENSE](LICENSE). Individual headers under `vendor/` carry their own SPDX license identifiers. Comply with all applicable terms when you use, build against, or redistribute these materials.

Copyright (c) 2024-2026, NVIDIA CORPORATION.

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

- `include/` — C++ headers to add to your compiler's include path.
- `lib/` or `release/` — prebuilt shared/static libraries to link against.

## Supported Platforms

| Platform | Architecture |
| --- | --- |
| Linux | x86_64 |
| Linux | aarch64 |
| Windows | x86_64 |

## Usage

1. Clone this repository (or add it as a submodule) next to or under your main project.
2. Point your build system's include and link search paths at the `vendor/<package>/include` and `vendor/<package>/lib` (or `release`) directories for the packages you need.
3. Keep this repository in sync with the Scene Optimizer release you are building against.

## License

This repository is distributed under the terms in [LICENSE](LICENSE). Individual headers under `vendor/` carry their own SPDX license identifiers. Comply with all applicable terms when you use, build against, or redistribute these materials.

Copyright (c) 2024-2026, NVIDIA CORPORATION.

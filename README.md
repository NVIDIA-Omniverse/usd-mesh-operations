# scene-optimizer-dependencies

Vendored third-party libraries and headers used by the Omniverse scene optimizer and related tooling. This repository exists so builds can pin known-good binary and header layouts without pulling from multiple sources at configure time.

## Layout

| Package | Purpose (summary) |
|--------|---------------------|
| `vendor/mesh_tools` | Mesh utilities (decimation, reconstruction, USD loading, visibility, etc.). |
| `vendor/omnimesh_ops_usd` | USD-facing mesh operations (remesh, subdivide, triangulate, attributes, etc.). |
| `vendor/shrinkwrap_openvdb` | Shrinkwrap tooling built on OpenVDB. |
| `vendor/autouv-core` | AutoUV core API headers. |

Each package under `vendor/` typically provides:

- `include/` — C++ headers to add to your compiler’s include path.
- `lib/` or `release/` — prebuilt libraries to link, when present.

Exact include names and library filenames depend on the consuming project’s build system; point your toolchain at the subdirectories above for the packages you need.

## Usage

1. Clone this repository (or add it as a submodule) next to or under your main project.
2. Configure your build so include directories and link search paths reference the `vendor/<package>/include` and `vendor/<package>/lib` (or `release`) paths you depend on.
3. Keep this repo in sync with the scene optimizer release you are building against when versions change.

## License

This repository is distributed under the terms in [LICENSE](LICENSE). Most headers under `vendor/` are marked `SPDX-License-Identifier: LicenseRef-NvidiaProprietary` in source; any separate SPDX or third-party notice files in this tree also apply. Comply with all applicable terms when you use, build against, or redistribute these materials.

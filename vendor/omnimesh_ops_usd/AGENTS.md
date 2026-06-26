<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

# AGENTS.md

Cross-agent entry point for any coding agent working with the omnimesh_ops_usd library.

## Overview

omnimesh_ops_usd is a C++ mesh-processing library (namespace `omo`) providing GPU- and CPU-accelerated operations on polygon meshes. It is consumed as a vendor dependency: C++ headers under `include/OmniMeshOps/` linked against pre-built shared libraries.

The public surface is a single flat header tree (`OmniMeshOps/*.h`). USD interop is provided by **free functions** in `OmniMeshOps/UsdIO.h` (there is no longer an `omo::usd` namespace or `OmniMeshOps/usd/` subfolder). Core mesh types and operations are residency-templated on `Host` (CPU) and `Device` (CUDA).

## Skills

Reusable, task-specific instructions live under `.claude/skills/<name>/SKILL.md`. They are plain Markdown — any agent can read and follow them.

When a task could match a skill, inventory `.claude/skills/` and read each `SKILL.md`'s YAML frontmatter `description` field to decide which (if any) applies.

## Key Types

| Type | Header | Purpose |
|------|--------|---------|
| `Mesh<R>` | `Mesh.h` | Central mesh container (vertices, faces, attributes, subsets) |
| `MeshData<R>` | `MeshData.h` | Lightweight mesh container; built from raw arrays, used by slice/clip/coorient/reverse/cleanup |
| `Attribute<R>` | `Attribute.h` | Per-vertex / per-corner / per-face / constant data attached to a mesh |
| `Subset<R>` | `Subset.h` | Named index subset (material assignments, selections) |
| `ScopedCudaContext` | `ScopedCudaContext.h` | RAII helper for GPU operations |

`R` is `omo::Host` (CPU) or `omo::Device` (CUDA). Cross-residency copies are supported via explicit conversion constructors, e.g. `DeviceMesh(hostMesh)` and `HostMesh(deviceMesh)`. Convenience aliases `HostMesh`/`DeviceMesh`, `HostMeshData`/`DeviceMeshData`, etc. are provided.

## USD I/O (`UsdIO.h`)

All USD bridging is done with free functions:

| Function | Direction | Notes |
|----------|-----------|-------|
| `importMesh(usdGeomMesh, opts, filters)` | USD → `HostMesh` | `opts` is `CleanupOptions` (default: no cleanup) |
| `importMeshData(usdGeomMesh, opts, filters)` | USD → `HostMeshData` | lightweight; used by slice/clip/coorient/reverse |
| `importAttribute(usdGeomPrimvar)` | USD → `HostAttribute` | |
| `importSubset(parentGeom, usdGeomSubset)` | USD → `HostSubset` | |
| `exportUsdGeomMesh(meshOrMeshData, stage, primPath)` | `HostMesh`/`HostMeshData` → USD | overwrites the prim at `primPath` |
| `exportUsdGeomGprim(primitive, stage, primPath)` | `Primitive` → USD | writes a fitted sphere/cylinder/cone/cube |
| `importUpAxis(tfToken, default)` | USD token → `PrimitiveUpAxis` | |
| `gfMatrixTransform(primitive)` | `Primitive` → `GfMatrix4d` | |
| `removeMeshSpecificAttributes(gprim)` / `removeShapeSpecificAttributes(gprim)` | — | cleanup helpers after conversions |

`#include <OmniMeshOps/UsdIO.h>` pulls in `UsdIncludes.h` (the pxr headers) and the core mesh types.

## Operations

| Operation | Header | Description |
|-----------|--------|-------------|
| `Decimate` | `Decimate.h` | Reduce vertex count with error-bounded decimation |
| `Subdivide` | `Subdivide.h` | Catmull-Clark or Loop subdivision |
| `Triangulate` | `Triangulate.h` | Convert polygonal faces to triangles |
| `Remesh` | `Remesh.h` | Isotropic remeshing with gradation control |
| `PrimitiveFit` | `Primitive.h` | Fit sphere/cylinder/cone/cube to a mesh |
| `MeshAttributeXfer` | `MeshAttributeXfer.h` | Transfer attributes between meshes |
| `Select` | `Select.h` | Select subset of mesh by attribute value range |
| `manifold()` | `Manifold.h` | Make a mesh manifold |
| `coorient()` | `Coorient.h` | Make face windings consistent |
| `reverse()` | `Reverse.h` | Reverse face winding order |
| `slice()` / `gridSlice()` / `boxClip()` | `Slice.h` | Cut mesh by planes, grids, or AABBs |
| `cornerNormals()` / `faceNormals()` / `vertexNormals()` | `Normals.h` | Compute normals with various bindings |
| `cleanup()` | `Cleanup.h` | Repair mesh defects (merge/contract/remove/coorient/manifold) |
| `createPrimitiveMesh()` | `Primitive.h` | Generate mesh geometry for a primitive type |

Decimate, Subdivide, Triangulate, Remesh, PrimitiveFit, MeshAttributeXfer and Select are classes constructed from a mesh and invoked via `operator()`. Manifold, coorient, reverse, slice/gridSlice/boxClip and cleanup are free functions.

## Validation Functions

| Function | Header | Checks |
|----------|--------|--------|
| `checkClean()` | `Cleanup.h` | Reports which of the requested `Defect`s are present (the read-only counterpart to `cleanup()`) |
| `checkNormalsUnitLength()` | `Normals.h` | Normals are unit-length |
| `checkNormalsConsistentWithWinding()` | `Normals.h` | Normals agree with face winding |
| `checkAttributeFinite()` | `Attribute.h` | Attribute values are finite (no NaN/Inf) |

## Usage Pattern

```cpp
#include <OmniMeshOps/Decimate.h>
#include <OmniMeshOps/UsdIO.h>

using namespace omo;

// 1. Read from USD (optionally requesting cleanup of manifest defects)
HostMesh inputMesh = importMesh(usdGeomMesh, { manifestDefects });

// 2. Optionally move to GPU
DeviceMesh deviceMesh(inputMesh);

// 3. Run an operation
DeviceDecimate decimate(deviceMesh, /*guideAttribute=*/{});
DeviceMesh result = decimate(targetVertexCount, maxMeanError);

// 4. Copy back to host
HostMesh hostResult(result);

// 5. Write to USD
exportUsdGeomMesh(hostResult, stage, primPath);
```

## Cleanup & Defects

Cleanup is decoupled from construction. Build/import a mesh, then call `cleanup(meshData, options)` (or request defects up front via the `CleanupOptions` argument to `importMesh`/`importMeshData`).

`CleanupOptions` (`Defines.h`) has `EnumSet<Defect> fixes` and `float vertexMergeTolerance`. The `Defect` enum:

- `CoincidentNeighborVertices` — vertices coincident with another vertex on the same face
- `CoincidentBoundaryVertices` — boundary vertices coincident with a boundary vertex on a different face
- `DegenerateEdges` — consecutively repeated vertex references around faces
- `DegenerateFaces` — faces with fewer than 3 distinct vertices
- `IsolatedVertices` — vertices not referenced by any face
- `DuplicateFaces` — duplicate copies (up to cyclic rotation) of a face
- `InconsistentlyOrientedFaces` — neighbouring faces with disagreeing winding
- `Nonmanifold` — edges shared by >2 faces, or non-disk vertex stars

Named `EnumSet<Defect>` constants: `noDefects`, `manifestDefects` (neighbor-coincident + degenerate edges/faces), `legacyManifestDefects`, `coincidentVertexDefects`, `allDefects`. Combine sets/values with `+`, `-`, `&`.

## Enums & Configuration

- `Residency` — `Host` or `Device`
- `Binding` — `None`, `Constant`, `PerFace`, `PerVertex`, `PerCorner`
- `DataType` — scalar, vector, matrix types (float/double/int variants)
- `Role` — `None`, `Tangent`, `Normal`, `UV`, `Color`, `Material`, `Sharpness`
- `Defect` — see above (used by `CleanupOptions`)
- `SubdivisionMethod` — `CatmullClark`, `Loop`
- `WeightMode` — `Angle`, `Area` (for vertex normal computation)
- `Orientation` — `RightHanded`, `LeftHanded`
- `PrimitiveType` — `Sphere`, `Cylinder`, `Cone`, `Cube`, `None`; `PrimitiveTypeMask` for fit filtering
- `PrimitiveUpAxis` — `X`, `Y`, `Z`
- `CoorientOptions` — `None`, `FlipNormalsOfReversedFaces`
- `ReverseOptions` — `None`, `FlipNormals`
- `Validation` — `{ bool passed; std::string message; }`, convertible to `bool`

## Logging

`omo::log` namespace (`Log.h`) provides configurable output/error streams and log levels (Error, Warning, Info, Debug) with optional ANSI coloring.

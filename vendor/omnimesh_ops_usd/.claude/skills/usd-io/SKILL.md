<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: usd-io
description: Read meshes from and write meshes to OpenUSD UsdGeomMesh prims using the free functions in OmniMeshOps/UsdIO.h.
allowed-tools: Read, Glob, Grep
---

# Read/Write Meshes via USD

Bridge between OpenUSD `UsdGeomMesh` prims and `omo::Mesh` / `omo::MeshData`. All USD interop is done with **free functions** in `OmniMeshOps/UsdIO.h` (there is no `omo::usd` namespace).

## Headers

```cpp
#include <OmniMeshOps/UsdIO.h>        // import*/export* free functions (also pulls in core types + pxr)
// #include <OmniMeshOps/UsdIncludes.h>  // included transitively; provides PXR_NS / UsdGeomMesh etc.
```

## Read from USD — Full Mesh

For operations that need the full topology graph (Decimate, Subdivide, Triangulate, Remesh, Manifold, PrimitiveFit, MeshAttributeXfer):

```cpp
using namespace omo;
PXR_NS::UsdGeomMesh usd_mesh(prim);

HostMesh mesh = importMesh(usd_mesh);                    // no cleanup (default CleanupOptions)
HostMesh mesh = importMesh(usd_mesh, { manifestDefects }); // request cleanup of manifest defects on import
```

## Read from USD — Filtered Attributes

The third argument restricts which primvars/attributes are imported:

```cpp
HostMesh mesh = importMesh(usd_mesh, { noDefects }, { "primvars:st", "primvars:normals" });
```

## Read from USD — Lightweight MeshData

For operations that work on `MeshData` (Slice, GridSlice, BoxClip, Coorient, Reverse, Cleanup):

```cpp
HostMeshData mesh_data = importMeshData(usd_mesh);
HostMeshData mesh_data = importMeshData(usd_mesh, { noDefects }, { "normals", "primvars:normals" });
```

## Write to USD

`exportUsdGeomMesh` overwrites the prim at `prim_path`. It is overloaded for both `HostMesh` and `HostMeshData`:

```cpp
exportUsdGeomMesh(result, stage, prim.GetPath().GetAsString());
exportUsdGeomGprim(primitive, stage, primPath);   // for a fitted omo::Primitive
```

## Single Attribute / Subset

```cpp
HostAttribute attr = importAttribute(usdGeomPrimvar);
HostSubset    subset = importSubset(parentGeom, usdGeomSubset);
```

## Helpers

```cpp
PrimitiveUpAxis up = importUpAxis(PXR_NS::UsdGeomGetStageUpAxis(stage)); // token -> enum
PXR_NS::GfMatrix4d tm = gfMatrixTransform(primitive);
removeMeshSpecificAttributes(PXR_NS::UsdGeomGprim(prim));
removeShapeSpecificAttributes(PXR_NS::UsdGeomGprim(prim));
```

## Key Notes

- The default `CleanupOptions{}` requests **no** cleanup. Pass `{ manifestDefects }` (or another defect set) to clean on import, or run `cleanup()` separately (see the `validate` skill).
- `stage` is passed as a `UsdStageWeakPtr`; a `UsdStageRefPtr` from `UsdStage::Open`/`CreateInMemory` converts implicitly.
- `importMesh` returns a value (`HostMesh`); move it into containers as needed.
- To select prims to process, traverse the stage and keep prims where `prim.IsA<PXR_NS::UsdGeomMesh>()`.

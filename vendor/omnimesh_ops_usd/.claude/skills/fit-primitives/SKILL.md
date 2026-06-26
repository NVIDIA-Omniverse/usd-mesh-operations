<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: fit-primitives
description: Fit geometric primitives (sphere, cylinder, cone, cube) to a mesh, and generate primitive meshes.
allowed-tools: Read, Glob, Grep
---

# Fit Primitives to a Mesh

Determine whether a mesh can be approximated by a geometric primitive, and (optionally) write the primitive back to USD or regenerate mesh geometry.

## Headers

```cpp
#include <OmniMeshOps/Primitive.h>
#include <OmniMeshOps/UsdIO.h>
```

## CPU Path — Mesh to Primitive

```cpp
using namespace omo;

HostMesh inputMesh = importMesh(usdGeomMesh);
PrimitiveUpAxis up = importUpAxis(PXR_NS::UsdGeomGetStageUpAxis(stage));

HostPrimitiveFit fit(inputMesh);
Primitive result = fit(
    up,
    int(PrimitiveTypeMask::All),
    /*volume_tol=*/0.01,
    /*vertex_tol=*/0.01,
    /*allow_negative_volume=*/true,
    /*allow_missing_endcaps=*/true);

if (result.type != PrimitiveType::None)
    exportUsdGeomGprim(result, stage, primPath);   // writes a UsdGeomSphere/Cylinder/Cone/Cube
```

## GPU Path

```cpp
ScopedCudaContext cudaCtx(Verbose{ false });
DevicePrimitiveFit fit(DeviceMesh{ inputMesh });
Primitive result = fit(up, int(PrimitiveTypeMask::All), 0.01, 0.01);
```

## Generate Mesh from Primitive

```cpp
HostMesh primMesh = createPrimitiveMesh<Host>(
    PrimitiveType::Cylinder,
    CylinderMeshParameters(32, 1, true),   // n_radial, n_length, capped
    bakedTransform,                        // std::array<double,16> row-major (default identity)
    localToWorldTransform);                // std::array<double,16> row-major (default identity)
```

## Primitive Types and Masks

- `PrimitiveType::Sphere`, `Cylinder`, `Cone`, `Cube`, and `None` (no fit found)
- Restrict which types to attempt with the `PrimitiveTypeMask` bitmask (`Sphere`/`Cylinder`/`Cone`/`Cube`/`All`)
- Custom fit logic via the `PrimitiveFitEvaluator` interface (`DefaultPrimitiveFitEvaluator` is the default)

## Mesh Parameters for Generation

- `SphereMeshParameters(n_longitude=32, n_latitude=16)`
- `CylinderMeshParameters(n_radial=32, n_length=1, endcaps=true)`
- `ConeMeshParameters(n_radial=64, n_length=3, base=true)`
- `CubeMeshParameters()` — no tessellation parameters

## Key Notes

- `Primitive` is `{ std::array<double,16> transform; PrimitiveType::Enum type; }`; `transform` is row-major.
- `importUpAxis(token, default)` converts a USD up-axis token to `PrimitiveUpAxis`; `gfMatrixTransform(primitive)` returns the transform as a `GfMatrix4d`.
- After mesh↔prim conversions, tidy the prim with `removeMeshSpecificAttributes(gprim)` / `removeShapeSpecificAttributes(gprim)` (`UsdIO.h`).
- To regularize a fitted prim back into a mesh that keeps the original attributes, generate it with `createPrimitiveMesh` and run a `HostMeshAttributeXfer` (see the `attribute-transfer` skill).

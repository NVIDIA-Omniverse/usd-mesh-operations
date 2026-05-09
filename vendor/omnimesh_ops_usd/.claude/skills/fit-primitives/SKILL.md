<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: fit-primitives
description: Fit geometric primitives (sphere, cylinder, cone, cube) to a mesh, and generate primitive meshes.
allowed-tools: Read, Glob, Grep
---

# Fit Primitives to a Mesh

Determine if a mesh can be approximated by a geometric primitive.

## Headers

```cpp
#include <OmniMeshOps/Primitive.h>
#include <OmniMeshOps/usd/Mesh.h>
#include <OmniMeshOps/usd/Prim.h>
```

## CPU Path

```cpp
omo::usd::HostMesh inputMesh(usdGeomMesh);
omo::HostPrimitiveFit fit(inputMesh);
omo::Primitive result = fit(
    omo::PrimitiveUpAxis::Z,
    int(omo::PrimitiveTypeMask::All),
    volumeTolerance,       // default 0.01
    vertexTolerance,       // default 0.01
    allowNegativeVolume,   // default true
    allowMissingEndcaps    // default true
);
if (result.type != omo::PrimitiveType::None) {
    omo::usd::HostPrim(result).writeToUsd(primPath, stage);
}
```

## GPU Path

```cpp
omo::ScopedCudaContext cudaCtx(omo::Verbose{false});
omo::DevicePrimitiveFit fit(omo::DeviceMesh{inputMesh});
omo::Primitive result = fit(upAxis, typeMask, volumeTol, vertexTol);
```

## Generate Mesh from Primitive

```cpp
omo::HostMesh primMesh = omo::createPrimitiveMesh<omo::Host>(
    omo::PrimitiveType::Cylinder,
    omo::CylinderMeshParameters(32, 1, true),  // n_radial, n_axial, capped
    bakedTransform,
    localToWorldTransform
);
```

## Primitive Types and Masks

- `PrimitiveType::Sphere`, `Cylinder`, `Cone`, `Cube`
- Control which types to attempt with `PrimitiveTypeMask` bitmask
- Custom fit evaluation via `PrimitiveFitEvaluator` interface

## Mesh Parameters for Generation

- `SphereMeshParameters(n_longitude, n_latitude)` — defaults 32, 16
- `CylinderMeshParameters(n_radial, n_length, endcaps)` — defaults 32, 1, true
- `ConeMeshParameters(n_radial, n_length, base)` — defaults 64, 3, true
- `CubeMeshParameters()` — no tessellation parameters

## Key Notes

- `result.transform` contains the 4x4 row-major transform matrix
- `omo::usd::HostPrim::convertUpAxis()` converts USD up-axis tokens to `PrimitiveUpAxis`
- `omo::usd::HostPrim::removeMeshSpecificAttributesAndPrimvarsFromGprim()` cleans up after mesh-to-prim conversion

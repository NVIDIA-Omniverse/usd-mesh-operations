<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: compute-normals
description: Compute or fix mesh normals (corner, face, vertex, or auto-binding) with sharpness angle and weighting control.
allowed-tools: Read, Glob, Grep
---

# Compute Normals

Generate normal attributes with various bindings and weighting modes. These are free functions in `Normals.h` that return an `Attribute` you then attach to the mesh.

## Headers

```cpp
#include <OmniMeshOps/Normals.h>
#include <OmniMeshOps/UsdIO.h>
```

## Corner / Face / Vertex Normals

```cpp
using namespace omo;

HostMesh mesh = importMesh(usdGeomMesh);

HostAttribute cornerNorm = cornerNormals(mesh, /*sharpnessAngleDeg=*/60.0f, WeightMode::Angle);
HostAttribute faceNorm   = faceNormals(mesh);
HostAttribute vertNorm   = vertexNormals(mesh, WeightMode::Area);
```

## Auto-binding Normals

```cpp
// Computes corner normals, then demotes to per-vertex or per-face if lossless.
HostAttribute attr = normals(mesh, /*sharpnessAngleDeg=*/60.0f, WeightMode::Angle);
```

## Fix Existing Normals

```cpp
auto existing = mesh.getAttribute("primvars:normals");
HostAttribute fixed = cornerNormals(mesh, existing, sharpnessAngleDeg, WeightMode::Angle);
```

## Attach and Write

```cpp
mesh.attachAttribute(attr, /*substitute_existing=*/true);
exportUsdGeomMesh(mesh, stage, primPath);
```

## Parameters

- `sharpnessAngleDeg` — dihedral angle threshold for sharp edges (degrees, default 60; range ]-180, 180], 0 = flat)
- `WeightMode` — `Angle` or `Area`

## Validation

```cpp
auto [unitOk, msg]  = checkNormalsUnitLength(attr, /*tolerance=*/1e-4, CheckNormalsOptions::Strict);
auto [windOk, msg2] = checkNormalsConsistentWithWinding(meshData, attr, Orientation::RightHanded);
```

## Key Notes

- The fix-existing overloads normalize finite nonzero entries and recompute the rest.
- `normals()` is the auto-binding variant (compute corner normals, then demote binding if lossless).
- GPU path: the same functions accept `DeviceMesh` and return `DeviceAttribute`.
- `CheckNormalsOptions::AllowZeroLength` ignores exactly-zero vectors during the unit-length check.

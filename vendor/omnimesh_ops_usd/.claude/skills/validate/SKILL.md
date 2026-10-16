---
name: validate
description: Check mesh topology defects (checkClean) and normals (unit-length, winding consistency) without modifying the mesh.
allowed-tools: Read, Glob, Grep
license: LicenseRef-NvidiaProprietary
metadata:
  version: "1.0.0"
---

<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

# Validate Mesh Data

Topology validation is the read-only counterpart of `cleanup()`: `checkClean()` reports which of the requested `Defect`s are present. Normals validation lives in `Normals.h`.

## Headers

```cpp
#include <OmniMeshOps/Cleanup.h>   // cleanup() + checkClean()
#include <OmniMeshOps/Normals.h>   // checkNormalsUnitLength / checkNormalsConsistentWithWinding
#include <OmniMeshOps/UsdIO.h>     // importMeshData
```

## Topology: checkClean

`checkClean(meshData, options)` reports every defect from `options.fixes` that is present. Its result is convertible to `bool` (true == clean) and carries a `presentDefects` set and a `message`.

```cpp
using namespace omo;

HostMeshData mesh_data = importMeshData(usd_mesh);

CleanupOptions options;
options.fixes = allDefects;          // or manifestDefects, or a hand-built set
options.vertexMergeTolerance = 0.0f; // L-infinity coincidence tolerance

CleanupValidation result = checkClean(mesh_data, options);
if (!result) {
    // result.presentDefects : EnumSet<Defect> of offending defects
    // result.message        : newline-joined, one line per reported defect
    std::cout << result.message << "\n";
}
```

To actually repair, call `cleanup()` with the same options (returns a new `MeshData`):

```cpp
HostMeshData cleaned = cleanup(mesh_data, options);
```

Both run on the GPU by wrapping the input in a `DeviceMeshData`:

```cpp
auto result  = checkClean(DeviceMeshData(mesh_data), options);
auto cleaned = HostMeshData(cleanup(DeviceMeshData(mesh_data), options));
```

## Normals checks

```cpp
auto normals_attr = mesh.getAttribute(Role::Normal);   // mesh is Host/DeviceMeshData
if (normals_attr)
{
    auto [unit_ok, unit_msg] =
        checkNormalsUnitLength(normals_attr, /*tolerance=*/1e-4,
                               CheckNormalsOptions::Strict);

    auto [wind_ok, wind_msg] =
        checkNormalsConsistentWithWinding(mesh, normals_attr, Orientation::RightHanded);
}
```

## Parameters

- `CleanupOptions::fixes` — which `Defect`s to check/fix (default: `noDefects`). See the AGENTS.md "Cleanup & Defects" section for the full enum and named sets (`manifestDefects`, `allDefects`, `coincidentVertexDefects`, …).
- `CleanupOptions::vertexMergeTolerance` — coincidence tolerance for the coincident-vertex defects (0 = exact).
- `checkNormalsUnitLength` tolerance defaults to `1e-4`; `CheckNormalsOptions::AllowZeroLength` ignores exactly-zero vectors, `Strict` does not.
- `checkNormalsConsistentWithWinding` takes `Orientation::RightHanded` or `LeftHanded` (map from `UsdGeomTokens->rightHanded` when reading from USD).

## Key Notes

- `checkClean` only reports defects that *persist* across the canonical cleanup pipeline; a defect whose disruption a later fixer undoes is not reported.
- `CleanupValidation` and `Validation` are both implicitly convertible to `bool`; `.message` carries detail.
- Topology checks operate on `MeshData`; normals checks operate on `MeshData` plus an `Attribute`. Both support `Host` and `Device` residency.

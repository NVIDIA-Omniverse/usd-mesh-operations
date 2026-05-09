<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: validate
description: Run mesh topology and attribute validation checks (manifold, degenerate, normals, finite values).
allowed-tools: Read, Glob, Grep
---

# Validate Mesh Data

Run validation checks on mesh topology and attributes.

## Headers

```cpp
#include <OmniMeshOps/ValidateMeshData.h>
#include <OmniMeshOps/Normals.h>
#include <OmniMeshOps/Attribute.h>
```

## Topology Checks

```cpp
omo::HostMeshData meshData(hostMesh);

auto manifoldResult = omo::checkManifold(meshData);
auto coincidentResult = omo::checkNoMergeableCoincidentVertices(meshData, tolerance);
auto edgeResult = omo::checkNoDegenerateEdges(meshData);
auto faceResult = omo::checkNoDegenerateFaces(meshData);
auto isolatedResult = omo::checkNoIsolatedVertices(meshData);
auto duplicateResult = omo::checkNoDuplicateFaces(meshData);

if (!manifoldResult) {
    std::string detail = manifoldResult.message;
}
```

## Attribute Checks

```cpp
auto finiteResult = omo::checkAttributeFinite(attribute);
auto normalResult = omo::checkNormalsUnitLength(attribute, tolerance, omo::CheckNormalsOptions::Strict);
auto windingResult = omo::checkNormalsConsistentWithWinding(meshData, attribute, omo::Orientation::RightHanded);
```

## Parameters

- `checkNoMergeableCoincidentVertices` tolerance defaults to 0.0 (exact match)
- `merge_neighbors` and `merge_boundaries` default to `{true}`
- `checkNormalsUnitLength` tolerance defaults to 1e-4
- `CheckNormalsOptions::AllowZeroLength` — ignore precisely zero vectors
- `Orientation::RightHanded` or `LeftHanded` for winding check

## Key Notes

- All validation functions return `ValidationResult` which is implicitly convertible to `bool`
- `ValidationResult::message` provides detailed information about failures
- These functions work with both `Host` and `Device` residency

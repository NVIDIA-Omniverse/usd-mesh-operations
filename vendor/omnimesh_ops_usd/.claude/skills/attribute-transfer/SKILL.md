<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: attribute-transfer
description: Transfer and interpolate mesh attributes from a source mesh to a destination mesh.
allowed-tools: Read, Glob, Grep
---

# Transfer Attributes Between Meshes

Interpolate and transfer attributes from a source mesh to a destination mesh.

## Headers

```cpp
#include <OmniMeshOps/MeshAttributeXfer.h>
#include <OmniMeshOps/usd/Mesh.h>
```

## Transfer All Attributes

```cpp
omo::HostMeshAttributeXfer xfer(srcMesh, transferNormals);
std::array<double, 16> transform = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
omo::HostMesh result = xfer(dstMesh, transform);
```

## Transfer Specific Attributes

```cpp
omo::HostMeshAttributeXfer xfer(srcMesh, {"primvars:st", "primvars:displayColor"});
omo::HostMesh result = xfer(dstMesh, transform);
```

## Parameters

- `transferNormals` — if true and src has Normal-role attributes (Vector3f/Vector3d), they are interpolated and transformed (default false)
- `transform` — 4x4 row-major matrix mapping dst_mesh local space to src_mesh local space (default identity)

## Key Notes

- Only floating-point attribute types are interpolated; other types are not transferred
- Corresponding attributes already on dst_mesh are copied directly into the return value
- The destination mesh is not modified; a copy with transferred attributes is returned

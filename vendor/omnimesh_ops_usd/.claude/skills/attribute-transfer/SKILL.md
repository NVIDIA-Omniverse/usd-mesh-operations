---
name: attribute-transfer
description: Transfer and interpolate mesh attributes from a source mesh to a destination mesh.
allowed-tools: Read, Glob, Grep
license: LicenseRef-NvidiaProprietary
metadata:
  version: "1.0.0"
---

<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

# Transfer Attributes Between Meshes

Interpolate and transfer attributes from a source mesh to a destination mesh.

## Headers

```cpp
#include <OmniMeshOps/MeshAttributeXfer.h>
#include <OmniMeshOps/UsdIO.h>
```

## Transfer All Attributes

```cpp
using namespace omo;

HostMesh srcMesh = importMesh(srcUsdGeomMesh);
HostMesh dstMesh = importMesh(dstUsdGeomMesh);

HostMeshAttributeXfer xfer(srcMesh, /*transferNormals=*/false);
std::array<double, 16> transform = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
HostMesh result = xfer(dstMesh, transform);

exportUsdGeomMesh(result, stage, dstPrimPath);
```

## Transfer Specific Attributes

```cpp
HostMeshAttributeXfer xfer(srcMesh, { "primvars:st", "primvars:displayColor" });
HostMesh result = xfer(dstMesh, transform);
```

## GPU Path

```cpp
DeviceMeshAttributeXfer xfer(DeviceMesh(srcMesh), /*transferNormals=*/false);
DeviceMesh deviceDst(dstMesh);
HostMesh result(xfer(deviceDst, transform));
```

## Parameters

- `transferNormals` — if true and the source has Normal-role attributes (`Vector3f`/`Vector3d`), they are interpolated and transformed (default false)
- `transform` — 4×4 row-major matrix mapping dst-mesh local space into src-mesh local space (default identity)

## Key Notes

- Only floating-point attribute types are interpolated; other types are not transferred.
- Attributes already present on the destination are copied directly into the returned mesh.
- The destination mesh is not modified; a copy with transferred attributes is returned.
- The returned mesh is falsy (`!result`) if the transfer failed.

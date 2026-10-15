---
name: slice-clip
description: Cut a mesh along planes, a 3D grid, or clip to an axis-aligned bounding box.
allowed-tools: Read, Glob, Grep
license: LicenseRef-NvidiaProprietary
metadata:
  version: "1.0.0"
---

<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

# Slice / Grid-Dice / Box-Clip a Mesh

Cut a mesh along planes, a 3D grid, or clip to an AABB. These are free functions operating on `MeshData`.

## Headers

```cpp
#include <OmniMeshOps/Slice.h>
#include <OmniMeshOps/UsdIO.h>
```

## Plane Slice

```cpp
using namespace omo;

HostMeshData meshData = importMeshData(usdGeomMesh);
std::array<double, 3> upVector = { 0, 1, 0 };
std::vector<double> heights = { 0.0, 5.0, 10.0 };

HostMeshData result = slice(meshData, upVector, heights);
// or, transferring only named attributes/subsets:
HostMeshData result = slice(meshData, transferredAttachments, upVector, heights);

exportUsdGeomMesh(result, stage, primPath);
```

## Grid Slice

```cpp
std::array<double, 3> cellSize = { 10.0, 10.0, 10.0 };
std::array<double, 3> origin   = { 0.0, 0.0, 0.0 };

HostMeshData result = gridSlice(meshData, transferredAttachments, cellSize, origin,
                                { 1, 0, 0 },   // x axis
                                { 0, 1, 0 },   // y axis
                                { 0, 0, 1 });  // z axis
```

## Box Clip

```cpp
std::array<double, 3> worldMin = { -10.0, -10.0, -10.0 };
std::array<double, 3> worldMax = {  10.0,  10.0,  10.0 };

HostMeshData result = boxClip(meshData, transferredAttachments, worldMin, worldMax,
                              { 1, 0, 0 }, { 0, 1, 0 }, { 0, 0, 1 });
```

## GPU

Wrap the input in a `DeviceMeshData` and read the result back:

```cpp
HostMeshData result(slice(DeviceMeshData(meshData), transferredAttachments, upVector, heights));
```

## Key Notes

- Each function has two overloads: one without `transferred_attachments` (keeps everything) and one with it as the first list argument.
- A zero-length axis up-vector skips slicing in that direction; a zero cell length in `gridSlice` skips that axis.
- Heights / grid coordinates are in mesh local space.
- These operate on `MeshData`, not `Mesh`; import with `importMeshData`.

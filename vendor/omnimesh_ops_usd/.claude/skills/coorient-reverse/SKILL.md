<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: coorient-reverse
description: Coorient mesh faces for consistent winding, or reverse face winding order with optional normal flipping.
allowed-tools: Read, Glob, Grep
---

# Coorient and Reverse Face Winding

Both are free functions operating on `MeshData`.

## Coorient — Make Face Windings Consistent

### Headers

```cpp
#include <OmniMeshOps/Coorient.h>
#include <OmniMeshOps/UsdIO.h>
```

### Usage

```cpp
using namespace omo;

HostMeshData meshData = importMeshData(usdGeomMesh);
HostMeshData result = coorient(meshData, CoorientOptions::FlipNormalsOfReversedFaces);
// transferring only named attributes/subsets:
HostMeshData result = coorient(meshData, transferredAttachments, CoorientOptions::FlipNormalsOfReversedFaces);
```

### Options

- `CoorientOptions::None` — only fix winding
- `CoorientOptions::FlipNormalsOfReversedFaces` (default) — also flip normals for reversed faces. `PerVertex` Normal attributes are promoted to `PerCorner` and flipped where faces are reversed.

---

## Reverse — Flip Face Winding Order

### Headers

```cpp
#include <OmniMeshOps/Reverse.h>
#include <OmniMeshOps/UsdIO.h>
```

### Usage

```cpp
HostMeshData meshData = importMeshData(usdGeomMesh);
HostMeshData result = reverse(meshData, ReverseOptions::FlipNormals);
// or: reverse(meshData, transferredAttachments, ReverseOptions::FlipNormals);
```

### Options

- `ReverseOptions::None` (default) — reverse faces only
- `ReverseOptions::FlipNormals` — also flip normal directions

## Key Notes

- Both operate on `MeshData`; import with `importMeshData` and write back with `exportUsdGeomMesh`.
- Both have overloads with and without `transferred_attachments` (the no-list form keeps everything).
- GPU: wrap the input in `DeviceMeshData` and convert the result back to `HostMeshData`.

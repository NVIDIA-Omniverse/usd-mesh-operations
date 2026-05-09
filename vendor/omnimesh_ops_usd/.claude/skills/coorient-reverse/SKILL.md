<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: coorient-reverse
description: Coorient mesh faces for consistent winding, or reverse face winding order with optional normal flipping.
allowed-tools: Read, Glob, Grep
---

# Coorient and Reverse Face Winding

## Coorient — Make Face Windings Consistent

### Headers

```cpp
#include <OmniMeshOps/Coorient.h>
#include <OmniMeshOps/usd/MeshData.h>
```

### Usage

```cpp
omo::usd::HostMeshData meshData(usdGeomMesh);
omo::HostMeshData result = omo::coorient(meshData, omo::CoorientOptions::FlipNormalsOfReversedFaces);
```

### With Attribute Transfer

```cpp
omo::HostMeshData result = omo::coorient(meshData, {"primvars:st"}, omo::CoorientOptions::FlipNormalsOfReversedFaces);
```

### Options

- `CoorientOptions::None` — only fix winding
- `CoorientOptions::FlipNormalsOfReversedFaces` — also flip normals for reversed faces (default). PerVertex Normal attributes are promoted to PerCorner and flipped where faces are reversed.

---

## Reverse — Flip Face Winding Order

### Headers

```cpp
#include <OmniMeshOps/Reverse.h>
#include <OmniMeshOps/usd/MeshData.h>
```

### Usage

```cpp
omo::usd::HostMeshData meshData(usdGeomMesh);
omo::HostMeshData result = omo::reverse(meshData, omo::ReverseOptions::FlipNormals);
```

### Options

- `ReverseOptions::None` — reverse faces only
- `ReverseOptions::FlipNormals` — also flip normal directions

## Key Notes

- Both operate on `MeshData`, not `Mesh`
- Both accept optional `transferred_attachments` vector

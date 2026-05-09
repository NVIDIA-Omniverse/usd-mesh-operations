<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: triangulate
description: Convert polygonal mesh faces (quads, n-gons) to triangles.
allowed-tools: Read, Glob, Grep
---

# Triangulate a Mesh

Convert polygonal faces (quads, n-gons) to triangles.

## Headers

```cpp
#include <OmniMeshOps/Triangulate.h>
#include <OmniMeshOps/usd/Mesh.h>
```

## CPU Path

```cpp
omo::usd::HostMesh inputMesh(usdGeomMesh);
omo::HostTriangulate triangulate(inputMesh);
omo::HostMesh result = triangulate();
```

## GPU Path

```cpp
omo::ScopedCudaContext cudaCtx(omo::Verbose{false});
omo::DeviceTriangulate triangulate(omo::DeviceMesh{inputMesh});
omo::HostMesh result(triangulate());
```

## Parameters

- Constructor optionally accepts `transferred_attributes` vector to control which attributes are transferred

## Key Notes

- Returns an empty Mesh if no source mesh was provided
- All attributes are transferred by default unless `transferred_attributes` is specified

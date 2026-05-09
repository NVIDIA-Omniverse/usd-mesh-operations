<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: subdivide
description: Refine a mesh using Catmull-Clark or Loop subdivision with optional crease/spike control.
allowed-tools: Read, Glob, Grep
---

# Subdivide a Mesh

Refine a mesh using Catmull-Clark or Loop subdivision.

## Headers

```cpp
#include <OmniMeshOps/Subdivide.h>
#include <OmniMeshOps/usd/Mesh.h>
```

## CPU Path

```cpp
omo::usd::HostMesh inputMesh(usdGeomMesh);
omo::HostSubdivide subdivide(inputMesh, omo::SubdivisionMethod::CatmullClark);
omo::HostMesh result = subdivide(iterationCount);
```

## GPU Path

```cpp
omo::ScopedCudaContext cudaCtx(omo::Verbose{false});
omo::DeviceSubdivide subdivide(omo::DeviceMesh{inputMesh}, omo::SubdivisionMethod::CatmullClark);
omo::HostMesh result(subdivide(iterationCount));
```

## Parameters

- `method` — `SubdivisionMethod::CatmullClark` or `SubdivisionMethod::Loop`
- `iterationCount` — number of subdivision passes (default 1)
- `linear` — if true, new vertices are linear combinations only (default false)
- Constructor accepts optional crease/spike arrays for sharp feature control

## Key Notes

- Loop subdivision triangulates the mesh first if necessary
- Face count grows as 4^n per iteration
- Pass `transferred_attachments` to control which attributes are interpolated
- Crease/spike sharpness follows DeRose et al. 1998 conventions
- Mesh attributes with `Role::Sharpness` are automatically used for creases (PerCorner) and spikes (PerVertex)

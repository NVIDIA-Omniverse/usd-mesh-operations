---
name: subdivide
description: Refine a mesh using Catmull-Clark or Loop subdivision with optional crease/spike control.
allowed-tools: Read, Glob, Grep
license: LicenseRef-NvidiaProprietary
metadata:
  version: "1.0.0"
---

<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

# Subdivide a Mesh

Refine a mesh using Catmull-Clark or Loop subdivision.

## Headers

```cpp
#include <OmniMeshOps/Subdivide.h>
#include <OmniMeshOps/UsdIO.h>
```

## CPU Path

```cpp
using namespace omo;

HostMesh inputMesh = importMesh(usdGeomMesh);
HostSubdivide subdivide(inputMesh, transferredAttachments, SubdivisionMethod::CatmullClark, /*linear=*/false);
HostMesh result = subdivide(iterationCount);

exportUsdGeomMesh(result, stage, primPath);
```

## GPU Path

```cpp
ScopedCudaContext cudaCtx(Verbose{ false });
DeviceSubdivide subdivide(DeviceMesh{ inputMesh }, transferredAttachments, SubdivisionMethod::CatmullClark, false);
HostMesh result(subdivide(iterationCount));
```

## Parameters

- `method` — `SubdivisionMethod::CatmullClark` or `SubdivisionMethod::Loop`
- `iterationCount` — number of subdivision passes (`uint32_t`, default 1)
- `linear` — if true, new vertices are linear combinations of feature boundaries (default false)
- `transferredAttachments` — `std::vector<std::string>` of attribute/subset names to interpolate

## Key Notes

- Loop subdivision triangulates the mesh first if necessary.
- Face count grows ~4× per Catmull-Clark iteration.
- Attributes carrying `Role::Sharpness` are used automatically as creases (`PerCorner`) and spikes (`PerVertex`); sharpness follows DeRose et al. 1998 conventions.
- Omit `transferredAttachments` (use a constructor overload without it) to interpolate everything.

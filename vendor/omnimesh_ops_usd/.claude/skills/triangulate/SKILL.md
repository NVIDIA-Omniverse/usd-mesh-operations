---
name: triangulate
description: Convert polygonal mesh faces (quads, n-gons) to triangles.
allowed-tools: Read, Glob, Grep
license: LicenseRef-NvidiaProprietary
metadata:
  version: "1.0.0"
---

<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

# Triangulate a Mesh

Convert polygonal faces (quads, n-gons) to triangles.

## Headers

```cpp
#include <OmniMeshOps/Triangulate.h>
#include <OmniMeshOps/UsdIO.h>
```

## CPU Path

```cpp
using namespace omo;

HostMesh inputMesh = importMesh(usdGeomMesh);
HostTriangulate triangulate(inputMesh);            // or HostTriangulate(inputMesh, transferredAttachments)
HostMesh result = triangulate();

exportUsdGeomMesh(result, stage, primPath);
```

## GPU Path

```cpp
ScopedCudaContext cudaCtx(Verbose{ false });
DeviceTriangulate triangulate(DeviceMesh{ inputMesh }, transferredAttachments);
HostMesh result(triangulate());
```

## Parameters

- The constructor optionally accepts `transferredAttachments` (`std::vector<std::string>`) to control which attributes/subsets are carried over.

## Key Notes

- `operator()` takes no arguments and returns the triangulated mesh.
- All attributes/subsets are transferred by default unless `transferredAttachments` is specified.
- Returns an empty mesh if no source mesh was provided.

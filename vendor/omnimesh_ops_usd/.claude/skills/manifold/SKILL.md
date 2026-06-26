<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: manifold
description: Create a manifold cutting of a non-manifold mesh.
allowed-tools: Read, Glob, Grep
---

# Make a Mesh Manifold

Create a manifold cutting of a non-manifold mesh. `manifold()` is a free function operating on a full `Mesh`.

## Headers

```cpp
#include <OmniMeshOps/Manifold.h>
#include <OmniMeshOps/UsdIO.h>
```

## Usage

```cpp
using namespace omo;

HostMesh mesh = importMesh(usdGeomMesh);
HostMesh manifoldMesh = manifold(mesh);

exportUsdGeomMesh(manifoldMesh, stage, primPath);
```

## With Explicit Attribute Transfer

```cpp
HostMesh manifoldMesh = manifold(mesh, { "primvars:st", "primvars:normals" });
```

## Key Notes

- Two overloads: `manifold(mesh)` transfers all attachments; `manifold(mesh, transferredAttachments)` transfers only the named attributes/subsets.
- Operates on `Mesh` (not `MeshData`); works for both `Host` and `Device` residency.
- CPU is typically faster than GPU here (host↔device copy overhead exceeds the compute time).
- Vertex count may increase as non-manifold vertices are split.
- Alternatively, request `Defect::Nonmanifold` via `cleanup()` / `CleanupOptions` to make a mesh manifold as part of a broader cleanup pass (see the `validate` skill).

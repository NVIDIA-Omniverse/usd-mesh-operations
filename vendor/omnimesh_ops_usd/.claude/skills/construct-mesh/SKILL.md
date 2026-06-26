<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: construct-mesh
description: Build a mesh from raw vertex coordinates, face sizes, and index arrays, then optionally clean it up.
allowed-tools: Read, Glob, Grep
---

# Construct a Mesh from Raw Data

Build a `MeshData` from arrays of vertex coordinates, face sizes, and indices. Construction no longer performs cleanup — build the raw data first, then run `cleanup()` if needed and convert to a `Mesh`.

## Headers

```cpp
#include <OmniMeshOps/MeshData.h>
#include <OmniMeshOps/Mesh.h>
#include <OmniMeshOps/Attribute.h>
#include <OmniMeshOps/Cleanup.h>   // only if you want to clean the result
```

## Usage

```cpp
using namespace omo;

HostMeshData mesh_data(
    faceSizes,            // const uint32_t* — corners per face
    faceCount,            // size_t — number of faces
    faceVertexIndices,    // const uint32_t* — vertex indices
    indexCount,           // size_t — number of indices
    vertexCoords,         // const float* — xyz triplets
    vertexCount,          // size_t — number of points, NOT number of floats
    attributes,           // const std::vector<HostAttribute>& (optional)
    subsets,              // const std::vector<HostSubset>& (optional)
    localToWorld);        // const double* — 16 values row-major (optional, nullptr by default)
```

## Cleanup (separate step)

Construction does no repair. To merge/contract/remove defects, run `cleanup()`:

```cpp
CleanupOptions opts;
opts.fixes = manifestDefects;        // or a hand-built EnumSet<Defect>
opts.vertexMergeTolerance = 0.001f;

HostMeshData cleaned = cleanup(mesh_data, opts);
```

See AGENTS.md "Cleanup & Defects" for the full `Defect` enum and named sets, and the `validate` skill for `checkClean()`.

## Convert to a full Mesh

```cpp
HostMesh mesh(cleaned);              // build a Mesh from MeshData
HostMeshData back(mesh);             // and back again
```

## Serialization

```cpp
std::vector<uint8_t> bytes = mesh.serialized();   // also available on MeshData
SerializationError error;
HostMesh restored(bytes, error);
if (error != SerializationError::None) { /* handle */ }
```

## Key Notes

- `vertexCount` is the number of *points*, not the number of float values (which is 3×).
- Attributes/subsets are passed as `std::vector` (not pointer + count).
- The raw-array constructor lives on `MeshData`; build a `Mesh` from it when an operation needs the full topology graph.
- Serialization is not a stable interchange format — same library version and CPU architecture only.

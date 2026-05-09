<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: construct-mesh
description: Build a mesh from raw vertex coordinates, face sizes, and index arrays with configurable defect cleanup.
allowed-tools: Read, Glob, Grep
---

# Construct a Mesh from Raw Data

Build a mesh from arrays of vertex coordinates, face sizes, and indices.

## Headers

```cpp
#include <OmniMeshOps/Mesh.h>
#include <OmniMeshOps/Attribute.h>
```

## Usage

```cpp
omo::HostMesh mesh(
    faceSizes,              // uint32_t* — corners per face
    faceCount,              // size_t
    faceVertexIndices,      // uint32_t* — vertex indices
    indexCount,             // size_t
    vertexCoords,           // float* — xyz triplets
    vertexCount,            // size_t — number of points, not floats
    attributes,             // omo::HostAttribute* (optional)
    attrCount,              // size_t (optional)
    subsets,                // omo::HostSubset* (optional)
    subsetCount,            // size_t (optional)
    {omo::Defect::Default}, // MeshConstructionOptions
    localToWorld            // double[16] row-major (optional)
);
```

## Construction Options

```cpp
omo::MeshConstructionOptions opts;
opts.fixes = omo::Defect::CoincidentVertices | omo::Defect::DegenerateEdges | omo::Defect::DegenerateFaces;
opts.mergeVerticesTolerance = 0.001f;
opts.mergeBoundaries = {true};
opts.mergeNeighbors = {true};
```

## Defect Flags

- `Defect::None` — no cleanup
- `Defect::CoincidentVertices` — merge overlapping vertices
- `Defect::DegenerateEdges` — merge consecutively repeated vertex references
- `Defect::DegenerateFaces` — remove faces with fewer than 3 distinct vertices
- `Defect::IsolatedVertices` — remove unreferenced vertices
- `Defect::DuplicateFaces` — remove duplicate face copies
- `Defect::Default` — CoincidentVertices | DegenerateEdges | DegenerateFaces

## From MeshData

```cpp
omo::HostMesh mesh(meshData);
```

## Serialization

```cpp
std::vector<uint8_t> bytes = mesh.serialized();
omo::SerializationError error;
omo::HostMesh restored(bytes, error);
```

## Key Notes

- `vertexCount` is the number of *points*, not the number of float values (which is 3x)
- Attributes/subsets passed to the constructor are adjusted automatically when construction options cause changes
- Serialization is not a stable interchange format — same library version and architecture only
- `MeshData` can also be constructed from raw data with the same signature

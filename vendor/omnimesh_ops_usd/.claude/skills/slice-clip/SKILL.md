---
name: slice-clip
description: Cut a mesh along planes, a 3D grid, or clip to an axis-aligned bounding box.
allowed-tools: Read, Glob, Grep
---

# Slice / Grid-Dice / Box-Clip a Mesh

Cut a mesh along planes, a 3D grid, or clip to an AABB.

## Headers

```cpp
#include <OmniMeshOps/Slice.h>
#include <OmniMeshOps/usd/MeshData.h>
```

## Plane Slice

```cpp
omo::usd::HostMeshData meshData(usdGeomMesh, {omo::Defect::None});
std::array<double, 3> upVector = {0, 1, 0};
std::vector<double> heights = {0.0, 5.0, 10.0};
omo::HostMeshData result = omo::slice(meshData, upVector, heights);
```

## Grid Slice

```cpp
std::array<double, 3> cellSize = {10.0, 10.0, 10.0};
std::array<double, 3> origin = {0.0, 0.0, 0.0};
omo::HostMeshData result = omo::gridSlice(meshData, cellSize, origin);
```

## Grid Slice with Custom Axes

```cpp
omo::HostMeshData result = omo::gridSlice(meshData, cellSize, origin,
    {1, 0, 0},  // up_x_axis
    {0, 1, 0},  // up_y_axis
    {0, 0, 1}); // up_z_axis
```

## Box Clip

```cpp
std::array<double, 3> worldMin = {-10.0, -10.0, -10.0};
std::array<double, 3> worldMax = {10.0, 10.0, 10.0};
omo::HostMeshData result = omo::boxClip(meshData, worldMin, worldMax);
```

## Key Notes

- All functions accept optional `transferred_attachments` vector as the second argument
- Grid slice supports custom axis orientations — pass zero-length up-vectors to skip slicing in a direction
- A zero cell length in `gridSlice` skips slicing in that direction
- These functions operate on `MeshData`, not `Mesh`
- Heights are in mesh local coordinate space

---
name: usd-io
description: Read meshes from and write meshes to OpenUSD UsdGeomMesh prims using the omo::usd layer.
allowed-tools: Read, Glob, Grep
---

# Read/Write Meshes via USD

Bridge between OpenUSD `UsdGeomMesh` prims and `omo::Mesh`.

## Headers

```cpp
#include <OmniMeshOps/usd/Mesh.h>
#include <OmniMeshOps/usd/MeshData.h>
#include <OmniMeshOps/usd/Attribute.h>
#include <OmniMeshOps/usd/Subset.h>
#include <OmniMeshOps/usd/Prim.h>
```

## Read from USD — Full Mesh

For operations that need the full topology graph (Decimate, Subdivide, Triangulate, Remesh, Manifold, PrimitiveFit):

```cpp
omo::usd::HostMesh mesh(usdGeomMesh);
omo::usd::HostMesh mesh(usdGeomMesh, {omo::Defect::CoincidentVertices | omo::Defect::DegenerateEdges});
```

## Read from USD — Filtered Primvars

```cpp
omo::usd::HostMesh mesh(usdGeomMesh, {"primvars:st", "primvars:normals"});
```

## Read from USD — Lightweight MeshData

For operations that work on raw mesh data (Slice, GridSlice, BoxClip, Coorient, Reverse):

```cpp
omo::usd::HostMeshData meshData(usdGeomMesh);
```

## Write to USD

```cpp
omo::usd::HostMesh(result).writeToUsd(primPath, stage);
omo::usd::HostMeshData(result).writeToUsd(primPath, stage);
omo::usd::HostPrim(primitive).writeToUsd(primPath, stage);
```

## USD Attribute / Subset

```cpp
omo::usd::HostAttribute attr(usdGeomPrimvar);
omo::usd::HostSubset subset(parentGeom, usdGeomSubset);
```

## Key Notes

- Default construction options are `Defect::None` for the USD wrappers (unlike the core `Mesh` which defaults to `Defect::Default`)
- `omo::usd::HostMesh` inherits from `omo::HostMesh` — it can be passed to any core operation
- All USD types live in the `omo::usd` namespace

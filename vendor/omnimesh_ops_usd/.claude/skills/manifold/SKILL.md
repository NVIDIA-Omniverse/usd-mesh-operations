---
name: manifold
description: Create a manifold cutting of a non-manifold mesh.
allowed-tools: Read, Glob, Grep
---

# Make a Mesh Manifold

Create a manifold cutting of a non-manifold mesh.

## Headers

```cpp
#include <OmniMeshOps/Manifold.h>
#include <OmniMeshOps/usd/Mesh.h>
```

## Usage

```cpp
omo::usd::HostMesh mesh(usdGeomMesh, {omo::Defect::None});
omo::HostMesh manifoldMesh = omo::manifold(mesh);
```

## With Explicit Attribute Transfer

```cpp
omo::HostMesh manifoldMesh = omo::manifold(mesh, {"primvars:st", "primvars:normals"});
```

## Key Notes

- All attachments are transferred by default; pass `transferred_attachments` to filter
- CPU-only is typically faster than GPU for this operation (host-device copy overhead exceeds the computation time)
- Vertex count may increase as non-manifold vertices are split

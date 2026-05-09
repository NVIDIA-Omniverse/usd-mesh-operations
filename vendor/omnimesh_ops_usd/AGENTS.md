# AGENTS.md

Cross-agent entry point for any coding agent working with the omnimesh_ops_usd library.

## Overview

omnimesh_ops_usd is a C++ mesh-processing library (namespace `omo`) providing GPU- and CPU-accelerated operations on polygon meshes. It is consumed as a header-only vendor dependency linked against pre-built shared libraries.

The library has two layers:

1. **Core layer** (`OmniMeshOps/*.h`) — residency-templated (`Host`/`Device`) mesh types and operations on raw data arrays.
2. **USD layer** (`OmniMeshOps/usd/*.h`) — thin wrappers bridging `omo::Mesh` with OpenUSD `UsdGeomMesh` prims.

## Skills

Reusable, task-specific instructions live under `.claude/skills/<name>/SKILL.md`. They are plain Markdown — any agent can read and follow them.

When a task could match a skill, inventory `.claude/skills/` and read each `SKILL.md`'s YAML frontmatter `description` field to decide which (if any) applies.

## Key Types

| Type | Header | Purpose |
|------|--------|---------|
| `Mesh<R>` | `Mesh.h` | Central mesh container (vertices, faces, attributes, subsets) |
| `MeshData<R>` | `MeshData.h` | Lightweight mesh-data view used by slice/clip ops |
| `Attribute<R>` | `Attribute.h` | Per-vertex / per-corner / per-face data attached to a mesh |
| `Subset<R>` | `Subset.h` | Named index subset (material assignments, selections) |
| `ScopedCudaContext` | `ScopedCudaContext.h` | RAII helper for GPU operations |

`R` is `omo::Host` (CPU) or `omo::Device` (CUDA). Cross-residency copies are supported via explicit template conversion constructors.

## Operations

| Operation | Header | Description |
|-----------|--------|-------------|
| `Decimate` | `Decimate.h` | Reduce vertex count with error-bounded decimation |
| `Subdivide` | `Subdivide.h` | Catmull-Clark or Loop subdivision |
| `Triangulate` | `Triangulate.h` | Convert polygonal faces to triangles |
| `Remesh` | `Remesh.h` | Isotropic remeshing with gradation control |
| `PrimitiveFit` | `Primitive.h` | Fit sphere/cylinder/cone/cube to a mesh |
| `MeshAttributeXfer` | `MeshAttributeXfer.h` | Transfer attributes between meshes |
| `Select` | `Select.h` | Select subset of mesh by attribute value range |
| `manifold()` | `Manifold.h` | Make a mesh manifold |
| `coorient()` | `Coorient.h` | Make face windings consistent |
| `reverse()` | `Reverse.h` | Reverse face winding order |
| `slice()` / `gridSlice()` / `boxClip()` | `Slice.h` | Cut mesh by planes, grids, or AABBs |
| `cornerNormals()` / `faceNormals()` / `vertexNormals()` / `normals()` | `Normals.h` | Compute normals with various bindings |
| `createPrimitiveMesh()` | `Primitive.h` | Generate mesh geometry for a primitive type |

## Validation Functions

| Function | Header | Checks |
|----------|--------|--------|
| `checkManifold()` | `ValidateMeshData.h` | Mesh is manifold |
| `checkNoMergeableCoincidentVertices()` | `ValidateMeshData.h` | No duplicate vertices within tolerance |
| `checkNoDegenerateEdges()` | `ValidateMeshData.h` | No zero-length edges |
| `checkNoDegenerateFaces()` | `ValidateMeshData.h` | No degenerate faces |
| `checkNoIsolatedVertices()` | `ValidateMeshData.h` | No unreferenced vertices |
| `checkNoDuplicateFaces()` | `ValidateMeshData.h` | No duplicate faces |
| `checkNormalsUnitLength()` | `Normals.h` | Normals are unit-length |
| `checkNormalsConsistentWithWinding()` | `Normals.h` | Normals agree with face winding |
| `checkAttributeFinite()` | `Attribute.h` | Attribute values are finite (no NaN/Inf) |

## USD Layer

The `omo::usd` namespace provides USD-aware subclasses:

- `omo::usd::HostMesh` — construct from `UsdGeomMesh`, write back with `writeToUsd()`
- `omo::usd::HostMeshData` — same pattern for lightweight mesh data
- `omo::usd::HostAttribute` — construct from `UsdGeomPrimvar`
- `omo::usd::HostSubset` — construct from `UsdGeomSubset`
- `omo::usd::HostPrim` — write fitted primitives back to USD

## Usage Pattern (from scene-optimizer-core)

```cpp
// 1. Read from USD
omo::usd::HostMesh inputMesh(usdGeomMesh, constructionOptions);

// 2. Optionally move to GPU
omo::DeviceMesh deviceMesh(inputMesh);

// 3. Run operation
omo::DeviceDecimate decimate(deviceMesh, guideAttribute);
auto result = decimate(targetVertexCount, maxMeanError, runParallel, pinBoundaries);

// 4. Copy back to host
omo::HostMesh hostResult(result);

// 5. Write to USD
omo::usd::HostMesh(hostResult).writeToUsd(primPath, stage);
```

## Enums & Configuration

- `Residency` — `Host` or `Device`
- `Binding` — `None`, `Constant`, `PerFace`, `PerVertex`, `PerCorner`
- `DataType` — scalar, vector, matrix types (float/double/int variants)
- `Role` — `None`, `Tangent`, `Normal`, `UV`, `Color`, `Material`, `Sharpness`
- `Defect` — bitmask for mesh construction cleanup (CoincidentVertices, DegenerateEdges, etc.)
- `SubdivisionMethod` — `CatmullClark`, `Loop`
- `WeightMode` — `Angle`, `Area` (for normal computation)
- `PrimitiveType` — `Sphere`, `Cylinder`, `Cone`, `Cube`
- `CoorientOptions` — `None`, `FlipNormalsOfReversedFaces`
- `ReverseOptions` — `None`, `FlipNormals`

## Logging

`omo::log` namespace (`Log.h`) provides configurable output/error streams and log levels (Error, Warning, Info, Debug) with optional ANSI coloring.

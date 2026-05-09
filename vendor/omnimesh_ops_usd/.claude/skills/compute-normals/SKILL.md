---
name: compute-normals
description: Compute or fix mesh normals (corner, face, vertex, or auto-binding) with sharpness angle and weighting control.
allowed-tools: Read, Glob, Grep
---

# Compute Normals

Generate normal attributes with various bindings and weighting modes.

## Headers

```cpp
#include <OmniMeshOps/Normals.h>
#include <OmniMeshOps/usd/Mesh.h>
#include <OmniMeshOps/usd/Attribute.h>
```

## Corner Normals (with sharpness angle control)

```cpp
omo::HostAttribute attr = omo::cornerNormals(hostMesh, sharpnessAngle, omo::WeightMode::Angle);
```

## Fix Existing Normals

```cpp
auto existing = hostMesh.getAttribute("primvars:normals");
omo::HostAttribute fixed = omo::cornerNormals(hostMesh, existing, sharpnessAngle, weightMode);
```

## Face / Vertex Normals

```cpp
omo::HostAttribute faceNorm = omo::faceNormals(hostMesh);
omo::HostAttribute vertNorm = omo::vertexNormals(hostMesh, omo::WeightMode::Angle);
```

## Auto-binding Normals

```cpp
// Computes corner normals, then demotes to per-vertex or per-face if lossless
omo::HostAttribute attr = omo::normals(hostMesh, sharpnessAngle, weightMode);
```

## Attach to Mesh

```cpp
attr.setName("primvars:normals");
hostMesh.attachAttribute(attr, true /* replace existing */);
```

## Parameters

- `sharpnessAngle` — dihedral angle threshold for sharp edges (degrees, default 60). Range ]-180, 180] with 0 being flat.
- `weightMode` — `WeightMode::Angle` or `WeightMode::Area`

## Validation

```cpp
auto [valid, msg] = omo::checkNormalsUnitLength(attribute, tolerance, omo::CheckNormalsOptions::Strict);
auto [consistent, msg2] = omo::checkNormalsConsistentWithWinding(meshData, attribute, omo::Orientation::RightHanded);
```

## Key Notes

- Fix-existing overloads normalize finite nonzero entries and recompute the rest
- `normals()` is the auto-binding variant: computes corner normals then demotes binding if lossless
- GPU path: same functions work with `DeviceMesh` / `DeviceAttribute`
- `CheckNormalsOptions::AllowZeroLength` ignores precisely zero vectors during unit-length check

---
name: decimate
description: Reduce mesh vertex/face count using error-bounded decimation, with CPU and GPU paths.
allowed-tools: Read, Glob, Grep
---

# Decimate a Mesh

Reduce vertex/face count while preserving shape within an error bound.

## Headers

```cpp
#include <OmniMeshOps/Decimate.h>
#include <OmniMeshOps/usd/Mesh.h>
```

## CPU Path

```cpp
omo::usd::HostMesh inputMesh(usdGeomMesh, {omo::Defect::CoincidentVertices | omo::Defect::DegenerateEdges | omo::Defect::DegenerateFaces});
omo::HostDecimate decimate(inputMesh, guideAttributeName);
omo::HostMesh result = decimate(targetVertexCount, maxMeanError, runParallel, pinBoundaries, canCutAndGlue);
double meanError = decimate.meanError();
```

## GPU Path

```cpp
omo::ScopedCudaContext cudaCtx(omo::Verbose{false});
omo::DeviceMesh deviceInput(inputMesh);
omo::DeviceDecimate decimate(deviceInput, guideAttributeName);
omo::HostMesh result(decimate(targetVertexCount, maxMeanError, true, pinBoundaries, canCutAndGlue));
```

## Parameters

- `targetVertexCount` — stop when this vertex count is reached
- `maxMeanError` — stop when mean error exceeds this (0 to disable)
- `runParallel` — use parallel CPU algorithm
- `pinBoundaries` — keep boundary vertices in place
- `canCutAndGlue` — allow topology changes (slower)
- `guideAttributeName` — optional attribute to guide decimation (PerVertex Color or PerCorner Normal, e.g. `"primvars:normals"` or `"primvars:displayColor"`)

## Key Notes

- Each call to `operator()` decimates from the *original* mesh, not the previous result
- Specify `transferred_attachments` in constructor to control which attributes/subsets are carried over
- Constructor overloads: `(mesh, guide)` or `(mesh, guide, transferred_attachments)`

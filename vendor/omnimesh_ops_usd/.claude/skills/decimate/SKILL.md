<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

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
#include <OmniMeshOps/UsdIO.h>
```

## CPU Path

```cpp
using namespace omo;

HostMesh inputMesh = importMesh(usdGeomMesh, { manifestDefects });
HostDecimate decimate(inputMesh, guideAttributeName, transferredAttachments);
HostMesh result = decimate(targetVertexCount, maxMeanError, runParallel, pinBoundaries, canCutAndGlue);
double meanError = decimate.meanError();

exportUsdGeomMesh(result, stage, primPath);
```

## GPU Path

```cpp
ScopedCudaContext cudaCtx(Verbose{ false });
DeviceMesh deviceInput(inputMesh);
DeviceDecimate decimate(deviceInput, guideAttributeName, transferredAttachments);
HostMesh result(decimate(targetVertexCount, maxMeanError));
```

## Constructor

```cpp
Decimate(mesh, guideAttributeName = {});
Decimate(mesh, guideAttributeName, transferredAttachments);  // std::vector<std::string>
```

## operator() Parameters

`operator()(size_t target_vertex_count, double max_mean_error, bool run_parallel = true, bool pin_boundaries = false, bool can_cut_and_glue = false)`

- `target_vertex_count` — stop when this vertex count is reached
- `max_mean_error` — stop when mean error reaches this (0 to disable)
- `run_parallel` — use the parallel CPU algorithm (vs. sequential)
- `pin_boundaries` — keep surface-boundary vertices in place
- `can_cut_and_glue` — allow the decimator to cut and glue the mesh
- `guideAttributeName` — optional guide attribute (`PerVertex` Color or `PerCorner` Normal, e.g. `"primvars:normals"` or `"primvars:displayColor"`)

## Key Notes

- Each call to `operator()` decimates from the *original* mesh, not the previous result, so a single `Decimate` can be invoked repeatedly with different targets.
- `transferredAttachments` lists attribute and subset names to carry over; omit it to transfer everything found.
- `meanError()` returns the actual mean error reached by the last decimation.

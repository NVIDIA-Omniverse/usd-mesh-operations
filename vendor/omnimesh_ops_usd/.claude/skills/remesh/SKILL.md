<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: remesh
description: Isotropic remeshing producing uniform triangle sizes with gradation and error control.
allowed-tools: Read, Glob, Grep
---

# Remesh a Mesh

Isotropic remeshing producing uniform triangle sizes.

## Headers

```cpp
#include <OmniMeshOps/Remesh.h>
#include <OmniMeshOps/UsdIO.h>
```

## CPU Path

```cpp
using namespace omo;

HostMesh inputMesh = importMesh(usdGeomMesh, { manifestDefects });
HostRemesh remesh(inputMesh);                       // or HostRemesh(inputMesh, transferredAttachments)
HostMesh result = remesh(gradation, maxError);

exportUsdGeomMesh(result, stage, primPath);
```

## GPU Path

```cpp
ScopedCudaContext cudaCtx(Verbose{ false });
DeviceRemesh remesh(DeviceMesh{ inputMesh }, transferredAttachments);
HostMesh result(remesh(gradation, maxError));
```

## Parameters

- `gradation` — controls triangle density variation (lower = more uniform; example default `0.1`)
- `maxError` — maximum geometric error tolerance (example default `0.1`; 0 to disable)

## Key Notes

- Each call remeshes the *original* mesh, not a previous result.
- The constructor optionally accepts a `transferredAttachments` (`std::vector<std::string>`) of attribute/subset names; omit to transfer everything.
- Import with `{ manifestDefects }` so degenerate input does not derail remeshing.

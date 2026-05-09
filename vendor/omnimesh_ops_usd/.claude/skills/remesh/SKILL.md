<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
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
#include <OmniMeshOps/usd/Mesh.h>
```

## CPU Path

```cpp
omo::usd::HostMesh inputMesh(usdGeomMesh, {omo::Defect::CoincidentVertices | omo::Defect::DegenerateEdges | omo::Defect::DegenerateFaces});
omo::HostRemesh remesh(inputMesh);
omo::HostMesh result = remesh(gradation, maxError);
```

## GPU Path

```cpp
omo::ScopedCudaContext cudaCtx(omo::Verbose{false});
omo::DeviceRemesh remesh(omo::DeviceMesh{inputMesh});
omo::HostMesh result(remesh(gradation, maxError));
```

## Parameters

- `gradation` — controls triangle density variation (0 = uniform, higher = more variation)
- `maxError` — maximum geometric error tolerance (0 to disable)

## Key Notes

- Each call remeshes the *original* mesh, not a previous result
- Constructor optionally accepts `transferred_attributes` for attribute transfer

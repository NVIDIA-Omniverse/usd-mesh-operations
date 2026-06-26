<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: gpu-acceleration
description: Move mesh computation to CUDA GPU using ScopedCudaContext and Host/Device residency pattern.
allowed-tools: Read, Glob, Grep
---

# GPU Acceleration Pattern

Move computation to CUDA device for large meshes.

## Headers

```cpp
#include <OmniMeshOps/ScopedCudaContext.h>
```

## Pattern

```cpp
// 1. Create scoped CUDA context (RAII — destroyed at end of scope)
omo::ScopedCudaContext cudaCtx(omo::Verbose{verbose}, cudaDevice, heapSize);

// 2. Copy mesh to device
omo::DeviceMesh deviceMesh(hostMesh);

// 3. Run device operation
omo::DeviceDecimate decimate(deviceMesh, guideAttr);
auto deviceResult = decimate(targetVerts, maxError, true, pinBoundaries);

// 4. Copy result back to host
omo::HostMesh hostResult(deviceResult);
```

## ScopedCudaContext Parameters

- `verbose` — `omo::Verbose{bool}`, enable verbose logging
- `cudaDevice` — CUDA device number (default 0)
- `heapSize` — CUDA heap size to allocate (default 64 MB). Only takes effect on first context creation per device.

## Type Aliases

All operation classes are templated on `Residency` with convenient aliases:

- `HostMesh` / `DeviceMesh`
- `HostDecimate` / `DeviceDecimate`
- `HostSubdivide` / `DeviceSubdivide`
- `HostTriangulate` / `DeviceTriangulate`
- `HostRemesh` / `DeviceRemesh`
- `HostPrimitiveFit` / `DevicePrimitiveFit`
- `HostMeshAttributeXfer` / `DeviceMeshAttributeXfer`
- `HostSelect` / `DeviceSelect`
- `HostAttribute` / `DeviceAttribute`
- `HostSubset` / `DeviceSubset`

## Key Notes

- Cross-residency copy constructors handle data transfer (e.g. `DeviceMesh(hostMesh)`, `HostMesh(deviceResult)`)
- `ScopedCudaContext` reuses existing thread-local contexts when available
- Typical thresholds from scene-optimizer-core: 100K-500K vertices for CPU parallel, 500K+ for GPU
- For manifold operations, CPU is often faster than GPU due to copy overhead

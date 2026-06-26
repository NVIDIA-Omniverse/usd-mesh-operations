<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

---
name: select
description: Select a subset of mesh elements based on attribute value ranges.
allowed-tools: Read, Glob, Grep
---

# Select by Attribute Value

Select a subset of mesh elements based on attribute value ranges.

## Headers

```cpp
#include <OmniMeshOps/Select.h>
#include <OmniMeshOps/UsdIO.h>
```

## Select by Attribute Name

```cpp
using namespace omo;

HostMesh mesh = importMesh(usdGeomMesh);
HostSelect select(mesh, "primvars:myAttribute");
HostSubset subset = select(minValue, maxValue, minInclusive, maxInclusive);
```

## Select by Attribute Reference

```cpp
auto attribute = mesh.getAttribute("primvars:myAttribute");
HostSelect select(mesh, attribute);
HostSubset subset = select(0.0f, 1.0f, true, true);
```

## Select Within Existing Subset

```cpp
HostSelect select(mesh, attribute, existingSubset);
HostSubset subset = select(min, max, minInclusive, maxInclusive);
```

## Overloads

`operator()` accepts `float`, `double`, or `int32_t` range bounds: `operator()(min, max, min_inclusive, max_inclusive)`. The returned subset's binding matches the attribute binding.

## Key Notes

- When limiting selection with an existing subset, the subset binding must match the attribute binding, otherwise it is ignored.
- Returns a `Subset` specifying which indices in the attribute data match the criteria.
- Available for both `Host` and `Device` residency (`HostSelect` / `DeviceSelect`).

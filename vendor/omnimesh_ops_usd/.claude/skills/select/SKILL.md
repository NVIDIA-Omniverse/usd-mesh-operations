<!-- SPDX-FileCopyrightText: Copyright (c) 2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
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
#include <OmniMeshOps/usd/Mesh.h>
```

## Select by Attribute Name

```cpp
omo::usd::HostMesh mesh(usdGeomMesh);
omo::HostSelect select(mesh, "primvars:myAttribute");
omo::HostSubset subset = select(minValue, maxValue, minInclusive, maxInclusive);
```

## Select by Attribute Reference

```cpp
omo::HostSelect select(mesh, attribute);
omo::HostSubset subset = select(0.0f, 1.0f, true, true);
```

## Select Within Existing Subset

```cpp
omo::HostSelect select(mesh, attribute, existingSubset);
omo::HostSubset subset = select(min, max, minInclusive, maxInclusive);
```

## Overloads

`operator()` accepts `float`, `double`, or `int32_t` range bounds. The subset binding matches the attribute binding.

## Key Notes

- When using an existing subset to limit selection, the subset binding must match the attribute binding, otherwise it is ignored
- Returns a `Subset` specifying which indices in the attribute data match the criteria

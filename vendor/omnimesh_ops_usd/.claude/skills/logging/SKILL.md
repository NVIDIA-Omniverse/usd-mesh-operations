---
name: logging
description: Configure omnimesh_ops_usd logging output streams, verbosity levels, and ANSI coloring.
allowed-tools: Read, Glob, Grep
license: LicenseRef-NvidiaProprietary
metadata:
  version: "1.0.0"
---

<!-- SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved. -->
<!-- SPDX-License-Identifier: LicenseRef-NvidiaProprietary -->

# Configure Logging

Control output verbosity and streams.

## Headers

```cpp
#include <OmniMeshOps/Log.h>
```

## Usage

```cpp
omo::log::setLevel(omo::log::Level::Warning);  // suppress Info and Debug
omo::log::setOutStream(&myStream);              // redirect output
omo::log::setErrStream(nullptr);                // suppress errors
omo::log::setColorize(false);                   // disable ANSI colors
```

## Query Current Settings

```cpp
omo::log::Level level = omo::log::getLevel();
std::ostream* out = omo::log::getOutStream();
std::ostream* err = omo::log::getErrStream();
bool colorized = omo::log::getColorize();
```

## Levels (most to least severe)

- `Level::Error` — always emitted unless error stream is nullptr
- `Level::Warning`
- `Level::Info`
- `Level::Debug` (default threshold)

## Key Notes

- Default output stream is `std::cout`, default error stream is `std::cerr`
- Pass `nullptr` to suppress output/error entirely
- When coloring is enabled, errors are red and warnings are yellow; info and debug are uncolored

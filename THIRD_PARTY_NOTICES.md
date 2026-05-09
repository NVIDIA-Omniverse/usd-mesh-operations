# Third-Party Notices

**USD Mesh Operations — Vendored Dependencies** is Copyright (c) 2024-2026, NVIDIA
CORPORATION and is distributed under the NVIDIA proprietary license. See the `LICENSE`
file at the root of this repository for the full license text.

This repository vendors prebuilt libraries and headers authored by NVIDIA. Some of those
libraries statically link or otherwise incorporate third-party open-source software. The
required copyright notices, attribution statements, and license texts for those components
are provided below.

---

# Vendored NVIDIA Components

The following packages are authored and distributed by NVIDIA CORPORATION under the NVIDIA
proprietary license (`LicenseRef-NvidiaProprietary`). Each may internally bundle additional
third-party software whose notices are listed in subsequent sections.

## NVIDIA CORPORATION — omnimesh_ops_usd — NVIDIA Proprietary

Component: `vendor/omnimesh_ops_usd`

Description: USD-facing mesh operations library providing remeshing, decimation,
subdivision, triangulation, normal computation, slicing, attribute transfer, primitive
fitting, and related functionality with CPU and GPU acceleration.

```
Copyright (c) 2020-2026, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property and
proprietary rights in and to this software, related documentation and any
modifications thereto. Any use, reproduction, disclosure or distribution of
this software and related documentation without an express license agreement
from NVIDIA CORPORATION is strictly prohibited.
```

---

## NVIDIA CORPORATION — mesh_tools — NVIDIA Proprietary

Component: `vendor/mesh_tools`

Description: Mesh-processing toolkit providing decimation, reconstruction, USD loading,
visibility analysis, clash detection, duplication checking, and related utilities.

```
Copyright (c) 2020-2026, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property and
proprietary rights in and to this software, related documentation and any
modifications thereto. Any use, reproduction, disclosure or distribution of
this software and related documentation without an express license agreement
from NVIDIA CORPORATION is strictly prohibited.
```

---

## NVIDIA CORPORATION — shrinkwrap_openvdb — NVIDIA Proprietary

Component: `vendor/shrinkwrap_openvdb`

Description: Shrinkwrap library that converts meshes to level-set volumes and extracts
watertight meshes. Incorporates the OpenVDB library (see below).

```
Copyright (c) 2020-2026, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property and
proprietary rights in and to this software, related documentation and any
modifications thereto. Any use, reproduction, disclosure or distribution of
this software and related documentation without an express license agreement
from NVIDIA CORPORATION is strictly prohibited.
```

---

## NVIDIA CORPORATION — autouv-core — NVIDIA Proprietary

Component: `vendor/autouv-core`

Description: Automatic UV unwrapping library. Internally bundles Eigen for linear algebra
(see below).

```
Copyright (c) 2020-2026, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property and
proprietary rights in and to this software, related documentation and any
modifications thereto. Any use, reproduction, disclosure or distribution of
this software and related documentation without an express license agreement
from NVIDIA CORPORATION is strictly prohibited.
```

---

# Third-Party Open Source Components

## Academy Software Foundation — OpenVDB — Mozilla Public License 2.0

Bundled in: `vendor/shrinkwrap_openvdb`

Attribution: The `shrinkwrap_openvdb` library statically links OpenVDB, a volumetric data
structure library used for level-set operations. Source code is available at
https://github.com/AcademySoftwareFoundation/openvdb.

License Text (https://github.com/AcademySoftwareFoundation/openvdb/blob/master/LICENSE)

```
Mozilla Public License Version 2.0
====================================

1. Definitions
...

(See the full MPL 2.0 text at https://www.mozilla.org/en-US/MPL/2.0/)

Copyright Contributors to the OpenVDB Project.
```

---

## Eigen — Mozilla Public License 2.0

Bundled in: `vendor/autouv-core`

Attribution: The `autouv-core` library bundles Eigen internally for linear algebra. Eigen
is not directly exposed via the public headers of this repository. Source code is available
at https://gitlab.com/libeigen/eigen.

License Text (https://www.mozilla.org/en-US/MPL/2.0/)

```
Eigen is primarily MPL2 licensed. See COPYING.MPL2 and these links:
http://www.mozilla.org/MPL/2.0/
http://www.mozilla.org/MPL/2.0/FAQ.html

Some files contain third-party code under BSD or LGPL licenses, whence the other
COPYING.* files here.

All the LGPL code is either LGPL 2.1-only, or LGPL 2.1-or-later.
For this reason, the COPYING.LGPL file contains the LGPL 2.1 text.

If you want to guarantee that the Eigen code that you are #including is licensed
under the MPL2 and possibly more permissive licenses (like BSD), #define this
preprocessor symbol: EIGEN_MPL2_ONLY
```

---

# Notes

* The canonical and most up-to-date copyright, license, and embedded third-party notices
  for each NVIDIA package are the ones shipped inside that package (typically as `LICENSE`,
  `LICENSE.txt`, or `THIRD_PARTY_NOTICES`). The entries here are a repository-level summary
  intended to satisfy OSS attribution requirements at the level of this distribution.
* For each third-party component above, only a summary of the license text is shown. The
  full license texts are available at the URLs provided.

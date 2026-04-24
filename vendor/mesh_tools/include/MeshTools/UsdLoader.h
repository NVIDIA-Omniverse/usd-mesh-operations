// SPDX-FileCopyrightText: Copyright (c) 2024-2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.
#pragma once

#include "Stage.h"

namespace MeshTools
{

/**
 * @brief Parameters for the UsdLoader.
 * @param flipCoords If true, the Y and Z coordinates are flipped during loading.
 * @param convertFromCm If true, the coordinates are converted from centimeters to meters during loading
 * @param applyTransform If true, the transforms of the USD file are applied to the loaded meshes.
 * @param maxMeshes The maximum number of meshes to load. If zero, all meshes are loaded.
 */
struct UsdLoaderParams
{
    bool flipCoords = false;
    bool convertFromCm = false;
    bool applyTransform = false;
    int maxMeshes = 0;
};

/**
 * @brief Load a USDA file. This is a simplified loader. It might fail on complex USD files.
 *
 */
class UsdLoader
{
public:
    /**
     * @brief Creates a new loader.
     */
    MESHTOOLS_EXPORT UsdLoader();
    MESHTOOLS_EXPORT ~UsdLoader();

    /**
     * @brief Loads a USDA file into the given stage. This is a simplified loader. It might fail on complex USD files.
     * @param filename The path to the USDA file to load.
     * @param params The parameters for loading the USDA file.
     * @param stage The stage to load the meshes into.
     * @param errorMsg A string to receive an error message if loading fails.
     * @return True if the file was successfully loaded, false otherwise.
     */
    MESHTOOLS_EXPORT bool load(const char* filename,
                               const UsdLoaderParams& params,
                               MeshTools::Stage& stage,
                               std::string& errorMsg);


    /*! \cond PRIVATE */
protected:
    class UsdLoaderImpl;
    std::unique_ptr<UsdLoaderImpl> m_impl;
    /*! \endcond */
};

}

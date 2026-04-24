// SPDX-FileCopyrightText: Copyright (c) 2026 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this material, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.
#pragma once

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifdef _MSC_VER
#define AUTOUV_EXPORT __declspec(dllexport)
#else
#define AUTOUV_EXPORT __attribute__((visibility("default")))
#endif

#include <cstdint>

namespace autouv {

// ====================================================
// == Options for parameterization
// ====================================================

enum class AtlasPackMethod { Hybrid = 0, BruteForce, BruteForcePixelMask, RectangleBin, None };
enum class AtlasScaling { UnitBox = 0, WorldSpace };

struct ParameterizeOptions {

  // == meta
  int verbosity = 0;
  bool enable_threading = true; // if false, all computation will be run in the single main thread

  // == debugging
  bool do_viz = false;
  bool do_debug_viz = false;
  bool do_sanity_checks = false;

  // == Cuts, cones, and distortion

  // The all-important allowed distortion threshold
  // should be >1, it's a length scale factor
  double distortion_thresh = 2.;
  double distortion_fraction = 0.97; // this % of faces (by area) must satisfy the threshold

  // How many rounds of cut-to-disk shortening to run
  // This is VERY EXPENSIVE on ultra-high-genus models; by far dominates runtime. Setting to 0 means
  // more seams but greatly speeds up the pipeline.
  int32_t cut_shorten_iters = 2;

  // How many cones will we place in a patch before we partition it into two smaller patches.
  int32_t max_cones_per_round = 16;

  // Each round, place the first cone via an distortion measure computed from the actual attempted
  // parameterization, rather than approximated via a proxy solve. Should usually be left 'true'.
  bool place_first_cone_via_measured_distortion = true;

  // Patches with more than this many vertices will be immediately split into sub-patches, without
  // attempting any flattening etc, as a performance optimization.
  int32_t max_patch_size = 200000;

  double distortion_cone_factor = .75;    // factor is heuristic to account for discretization
  double partition_minimum_frac = .2; // must be < 1/4

  bool cut_until_injectivity_reached = true;
  bool cut_until_distortion_thresh_reached = true;

  bool use_intrinsic_mollification = true;
  double intrinsic_mollification_eps = 1e-5;

  bool modulate_edge_weights_by_dihedral = true;
  double modulate_edge_weights_by_dihedral_max_mult = 5.;
  double modulate_edge_weights_by_dihedral_cutoff_rad = 30 /* degrees */ * M_PI / 180.;

  // =====================================================
  // ==== Atlas Packing
  // =====================================================

  // Strategy used to pack the texture atlas
  //   AtlasPackMethod::None                -- no packing, place patches on top of each other
  //   AtlasPackMethod::Hybrid              -- automatic 'smart' strategy combining others
  //   AtlasPackMethod::RectangleBin        -- rectangle bin packing
  //   AtlasPackMethod::BruteForce          -- nonconvex packing (deprecated algorithm)
  //   AtlasPackMethod::BruteForcePixelMask -- nonconvex packing with a test grid mask
  AtlasPackMethod atlas_pack_method = AtlasPackMethod::Hybrid;

  // What policy should be used to scale the atlas?
  //   AtlasScaling::Unit   -- scale to [0,1]
  //   AtlasScaling::World  -- scale to world-space units as in the input mesh
  AtlasScaling atlas_scaling = AtlasScaling::UnitBox;

  // For the hybrid strategy, only brute-force pack for the the first N patches
  int32_t atlas_pack_hybrid_brute_count = 512;

  // For strategies which approximate packing on a grid, what resolution grid to use.
  // Also, when defining margin, this is used as the scale of the hypothetical image. That is, a
  // margin of "1" means "pack with margin of roughly 1 pixel if the UVs were used for a 512x512
  // pixel texture image", where this value is the 512.
  //
  // NOTE: At no point does this library construct an actual texture image of any resolution, and UV
  // coordinates are still reported as continous values on [0,1] (or world coords).  This is an
  // intermediate algorithm detail, which also lets us meaningfully define the margin in pixels.
  int32_t atlas_pack_res = 512;

  // How much space to leave between packed islands.
  //
  // Note that this is in terms of an image with resolution atlas_pack_image_res. That is, a
  // margin of "1" means "pack with margin of roughly 1 pixel if the UVs were used for a 512x512
  // pixel texture image", where this value is the 1.
  int32_t atlas_pack_margin = 1;

  // For brute-force packing strategies, how many rotations to try
  int32_t atlas_pack_brute_nrot = 8;

  // For brute-force packing strategies, how big of a jump to try translating (in 'pixel' units as
  // above)
  int32_t atlas_pack_brute_translate_inc = 8;

  // Enable a post-processing step that attempts to orient islands in UV space
  bool orient_islands_by_curvature = false;
};

} // namespace autouv

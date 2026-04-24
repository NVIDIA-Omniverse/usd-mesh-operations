// SPDX-FileCopyrightText: Copyright (c) 2020-2025 NVIDIA CORPORATION & AFFILIATES. All rights reserved.
// SPDX-License-Identifier: LicenseRef-NvidiaProprietary
//
// NVIDIA CORPORATION, its affiliates and licensors retain all intellectual
// property and proprietary rights in and to this software, related
// documentation and any modifications thereto. Any use, reproduction,
// disclosure or distribution of this material and related documentation
// without an express license agreement from NVIDIA CORPORATION or
// its affiliates is strictly prohibited.

#pragma once

#include <OmniMeshOps/Defines.h>
#include <iosfwd>

namespace omo::log
{

/// @brief Log severity levels, in order from most to least severe.
enum class Level
{
    Error = 0, ///< Error conditions; always routed to the error stream
    Warning,   ///< Conditions that are unexpected but not fatal
    Info,      ///< General informational messages
    Debug,     ///< Detailed diagnostic messages
};

/// @brief Returns the stream used for informational output (default: std::cout).
///
/// @return The current output stream, or nullptr if output is suppressed.
OMO_EXPORT
std::ostream*
getOutStream();

/// @brief Sets the stream used for informational output.
///
/// Pass nullptr to suppress all non-error output.
/// @param stream The stream to use, or nullptr to suppress.
OMO_EXPORT
void
setOutStream(std::ostream* stream);

/// @brief Returns the stream used for error output (default: std::cerr).
///
/// @return The current error stream, or nullptr if error output is suppressed.
OMO_EXPORT
std::ostream*
getErrStream();

/// @brief Sets the stream used for error output.
///
/// Pass nullptr to suppress error output.
/// @param stream The stream to use, or nullptr to suppress.
OMO_EXPORT
void
setErrStream(std::ostream* stream);

/// @brief Returns the current log level threshold (default: Level::Debug).
///
/// Messages with a level above this threshold are suppressed.
/// Level::Error messages are always emitted regardless of this setting.
OMO_EXPORT
Level
getLevel();

/// @brief Sets the log level threshold.
///
/// Only messages at or below this level will be emitted. Level::Error messages
/// are always emitted and can only be suppressed by passing nullptr to setErrStream().
/// @param level The maximum level to emit.
OMO_EXPORT
void
setLevel(Level level);

/// @brief Returns whether ANSI color codes are applied to output (default: true).
OMO_EXPORT
bool
getColorize();

/// @brief Sets whether ANSI color codes are applied to output.
///
/// When enabled, error messages are colored red and warnings yellow.
/// Info and debug messages are never colored.
/// @param colorize True to enable ANSI coloring, false to disable.
OMO_EXPORT
void
setColorize(bool colorize);

} // namespace omo::log

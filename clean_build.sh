#!/bin/sh
#
# Convenience script to create a fresh build of Input Leap. The script
# locates an appropriate CMake binary, prepares an isolated build
# directory, and then compiles the project.

# Ensure the working directory is the repository root so relative paths
# behave as expected.
cd "$(dirname "$0")" || exit 1

# some environments have cmake v2 as 'cmake' and v3 as 'cmake3'
# check for cmake3 first then fallback to just cmake
[ -n "$B_CMAKE" ] || B_CMAKE=$(command -v cmake3)
[ -n "$B_CMAKE" ] || B_CMAKE=$(command -v cmake)
if [ -z "$B_CMAKE" ]; then
    echo "ERROR: CMake not in $PATH, cannot build! Please install CMake, or if this persists, file a bug report."
    exit 1
fi

# Default location for generated build files; can be overridden with
# the B_BUILD_DIR environment variable.
B_BUILD_DIR="${B_BUILD_DIR:-build}"
# Build type defaults to Debug unless B_BUILD_TYPE is provided.
B_BUILD_TYPE="${B_BUILD_TYPE:-Debug}"
# Initial CMake flags used for configuration; callers may append more via
# B_CMAKE_FLAGS.
B_CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=${B_BUILD_TYPE} ${B_CMAKE_FLAGS:-}"

if [ "$(uname)" = "Darwin" ]; then
    # macOS needs a little help, so we source this environment script to fix paths.
    [ -e ./macos_environment.sh ] && . ./macos_environment.sh
    B_CMAKE_FLAGS="${B_CMAKE_FLAGS} -DCMAKE_OSX_SYSROOT=$(xcrun --sdk macosx --show-sdk-path) -DCMAKE_OSX_DEPLOYMENT_TARGET=10.9"
fi

# Prefer the Ninja generator if it exists to speed up builds.
if command -v ninja 2>/dev/null; then
    B_CMAKE_FLAGS="-GNinja ${B_CMAKE_FLAGS}"
fi

# allow local customizations to build environment
[ -r ./build_env.sh ] && . ./build_env.sh

# Exit immediately if any command fails to prevent continuing from a
# partially configured state.
set -e

# Ensure all required third-party code is available.  This step pulls
# down and updates any git submodules the project depends on.
git submodule update --init --recursive

# Remove any existing build directory to ensure a clean start
if [ -z "$B_BUILD_DIR" ]; then
    echo "ERROR: B_BUILD_DIR is empty; refusing to clean" >&2
    exit 1
fi

REPO_ROOT=$(pwd -P)
B_BUILD_DIR_ABS=$(python3 -c 'import os,sys; print(os.path.realpath(sys.argv[1]))' "$B_BUILD_DIR")


# Validate that the build directory resides within the repository and is
# not the repository root itself.  This guards against accidental deletion
# of arbitrary paths when cleaning the build directory.
case "$B_BUILD_DIR_ABS" in
    "$REPO_ROOT")
        echo "ERROR: B_BUILD_DIR '$B_BUILD_DIR' is the repository root" >&2
        exit 1
        ;;
    "$REPO_ROOT"/*)
        ;;
    *)
        echo "ERROR: B_BUILD_DIR '$B_BUILD_DIR' is not within the repository root" >&2
        exit 1
        ;;
esac

rm -rf "$B_BUILD_DIR"
mkdir "$B_BUILD_DIR"
cd "$B_BUILD_DIR"
echo "Starting Input Leap $B_BUILD_TYPE build in '${B_BUILD_DIR}'..."
# Configure the project
"$B_CMAKE" $B_CMAKE_FLAGS ..
# Build using all available cores
"$B_CMAKE" --build . --parallel
echo "Build completed successfully"

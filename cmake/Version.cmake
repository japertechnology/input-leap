# Define the project's version components. These values are used throughout
# the build to embed version information into the binaries.
set(INPUTLEAP_VERSION_MAJOR 3)
set(INPUTLEAP_VERSION_MINOR 0)
set(INPUTLEAP_VERSION_PATCH 3)

# Determine a descriptive version string if one was not provided. When building
# from a Git checkout we capture the commit date and hash; otherwise we fall back
# to "unknown" to avoid failing the build when Git is unavailable.
if((NOT DEFINED INPUTLEAP_VERSION_DESC) OR (INPUTLEAP_VERSION_DESC STREQUAL git))
    find_program (GIT_BINARY git)
    if (GIT_BINARY STREQUAL "GIT_BINARY-NOTFOUND")
        set(INPUTLEAP_VERSION_DESC unknown)
    else()
        execute_process (
            COMMAND git log --pretty=format:'git-%cs-%h' -1
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            OUTPUT_VARIABLE INPUTLEAP_VERSION_DESC
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
    endif()
endif()

# Record the build timestamp in several formats for use in about dialogs and
# embedded diagnostics.
string(TIMESTAMP INPUTLEAP_BUILD_DATE "%Y%m%d" UTC)
string(TIMESTAMP INPUTLEAP_BUILD_TIME "%H%M" UTC)
string(TIMESTAMP INPUTLEAP_BUILD_DATE_ISO "%Y-%m-%d" UTC)

# Compose the full version string and log it for developers building the
# project.
set(INPUTLEAP_VERSION "${INPUTLEAP_VERSION_MAJOR}.${INPUTLEAP_VERSION_MINOR}.${INPUTLEAP_VERSION_PATCH}-${INPUTLEAP_VERSION_DESC}")
message(STATUS "Full InputLeap version string is '" ${INPUTLEAP_VERSION} "'")

# Export version details as preprocessor definitions so the application can
# display its version and build metadata at runtime.
add_definitions(-DINPUTLEAP_VERSION="${INPUTLEAP_VERSION}")
add_definitions(-DINPUTLEAP_VERSION_STRING="${INPUTLEAP_VERSION_STRING}")
add_definitions(-DINPUTLEAP_VERSION_MAJOR=${INPUTLEAP_VERSION_MAJOR})
add_definitions(-DINPUTLEAP_VERSION_MINOR=${INPUTLEAP_VERSION_MINOR})
add_definitions(-DINPUTLEAP_VERSION_PATCH=${INPUTLEAP_VERSION_PATCH})
add_definitions(-DINPUTLEAP_REVISION="${INPUTLEAP_REVISION}")
add_definitions(-DINPUTLEAP_BUILD_DATE="${INPUTLEAP_BUILD_DATE}")


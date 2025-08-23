# Configure CPack packaging only when building on UNIX-like platforms.
if (UNIX)
    # Propagate the project version into the generated package metadata.
    set(CPACK_PACKAGE_VERSION_MAJOR ${INPUTLEAP_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${INPUTLEAP_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${INPUTLEAP_VERSION_PATCH})

    # Produce a compressed tarball for binary packages and a separate source
    # archive containing the original files.
    set (CPACK_GENERATOR "TBZ2")
    set (CPACK_SOURCE_GENERATOR "TXZ")

    # Exclude common build artifacts and repository metadata from the source
    # archive to keep the distribution clean.
    set (CPACK_SOURCE_IGNORE_FILES "/build/;\.gitignore$;/\.git/;/.github/;\.swp$;build_env\.*")
    include (CPack)
endif()

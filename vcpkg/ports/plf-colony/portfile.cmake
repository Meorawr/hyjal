# header-only library

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO mattreecebentley/plf_colony
    REF 28abaa9acb683d064033fe32d58ec7d1267224d2
    SHA512 857d8635a24d402a81625fae9a2ab26a6e151faeebb34e2d5a6b6cb7cf67e61224de727d38789ba4e486d8b13ca7ab21b93e2228c222f333d6a1b12e0b8db3a1
    HEAD_REF master
)

file(COPY "${SOURCE_PATH}/plf_colony.h" DESTINATION "${CURRENT_PACKAGES_DIR}/include")

file(INSTALL "${SOURCE_PATH}/LICENSE.md" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

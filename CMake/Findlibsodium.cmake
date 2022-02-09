#
# Finds libsodium and populates variables and imported targets
# 
# Hints:
#   libsodium_ROOT
#   libsodium_INCLUDEDIR
#   libsodium_LIBRARYDIR
#   libsodium_NO_SYSTEM_PATHS
#
# Variables:
#   libsodium_FOUND
#   libsodium_INCLUDE_DIR
#   libsodium_LIBRARY
# 
# Targets:
#   libsodium::libsodium
#

set(INCLUDE_SEARCH_PATHS)
set(LIBRARY_SEARCH_PATHS)

if(libsodium_ROOT)
    list(APPEND INCLUDE_SEARCH_PATHS ${libsodium_ROOT}/include)
    list(APPEND LIBRARY_SEARCH_PATHS ${libsodium_ROOT}/lib)
endif()

if(libsodium_INCLUDEDIR)
    list(APPEND INCLUDE_SEARCH_PATHS ${libsodium_INCLUDEDIR})
endif()

if(libsodium_LIBRARYDIR)
    list(APPEND LIBRARY_SEARCH_PATHS ${libsodium_LIBRARYDIR})
endif()

find_package(PkgConfig)
pkg_check_modules(PC_libsodium QUIET libsodium)

list(APPEND INCLUDE_SEARCH_PATHS ${PC_libsodium_INCLUDE_DIRS})
list(APPEND LIBRARY_SEARCH_PATHS ${PC_libsodium_LIBRARY_DIRS})
set(libsodium_VERSION ${PC_libsodium_VERSION})


if(libsodium_NO_SYSTEM_PATHS)
    find_path(libsodium_INCLUDE_DIR
        NAMES sodium.h
        PATHS 
            ${INCLUDE_SEARCH_PATHS}
            ${CMAKE_INCLUDE_PATH}
            ${CMAKE_SYSTEM_INCLUDE_PATH}
            ${CMAKE_INSTALL_PREFIX}/include
        NO_DEFAULT_PATH)

    find_library(libsodium_LIBRARY
        NAMES sodium
        PATHS
            ${LIBRARY_SEARCH_PATHS}
            ${CMAKE_LIBRARY_PATH}
            ${CMAKE_SYSTEM_LIBRARY_PATH}
            ${CMAKE_INSTALL_PREFIX}/lib
        NO_DEFAULT_PATH)
else()
    find_path(libsodium_INCLUDE_DIR
        NAMES sodium.h
        PATHS ${INCLUDE_SEARCH_PATHS})
    find_library(libsodium_LIBRARY
        NAMES sodium
        PATHS ${LIBRARY_SEARCH_PATHS})
endif()

mark_as_advanced(
    libsodium_INCLUDE_DIR
    libsodium_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libsodium
    FOUND_VAR libsodium_FOUND
    REQUIRED_VARS
        libsodium_LIBRARY
        libsodium_INCLUDE_DIR
    VERSION_VAR libsodium_VERSION)

if(libsodium_FOUND)
    set(libsodium_LIBRARIES ${libsodium_LIBRARY})
    set(libsodium_INCLUDE_DIRS ${libsodium_INCLUDE_DIR})

    if(NOT TARGET libsodium::libsodium)
        add_library(libsodium::libsodium UNKNOWN IMPORTED)
        set_target_properties(libsodium::libsodium PROPERTIES
            IMPORTED_LOCATION ${libsodium_LIBRARY}
            INTERFACE_INCLUDE_DIRECTORIES ${libsodium_INCLUDE_DIR})
    endif()
endif()


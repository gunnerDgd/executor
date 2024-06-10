cmake_minimum_required (VERSION 3.23)
project                (vapor)

if   (NOT DEFINED PRESET_ARCH)
    message("[vapor] Architecture Not Defined. Abort.")
    message(SEND_ERROR)
endif()

if   (NOT DEFINED PRESET_OS)
    message("[vapor] OS Not Defined. Abort.")
    message(SEND_ERROR)
endif()

add_subdirectory   (src)
add_subdirectory   (package)
if (PRESET_BUILD_EXAMPLES)
    add_subdirectory   (examples)
endif()
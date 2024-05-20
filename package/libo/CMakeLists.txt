add_library(libo STATIC
    src/libo.h
    src/libo.c

    src/arg.h
    src/arg.c

    src/image.h
    src/image.c

    src/kernel.h
    src/kernel.c
)

target_include_directories(libo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
if   (WIN32)
    target_include_directories(libo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_library(libo-include STATIC
            src/asm/bootparam.h
            src/asm/ist.h

            src/linux/apm_bios.h
            src/linux/edd.h
            src/linux/screen_info.h
            src/linux/type.h

            src/video/edid.h
            include/libo/libo.h
    )

    set_target_properties(libo-include PROPERTIES LINKER_LANGUAGE C)
else ()
    target_include_directories(libo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
    add_library               (libo-include STATIC include/libo/libo.h)

    set_target_properties(libo-include PROPERTIES LINKER_LANGUAGE C)
endif()
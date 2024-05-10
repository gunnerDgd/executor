if   (PRESET_LINUX)
    if   (PRESET_ARCH_X86_64)
        add_subdirectory(core/x86_64/kvm)
    endif()
endif()

add_subdirectory(bus)
add_subdirectory(dev)
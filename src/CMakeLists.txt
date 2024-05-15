if   (PRESET_ARCH_X86_64)
    if     (UNIX AND NOT APPLE)
        add_subdirectory(core/x86_64/kvm)
    elseif (WIN32)
        add_subdirectory(core/x86_64/hyperv)
    endif  ()
endif()

add_subdirectory(bus)
add_subdirectory(dev)
add_subdirectory(firmware)

add_library          (vapor INTERFACE)
target_link_libraries(vapor INTERFACE
    vapor-core
    vapor-bus
    vapor-dev
    asterisk
    crisp
)
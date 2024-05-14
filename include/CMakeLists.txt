add_library (vapor-include STATIC
    vp/core.h
    vp/core/bus.h
    vp/core/cpu.h
    vp/core/irq.h
    vp/core/map.h
    vp/core/pa.h
    vp/core/va.h
    vp/core/pic.h
    vp/core/root.h
)

set_target_properties(vapor-include PROPERTIES LINKER_LANGUAGE C)
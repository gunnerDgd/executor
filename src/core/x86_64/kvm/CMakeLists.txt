add_library (vapor-core STATIC
    ../../../core.h
    ../../x86_64.h
    ../kvm.h
    ../kvm.c

    root.h
    root.c

    cpu.h
    cpu.c

    pa.h
    pa.c

    va.h
    va.c

    bus.h
    bus.c

    map.h
    map.c

    pic.h
    pic.c

)
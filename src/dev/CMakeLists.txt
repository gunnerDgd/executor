add_library (vapor-dev STATIC
    ../dev.h

    pio.h
    pio.c
)

target_link_libraries (vapor-dev PRIVATE crisp)
add_executable            (vapor-dev-01-pio pio.c)
target_link_libraries     (vapor-dev-01-pio crisp asterisk vapor)
target_include_directories(vapor-dev-01-pio PRIVATE ../../include)
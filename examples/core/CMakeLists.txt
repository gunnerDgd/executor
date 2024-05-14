add_executable            (vapor-core-01-basic basic.c)
target_link_libraries     (vapor-core-01-basic crisp asterisk vapor-core)
target_include_directories(vapor-core-01-basic PRIVATE ../../include)
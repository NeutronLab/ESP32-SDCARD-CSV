idf_component_register(SRCS "csv_file.cpp" "sdcard.cpp" "file_operations.cpp"
                    INCLUDE_DIRS ".")
target_compile_options(${COMPONENT_LIB} PRIVATE "-std=gnu++17")

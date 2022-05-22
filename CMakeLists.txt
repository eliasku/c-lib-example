cmake_minimum_required(VERSION 3.19)
project(mylib C)

if ($ENV{CLION_IDE})
    set(UNIT_TESTING ON)
endif ()

include(cmake/code-coverage.cmake)

add_library(${PROJECT_NAME} STATIC
        src/mylib-all.c
        )

target_include_directories(${PROJECT_NAME} PUBLIC include PRIVATE test)
target_code_coverage(${PROJECT_NAME} PUBLIC)

if (UNIT_TESTING)
    enable_testing()

    add_subdirectory(test)
endif ()

if (UNIT_TESTING)
    add_executable(${PROJECT_NAME}-test
            src/mylib-all.c
            )
    target_compile_definitions(${PROJECT_NAME}-test PUBLIC UNIT_TESTING)
    target_include_directories(${PROJECT_NAME}-test PUBLIC include PRIVATE test)
    add_test(NAME ${PROJECT_NAME}-test COMMAND $<TARGET_FILE:${PROJECT_NAME}-test>)
    test_code_coverage(${PROJECT_NAME}-test)
endif()

coverage_all()


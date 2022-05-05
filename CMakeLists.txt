cmake_minimum_required(VERSION 3.19)
project(mylib C)

function(target_code_coverage TARGET_NAME)
    # Argument parsing
    set(options AUTO ALL EXTERNAL PUBLIC INTERFACE)
    set(single_value_keywords COVERAGE_TARGET_NAME)
    set(multi_value_keywords EXCLUDE OBJECTS ARGS)
    cmake_parse_arguments(target_code_coverage "${options}" "${single_value_keywords}" "${multi_value_keywords}" ${ARGN})
    # Set the visibility of target functions to PUBLIC, INTERFACE or default to PRIVATE
    if (target_code_coverage_PUBLIC)
        set(TARGET_VISIBILITY PUBLIC)
    elseif (target_code_coverage_INTERFACE)
        set(TARGET_VISIBILITY INTERFACE)
    else ()
        set(TARGET_VISIBILITY PRIVATE)
    endif ()

    if (NOT target_code_coverage_COVERAGE_TARGET_NAME)
        # If a specific name was given, use that instead.
        set(target_code_coverage_COVERAGE_TARGET_NAME ${TARGET_NAME})
    endif ()

    if (CODE_COVERAGE)
        target_compile_options(${TARGET_NAME} ${TARGET_VISIBILITY} -fprofile-instr-generate -fcoverage-mapping)
        target_link_options(${TARGET_NAME} ${TARGET_VISIBILITY} -fprofile-instr-generate -fcoverage-mapping)
    endif ()
endfunction()

function(test_code_coverage TARGET_NAME)
    if (CODE_COVERAGE)
        list(APPEND COVERAGE_COLLECT_TARGETS coverage-collect-${TARGET_NAME})
        set(COVERAGE_COLLECT_TARGETS ${COVERAGE_COLLECT_TARGETS} PARENT_SCOPE)

        list(APPEND COVERAGE_RAW_PROFILES ${CMAKE_BINARY_DIR}/${TARGET_NAME}.profraw)
        set(COVERAGE_RAW_PROFILES ${COVERAGE_RAW_PROFILES} PARENT_SCOPE)

        list(APPEND COVERAGE_OBJECT_FILES -object $<TARGET_FILE:${TARGET_NAME}>)
        set(COVERAGE_OBJECT_FILES ${COVERAGE_OBJECT_FILES} PARENT_SCOPE)

        add_custom_target(coverage-collect-${TARGET_NAME}
                COMMAND LLVM_PROFILE_FILE=${CMAKE_BINARY_DIR}/${TARGET_NAME}.profraw $<TARGET_FILE:${TARGET_NAME}>
                )
    endif ()
endfunction()

function(coverage_all)
    if (CODE_COVERAGE)
        if (${CMAKE_SYSTEM_NAME} MATCHES "iOS|Darwin")
            set(LLVM_PROFDATA xcrun llvm-profdata)
            set(LLVM_COV xcrun llvm-cov)
        else ()
            set(LLVM_PROFDATA llvm-profdata)
            set(LLVM_COV llvm-cov)
        endif ()
        set(COVERAGE_IGNORE_PATTERN test/*)
        add_custom_target(coverage
                COMMAND ${LLVM_PROFDATA} merge -sparse ${COVERAGE_RAW_PROFILES} -o ${CMAKE_BINARY_DIR}/coverage.profdata
                COMMAND ${LLVM_COV} show ${COVERAGE_OBJECT_FILES} -instr-profile=${CMAKE_BINARY_DIR}/coverage.profdata -ignore-filename-regex='test/*' > ${CMAKE_BINARY_DIR}/coverage.txt
                COMMAND ${LLVM_COV} report ${COVERAGE_OBJECT_FILES} -instr-profile=${CMAKE_BINARY_DIR}/coverage.profdata -use-color=1 -ignore-filename-regex='${COVERAGE_IGNORE_PATTERN}'
                DEPENDS ${COVERAGE_COLLECT_TARGETS}
                )
    endif ()
endfunction()

add_library(${PROJECT_NAME} STATIC
        src/mylib-all.c
        )

target_include_directories(${PROJECT_NAME} PUBLIC include)
target_code_coverage(${PROJECT_NAME} PUBLIC)

set(TARGET_NAME test_${PROJECT_NAME}_simple)
enable_testing()
add_executable(${TARGET_NAME} test/simple/main.c)
target_link_libraries(${TARGET_NAME} PRIVATE mylib)
add_test(NAME ${TARGET_NAME} COMMAND $<TARGET_FILE:${TARGET_NAME}>)

add_executable(test_init_term test/simple/init-term.c)
target_link_libraries(test_init_term PRIVATE mylib)
add_test(NAME test_init_term COMMAND $<TARGET_FILE:test_init_term>)

test_code_coverage(${TARGET_NAME})
test_code_coverage(test_init_term)

coverage_all()

#foreach (line IN LISTS txt)
#    string(APPEND multiline "${line}\n")
#endforeach ()

#add_custom_target(coverage
#        COMMAND LLVM_PROFILE_FILE=coverage.profraw $<TARGET_FILE:${TARGET_NAME}> & $<TARGET_FILE:test_init_term>
#        COMMAND xcrun llvm-profdata merge -sparse coverage.profraw -o coverage.profdata
#        COMMAND xcrun llvm-cov show $<TARGET_FILE:${TARGET_NAME}> $<TARGET_FILE:test_init_term> -instr-profile=coverage.profdata -ignore-filename-regex='test/*' > coverage.txt
#        COMMAND xcrun llvm-cov report $<TARGET_FILE:${TARGET_NAME}> $<TARGET_FILE:test_init_term> -instr-profile=coverage.profdata -use-color=1 -ignore-filename-regex='test/*'
#        DEPENDS ${TARGET_NAME} test_init_term
#        )
#
#add_custom_target(coverage
##        COMMAND ;
#        DEPENDS ${COVERAGE_TARGETS}
#        )

#add_custom_target(ccov
#        COMMAND xcrun llvm-profdata merge -sparse "${PROFRAW_FILES}" -o coverage_data.profdata
#        COMMAND xcrun llvm-cov report $<TARGET_FILE:${TARGET_NAME}> -instr-profile=${TARGET_NAME}.profdata -use-color=1
#        DEPENDS "${COV_TARGETS}"
#        )


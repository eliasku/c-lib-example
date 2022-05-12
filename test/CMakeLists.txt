cmake_minimum_required(VERSION 3.19)

enable_testing()

project(mylib-simple-test C)
add_executable(${PROJECT_NAME} simple/main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE mylib)
add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
test_code_coverage(${PROJECT_NAME})

project(mylib-simple-test2 C)
add_executable(${PROJECT_NAME} simple/init-term.c)
target_link_libraries(${PROJECT_NAME} PRIVATE mylib)
add_test(NAME ${PROJECT_NAME} COMMAND $<TARGET_FILE:${PROJECT_NAME}>)
test_code_coverage(${PROJECT_NAME})

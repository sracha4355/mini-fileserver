include(FetchContent)
FetchContent_Declare(
	googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
	GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)



file(GLOB_RECURSE PARSER_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/parser/*.cpp")
file(GLOB_RECURSE TOKENIZER_TESTS "${CMAKE_CURRENT_SOURCE_DIR}/tokenizer/*.cpp")

message(STATUS "parser tests " ${PARSER_TESTS})

enable_testing()
add_executable(
	parser_tests
	${PARSER_TESTS}
)
add_executable(
	tokenizer_tests
	${TOKENIZER_TESTS}
)
message(STATUS "include path ${CMAKE_CURRENT_SOURCE_DIR}/../include")
target_include_directories(parser_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(tokenizer_tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_link_libraries(parser_tests parserlib)
target_link_libraries(parser_tests GTest::gtest_main)

target_link_libraries(tokenizer_tests parserlib)
target_link_libraries(tokenizer_tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(parser_tests)
gtest_discover_tests(tokenizer_tests)

set_target_properties(
	parser_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/../build/tests
)
set_target_properties(
	tokenizer_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY
	${CMAKE_CURRENT_SOURCE_DIR}/../build/tests
)


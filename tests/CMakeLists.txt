include(FetchContent)
# find_package(GTest REQUIRED)

# if(NOT GTest_FOUND)
  # message("!!!!!!!!!!!!! NOT GTest_FOUND !!!!!!!!!!!!!!")
  # download gtest
  FetchContent_Declare(
    googletest
    #URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.13.0
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
# endif()

enable_testing()

# add_subdirectory()

add_executable(
  tests
  models/store_test.cc
    #   sqlstatementbuilder.h
    # sqlmapper.h
)
target_link_libraries(
    tests
    PRIVATE 
    # store
    GTest::gtest
    # GTest::gmock
    GTest::gtest_main
    pqxx
)
include(GoogleTest)
gtest_discover_tests(tests)

# if (WIN32)
#   add_custom_command(
#         TARGET tests POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_BINARY_DIR}/_deps/libpqxx-build/src/Debug" "$<TARGET_FILE_DIR:tests>"
#         VERBATIM
#     )
#     add_custom_command(
#         TARGET tests POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy_directory "C:/Program Files/PostgreSQL/16/bin" "$<TARGET_FILE_DIR:tests>"
#         VERBATIM
#     )
# endif()
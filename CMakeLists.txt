cmake_minimum_required(VERSION 3.25)
project(app)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# find_package(PostgreSQL REQUIRED)
# include_directories (${PostgreSQL_INCLUDE_DIRS})
# link_directories(${PostgreSQL_LIBRARY_DIRS})

# # The PostgreSQL library.
# set (PostgreSQL_LIBRARY_TO_FIND pq)
# # Setting some more prefixes for the library
# set (PostgreSQL_LIB_PREFIX "")
# if ( WIN32 )
#   set (PostgreSQL_LIB_PREFIX ${PostgreSQL_LIB_PREFIX} "lib")
#   set (PostgreSQL_LIBRARY_TO_FIND ${PostgreSQL_LIB_PREFIX}${PostgreSQL_LIBRARY_TO_FIND})
# endif()

# find_library(PostgreSQL_LIBRARY
#  NAMES ${PostgreSQL_LIBRARY_TO_FIND}
#  PATHS
#    ${PostgreSQL_ROOT_DIRECTORIES}
#  PATH_SUFFIXES
#    lib
#    ${PostgreSQL_LIBRARY_ADDITIONAL_SEARCH_SUFFIXES}
#  # Help the user find it if we cannot.
#  DOC "The ${PostgreSQL_LIBRARY_DIR_MESSAGE}"
# )
include(cmake/libpqxx.cmake)
add_subdirectory(src)
add_subdirectory(tests)

add_executable(${PROJECT_NAME})

# get_target_property(LIBPQXX_BIN_DIR libpqxx LOCATION)
# message(${LIBPQXX_BIN_DIR})
# include(cmake/libodb.cmake)
# include(cmake/libgtest.cmake)

target_sources(
    ${PROJECT_NAME}
    PUBLIC
    src/main.cc
    src/sqlmapper.h
    src/dbconfig.h
    src/sqlstatementbuilder.h
    src/models.h
    src/types.h
)

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    pqxx
    # models
    # sqlmapper
)

# if (WIN32)
# # C:\term_7\рпбд\l1\build\_deps\libpqxx-build\src\Debug
# # message("AZAZAZAZAZAZ ${PROJECT_SOURCE_DIR}")
# # message("AZAZAZAZAZAZ ${CMAKE_BINARY_DIR}")
# # message("$<TARGET_FILE_DIR:${PROJECT_NAME}>")
# # message("${RUNTIME_OUTPUT_DIRECTORY}")
# # message("$<RUNTIME_OUTPUT_DIRECTORY:${PROJECT_NAME}>")
#     add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_BINARY_DIR}/_deps/libpqxx-build/src/Debug" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#         VERBATIM
#     ) # visual studio is so stupid, copy pqxx.lib to bin dir
# endif()

# if (WIN32)
#     set(LIBPQXX_DLL_PATH "${CMAKE_BINARY_DIR}/_deps/libpqxx-build/src/Debug")
#     # Пример использования set_target_properties для отладки в Visual Studio
#     set_target_properties(${PROJECT_NAME} PROPERTIES 
#         VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;${LIBPQXX_DLL_PATH}"
#     )
# endif()

# set(POSTGRESQL_BINARY_DIR "C:/Program Files/PostgreSQL/16/bin")
# if (WIN32)
#     add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_BINARY_DIR}/_deps/libpqxx-build/src/Debug" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#         VERBATIM
#     ) # visual studio is so stupid, copy pqxx.lib to bin dir
#     add_custom_command(
#         TARGET ${PROJECT_NAME} POST_BUILD
#         COMMAND "${CMAKE_COMMAND}" -E copy_directory "${POSTGRESQL_BINARY_DIR}" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
#         VERBATIM
#     )
#     # set_target_properties(${PROJECT_NAME} PROPERTIES 
#     #     VS_DEBUGGER_ENVIRONMENT "PATH=%PATH%;$(ExecutablePath)"
#     # )
# endif()
# cmake -S . -B build
find_package(libpqxx 7.9.2 QUIET)

if(NOT libpqxx_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG 7.9.2
    )
    set(SKIP_BUILD_TEST ON)
    # On Windows we generally recommend building libpqxx as a shared
    # library.  On other platforms, we recommend a static library.
    if(WIN32)
        set(BUILD_SHARED_LIBS ON)
    else()
        set(BUILD_SHARED_LIBS OFF)
    endif()

    FetchContent_MakeAvailable(libpqxx)
endif()

# set(BUILD_DOC OFF)
# set(SKIP_PQXX_SHARED ON)
# set(SKIP_BUILD_TEST ON)
# set(BUILD_SHARED_LIBS OFF)
# On Windows we generally recommend building libpqxx as a shared
# library.  On other platforms, we recommend a static library.
# IF (WIN32)
#     set(BUILD_SHARED_LIBS ON)
# ELSE()
#     set(BUILD_SHARED_LIBS OFF)
# ENDIF()

# message("-- Linking SDL")
# add_library(pqxx SDL2.dll)
# set_target_properties(SDL2 PROPERTIES LINKER_LANGUAGE CXX)

# FetchContent_GetProperties(pqxx)
# option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
# # -DBUILD_SHARED_LIBS=
# # You can usually skip building the tests.
# set(SKIP_BUILD_TEST ON)



# get_target_property(LIBPQXX_OUTPUT_DIR pqxx LOCATION)
# message("AZAZAZAZAZAZ ${LIBPQXX_OUTPUT_DIR}")

# target_link_libraries(${PROJECT_NAME} PRIVATE pqxx)
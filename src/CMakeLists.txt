add_library(types SHARED)
target_sources(
    types
    PUBLIC
    FILE_SET CXX_MODULES FILES
    types.cxx
)

add_library(models SHARED)
target_sources(
    models
    PUBLIC
    FILE_SET CXX_MODULES FILES
    models.cxx
)
target_link_libraries(
    models
    types
)

add_subdirectory(mapper)
# target_sources(
#     ${PROJECT_NAME} 
#     PUBLIC
#     FILE_SET CXX_MODULES FILES
#     store.cxx
# )

# add_library(mapper SHARED)
# target_sources(
#     mapper
#     PUBLIC
#     FILE_SET CXX_MODULES FILES
#     models.cxx
# )

add_library(
    sqlstatementbuilder
)
target_sources(
    sqlstatementbuilder
    PUBLIC
    FILE_SET CXX_MODULES FILES
    sqlstatementbuilder.cxx
)
target_link_libraries(
    sqlstatementbuilder
    types
)

add_library(sqlmapper SHARED)
target_sources(
    sqlmapper
    PUBLIC
    FILE_SET CXX_MODULES FILES
    sqlmapper.cxx
)
target_link_libraries(
    sqlmapper
    pqxx
    types
    sqlstatementbuilder
)
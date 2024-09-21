add_library(store SHARED)
target_sources(
    store
    PUBLIC
    FILE_SET CXX_MODULES FILES
    store.cxx
)

# add_library(
#   tokenizer
#   tokenizer.h
#   tokenizer.cc
# )
# target_link_libraries(
#     tokenizer
#     token
# )

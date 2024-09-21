# set(RESINSIGHT_ODB_VERSION
#     "2020"
#     CACHE STRING "Abaqus API version to use, 2020 or 2024"
# )
# FetchContent_Declare(
#     odb-library-from-server
#     URL http://10.10.0.26:8080/job/resinsight-dependencies/ws/${RESINSIGHT_ODB_VERSION}/odb_api_win64.zip
#   )

#   FetchContent_Populate(odb-library-from-server)
#   set(RESINSIGHT_ODB_API_DIR ${odb-library-from-server_SOURCE_DIR})
#   message("${RESINSIGHT_ODB_API_DIR}")
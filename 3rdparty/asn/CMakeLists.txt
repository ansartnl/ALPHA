cmake_minimum_required(VERSION 2.8.9)

project(asn)

message("Building ASN solution")

include(cmake/output-directories.cmake)
include(cmake/compiler-flags.cmake)
include(cmake/using-preset-defaults.cmake)

declare_output_directories()

set_multithreaded_build()
set_cxx11_build()
set_warnings_as_errors_build()
set_default_definitions()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_debug_postfix("d")
set(PROJECT_FOLDER "ASN")

include(cmake/set-executable-specifics.cmake)
include(cmake/set-version.cmake)
include(cmake/copy-files.cmake)
include(cmake/sources-management.cmake)
include(cmake/resources-management.cmake)
include(cmake/translation-management.cmake)
include(cmake/version-dep-commands.cmake)

add_subproject(asn1 asn1)
add_subproject(asnparser asnparser)

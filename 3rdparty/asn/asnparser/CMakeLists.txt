cmake_minimum_required(VERSION 2.8.9)

###################################################
###  SECTION Project Settings
###################################################

project(asnparser)

message("Building asnparser")

set_version(asnparser 1 0 0)

###################################################
###  END OF SECTION Project Settings
###################################################


###################################################
###  SECTION Frameworks and Libraries
###################################################

#########
# boost
#########

find_package(Boost REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

###################################################
###  END OF SECTION Frameworks and Libraries
###################################################


####################################################
###  SECTION Sources
###################################################

add_group("" ""
	asn_grammar.cxx
	asn_grammar.h
	asn_lex.cxx
	asn_ref_grammar.cxx
	asn_ref_grammar.h
	asn_ref_lex.cxx
	getopt.c
	main.cxx
	main.h
	unistd.h
)

add_group("others" "others"
	asn_grammar.y
	asn_lex.l
	asn_ref_grammar.y
	asn_ref_lex.l
)

###################################################
###  END OF SECTION Sources
###################################################


###################################################
###  SECTION Build
###################################################

prepare_build_components()

add_executable(asnparser ${BUILD_COMPONENTS})

set_property(TARGET asnparser PROPERTY FOLDER ${PROJECT_FOLDER})

configure_build(asnparser)

set_output_directories(asnparser)

###################################################
###  END OF SECTION Build
###################################################

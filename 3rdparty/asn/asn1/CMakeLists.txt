cmake_minimum_required(VERSION 2.8.9)

###################################################
###  SECTION Project Settings
###################################################

project(asn1)

message("Building asn1")

set_version(asn1 1 0 0)

add_definitions(
	-DASN1_HAS_IOSTREAM
	-DASN1_STATIC
)

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif(UNIX)

###################################################
###  END OF SECTION Project Settings
###################################################


###################################################
###  SECTION Frameworks and Libraries
###################################################

######
# Qt
######

find_qt(Core)


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
	asn_factory.h
	asn1.cxx
	asn1.h
	AssocVector.h
	AVNDecoder.cxx
	AVNEncoder.cxx
	BERDecoder.cxx
	BEREncoder.cxx
	CoderEnv.cxx
	InvalidTracer.cxx
	ios_helper.h
	PERDecoder.cxx
	PEREncoder.cxx
	singleton.h
	ValidChecker.cxx
)

###################################################
###  END OF SECTION Sources
###################################################


###################################################
###  SECTION Build
###################################################

prepare_build_components()

add_library(asn1 STATIC ${BUILD_COMPONENTS})

set_property(TARGET asn1 PROPERTY FOLDER ${PROJECT_FOLDER})

link_qt(asn1)

configure_build(asn1)

set_output_directories(asn1)

###################################################
###  END OF SECTION Build
###################################################


###################################################
###  SECTION Post-Build
###################################################

copy_files(asn1 ${INCLUDE_INSTALLATION_PATH}/asn1 ${CMAKE_CURRENT_SOURCE_DIR}/ 
	asn_factory.h
	asn1.h
	AssocVector.h
	singleton.h
)

###################################################
###  END OF SECTION Post-Build
###################################################

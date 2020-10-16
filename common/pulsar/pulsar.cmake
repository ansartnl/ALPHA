get_filename_component(pulsar_CURRENTDIR ${CMAKE_CURRENT_LIST_FILE} PATH)

include_directories("${pulsar_CURRENTDIR}")

list(APPEND SOURCES ${pulsar_CURRENTDIR}/Pulsar.cpp
  ${pulsar_CURRENTDIR}/PulsReceiverPrivate.cpp
  ${pulsar_CURRENTDIR}/PulsarPrivate.cpp
  ${pulsar_CURRENTDIR}/PulsReceiver.cpp
  ${pulsar_CURRENTDIR}/PulsarShareFunctions.cpp)
    
list(APPEND Q_HEADERS ${pulsar_CURRENTDIR}/Pulsar.h
  ${pulsar_CURRENTDIR}/PulsReceiverPrivate.h
  ${pulsar_CURRENTDIR}/PulsarPrivate.h
  ${pulsar_CURRENTDIR}/PulsReceiver.h)

list(APPEND HEADERS 
  ${pulsar_CURRENTDIR}/PulsarShareFunctions.h)

include("${pulsar_CURRENTDIR}/../compare/compare.cmake")
include("${pulsar_CURRENTDIR}/../buildvariation/buildvariation.cmake")

message("Welcome to Pulsar engine.")


add_group("asyncreqrep" "asyncreqrep"
	asyncreqrep/DealerClient.h
	asyncreqrep/MessageQueueBroker.h
	asyncreqrep/RouterServer.h
	asyncreqrep/DealerClient.cpp
	asyncreqrep/MessageQueueBroker.cpp
	asyncreqrep/RouterServer.cpp
)

add_group("autoconnector" "autoconnector"
	autoconnector/asyncconnector.h
	autoconnector/autoconnector.h
	autoconnector/pingpong.h
	autoconnector/asyncconnector.cpp
	autoconnector/autoconnector.cpp
	autoconnector/pingpong.cpp
)

add_group("monitor" "monitor"
	monitor/Monitor.h
	monitor/Monitor.cpp 
)

add_group("nzmqt" "nzmqt"
	nzmqt/nzmqt.hpp
)

add_group("properties" "properties"
	properties/ModeProperty.h
	properties/StartedProperty.h
)

add_group("pubsub" "pubsub"
	pubsub/PublishQueue.h
	pubsub/SubscribeQueue.h
	pubsub/XPubSubProxy.h 
	pubsub/PublishQueue.cpp 
	pubsub/SubscribeQueue.cpp
	pubsub/XPubSubProxy.cpp
)

add_group("pushpull" "pushpull"
	pushpull/PullQueue.h
	pushpull/PushQueue.h
	pushpull/PullQueue.cpp
	pushpull/PushQueue.cpp
)

add_group("reqrep" "reqrep"
	reqrep/ReplyQueue.h
	reqrep/RequestQueue.h
	reqrep/ReplyQueue.cpp 
	reqrep/RequestQueue.cpp 
)

add_group(HEADERS_ZMQ
	zmq/zmq.h
	zmq/zmq.hpp
	zmq/zmq_utils.h 
)

add_group("" ""
	izmq_global.h
)

set(BUILD_TYPE SHARED)
if(IZMQT_STATIC)
	set(BUILD_TYPE STATIC)
endif(IZMQT_STATIC)


prepare_build_components()

add_library(izmqt ${BUILD_TYPE} ${BUILD_COMPONENTS})

set_property(TARGET izmqt PROPERTY FOLDER "Common") 
link_qt(izmqt)
configure_build(izmqt)
set_output_directories(izmqt)

target_link_libraries(izmqt zmq)

copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/asyncreqrep ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_ASYNCREQREP})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/autoconnector ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_AUTOCONNECTOR})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/monitor ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_MONITOR})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/nzmqt ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_NZMQT})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/properties ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_PROPERTIES})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/pubsub ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_PUBSUB})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/pushpull ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_PUSHPULL})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/reqrep ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_REQREP})
copy_files(izmqt ${INCLUDE_INSTALLATION_PATH}/izmqt/zmq ${CMAKE_CURRENT_SOURCE_DIR}/ ${HEADERS_ZMQ})


if(IZMQT_SUBPROJECT)
	file(GLOB LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${PLATFORM_PREFIX}/*.*)
	copy_files(izmqt ${LIB_INSTALLATION_PATH} "" ${LIBRARIES})

	if(NOT IZMQT_STATIC)
		file(GLOB BINARIES ${CMAKE_CURRENT_SOURCE_DIR}/../bin/${PLATFORM_PREFIX}/*.*)
		copy_files(izmqt ${BIN_INSTALLATION_PATH} "" ${BINARIES})
	endif(NOT IZMQT_STATIC)
endif(IZMQT_SUBPROJECT)

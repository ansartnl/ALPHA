
macro(SET_MULTITHREADED_BUILD)

	if(MSVC)
		set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " /MP") 
		string(REPLACE "\"" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) 
		message(STATUS "Parallel build arguments added to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
	endif()

endmacro(SET_MULTITHREADED_BUILD)


macro(SET_CXX11_BUILD)

	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR MINGW)
                set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11")
                set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g")
                string(REPLACE ";" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
                message(STATUS "C++11 support added to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
	endif()

endmacro(SET_CXX11_BUILD)


macro(SET_WARNINGS_AS_ERRORS_BUILD)

	if(MSVC)
		set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " /W4 /WX /wd4512 /wd4127 /wd4718 /wd4129") 
		string(REPLACE " /W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) 
		string(REPLACE "\"" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}) 
	endif()
	if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR MINGW)
                set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -Wall -Wextra -Werror")
                string(REPLACE ";" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	endif()
	message(STATUS "Warnings will be treated as errors (added to CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS})")

endmacro(SET_WARNINGS_AS_ERRORS_BUILD)

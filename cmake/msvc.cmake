if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP$ENV{NUMBER_OF_PROCESSORS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP$ENV{NUMBER_OF_PROCESSORS}")
endif()

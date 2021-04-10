#------------------------------------------------------------------------------
# Sets up the build global build options
#------------------------------------------------------------------------------
OPTION(ENABLE_PIE "Enables position independent code (pic)" OFF)
if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.14.0 AND ${ENABLE_PIE})
    include(CheckPIESupported)
    check_pie_supported()

    if(NOT CMAKE_CXX_LINK_PIE_SUPPORTED)
        message(STATUS "POSITION INDEPENDENT CODE: \t OFF")
    else()
        message(STATUS "POSITION INDEPENDENT CODE: \t ON")
        #set(CMAKE_POSITION_INDEPENDENT_CODE  ON)
        set(CMAKE_POSITION_INDEPENDENT_CODE  ON CACHE  BOOL "PIE" FORCE)
    endif()
endif()





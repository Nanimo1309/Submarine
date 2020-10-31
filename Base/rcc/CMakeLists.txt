if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE
                   winResources.rc)
endif()
set(APP_SOURCES
    main.cpp)
set(APP_INCLUDES
    )

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${APP_INCLUDES})
target_link_libraries(${PROJECT_NAME} engine Stbipp::Stbipp)
message(STATUS ${WIN_SHARED_LIBRARIES})
if(WIN32)
    ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
           ${WIN_SHARED_LIBRARIES}
           ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Copying DLLs executable path '${CMAKE_CURRENT_BINARY_DIR}'")
endif()

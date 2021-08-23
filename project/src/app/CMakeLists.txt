set(APP_SOURCES
    main.cpp)
set(APP_INCLUDES
    )

add_executable(${PROJECT_NAME} ${APP_SOURCES} ${APP_INCLUDES})
target_link_libraries(${PROJECT_NAME} engine Stbipp::Stbipp)

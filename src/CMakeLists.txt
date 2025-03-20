add_executable(${PROJECT_NAME} WIN32)

target_sources(${PROJECT_NAME} PUBLIC 
    window.h
    window.cpp
    main.cpp
    app.h
    app.cpp
    controls/keyboardController.h
    controls/keyboardController.cpp
    controls/mouseController.cpp
    controls/mouseController.h
    controls/mouseEvent.h
    exceptions/directxException.cpp
    exceptions/directxException.h
    exceptions/windowException.h
    exceptions/windowException.cpp
    resources.h
    resources.rc
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE
    kernel32.lib user32.lib gdi32.lib
    )
endif()

file (COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

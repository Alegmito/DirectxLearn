add_executable(${PROJECT_NAME} WIN32)

target_sources(${PROJECT_NAME} PUBLIC 
    keyboardController.h
    keyboardController.cpp
    window.h
    window.cpp
    main.cpp
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

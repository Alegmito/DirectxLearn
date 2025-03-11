add_executable(${PROJECT_NAME} WIN32)

target_sources(${PROJECT_NAME} PUBLIC 
    window.h    
    window.cpp
    main.cpp
)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(${PROJECT_NAME} PRIVATE
    kernel32.lib user32.lib gdi32.lib
    )
endif()

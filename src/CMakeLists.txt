add_executable(${PROJECT_NAME} WIN32)

target_sources(${PROJECT_NAME} PUBLIC 
    window.h
    window.cpp
    main.cpp
    app.h
    app.cpp
    directxTimer.h
    directxTimer.cpp
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
    graphics/graphics.h
    graphics/graphics.cpp
    graphics/exceptions/dxerr.h
    graphics/exceptions/dxerr.cpp
    graphics/exceptions/DXTrace.inl
    graphics/exceptions/DXGetErrorDescription.inl
    graphics/exceptions/DXGetErrorString.inl
    graphics/exceptions/graphicsException.h
    graphics/exceptions/graphicsException.cpp
    graphics/exceptions/graphicsExMacros.h
    graphics/shaders/vertexShader.hlsl
    logger/dxgiLogger.h
    logger/dxgiLogger.cpp
    utils/utils.hpp
    utils/GraphicsUtils.hpp
)

target_include_directories (${PROJECT_NAME} PUBLIC 
    utils
    controls
    logger
    exceptions
    graphics
)


if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(
        ${PROJECT_NAME} PRIVATE
        kernel32.lib
        user32.lib
        gdi32.lib
    )
endif()

add_subdirectory(graphics)
add_dependencies(${PROJECT_NAME} shaders)

file (COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(bindable)
add_subdirectory(drawable)
add_subdirectory(shaders)

target_include_directories(${PROJECT_NAME} PRIVATE
    bindable
    drawable
    shaders
)

target_sources(${PROJECT_NAME} PRIVATE
    Sphere.h
    IndexedTriangleList.h
    Plane.h
    Cube.h
    Prism.h
)

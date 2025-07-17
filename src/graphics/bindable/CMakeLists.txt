target_sources ( ${PROJECT_NAME} PUBLIC
    IBindable.cpp
    InputLayout.h
    Topology.cpp
    TransformConstBuf.cpp

    buffers/ConstantBuffer.h
    buffers/IndexBuffer.cpp
    buffers/VertexBuffer.cpp

    shaders/PixelShader.cpp
    shaders/VertexShader.cpp
)

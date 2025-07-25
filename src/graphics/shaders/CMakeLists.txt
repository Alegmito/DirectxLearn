add_custom_target(shaders)

set(HLSL_SHADER_FILES vertexShader.hlsl pixelShader.hlsl)
set_source_files_properties(vertexShader.hlsl PROPERTIES ShaderType "vs")

set_source_files_properties(pixelShader.hlsl PROPERTIES ShaderType "ps")

set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "5_0")

foreach(FILE ${HLSL_SHADER_FILES})
    get_filename_component(FILE_WE ${FILE} NAME_WE)
    get_source_file_property(shadertype ${FILE} ShaderType)
    get_source_file_property(shadermodel ${FILE} ShaderModel)
    add_custom_command(TARGET shaders
         COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/src/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/src/${FILE_WE}.pdb ${FILE}
         MAIN_DEPENDENCY ${FILE}
         COMMENT "HLSL ${FILE}"
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
         VERBATIM)
endforeach()

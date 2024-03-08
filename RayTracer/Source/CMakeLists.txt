set(SourcePath ${PROJECT_SOURCE_DIR}/Source)

file(GLOB_RECURSE Renderer_Head     ${SourcePath}/Renderer/*.hpp)
file(GLOB_RECURSE Renderer_Src      ${SourcePath}/Renderer/*.cpp)
file(GLOB_RECURSE Utilities_Head    ${SourcePath}/Utilities/*.hpp)
file(GLOB_RECURSE Utilities_Src     ${SourcePath}/Utilities/*.cpp)
file(GLOB_RECURSE Scene_Head        ${SourcePath}/Scene/*.hpp)
file(GLOB_RECURSE Scene_Src         ${SourcePath}/Scene/*.cpp)

add_library(SourceLib STATIC 
    ${Renderer_Head}    ${Renderer_Src}
    ${Utilities_Head}   ${Utilities_Src}
    ${Scene_Head}       ${Scene_Src}
)
target_link_libraries(SourceLib PUBLIC OpenMP::OpenMP_CXX)
target_include_directories(SourceLib PUBLIC ${SourcePath} ${Stb_INCLUDE_DIR})
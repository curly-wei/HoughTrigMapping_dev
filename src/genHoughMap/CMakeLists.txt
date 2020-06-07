cmake_minimum_required(VERSION 3.13)

set(project_name genHoughMap)
project(${project_name})


list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMakeModules3rdParty/functions")
include(dolChkBuildEnv)
dolChkIf64()
dolChkCompilerVerGeneral()

set(src_files                                    genHoughMap.cc) 

set(CMAKE_DEBUG_POSTFIX                         -d) #mark -d if build mode is debug

set(CMAKE_POSITION_INDEPENDENT_CODE              ON) #-fPIC

set (output_bin_dir                             "${CMAKE_BINARY_DIR}/output/bin") #not use project_BINARY_DIR
set (output_lib_dir                             "${CMAKE_BINARY_DIR}/output/lib") #not use project_BINARY_DIR

add_library(${project_name} SHARED ${src_files})

set_target_properties( 
    ${project_name} 
    PROPERTIES
        CXX_STANDARD                  17
        CXX_STANDARD_REQUIRED         ON
        CXX_EXTENSIONS                OFF

        ARCHIVE_OUTPUT_DIRECTORY      ${output_lib_dir}
        LIBRARY_OUTPUT_DIRECTORY      ${output_lib_dir}
        RUNTIME_OUTPUT_DIRECTORY      ${output_bin_dir}   
        
        PREFIX                        "lib"
        OUTPUT_NAME                   ${project_name} 

        VERSION                       ${now_version}
)

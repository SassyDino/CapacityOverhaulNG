cmake_minimum_required(VERSION 4.0.2)

project(CapacityOverhaulNG VERSION 0.1.0 LANGUAGES CXX)
set(PROJECT_NAME_STR Capacity\ Overhaul\ NG)
set(PROJECT_AUTHOR SassyDino)

set(BUILD_TESTS OFF)

#string(REGEX REPLACE "[/|-]RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

#add_compile_options(-O2)

string(TOUPPER "${CMAKE_BUILD_TYPE}" UPPER_BUILD_TYPE)
set(BUILD_FLAGS "${CMAKE_CXX_FLAGS_${UPPER_BUILD_TYPE}}")
if(NOT BUILD_FLAGS)
	set(OPT_LEVEL Default)
else()
	set(OPT_LEVEL ${BUILD_FLAGS})
endif()

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

file(GLOB_RECURSE source_files src/*.cpp)
add_subdirectory(extern/CommonLibVR)
include(${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibVR/cmake/CommonLibSSE.cmake)
add_commonlibsse_plugin(
	${PROJECT_NAME}
	SOURCES
	${source_files}
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE include/PCH.h)

target_include_directories(
	${PROJECT_NAME}
	PRIVATE
	include
	${CMAKE_CURRENT_BINARY_DIR}/include
)

find_path(SKSE_MCP_INCLUDE_DIRS "SKSEMCP/utils.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE	${SKSE_MCP_INCLUDE_DIRS})

target_link_libraries(
	${PROJECT_NAME} 
	PUBLIC
		CommonLibSSE::CommonLibSSE
)

if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

# Copy configs and resource files to mod staging folder
function(copy_all_changed source_dir dest_dir)
	file (GLOB_RECURSE files RELATIVE "${source_dir}" "${source_dir}/*")
	set(copy_commands "")

	foreach(file ${files})
		set(src "${source_dir}/${file}")
        set(dst "${dest_dir}/${file}")

		 # Create needed subdirectory
        get_filename_component(dst_dirpath "${dst}" DIRECTORY)
        
		list(APPEND copy_commands
			COMMAND "${CMAKE_COMMAND}" -E make_directory "${dst_dirpath}"
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${src}" "${dst}"
		)
	endforeach()

	add_custom_command(
		TARGET "${PROJECT_NAME}"
		POST_BUILD
		${copy_commands}
		VERBATIM
	)
endfunction()

# Copy .dll file to mod staging folder
if(DEFINED OUTPUT_FOLDER)
    set(DLL_FOLDER "${OUTPUT_FOLDER}/SKSE/Plugins")

	set(RESOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SkyrimSE/Data")

    message(STATUS "SKSE plugin output folder: ${DLL_FOLDER}")

    add_custom_command(
        TARGET "${PROJECT_NAME}"
        POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E make_directory "${DLL_FOLDER}"
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_FILE_NAME:${PROJECT_NAME}>"

        VERBATIM
    )
	
	copy_all_changed("${RESOURCE_DIR}" "${OUTPUT_FOLDER}")

    # If you perform a "Debug" build, also copy .pdb file
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()
endif()
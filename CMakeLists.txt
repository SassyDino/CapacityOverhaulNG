# It's recommended to set a minimum CMake version.
# If you use CMake features from higher versions, update this to match.
cmake_minimum_required(VERSION 4.0.2)

# Set your project name. This will be the name of your SKSE .dll file.
project(CapacityOverhaulNG VERSION 0.0.1 LANGUAGES CXX)

set(BUILD_TESTS OFF)

#
# YOU DO NOT NEED TO EDIT ANYTHING BELOW HERE
#
# Please see the "Project setup" section of README.md
#

# If you're not using a mod manager, you probably want the SKSE plugin to go
# inside of your Skyrim "Data" folder.
#
# To do this automatically, set the `SKYRIM_FOLDER` environment variable
# to the path of your Skyrim Special Edition folder
if(DEFINED ENV{SKYRIM_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_FOLDER}/Data")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_FOLDER}/Data")
endif()

# If you're using Mod Organizer 2 or Vortex, you might want this to go inside
# of your "mods" folder, inside of a subfolder named "<your mod>".
#
# To do this automatically, set the `SKYRIM_MODS_FOLDER` environment variable
# to the path of your "mods" folder
if(DEFINED ENV{SKYRIM_MODS_FOLDER} AND IS_DIRECTORY "$ENV{SKYRIM_MODS_FOLDER}")
    set(OUTPUT_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
endif()

file(GLOB_RECURSE source_files src/*.cpp)
add_subdirectory(extern/CommonLibVR)
include(${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibVR/cmake/CommonLibSSE.cmake)

find_package(implot CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Setup your SKSE plugin as an SKSE plugin!
#find_package(CommonLibSSE REQUIRED)
add_commonlibsse_plugin(${PROJECT_NAME} SOURCES ${source_files})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23) # <--- use C++23 standard
target_precompile_headers(${PROJECT_NAME} PRIVATE include/PCH.h) # <--- PCH.h is required!
target_include_directories(${PROJECT_NAME} PRIVATE include)

find_path(SKSE_MCP_INCLUDE_DIRS "SKSEMCP/utils.hpp")
target_include_directories(${PROJECT_NAME} PRIVATE	${SKSE_MCP_INCLUDE_DIRS})

target_link_libraries(
	${PROJECT_NAME} 
	PRIVATE 
		implot::implot
		imgui::imgui
)

target_link_libraries(
	${PROJECT_NAME} 
	PUBLIC
		CommonLibSSE::CommonLibSSE
)

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

# When your SKSE .dll is compiled, this will automatically copy the .dll into your mods folder.
# Only works if you configure DEPLOY_ROOT above (or set the SKYRIM_MODS_FOLDER environment variable)
if(DEFINED OUTPUT_FOLDER)
    # If you specify an <OUTPUT_FOLDER> (including via environment variables)
    # then we'll copy your mod files into Skyrim or a mod manager for you!

    # Output directory
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

    # If you perform a "Debug" build, also copy .pdb file (for debug symbols)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_custom_command(
            TARGET "${PROJECT_NAME}"
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_PDB_FILE:${PROJECT_NAME}>" "${DLL_FOLDER}/$<TARGET_PDB_FILE_NAME:${PROJECT_NAME}>"
            VERBATIM
        )
    endif()
endif()

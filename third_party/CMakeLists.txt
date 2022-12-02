include(FetchContent)

#
# raylib
#
FetchContent_Declare(raylib
	URL https://github.com/raysan5/raylib/releases/download/4.2.0/raylib-4.2.0_win64_msvc16.zip
	CONFIGURE_COMMAND ""
	BUILD_COMMAND "")

FetchContent_GetProperties(raylib)
if (NOT raylib_POPULATED)
	FetchContent_Populate(raylib)
endif()

add_library(raylib INTERFACE)
target_include_directories(raylib INTERFACE ${raylib_SOURCE_DIR}/include)
target_link_libraries(raylib INTERFACE ${raylib_SOURCE_DIR}/lib/raylibdll.lib)
# STUDENT CMakeLists.txt
#
# tabs: 8 space

# CMake initialization

cmake_minimum_required(VERSION 3.20)
project("ProjetC_IG")

# Paths

get_filename_component(ROOT_DIR		${CMAKE_CURRENT_LIST_FILE} PATH)
set(HEADER_DIRS				"${ROOT_DIR}/api")
set(TEST_DIR				"${ROOT_DIR}/tests")
set(DOCS_DIR				"${ROOT_DIR}/docs")
set(SRC					"${ROOT_DIR}/implem")
set(CMAKE_BUILD_TYPE Debug)

set(LIB_FLAGS				-lfreeimage -lSDL2 -lSDL2_ttf -lSDL2_gfx)
# set(CMAKE_C_FLAGS "-pg")

if(IG_ASAN)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
endif(IG_ASAN)

# Platform specific definitions

if(${APPLE})
	set(HEADER_DIRS			${HEADER_DIRS}
					"/opt/local/include/SDL2"
					"/opt/local/include")
	set(PLATFORM_DIR		"${ROOT_DIR}/_macos")
	set(PLATFORM_LIB_FLAGS		eibase -L/opt/local/lib ${LIB_FLAGS})

	message(STATUS "Building for MacOS")

elseif(${UNIX})

	set(HEADER_DIRS			${HEADER_DIRS}
					"/usr/include/SDL2")
	set(PLATFORM_DIR		"${ROOT_DIR}/_x11")
	set(PLATFORM_LIB_FLAGS		eibase ${LIB_FLAGS} -lm)

	message(STATUS "Building for Linux")

else()

	set(SDL				"C:/projetc/SDL2_windows")
	set(HEADER_DIRS			${HEADER_DIRS}
					"${SDL}/include")
	add_definitions(-D_AMD64_=1 -D__WIN__=1 -D_CRT_SECURE_NO_WARNINGS=1 /wd4068)

	set(PLATFORM_DIR		"${ROOT_DIR}/_win")
	set(PLATFORM_LIB_FLAGS		eibase
					${SDL}/lib/${ARCH}/SDL2.lib
					${SDL}/lib/${ARCH}/SDL2_gfx.lib
					${SDL}/lib/${ARCH}/SDL2_ttf.lib
					${SDL}/lib/${ARCH}/FreeImage.lib)

	message(STATUS "Building for Windows")

endif(${APPLE})

# General definitions for all targets

include_directories(${HEADER_DIRS})
link_directories(${PLATFORM_DIR})


# target ei (libei)

set(LIB_EI_SOURCES
	${SRC}/src/geometry_managers/ei_placer.c
	${SRC}/src/geometry_managers/ei_gridder.c
	${SRC}/src/widgets/ei_frame.c
	${SRC}/src/widgets/ei_button.c
	${SRC}/src/widgets/ei_toplevel.c
	${SRC}/src/widgets/ei_entry.c
	${SRC}/src/widgets/ei_radiobutton.c
	${SRC}/src/parser/ei_lexer.c
	${SRC}/src/parser/ei_parser.c
	${SRC}/src/parser/ei_parser_types.c
	${SRC}/src/parser/ei_parser_option_value_configure.c
	${SRC}/src/parser/ei_parser_option_value_conversion.c
	${SRC}/src/ei_application.c
	${SRC}/src/ei_widget.c
	${SRC}/src/ei_widget_configure.c
	${SRC}/src/ei_widget_attributes.c
	${SRC}/src/ei_widgetclass.c
	${SRC}/src/ei_geometrymanager.c
	${SRC}/src/ei_draw.c
	${SRC}/src/ei_utils.c
	${SRC}/src/ei_event.c
	${SRC}/src/ei_internal_callbacks.c
	${SRC}/src/ei_implementation.c
	${SRC}/src/ei_entry.c
)

add_library(ei STATIC			${LIB_EI_SOURCES})

# target minimal

add_executable(minimal 			${TEST_DIR}/minimal.c)
target_link_libraries(minimal		${PLATFORM_LIB_FLAGS})

# target frame

add_executable(frame			${TEST_DIR}/frame.c)
target_link_libraries(frame		ei ${PLATFORM_LIB_FLAGS})

# target button

add_executable(button			${TEST_DIR}/button.c)
target_link_libraries(button		ei ${PLATFORM_LIB_FLAGS})

# target hello_world

add_executable(hello_world		${TEST_DIR}/hello_world.c)
target_link_libraries(hello_world	ei ${PLATFORM_LIB_FLAGS})

# target entry

add_executable(entry			${TEST_DIR}/entry.c)
target_link_libraries(entry		ei ${PLATFORM_LIB_FLAGS})

# target puzzle

add_executable(puzzle			${TEST_DIR}/puzzle.c)
target_link_libraries(puzzle		ei ${PLATFORM_LIB_FLAGS})

# target two048

add_executable(two048			${TEST_DIR}/two048.c)
target_link_libraries(two048		ei ${PLATFORM_LIB_FLAGS})

# target minesweeper

add_executable(minesweeper		${TEST_DIR}/minesweeper.c)
target_link_libraries(minesweeper	ei ${PLATFORM_LIB_FLAGS})

# target ext_testclass

add_executable(ext_testclass		${TEST_DIR}/ext_testclass.c)
target_link_libraries(ext_testclass	testclass ei ${PLATFORM_LIB_FLAGS})

# target test_ext_gm

add_executable(ext_testgm		${TEST_DIR}/ext_testgm.c)
target_link_libraries(ext_testgm	testgm ei ${PLATFORM_LIB_FLAGS})

# target parsing

add_executable(parsing			${TEST_DIR}/parsing.c)
target_link_libraries(parsing		ei ${PLATFORM_LIB_FLAGS})

# target parsing_ko

add_executable(parsing_ko			${TEST_DIR}/parsing_ko.c)
target_link_libraries(parsing_ko		ei ${PLATFORM_LIB_FLAGS})

# target custom_tags

add_executable(tags			${TEST_DIR}/custom_tags.c)
target_link_libraries(tags		ei ${PLATFORM_LIB_FLAGS})

# target tags_parsing

add_executable(tags_parsing			${TEST_DIR}/tags_parsing.c)
target_link_libraries(tags_parsing		ei ${PLATFORM_LIB_FLAGS})

# target text

add_executable(text			${TEST_DIR}/text.c)
target_link_libraries(text		ei ${PLATFORM_LIB_FLAGS})

# target resize_toplevel

add_executable(resize_toplevel			${TEST_DIR}/resize_toplevel.c)
target_link_libraries(resize_toplevel		ei ${PLATFORM_LIB_FLAGS})

# target image

add_executable(image			${TEST_DIR}/image.c)
target_link_libraries(image		ei ${PLATFORM_LIB_FLAGS})

# target placer

add_executable(placer			${TEST_DIR}/placer.c)
target_link_libraries(placer		ei ${PLATFORM_LIB_FLAGS})

# target content_rect_visualization

add_executable(content_rect_visualization			${TEST_DIR}/content_rect_visualization.c)
target_link_libraries(content_rect_visualization		ei ${PLATFORM_LIB_FLAGS})

# target radiobutton

add_executable(radiobutton			${TEST_DIR}/radiobutton.c)
target_link_libraries(radiobutton		ei ${PLATFORM_LIB_FLAGS})


# target personal_information_grid

add_executable(pi_grid			${TEST_DIR}/personal_information_grid.c)
target_link_libraries(pi_grid		ei ${PLATFORM_LIB_FLAGS})

# target grid_playground

add_executable(grid_playground			${TEST_DIR}/grid_playground.c)
target_link_libraries(grid_playground		ei ${PLATFORM_LIB_FLAGS})

# target to build the documentation

add_custom_target(doc doxygen		${DOCS_DIR}/doxygen.cfg WORKING_DIRECTORY ${ROOT_DIR})

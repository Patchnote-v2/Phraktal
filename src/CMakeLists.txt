project(src)

find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})

file(GLOB allFiles
        "*.cpp"
        )

add_executable(src ${allFiles})
target_link_libraries(src ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_TTF_LIBRARY})
install(TARGETS src RUNTIME DESTINATION ${BIN_DIR})
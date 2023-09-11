
add_subdirectory(Tiled)
add_subdirectory(Collision)

target_sources(${PROJECT_NAME} PUBLIC
	MainView.h
	MainView.cpp
	Tile.h
	Tile.cpp
	Pacman.h
	Pacman.cpp
	Scene.h
	Scene.cpp
	Map.h
	Map.cpp
)
# Tiled Reference
Engine uses Tiled 2D's map editor program's tileset's and tilemap's to create a 2D map from **tmx** and **tsx** file formats which are based on xml.

For us to use tilemaps and tilesets on our engine we are using Matt Marchant's **tmxlite** library to read and edit our files in engine.

## Map loading and drawing

All the functions needed to draw a map on the screen are located inside Tilemap.h. No other includes are needed in the main souurce file.

`#include <engine/Tilemap.h>`

Inside the **main** function we need to create Tilemap object which gets camera objects address as variable.
After that we can use tilemaps `loadMap()` function to draw tilemap on the screen and set tilemaps position on the screen.

```cpp
Tilemap map(&cam);
map.loadMap("address where tilemap is located");
map.position = glm::vec3(0.0f, 0.0f, 0.0f);
```


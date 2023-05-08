# Tiled Reference
Engine uses Tiled 2D's map editors tileset's and tilemap's to create a 2D map from **tmx** and **tsx** file formats.


## Map loading and drawing

All the functions needed to draw a map on the screen are located inside Tilemap.h. No other includes are needed in the main souurce file.

`#include <engine/Tilemap.h>`

So you can draw your tilemap that you created in Tiled map editor, you need to first create Tilemap object which gets camera objects address.

`Tilemap map(&cam);`


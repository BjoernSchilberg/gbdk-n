#!/bin/bash

ASSET_FILE=../include/assets.h

# Build tilesets
ggbgfx tileset numbers.png screen.png window.png gameover.png -o tileset.png

# Build sprites
ggbgfx sprite sprites.png > $ASSET_FILE

# Build tiledata
ggbgfx tiledata tileset.png >> $ASSET_FILE

# Build tilemaps
ggbgfx tilemap -n screen_tiles screen.png tileset.png >> $ASSET_FILE
ggbgfx tilemap -n window_tiles window.png tileset.png >> $ASSET_FILE
ggbgfx tilemap -n gameover_tiles gameover.png tileset.png >> $ASSET_FILE

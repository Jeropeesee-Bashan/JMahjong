/**
 * JMahjong: My own try on implementing the classic solitaire game
 * Copyright (C) 2023 Andrey Kulenko
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef LAYOUT_H_
#define LAYOUT_H_

#include <stdint.h>

#define N_TILES 144
#define FIELD_WIDTH 15
#define FIELD_HEIGHT 8
#define FIELD_LAYER (FIELD_WIDTH * FIELD_HEIGHT)

typedef uint16_t layout_t[N_TILES];

/* Tile feature represents which side a tile is shifted to.
   The side may be none, down, right, or both.*/
enum tile_feature {
	TILE_FEATURE_NONE,
	TILE_FEATURE_DOWN,
	TILE_FEATURE_RIGHT,
	TILE_FEATURE_DOWN_RIGHT
};

/* Basically, it converts each 16-bit element in binary from big-endian to
   platform's native endianness and copies them into layout. */
void read_layout(const void *binary, layout_t layout);

/* Returns 14 most significant bits which represent tile's offset. */
uint16_t tile_offset(uint16_t tile);

/* Returns 2 least significant bits which represent tile's feature. */
enum tile_feature tile_feature(uint16_t tile);

#endif /* LAYOUT_H_ */

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
#ifndef MAHJONG_H_
#define MAHJONG_H_

#include "layout.h"

#include <stddef.h>
#include <stdbool.h>

/* struct tile represents a single tile on a play field. */
struct tile {
	enum tile_feature feature;
	bool available;
};

/* This is a basic dictionary that has tile's type as keys and all four tiles
   as values. It's useful when applying mahjong solving algorithms. */
struct pairs {
	struct tile *tiles[4];
	uint8_t size;
};

/* This structure consists of the mahjong tiles themselves, the game field,
   structure of which follows a specific layout, and a dictionary of pairs.
   It is the game itself. */
struct mahjong {
	struct tile tiles[N_TILES];
	struct pairs pairs[N_TILES / 4];
	struct tile **field;
	size_t field_size;
};

/* Returns struct mahjong with allocated memory and properly set values.
   The play field matches `layout`. */
struct mahjong *create_mahjong_game(const layout_t layout);

void mahjong_place_tiles(const layout_t layout, struct mahjong *game);
void mahjong_shuffle(struct mahjong *game);
void mahjong_update(struct mahjong *game);

/* Properly deallocates memory for `game`'s data members and `game` itself. */
void delete_mahjong_game(struct mahjong *game);

#endif /* MAHJONG_H_ */

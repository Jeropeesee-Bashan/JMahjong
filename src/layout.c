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
#include "layout.h"

#include <stddef.h>

void read_layout(const void *binary, layout_t layout)
{
	size_t i;

	for (i = 0; i < N_TILES; ++i)
		layout[i] = *(const uint8_t *)(binary + i * 2) << 8 |
			    *(const uint8_t *)(binary + i * 2 + 1);
}

uint16_t tile_offset(uint16_t tile)
{
	return tile >> 2;
}

enum tile_feature tile_feature(uint16_t tile)
{
	return tile & 0b11;
}

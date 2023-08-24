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
#include <mahjong.h>
#include <layout.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define three_equal(a, b, c) (a) == (b) & (a) == (c) && (b) == (c)

static void offset_to_coord(uint16_t offset, uint8_t *x, uint8_t *y, uint8_t *z)
{
	if (x)
		*x = offset % FIELD_WIDTH;
	if (y)
		*y = offset / FIELD_WIDTH % FIELD_HEIGHT;
	if (z)
		*z = offset / FIELD_LAYER;
}

static struct tile *tile_xyz(struct tile **field, uint8_t depth, uint8_t x,
			     uint8_t y, uint8_t z)
{
	if (field == NULL)
		return NULL;

	return (x < FIELD_WIDTH && y < FIELD_HEIGHT && z < depth) ?
		       field[x + y * FIELD_WIDTH + z * FIELD_LAYER] :
		       NULL;
}

static struct tile *tile_relative(struct tile **field, uint16_t offset,
				  uint8_t depth, int dx, int dy, int dz)
{
	uint8_t x, y, z;

	offset_to_coord(offset, &x, &y, &z);

	return (dx + x >= 0 && dy + y >= 0 && dz + z >= 0) ?
		       tile_xyz(field, depth, x + dx, y + dy, z + dz) :
		       NULL;
}

static void tile_neighbors(struct tile **field, uint16_t offset, uint8_t depth,
			   const struct tile *out[11])
{
	if (field == NULL || out == NULL)
		return;

	/* Left */
	out[0] = tile_relative(field, offset, depth, -1, 0, 0);
	/* Right */
	out[1] = tile_relative(field, offset, depth, 1, 0, 0);
	/* Deeper up left */
	out[2] = tile_relative(field, offset, depth, -1, -1, 1);
	/* Deeper up */
	out[3] = tile_relative(field, offset, depth, 0, -1, 1);
	/* Deeper up right */
	out[4] = tile_relative(field, offset, depth, 1, -1, 1);
	/* Deeper left */
	out[5] = tile_relative(field, offset, depth, -1, 0, 1);
	/* Deeper */
	out[6] = tile_relative(field, offset, depth, 0, 0, 1);
	/* Deeper right */
	out[7] = tile_relative(field, offset, depth, 1, 0, 1);
	/* Deeper down left */
	out[8] = tile_relative(field, offset, depth, -1, 1, 1);
	/* Deeper down */
	out[9] = tile_relative(field, offset, depth, 0, 1, 1);
	/* Deeper down right */
	out[10] = tile_relative(field, offset, depth, 1, 1, 1);
}

/* FIXME: Refactor the function. Looks too ugly. */
static bool tile_available(struct tile **field, uint16_t offset, uint8_t depth)
{
	const struct tile *neighbors[11];
	const enum tile_feature *f[11];
	size_t i;
	enum tile_feature tile_feature;

	assert(field != NULL);

	tile_neighbors(field, offset, depth, neighbors);

	tile_feature = field[offset]->feature;
	for (i = 0; i < 11; ++i)
		f[i] = (neighbors[i] == NULL) ? NULL : &neighbors[i]->feature;

	if (f[6] != NULL ||
	    (f[0] != NULL && f[1] != NULL &&
	     three_equal(*f[0] >> 1, *f[1] >> 1, tile_feature) >> 1)) {
		return false;
	}

	switch (tile_feature) {
	case TILE_FEATURE_NONE:
		if (f[2] != NULL && *f[2] == TILE_FEATURE_DOWN_RIGHT) {
			return false;
		}
		if (f[3] != NULL && *f[3] & 0b01) {
			return false;
		}
		if (f[5] != NULL && *f[5] >> 1) {
			return false;
		}
		break;
	case TILE_FEATURE_DOWN:
		if (f[5] != NULL && *f[5] >> 1) {
			return false;
		}
		if (f[8] != NULL && *f[8] == TILE_FEATURE_RIGHT) {
			return false;
		}
		if (f[9] != NULL && !(*f[9] & 0b01)) {
			return false;
		}
		break;
	case TILE_FEATURE_RIGHT:
		if (f[3] != NULL && *f[3] & 0b01) {
			return false;
		}
		if (f[4] != NULL && *f[4] == TILE_FEATURE_DOWN) {
			return false;
		}
		if (f[7] != NULL && !(*f[7] >> 1)) {
			return false;
		}
		break;
	case TILE_FEATURE_DOWN_RIGHT:
		if (f[7] != NULL && !(*f[7] >> 1)) {
			return false;
		}
		if (f[9] != NULL && !(*f[9] & 0b01)) {
			return false;
		}
		if (f[10] != NULL && *f[10] == TILE_FEATURE_NONE) {
			return false;
		}
	default:
		break;
	}

	return true;
}

static void generate_types(int types[N_TILES])
{
	size_t i, j;
	int temp;

	for (i = 0; i < N_TILES / 4; ++i) {
		for (j = 0; j < 4; j++) {
			types[i * 4 + j] = i;
		}
	}

	for (i = N_TILES - 1; i > 0; --i) {
		j = rand() % (i + 1);
		temp = types[i];
		types[i] = types[j];
		types[j] = temp;
	}
}

void mahjong_place_tiles(const layout_t layout, struct mahjong *game)
{
	uint16_t offset;
	size_t i;

	for (i = 0; i < N_TILES; ++i) {
		offset = tile_offset(layout[i]);
		game->tiles[i].feature = tile_feature(layout[i]);
		game->field[offset] = &game->tiles[i];
	}
}

void mahjong_shuffle(struct mahjong *game)
{
	int types[N_TILES];
	struct pairs *pairs;
	size_t i;

	memset((void *)game->pairs, 0, N_TILES / 4 * sizeof(struct pairs));
	generate_types(types);

	for (i = 0; i < N_TILES; ++i) {
		pairs = &game->pairs[types[i]];

		pairs->tiles[pairs->size] = &game->tiles[i];
		pairs->size += 1;
	}
}

void mahjong_update(struct mahjong *game)
{
	size_t i;

	if (game == NULL)
		return;

	for (i = 0; i < game->field_size; ++i) {
		if (game->field[i] == NULL)
			continue;
		game->field[i]->available = tile_available(
			game->field, i, game->field_size / FIELD_LAYER);
	}
}

struct mahjong *create_mahjong_game(const layout_t layout)
{
	struct mahjong *result;
	uint16_t last_offset;

	if (layout == NULL)
		return NULL;

	last_offset = tile_offset(layout[N_TILES - 1]);

	result = (struct mahjong *)malloc(sizeof(struct mahjong));
	if (result == NULL)
		return NULL;

	result->field_size = (last_offset / FIELD_LAYER + 1) * FIELD_LAYER;
	result->field = (struct tile **)calloc(result->field_size,
					       sizeof(struct tile *));
	if (result->field == NULL) {
		free((void *)result);
		return NULL;
	}

	mahjong_place_tiles(layout, result);
	mahjong_shuffle(result);
	mahjong_update(result);

	/* TODO: Implement checking if the game is solvable. */

	return result;
}

void delete_mahjong_game(struct mahjong *game)
{
	if (game == NULL)
		return;

	free((void *)game->field);
	free((void *)game);
}

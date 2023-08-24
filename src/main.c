#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <mahjong.h>

/* Dummy */
int main(int argc, char *argv[])
{
	layout_t spider;
	uint16_t buffer[N_TILES];
	struct mahjong *game;
	FILE *spider_f;
	size_t i;

	if (argc <= 1) {
		fprintf(stderr, "You should provide a file name"
				"as an argument!\n");
		return 1;
	}

	srand(time(NULL));

	spider_f = fopen(argv[1], "rb");
	if (spider_f == NULL) {
		fprintf(stderr, "Couldn't open %s!\n", argv[1]);
		return 2;
	}

	fread((void *)buffer, 2, N_TILES, spider_f);
	read_layout((const void *)buffer, spider);

	fclose(spider_f);

	game = create_mahjong_game(spider);

	for (i = 0; i < N_TILES; ++i) {
		if (game->tiles[i].available) {
			printf("Tile %zu is available.\n", i);
		}
	}

	delete_mahjong_game(game);
	return 0;
}

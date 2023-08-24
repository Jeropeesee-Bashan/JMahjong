# JMahjong

## My own try on implementing the classic solitaire game
I played a couple of mahjong solitaire games and got curious how hard it is to
make such a game! This project is a try on implementing a satisfactory mahjong
that wouldn't throw to you a lot of "no more picks" and put you in confusing
conditions. I'll see if it's even possible... :sweat_smile:

## Manual build and Installation
JMahjong uses CMake as its build system, so these steps should be familiar to
you.

```sh
  mkdir build
  cmake -B build -DCMAKE_BUILD_TYPE=Release

  # You can just build the game
  cmake --build build

  # ... or install it right away.
  cmake --install build
```

## Project status
JMahjong is in early development and I'm not sure if I'm going to work on it in
the future.

## License
JMahjong's source code is licensed under GNU General Public License, version 3.

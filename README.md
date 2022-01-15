# Fish Blater (Revival Jam)

This project was made for 2020's **[Revival Jam](https://itch.io/jam/revival-jam)**.
It is a simple 2D arcade blaster about getting the highest score possible A download
for the game can be found over on **[itch.io](https://jrob774.itch.io/fish-blaster)**,
the game is also playable in browser on the same page.

## Development

The game was developed in C using the **[SDL2](https://libsdl.org/)** library and compiled
using the **[TCC](https://bellard.org/tcc/)** compiler. A port for web browsers was also
developed using **[emscripten](https://github.com/emscripten-core/emscripten)**.

## Building

### Windows

Run the `buildwin32.bat` file to build the application as a 32-bit executable and
`buildwin64.bat` to build as a 64-bit executable. The script is setup to require *TCC*
in order to build (which has been packaged in this repository), however setting up the
project to build with another C/C++ compiler would be relatively simple. All external
dependencies are provided with the repo.

### Web

Run the `buildweb.bat` file in order to build the application. This requires
*emscripten* to be setup in order for the build to work. All external dependencies
are provided with the repo.

## License

The project's code is available under the **[MIT License](https://github.com/JROB774/revivaljam/blob/master/LICENSE)**.

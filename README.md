# TYPINGS

A terminal/cli typing test heavily inspired/based off @briano1905's [typings.gg](https://typings.gg) | [github page](https://github.com/briano1905/typings).

## Requirements
- A terminal with support for xterm-256 colors
- [Termbox](https://github.com/nsf/termbox) installed
- A keyboard

## Usage

- `Ctrl+R` Reset
- `Ctrl+L` Change the number of words
- `Ctrl+X` Change the main widow's width
- `Ctrl+T` Change the theme

All controls are also displayed in the footer at the bottom of the application window.

## Configuration

- Themes can be added / removed by modifying the vector in `themes.hpp`. Each element of the vector is a ColorTheme struct (also defined in `themes.hpp`).
- The list of possible words can be changed by modifying the vector in `dictionary.hpp`. This is where you can provide support for other languages. All UTF-8 characters should be able to display, but currently user input is limited to ASCII.
- Much more can be easily changed through modifications in `typings.cpp`.

## TODO
- [ ] Better user input (Full Unicode support)
- [ ] Remove need for extra space once finished typing
- [ ] Testing on other OS's, so far only WSL2 tested

## Additional Notes
- Currently WSL1 is incompatable with Termbox. If you want to use this program through WSL, please upgrade to WSL2. (It's better anyway)
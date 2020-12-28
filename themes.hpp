#ifndef THEMES_H
#define THEMES_H

#include <vector>
#include <string>

#include "termbox_utils.hpp"

/**
 * Simple struct to store color theme
 */
struct ColorTheme {    
    std::string name;
    uint16_t background;
    uint16_t text;
    uint16_t highlight;
    uint16_t correct;
    uint16_t wrong;
    uint16_t typing_area;
    uint16_t input_box;    
};

/**
 * Vector of ColorThemes selectable in the program
 */
std::vector<ColorTheme> THEMES = {
    {
        .name = "Dots",
        .background = HEX(0x191b25),
        .text = HEX(0xfafafa),
        .highlight = HEX(0x9b57ff),
        .correct = HEX(0x32cb8b),
        .wrong = HEX(0xfe494e),
        .typing_area = HEX(0x232533),
        .input_box = HEX(0x2c2e40),
    },
    {
        .name = "Light",
        .background = HEX(0xfafafa),
        .text = HEX(0x1a1a1a),
        .highlight = HEX(0xa56de2),
        .correct = HEX(0x68b723),
        .wrong = HEX(0xc6262e),
        .typing_area = HEX(0xeaeaea),
        .input_box = HEX(0xfafafa),
    },
    {
        .name = "Burgandy",
        .background = HEX(0x1a1a1a),
        .text = HEX(0xfafafa),
        .highlight = HEX(0xf9c440),
        .correct = HEX(0x68b723),
        .wrong = HEX(0xc6262e),
        .typing_area = HEX(0x691f28),
        .input_box = HEX(0x2f080e)
    },
    {
        .name = "Eclipse",
        .background = HEX(0x314f6f),
        .text = HEX(0xabc6dc),
        .highlight = HEX(0xa56de2),
        .correct = HEX(0x68b723),
        .wrong = HEX(0xc6262e),
        .typing_area = HEX(0x3a3a3a),
        .input_box = HEX(0x333333)
    }, 
    {
        .name = "9009",
        .background = HEX(0xb6b09a),
        .text = HEX(0x2e2f33),
        .highlight = HEX(0x96907a),
        .correct = HEX(0x6f8c70),
        .wrong = HEX(0xd28782),
        .typing_area = HEX(0xd9d2c8),
        .input_box = HEX(0xb6b09a)
    },
     {
        .name = "blue",
        .background = HEX(0x0000FF),
        .text = HEX(0x0000FF),
        .highlight = HEX(0x0000FF),
        .correct = HEX(0x0000FF),
        .wrong = HEX(0x0000FF),
        .typing_area = HEX(0x0000FF),
        .input_box = HEX(0x0000FF)
    }

    // Add more themes here!

};

#endif // THEMES_H
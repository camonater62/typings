#include <iostream>
#include <termbox.h>
#include <string>
#include <sstream>
#include <vector>
#include <chrono>

#include "termbox_utils.hpp"
#include "themes.hpp"
#include "dictionary.hpp"

using namespace std;
using namespace chrono;

// vector THEMES defined in themes.hpp
int current_theme = 0;

vector<int> length_options = { 10, 25, 50, 100, 200 };
int current_length = 2;

vector<float> width_options = { 0.4f, 0.5f, 0.6f, };
int current_width = 0;

// vector DICTIONARY defined in dictionary.hpp
int current_word = 0;
vector<int> wrong_words; // positions of wrong words
vector<string> words;
string user_input = "";

time_point<system_clock> start_time;
time_point<system_clock> end_time;

int wpm = 0;
int acc = 0;

bool started = false;
bool stopped = false;

/**
 * @param id index of possibly wrong word
 * @returns true if the provided id is the index of a wrong word
 */
bool is_wrong(int id) {
    for(int i = 0; i < (int)wrong_words.size(); i++) {
        if(wrong_words[i] == id) return true;
    }
    return false;
}

/**
 * Draws the typings logo at the top of the window
 */
void draw_header() {
    string header = "typings";
    int x = tb_width() / 2 - header.length() / 2;
    int y = 0.05f * tb_height();
    tb_draw_string(x, y, header, THEMES[current_theme].text, THEMES[current_theme].background);
}

/**
 * Draws the footer which shows the controls for the program
 */
void draw_footer() {
    string footer = "^R to reset, ^L to change length, ^X to change width, ^T to change theme";
    int x = tb_width() / 2 - footer.length() / 2;
    int y = 0.95f * tb_height();
    tb_draw_string(x, y, footer, THEMES[current_theme].text, THEMES[current_theme].background);
}

/**
 * Draw the main box of the program; handles drawing user input and the current text
 */
void draw_typing_area() {
    int width = width_options[current_width] * tb_width();
    int height = 7; 
    int x = tb_width() / 2 - width / 2;

    // Get correct height for typing area
    int ix = x;
    for(int i = 0; i < (int)words.size(); i++) {
        if(ix - x + (int)words[i].length() + 2 >= width - 1) {
            height++;
            ix = x;
        }
        ix += words[i].length() + 1;
    }   
    
    int y = tb_height() / 2 - height / 2;
    
    int iy = y + 1;
    ix = x;

    // Draw the background box
    tb_draw_box(x, y, width, height, THEMES[current_theme].typing_area, THEMES[current_theme].typing_area);

    // Draw lengths
    int offset = 0;
    for(int i = 0; i < (int)length_options.size(); i++) {
        uint32_t color = THEMES[current_theme].text;
        if(i == current_length % (int)length_options.size()) {
            color = THEMES[current_theme].highlight;
        }

        string num_string = to_string(length_options[i]);        

        tb_draw_string(x + offset, y - 2, num_string, color, THEMES[current_theme].background);
        offset += num_string.length();
        if(i != (int)length_options.size() - 1) {
            tb_draw_string(x + offset, y - 2, " / ", THEMES[current_theme].text, THEMES[current_theme].background);
            offset += 3;
        }
    }

    // Draw lengths
    offset = 0;
    for(int i = 0; i < (int)THEMES.size(); i++) {
        uint32_t color = THEMES[current_theme].text;
        if(i == current_theme) {
            color = THEMES[current_theme].highlight;
        }

        string theme_string = THEMES[i].name;        

        tb_draw_string(x + offset, y + height + 1, theme_string, color, THEMES[current_theme].background);
        offset += theme_string.length();
        if(i != (int)THEMES.size() - 1) {
            tb_draw_string(x + offset, y + height + 1, " / ", THEMES[current_theme].text, THEMES[current_theme].background);
            offset += 3;
        }
    }

    // Draw current stats
    string stats = "WPM: " + to_string(wpm) + " / ACC: " + to_string(acc);
    tb_draw_string(x + width - stats.length(), y - 2, stats, THEMES[current_theme].text, THEMES[current_theme].background); 

    // Draw all the words
    for(int i = 0; i < (int)words.size(); i++) {
        if(ix - x + (int)words[i].length() + 2 >= width - 1) {
            iy++;
            ix = x;
        }

        // Set correct text color
        uint16_t color = THEMES[current_theme].text;
        if(i == current_word) {
            color = THEMES[current_theme].highlight;
        } else if(is_wrong(i)) {
            color = THEMES[current_theme].wrong;
        } else if(i < current_word) {
            color = THEMES[current_theme].correct;
        }
        tb_draw_string(ix + 2, iy, words[i], color, THEMES[current_theme].typing_area);
        ix += words[i].length() + 1;
    }

    uint32_t input_color = THEMES[current_theme].text;
    for(int i = 0; i < (int)user_input.length(); i++) {
        if(user_input[i] != words[current_word][i])
            input_color = THEMES[current_theme].wrong;
    }

    // Draw the input box
    tb_draw_box(x + 1, y + height - 4, width - 2, 3, THEMES[current_theme].input_box, THEMES[current_theme].input_box);
    tb_draw_string(x + 2, y + height - 3, user_input, input_color, THEMES[current_theme].input_box);

    tb_set_cursor(x + 2 + user_input.length(), y + height - 3);
}

/**
 * Resets the test
 */
void reset() {
    started = false;
    stopped = false;
    current_word = 0;
    wrong_words.clear();
    words.clear();
    user_input = "";
    for(int i = 0; i < length_options[current_length % length_options.size()]; i++) {
        string w = "";
        do {
            // Ensure current word is not the same as the previous word
            w = DICTIONARY[rand() % DICTIONARY.size()];
        } while(i != 0 && w == words.back());
        words.push_back(w);
    }
}

int main() {    
    srand(time(NULL));

    tb_init();

    tb_select_output_mode(TB_OUTPUT_256);
    
    tb_event e;

    bool active = true;
    
    reset();

    while(active) {
        tb_set_clear_attributes(THEMES[current_theme].text, THEMES[current_theme].background);

        if(!stopped && current_word >= (int)words.size()) { 
            // Finished typing
            end_time = system_clock::now();

            stopped = true;

            double time = duration_cast<milliseconds>(end_time - start_time).count() / 1000.0;

            int chars = words.size(); // Account for spaces

            for(int i = 0; i < (int)words.size(); i++) {
                if(!is_wrong(i)) {
                    chars += words[i].length();
                }
            }

            wpm = 60 * chars / time / 5;
            acc = 100 - 100 * wrong_words.size() / words.size();
        }

        tb_clear();

        draw_header();
        draw_footer();

        draw_typing_area();

        tb_present();

        bool input = tb_poll_event(&e);

        if(input) {
            switch(e.type) {
                case TB_EVENT_KEY: 
                    switch(e.key) {
                        case TB_KEY_CTRL_C: 
                        case TB_KEY_ESC: 
                            active = false; 
                            break;

                        case TB_KEY_BACKSPACE:
                        case TB_KEY_BACKSPACE2: 
                            if(user_input.length() > 0) 
                                user_input.pop_back(); 
                            break;

                        case TB_KEY_SPACE:
                            if(words[current_word] != user_input)
                                wrong_words.push_back(current_word);
                            current_word++;
                            user_input = "";
                            break;

                        case TB_KEY_CTRL_L: 
                            current_length++;
                            if(current_length >= (int)length_options.size())
                                current_length = 0;
                            reset();
                            break;

                        case TB_KEY_CTRL_T:
                            current_theme++;
                            if(current_theme >= (int)THEMES.size())
                                current_theme = 0;
                            reset();
                            break;

                        case TB_KEY_CTRL_X:
                            current_width++;
                            if(current_width >= (int)width_options.size())
                                current_width = 0;
                            reset();
                            break;

                        case TB_KEY_CTRL_R: 
                            reset(); 
                            break;

                        default:
                            if(e.ch >= L'!' && e.ch <= L'~') { 
                                // Range of displayable characters
	                            if(!started) {
	                                started = true; 
	                                start_time = system_clock::now();
	                            }
	                            if(current_word < (int)words.size()) {
	                                user_input += e.ch;
	                            }
                            }
                                                  
                    }
                    break;
                case TB_EVENT_RESIZE: break;
            }
        }
    }    

    tb_shutdown();
}
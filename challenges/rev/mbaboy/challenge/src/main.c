#include <stdint.h>

#include <gb/gb.h>
#include <gb/drawing.h>

#include "checker_state.h"
#include "config.h"
#include "menu_buttons.h"
#include "flag_checker.h"

/// \brief An util function that would erase the char from the screen
/// \param x x coord
/// \param y y coord
static void clear_char(uint8_t x, uint8_t y)
{
    /// \note es3n1n: Feels wrong
    gotogxy(x, y);
    gprint(" ");
}

/// \brief An util function that would either draw a text, or erase it from the screen
/// \param cond The condition. true - draw, false - erase
/// \param s Text
/// \param x X pos
/// \param y Y pos
/// \param size size in chars
static void draw_or_clear(uint8_t cond, char *s, uint8_t x, uint8_t y)
{
    /// Draw text if needed
    if (cond)
    {
        gotogxy(x, y);
        gprint(s);
        return;
    }

    /// Clear the screen space otherwise
    clear_char(x, y);
}

/// \brief Joy buttons handler
/// \param state current state
static void joy_handler(uint8_t raw_state)
{
    /// Previous joypad state
    static uint8_t prev = 0;
    /// Keys that we're checking for input
    static uint8_t keys[] = {J_LEFT, J_RIGHT, J_UP, J_DOWN, J_A, J_B, J_START, J_SELECT};

    /// Create new copy of the state, because we don't want to modify raw_state
    /// before saving it to the `prev` var
    uint8_t state = raw_state;

    /// Iterating over the keys that we're checking for
    for (unsigned short i = 0; i < sizeof(keys) / sizeof(keys[0]); ++i)
    {
        /// If was pressed in last frame and pressed in this one
        if ((prev & keys[i]) && (state & keys[i]))
        {
            /// Remove it from the state
            state &= ~keys[i];
        }
    }

    /// Backup the state
    prev = raw_state;

    /// Don't process input if its blocked
    if (g_state.input_blocked)
        return;

    /// Handle joy button left
    if (state & J_LEFT)
        state_dec_button_index(&g_state, 1);

    /// Handle joy button right
    if (state & J_RIGHT)
        state_inc_button_index(&g_state, 1);

    /// Handle joy button up
    if (state & J_UP)
        state_dec_button_index(&g_state, KEYBOARD_LINE_SIZE);

    /// Handle joy button down
    if (state & J_DOWN)
        state_inc_button_index(&g_state, KEYBOARD_LINE_SIZE);

    /// Treat A/B/C/START/X/Y/Z/MODE buttons as the enter
    if ((state & J_A) || (state & J_B) || (state & J_START) || (state & J_SELECT))
    {
        /// Get the selected button and invoke its callback
        PMENU_BUTTON button = &g_menu_buttons[g_state.selected_button];
        button->cbk(button);
    }
}

/// \brief Draw the keyboard
static void draw_keyboard(void)
{
    /// Constants
    const uint8_t start_x = 3, start_y = 6;
    const uint8_t special_start_x = start_x + 2, special_start_y = start_y + 10;

    /// State
    uint8_t x = start_x;
    uint8_t y = start_y;

    /// Print chars
    for (unsigned short index = 0; index < CHARS_SIZE; ++index, x += 2)
    {
        /// Move cursor to the next line, if needed
        if (index != 0 && index % KEYBOARD_LINE_SIZE == 0)
        {
            y += 2;
            x = start_x;
        }

        /// Render button label
        gotogxy(x, y);
        gprint(g_menu_buttons[index].label);

        /// Render the "^" if selected
        draw_or_clear(g_state.selected_button == index, "^", x, y + 1);
    }

    /// Set start coords
    x = special_start_x;
    y = special_start_y;

    /// Print other buttons
    for (unsigned short index = CHARS_SIZE; index < MAX_MENU_BUTTONS; ++index, x += 5)
    {
        gotogxy(x, y);
        gprint(g_menu_buttons[index].label);

        draw_or_clear(g_state.selected_button == index, ">", x - 1, y);
    }
}

/// \brief Draw current flag value
static void draw_flag(void)
{
    const uint8_t x = 2;
    const uint8_t y = 4;

    /// Draw the flag value
    gotogxy(x, y);
    gprintf(">%s", g_state.flag);

    /// Clear the next char
    clear_char(x + (g_state.char_index + 1), y);
}

/// \brief The whole render function
static void redraw_frame(void)
{
    /// Return if render is blocked
    if (g_state.render_blocked)
        return;

    /// Draw header
    gotogxy(5, 1);
    gprint("Flagchecker");

    /// Draw keyboard
    draw_keyboard();

    /// Draw the flag content
    draw_flag();
}

/// \brief Check the flag
/// \param pbutton unused button ptr
static void check_flag(void *pbutton)
{
    /// Disard unused arguments
    (void)pbutton;

    /// Blocking the input/render once `CHECK` button was hit
    g_state.input_blocked = 1;
    g_state.render_blocked = 1;

    /// Check whether flag is valid or not
    volatile uint8_t flag_matches = is_flag_valid(g_state.flag, g_state.char_index + 1);

    /// Deduce the message text
    const char *text_vals[] = {
        "INVALID ",
        "INVALID ",
        "INVALID ",
        "CORRECT "};
    const char *text = text_vals[flag_matches];

    /// Write the message
    uint8_t x = 0, y = 0;
    for (unsigned short i = 0; i < 18; ++i)
    {
        gotogxy(x, y);

        for (unsigned short j = 0; j < 5; ++j)
            gprint(text);

        y += 1;
    }
}

int main(void)
{
    /// Update the `check` button handler
    g_menu_buttons[MAX_MENU_BUTTONS - 1].cbk = check_flag;

    /// Main render loop
    while (1)
    {
        /// Poll and dispatch joy events
        joy_handler(joypad());

        /// Render frame
        redraw_frame();

        /// Wait for vsync
        vsync();
    }
}
#ifndef __MENU_BUTTONS_H__
#define __MENU_BUTTONS_H__
#include "checker_state.h"
#include "config.h"

/// \brief Button callback
typedef void (*BUTTON_CBK)(void *);
/// \brief Button info structure
typedef struct _MENU_BUTTON
{
    /// Button label, aka its text
    const char *label;
    /// Button callback
    BUTTON_CBK cbk;
} MENU_BUTTON, *PMENU_BUTTON;

/// \brief General callback for the keyboard button
/// \param pthis this ptr
void enter_char_menu_button(void *pthis)
{
    /// Cast pthis to the menu button type
    PMENU_BUTTON button = (PMENU_BUTTON)pthis;
    /// Add first char from the label to flag
    state_set_char(&g_state, button->label[0]);
}

/// \brief Remove last character from the flag
/// \param pthis button this ptr
void del_char_menu_button(void *pthis)
{
    (void)pthis;
    state_del_char(&g_state);
}

#define MAKE_CHAR_BUTTON(x)       \
    {                             \
        x, enter_char_menu_button \
    }
MENU_BUTTON g_menu_buttons[MAX_MENU_BUTTONS] = {
    /// Chars
    MAKE_CHAR_BUTTON("0"),
    MAKE_CHAR_BUTTON("1"),
    MAKE_CHAR_BUTTON("2"),
    MAKE_CHAR_BUTTON("3"),
    MAKE_CHAR_BUTTON("4"),
    MAKE_CHAR_BUTTON("5"),
    MAKE_CHAR_BUTTON("6"),
    MAKE_CHAR_BUTTON("7"),
    MAKE_CHAR_BUTTON("8"),
    MAKE_CHAR_BUTTON("9"),
    MAKE_CHAR_BUTTON("A"),
    MAKE_CHAR_BUTTON("B"),
    MAKE_CHAR_BUTTON("C"),
    MAKE_CHAR_BUTTON("D"),
    MAKE_CHAR_BUTTON("E"),
    MAKE_CHAR_BUTTON("F"),
    MAKE_CHAR_BUTTON("G"),
    MAKE_CHAR_BUTTON("H"),
    MAKE_CHAR_BUTTON("I"),
    MAKE_CHAR_BUTTON("J"),
    MAKE_CHAR_BUTTON("K"),
    MAKE_CHAR_BUTTON("L"),
    MAKE_CHAR_BUTTON("M"),
    MAKE_CHAR_BUTTON("N"),
    MAKE_CHAR_BUTTON("O"),
    MAKE_CHAR_BUTTON("P"),
    MAKE_CHAR_BUTTON("Q"),
    MAKE_CHAR_BUTTON("R"),
    MAKE_CHAR_BUTTON("S"),
    MAKE_CHAR_BUTTON("T"),
    MAKE_CHAR_BUTTON("U"),
    MAKE_CHAR_BUTTON("V"),
    MAKE_CHAR_BUTTON("W"),
    MAKE_CHAR_BUTTON("X"),
    MAKE_CHAR_BUTTON("Y"),
    MAKE_CHAR_BUTTON("Z"),
    MAKE_CHAR_BUTTON("_"),
    MAKE_CHAR_BUTTON("-"),
    MAKE_CHAR_BUTTON("{"),
    MAKE_CHAR_BUTTON("}"),
    /// Special buttons
    {"DEL", del_char_menu_button},
    {"CHECK", 0}};
#undef MAKE_CHAR_BUTTON
#endif
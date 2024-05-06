#ifndef __STATE_H__
#define __STATE_H__
#include <stdint.h>

/// \brief Flag checker state that contains all the stuff that we need to know
typedef struct _CHECKER_STATE
{
    /// Current flag content
    char flag[16];
    /// Current flag length
    unsigned short char_index;
    /// Current selected button
    unsigned short selected_button;
    /// Input blocker state
    uint8_t input_blocked;
    /// Render blocker state
    uint8_t render_blocked;
} CHECKER_STATE, *PCHECKER_STATE;

/// \brief Increment the button index
void state_inc_button_index(PCHECKER_STATE pthis, unsigned short count);
/// \brief Decrement the button index
void state_dec_button_index(PCHECKER_STATE pthis, unsigned short count);
/// \brief Add char to the flag
void state_set_char(PCHECKER_STATE pthis, char ch);
/// \brief Remove last char from the flag
void state_del_char(PCHECKER_STATE pthis);

/// Allocate the global state
static CHECKER_STATE g_state = {{0}, 0, 0, 0};
#endif
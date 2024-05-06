#include "checker_state.h"
#include "config.h"

/// \brief Increment button index
/// \param pthis checker state ptr
/// \param count count that would be added to the `selected_button`
void state_inc_button_index(PCHECKER_STATE pthis, unsigned short count)
{
    /// If oob
    if ((pthis->selected_button + count) >= MAX_MENU_BUTTONS)
    {
        /// Clamp to the max value
        pthis->selected_button = MAX_MENU_BUTTONS - 1;
        return;
    }

    /// Otherwise add the count
    pthis->selected_button += count;
}

/// \brief Decrement button index
/// \param pthis checker state ptr
/// \param count count that would be extracted from the `selected_button`
void state_dec_button_index(PCHECKER_STATE pthis, unsigned short count)
{
    /// If oob
    if (pthis->selected_button < count)
    {
        /// Clamp to the min value
        pthis->selected_button = 0;
        return;
    }

    /// Substract
    pthis->selected_button -= count;
}

/// \brief Set current char value
/// \param pthis checker state ptr
/// \param ch char value
void state_set_char(PCHECKER_STATE pthis, char ch)
{
    /// If oob
    if (pthis->char_index >= (sizeof(pthis->flag) - 1))
    {
        return;
    }

    /// Set the char value and increment the char index
    pthis->flag[pthis->char_index] = ch;
    pthis->char_index += 1;
}

/// \brief Delete the current char
/// \param pthis checker state ptr
void state_del_char(PCHECKER_STATE pthis)
{
    /// If we already are on the first index
    if (pthis->char_index == 0)
    {
        return;
    }

    /// Clear the char, decrement the index
    pthis->flag[pthis->char_index] = 0;
    pthis->char_index -= 1;
}
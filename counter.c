/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 *
 * 2021 - Caleb Martin
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include "plugin.h"
#include "lib/pluginlib_actions.h"

// PLA context setup
static const struct button_mapping *plugin_contexts[] = {
    pla_main_ctx,
#ifdef HAVE_REMOTE_LCD
    pla_remote_ctx,
#endif
};

// define buttons with PLA
#define COUNTER_QUIT PLA_EXIT
#define COUNTER_QUIT2 PLA_CANCEL
#define COUNTER_LEFT PLA_LEFT
#define COUNTER_RIGHT PLA_RIGHT
#define COUNTER_DOWN PLA_DOWN
#define COUNTER_UP PLA_UP
#define COUNTER_SELECT PLA_SELECT

static void update_screen(char* buf, int count) {
    rb->lcd_clear_display();
    rb->snprintf(buf, sizeof(buf), "%d", count);
    rb->lcd_puts(0, 0, buf);
    rb->lcd_update();
    #ifdef HAVE_REMOTE_LCD
        rb->lcd_remote_puts(0, 0, buf);
        rb->lcd_remote_update();
    #endif
}

/* plugin entry point */
enum plugin_status plugin_start(const void *parameter)
{
    (void)parameter;
    char buf[32];
    int count = 0;
    int button;
    update_screen(buf, count);

    while (1)
    {
        // Get PLA button
        button = pluginlib_getaction(TIMEOUT_NOBLOCK, plugin_contexts, ARRAYLEN(plugin_contexts));

        switch (button)
        {
        case COUNTER_QUIT:
        case COUNTER_QUIT2:
            return PLUGIN_OK;
        case COUNTER_LEFT:
            count--;
            update_screen(buf, count);
            break;
        case COUNTER_DOWN:
            count = 0;
            update_screen(buf, count);
            break;
        case COUNTER_RIGHT:
        case COUNTER_UP:
        case COUNTER_SELECT:
            count++;
            update_screen(buf, count);
            break;
        default:
            if (rb->default_event_handler(button) == SYS_USB_CONNECTED)
            {
                return PLUGIN_USB_CONNECTED;
            }
            break;
        }
    }
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "SDL2/SDL.h"
#include "lvgl/lvgl.h"
#include "lvgl/lv_drivers/display/monitor.h"
#include "lvgl/lv_drivers/indev/mouse.h"

#include "page1.h"
#include "io.h"


static int tick_thread(void *data);
static void hal_init(void);


int main(int argc, char** argv)
{
    // Initialize LittleVGL
    lv_init();

    // Initialize Hardware Abstraction Layer
    hal_init();

    set_theme();
    page1_init();

    // Start operational threads
    if (terminal_io_init() == -1) {
        printf("Error while initializing terminal\n");
        exit(-1);
    }

    while(1) {

        lv_task_handler();
        usleep(1000);
    }
    
    return 0;
}

static void hal_init(void)
{
    monitor_init();
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.disp_flush = monitor_flush;
    disp_drv.disp_fill = monitor_fill;
    disp_drv.disp_map = monitor_map;
    lv_disp_drv_register(&disp_drv);

    // Add mouse
    mouse_init();
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);  // Basic initialization
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read = mouse_read;
    lv_indev_drv_register(&indev_drv);

    // Call lv_tick_handler() every millisecond by creating an SDL thread
    SDL_CreateThread(tick_thread, "tick", NULL);
}

static int tick_thread(void *data)
{
    while(1) {
        lv_tick_inc(1);
        SDL_Delay(1);   // Sleep for 1ms
    }
    return 0;
}

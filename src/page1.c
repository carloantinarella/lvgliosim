#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "page1.h"
#include "lvgl/lvgl.h"
#include "io.h"

// Digital input actions
static lv_res_t I1_click(lv_obj_t *);
static lv_res_t I1_pr(lv_obj_t *);

static lv_res_t I2_click(lv_obj_t *);
static lv_res_t I2_pr(lv_obj_t *);

static lv_res_t I3_click(lv_obj_t *);
static lv_res_t I3_pr(lv_obj_t *);

static lv_res_t I4_click(lv_obj_t *);
static lv_res_t I4_pr(lv_obj_t *);

// Analog input actions
static lv_res_t POT1_action(lv_obj_t *);

static void* out_led_loop(void *);


/* Led objects */
static lv_obj_t *o1_led;
static lv_obj_t *o2_led;
static lv_obj_t *o3_led;
static lv_obj_t *o4_led;

/* Gauge objects */
static lv_obj_t *ana1_gauge;

void set_theme(void)
{
    // Initialize and load theme
    lv_theme_t *th = lv_theme_night_init(210, NULL);
    // lv_theme_t *th = lv_theme_default_init(210, NULL);
    // lv_theme_t *th = lv_theme_night_init(210, NULL);
    lv_theme_set_current(th);
    printf("LV theme initialized\n");
}

void page1_init(void)
{
    lv_obj_t *scr = lv_obj_create(NULL, NULL);
    lv_scr_load(scr);
    
    // Create a label on the screen
    lv_obj_t *title = lv_label_create(scr, NULL);
    lv_label_set_text(title, "lvgl I/O Simulator");
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

    // Create input buttons

    // I1
    lv_obj_t *i1_btn = lv_btn_create(scr, NULL);
    lv_obj_t *i1_btn_title = lv_label_create(i1_btn, NULL);
    lv_label_set_text(i1_btn_title, "I1");
    lv_obj_align(i1_btn, NULL, LV_ALIGN_OUT_TOP_LEFT, 20, 150);
    lv_btn_set_action(i1_btn, LV_BTN_ACTION_CLICK, I1_click);
    lv_btn_set_action(i1_btn, LV_BTN_ACTION_PR, I1_pr);
    

    // I2
    lv_obj_t *i2_btn = lv_btn_create(scr, NULL);
    lv_obj_t *i2_btn_title = lv_label_create(i2_btn, NULL);
    lv_label_set_text(i2_btn_title, "I2");
    lv_obj_align(i2_btn, NULL, LV_ALIGN_OUT_TOP_LEFT, 20, 250);
    lv_btn_set_action(i2_btn, LV_BTN_ACTION_CLICK, I2_click);
    lv_btn_set_action(i2_btn, LV_BTN_ACTION_PR, I2_pr);

    // I3
    lv_obj_t *i3_btn = lv_btn_create(scr, NULL);
    lv_obj_t *i3_btn_title = lv_label_create(i3_btn, NULL);
    lv_label_set_text(i3_btn_title, "I3");
    lv_obj_align(i3_btn, NULL, LV_ALIGN_OUT_TOP_LEFT, 20, 350);
    lv_btn_set_action(i3_btn, LV_BTN_ACTION_CLICK, I3_click);
    lv_btn_set_action(i3_btn, LV_BTN_ACTION_PR, I3_pr);

    // I4
    lv_obj_t *i4_btn = lv_btn_create(scr, NULL);
    lv_obj_t *i4_btn_title = lv_label_create(i4_btn, NULL);
    lv_label_set_text(i4_btn_title, "I4");
    lv_obj_align(i4_btn, NULL, LV_ALIGN_OUT_TOP_LEFT, 20, 450);
    lv_btn_set_action(i4_btn, LV_BTN_ACTION_CLICK, I4_click);
    lv_btn_set_action(i4_btn, LV_BTN_ACTION_PR, I4_pr);

    // Create input sliders

    // POT 1
    lv_obj_t *pot1_sl = lv_slider_create(scr, NULL);
    lv_slider_set_range(pot1_sl, 0, 100);
    lv_slider_set_action(pot1_sl, POT1_action);
    lv_obj_align(pot1_sl, NULL, LV_ALIGN_IN_TOP_MID, 0, 450);
    lv_obj_t *pot1_label = lv_label_create(scr, NULL);
    lv_label_set_text(pot1_label, "POT 1");
    lv_obj_align(pot1_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 420);

    // Create output leds
    
    // O1
    o1_led = lv_led_create(scr, NULL);
    lv_obj_align(o1_led, NULL, LV_ALIGN_OUT_TOP_RIGHT, -20, 100);
    lv_obj_t *o1_label = lv_label_create(scr, NULL);
    lv_label_set_text(o1_label, "O1");
    lv_obj_align(o1_label, NULL, LV_ALIGN_OUT_TOP_RIGHT, -60, 90);

    // O2
    o2_led = lv_led_create(scr, NULL);
    lv_obj_align(o2_led, NULL, LV_ALIGN_OUT_TOP_RIGHT, -20, 200);
    lv_obj_t *o2_label = lv_label_create(scr, NULL);
    lv_label_set_text(o2_label, "O2");
    lv_obj_align(o2_label, NULL, LV_ALIGN_OUT_TOP_RIGHT, -60, 190);
    
    // O3
    o3_led = lv_led_create(scr, NULL);
    lv_obj_align(o3_led, NULL, LV_ALIGN_OUT_TOP_RIGHT, -20, 300);
    lv_obj_t *o3_label = lv_label_create(scr, NULL);
    lv_label_set_text(o3_label, "O3");
    lv_obj_align(o3_label, NULL, LV_ALIGN_OUT_TOP_RIGHT, -60, 290);

    // O4
    o4_led = lv_led_create(scr, NULL);
    lv_obj_align(o4_led, NULL, LV_ALIGN_OUT_TOP_RIGHT, -20, 400);
    lv_obj_t *o4_label = lv_label_create(scr, NULL);
    lv_label_set_text(o4_label, "O4");
    lv_obj_align(o4_label, NULL, LV_ALIGN_OUT_TOP_RIGHT, -60, 390);

    // ANA 1
    ana1_gauge = lv_gauge_create(scr, NULL);
    lv_obj_align(ana1_gauge, NULL, LV_ALIGN_OUT_TOP_MID, 0, 400);
    lv_gauge_set_range(ana1_gauge, 0, 100);
    lv_obj_t *ana1_label = lv_label_create(scr, NULL);
    lv_label_set_text(ana1_label, "ANALOG 1");
    lv_obj_align(ana1_label, NULL, LV_ALIGN_OUT_TOP_MID, 0, 190);

    // Start thread for OUT reading
    pthread_t out_read_th;
    pthread_attr_t out_read_th_att;
    pthread_attr_init(&out_read_th_att); // default thread attributes 
    pthread_create(&out_read_th, &out_read_th_att, out_led_loop, NULL);


}

static lv_res_t I1_click(lv_obj_t *btn)
{
    // Action when the button is released
    // printf("Button released\n");
    set_i1(0);
    return LV_RES_OK;
}

static lv_res_t I1_pr(lv_obj_t *btn)
{
    // Action when the button is pressed
    // printf("Button pressed\n");
    set_i1(1);
    return LV_RES_OK;
}

static lv_res_t I2_click(lv_obj_t *btn)
{
    // Action when the button is released
    // printf("Button released\n");
    set_i2(0);
    return LV_RES_OK;
}

static lv_res_t I2_pr(lv_obj_t *btn)
{
    // Action when the button is pressed
    // printf("Button pressed\n");
    set_i2(1);
    return LV_RES_OK;
}

static lv_res_t I3_click(lv_obj_t *btn)
{
    // Action when the button is released
    // printf("Button released\n");
    set_i3(0);
    return LV_RES_OK;
}

static lv_res_t I3_pr(lv_obj_t *btn)
{
    // Action when the button is pressed
    // printf("Button pressed\n");
    set_i3(1);
    return LV_RES_OK;
}

static lv_res_t I4_click(lv_obj_t *btn)
{
    // Action when the button is released
    // printf("Button released\n");
    set_i4(0);
    return LV_RES_OK;
}

static lv_res_t I4_pr(lv_obj_t *btn)
{
    // Action when the button is pressed
    // printf("Button pressed\n");
    set_i4(1);
    return LV_RES_OK;
}

static lv_res_t POT1_action(lv_obj_t *sl)
{
    // Action when slider value changes
    int v = lv_slider_get_value(sl);
    set_pot1((65535 * v) / 100);
    return LV_RES_OK;
}

static void* out_led_loop(void *arg)
{
    /* This must be started as thread */
    
    while(1) {

        // OUT 1
        if (get_o1() == 0) {
            lv_led_set_bright(o1_led, 0);
        } else {
            lv_led_set_bright(o1_led, 255);
        }

        // OUT 2
        if (get_o2() == 0) {
            lv_led_set_bright(o2_led, 0);
        } else {
            lv_led_set_bright(o2_led, 255);
        }

        // OUT 3
        if (get_o3() == 0) {
            lv_led_set_bright(o3_led, 0);
        } else {
            lv_led_set_bright(o3_led, 255);
        }

        // OUT 4
        if (get_o4() == 0) {
            lv_led_set_bright(o4_led, 0);
        } else {
            lv_led_set_bright(o4_led, 255);
        }

        // ANA 1
        int normalized_ana1 = (100 * get_ana1()) / 65535;
        lv_gauge_set_value(ana1_gauge, 0, normalized_ana1);

        usleep(50000);
    }

    return NULL;
}
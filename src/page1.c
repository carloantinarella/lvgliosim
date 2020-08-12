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

// Analog input actions
static lv_res_t POT1_action(lv_obj_t *);

static void* out_led_loop(void *);


/* Led objects */
static lv_obj_t *o1_led;
static lv_obj_t *o2_led;
static lv_obj_t *o3_led;

/* Gauge objects */
static lv_obj_t *ana1_gauge;

void set_theme(void)
{
    // Initialize and load theme
    lv_theme_t *th = lv_theme_default_init(210, NULL);
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
    lv_label_set_text(title, "Home Page");
    lv_obj_align(title, NULL, LV_ALIGN_IN_TOP_MID, 0, 20);

    // Create input buttons

    // I1
    lv_obj_t *i1_btn = lv_btn_create(scr, NULL);
    lv_obj_t *i1_btn_title = lv_label_create(i1_btn, NULL);
    lv_label_set_text(i1_btn_title, "I1");
    lv_obj_align(i1_btn, NULL, LV_ALIGN_OUT_TOP_LEFT, 0, 50);
    lv_btn_set_action(i1_btn, LV_BTN_ACTION_CLICK, I1_click);
    lv_btn_set_action(i1_btn, LV_BTN_ACTION_PR, I1_pr);
    

    // I2
    lv_obj_t *i2_btn = lv_btn_create(scr, NULL);
    lv_obj_t *i2_btn_title = lv_label_create(i2_btn, NULL);
    lv_label_set_text(i2_btn_title, "I2");
    lv_obj_align(i2_btn, NULL, LV_ALIGN_OUT_TOP_LEFT, 0, 150);
    lv_btn_set_action(i2_btn, LV_BTN_ACTION_CLICK, I2_click);
    lv_btn_set_action(i2_btn, LV_BTN_ACTION_PR, I2_pr);

    // I3
    lv_obj_t *i3_btn = lv_btn_create(scr, NULL);
    lv_obj_t *i3_btn_title = lv_label_create(i3_btn, NULL);
    lv_label_set_text(i3_btn_title, "I3");
    lv_obj_align(i3_btn, NULL, LV_ALIGN_OUT_TOP_LEFT, 0, 250);
    lv_btn_set_action(i3_btn, LV_BTN_ACTION_CLICK, I3_click);
    lv_btn_set_action(i3_btn, LV_BTN_ACTION_PR, I3_pr);

    // Create input sliders

    // POT 1
    lv_obj_t *pot1_sl = lv_slider_create(scr, NULL);
    lv_slider_set_range(pot1_sl, 0, 100);
    lv_slider_set_action(pot1_sl, POT1_action);
    lv_obj_align(pot1_sl, NULL, LV_ALIGN_IN_TOP_MID, 0, 300);

    // Create output leds

    // O1
    o1_led = lv_led_create(scr, NULL);
    lv_obj_align(o1_led, NULL, LV_ALIGN_OUT_TOP_RIGHT, 0, 50);

    // O2
    o2_led = lv_led_create(scr, NULL);
    lv_obj_align(o2_led, NULL, LV_ALIGN_OUT_TOP_RIGHT, 0, 150);

    // O3
    o3_led = lv_led_create(scr, NULL);
    lv_obj_align(o3_led, NULL, LV_ALIGN_OUT_TOP_RIGHT, 0, 250);

    // ANA 1
    ana1_gauge = lv_gauge_create(scr, NULL);
    lv_obj_align(ana1_gauge, NULL, LV_ALIGN_OUT_TOP_MID, 0, 250);
    lv_gauge_set_range(ana1_gauge, 0, 100);

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

        // ANA 1
        int normalized_ana1 = (100 * get_ana1()) / 65535;
        lv_gauge_set_value(ana1_gauge, 0, normalized_ana1);

        usleep(50000);
    }

    return NULL;
}
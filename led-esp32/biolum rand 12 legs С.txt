#include "driver/rmt_tx.h"
#include "led_strip.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_LEGS 12         // Количество ног
#define NUM_PIXELS 102      // Количество светодиодов на каждой ноге
#define DECAY_SPEED 0.05f   // Скорость затухания свечения
#define SPARKLE_PROB 0.08f  // Вероятность вспышки
#define UPDATE_SPEED 50     // Скорость обновления в миллисекундах

// Пины для 12 ног
int LED_PINS[NUM_LEGS] = {2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23};

// Массивы яркости для каждой ноги
float brightness[NUM_LEGS][NUM_PIXELS];

// Создание объектов светодиодных лент
led_strip_handle_t led_strips[NUM_LEGS];

void setup_led_strips() {
    for (int i = 0; i < NUM_LEGS; i++) {
        led_strip_config_t strip_config = {
            .strip_gpio_num = LED_PINS[i],
            .max_leds = NUM_PIXELS,
        };

        led_strip_rmt_config_t rmt_config = {
            .resolution_hz = 10 * 1000 * 1000, // 10 MHz
        };

        led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strips[i]);
    }
}

void get_bioluminescent_color(float brightness, uint8_t *r, uint8_t *g, uint8_t *b) {
    *r = 0;  // Красный = 0
    *g = (uint8_t)(30 + 225 * brightness);
    *b = (uint8_t)(50 + 205 * brightness);
}

void update_leg(led_strip_handle_t strip, float *brightness_array) {
    for (int i = 0; i < NUM_PIXELS; i++) {
        if ((float)rand() / RAND_MAX < SPARKLE_PROB) {
            brightness_array[i] = 1.0f; // Новая вспышка
        }

        if (brightness_array[i] > 0) {
            brightness_array[i] -= DECAY_SPEED;
            if (brightness_array[i] < 0) {
                brightness_array[i] = 0;
            }
        }

        uint8_t r, g, b;
        get_bioluminescent_color(brightness_array[i], &r, &g, &b);
        led_strip_set_pixel(strip, i, r, g, b);
    }

    led_strip_refresh(strip);
}

void bioluminescence_effect() {
    while (1) {
        for (int i = 0; i < NUM_LEGS; i++) {
            update_leg(led_strips[i], brightness[i]);
        }
        vTaskDelay(pdMS_TO_TICKS(UPDATE_SPEED)); // Задержка перед следующим обновлением
    }
}

void app_main() {
    srand(time(NULL));
    setup_led_strips();
    bioluminescence_effect();
}

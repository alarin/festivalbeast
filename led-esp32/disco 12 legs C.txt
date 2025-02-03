#include <FastLED.h>

#define NUM_LEGS 12         // Количество ног
#define NUM_PIXELS 102      // Количество светодиодов в каждой ноге
#define DISCO_SPEED 100     // Задержка в миллисекундах (чем меньше, тем быстрее)
#define BRIGHTNESS 150      // Максимальная яркость (0-255)

// Пины для 12 ног
const int LED_PINS[NUM_LEGS] = {2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23};

// Создаём массив лент для каждой ноги
CRGB leds[NUM_LEGS][NUM_PIXELS];

void get_random_color(CRGB &color) {
    color.r = random(50, BRIGHTNESS);
    color.g = random(50, BRIGHTNESS);
    color.b = random(50, BRIGHTNESS);
}

void update_legs() {
    for (int leg = 0; leg < NUM_LEGS; leg++) {
        for (int i = 0; i < NUM_PIXELS; i++) {
            get_random_color(leds[leg][i]);
        }
    }
    FastLED.show();
}

void setup() {
    for (int i = 0; i < NUM_LEGS; i++) {
        FastLED.addLeds<WS2812, LED_PINS[i], GRB>(leds[i], NUM_PIXELS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(BRIGHTNESS);
    }
    randomSeed(analogRead(0)); // Инициализация случайных чисел
}

void loop() {
    update_legs();
    delay(DISCO_SPEED);
}

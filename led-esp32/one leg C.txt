#include <Adafruit_NeoPixel.h>

#define PIN_WS2812B 26  // Пин ESP32, подключенный к ленте WS2812B
#define NUM_PIXELS 102  // Общее количество светодиодов

Adafruit_NeoPixel np(NUM_PIXELS, PIN_WS2812B, NEO_GRB + NEO_KHZ800);

// Исходный фиолетовый цвет (RGB)
int baseR = 148, baseG = 0, baseB = 211;
// Бирюзовый цвет (RGB)
int targetR = 0, targetG = 255, targetB = 255;

// Коэффициент изменения цвета
float colorShiftFactor = 0.05;

// Параметр скорости (чем больше, тем быстрее)
int speedFactor = 40;

// Функция для получения промежуточного цвета
uint32_t get_gradient_color(int step) {
    int r = baseR + (targetR - baseR) * step * colorShiftFactor;
    int g = baseG + (targetG - baseG) * step * colorShiftFactor;
    int b = baseB + (targetB - baseB) * step * colorShiftFactor;

    r = constrain(r, 0, 255);
    g = constrain(g, 0, 255);
    b = constrain(b, 0, 255);

    return np.Color(r, g, b);
}

// Функция плавного включения
void wave_on() {
    for (int i = 0; i < 16; i++) {
        if (67 - i >= 0) np.setPixelColor(67 - i, get_gradient_color(i));  // Влево
        if (67 + i < NUM_PIXELS) np.setPixelColor(67 + i, get_gradient_color(i));  // Вправо
        np.show();
        delay(1000 / speedFactor);
    }

    for (int i = 0; i < 10; i++) {
        if (83 + i < NUM_PIXELS) np.setPixelColor(83 + i, get_gradient_color(i + 20));
        if (92 + i < NUM_PIXELS) np.setPixelColor(92 + i, get_gradient_color(i + 20));
        if (51 - i >= 0) np.setPixelColor(51 - i, get_gradient_color(i + 20));
        np.show();
        delay(1000 / speedFactor);
    }
}

// Функция плавного выключения (снизу вверх)
void wave_off_bottom_to_top() {
    for (int i = 0; i < 16; i++) {
        if (67 - i >= 0) np.setPixelColor(67 - i, 0);
        if (67 + i < NUM_PIXELS) np.setPixelColor(67 + i, 0);
        np.show();
        delay(1000 / speedFactor);
    }

    for (int i = 0; i < 10; i++) {
        if (83 + i < NUM_PIXELS) np.setPixelColor(83 + i, 0);
        if (92 + i < NUM_PIXELS) np.setPixelColor(92 + i, 0);
        if (51 - i >= 0) np.setPixelColor(51 - i, 0);
        np.show();
        delay(1000 / speedFactor);
    }
}

// Функция плавного выключения (сверху вниз)
void wave_off_top_to_bottom() {
    for (int i = 9; i >= 0; i--) {
        if (83 + i < NUM_PIXELS) np.setPixelColor(83 + i, 0);
        if (93 + i < NUM_PIXELS) np.setPixelColor(93 + i, 0);
        if (51 - i >= 0) np.setPixelColor(51 - i, 0);
        np.show();
        delay(1000 / speedFactor);
    }

    for (int i = 15; i >= 0; i--) {
        if (67 - i >= 0) np.setPixelColor(67 - i, 0);
        if (67 + i < NUM_PIXELS) np.setPixelColor(67 + i, 0);
        np.show();
        delay(1000 / speedFactor);
    }
}

// Функция интенсивного огненного мерцания
void random_blink_fire() {
    int active_pixels[NUM_PIXELS] = {0};  // Массив для хранения зажженных пикселей
    int num_active = 0;

    for (int cycle = 0; cycle < 20; cycle++) {
        int num_lights = random(10, 30);

        for (int j = 0; j < num_lights; j++) {
            int random_pixel = random(0, 41);
            uint32_t fire_color = np.Color(
                random(200, 255),  // Красный компонент
                random(50, 150),   // Зеленый компонент (создает оранжевые оттенки)
                random(0, 50)      // Синий компонент (минимальный для теплых цветов)
            );

            np.setPixelColor(random_pixel, fire_color);
            active_pixels[num_active++] = random_pixel;
        }

        np.show();
        delay(random(20, 50));
    }

    // Постепенное затухание случайных пикселей
    while (num_active > 0) {
        int pixel_to_dim = active_pixels[--num_active];
        np.setPixelColor(pixel_to_dim, 0);
        np.show();
        delay(random(20, 100));
    }
}

void setup() {
    pinMode(PIN_WS2812B, OUTPUT);
    np.begin();
    np.clear();
    np.show();
    randomSeed(analogRead(0));  // Инициализация генератора случайных чисел
}

void loop() {
    wave_on();
    random_blink_fire();
    delay(500 / speedFactor);

    wave_off_bottom_to_top();
    delay(500 / speedFactor);
}

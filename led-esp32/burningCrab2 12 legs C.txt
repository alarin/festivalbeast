#include <Adafruit_NeoPixel.h>

#define NUM_LEGS 12  // Количество ног
#define NUM_PIXELS 102  // Количество светодиодов на каждой ноге

// Пины для 12 ног
int LED_PINS[NUM_LEGS] = {2, 4, 5, 12, 13, 14, 15, 18, 19, 21, 22, 23};

// Создаем объекты NeoPixel для каждой ноги
Adafruit_NeoPixel legs[NUM_LEGS] = {
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[0], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[1], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[2], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[3], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[4], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[5], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[6], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[7], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[8], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[9], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[10], NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(NUM_PIXELS, LED_PINS[11], NEO_GRB + NEO_KHZ800)
};

// Настройки огня
float WAVE_SPEED = 0.08;  // Скорость обновления анимации
float FLICKER_PROBABILITY = 0.82;  // Шанс случайной вспышки
int MAX_WAVES = 19;  // Максимальное количество активных волн на каждой ноге

// Градиент огня (по мере угасания)
int FIRE_COLORS[5][3] = {
  {0, 0, 0},       // Нет огня
  {50, 0, 0},      // Тёмно-красный (слабое тление)
  {150, 30, 0},    // Красно-оранжевый (разгорается)
  {255, 80, 0},    // Яркий оранжевый (пик огня)
  {255, 150, 0}    // Ослабевающий огонь
};

// Для каждой ноги создаём свой список волн
struct FireWave {
  int center;
  int radius;
  int max_radius;
  int intensity;
};

struct FireWave fire_waves[NUM_LEGS][MAX_WAVES];  // Массив для волн на каждой ноге
int wave_count[NUM_LEGS] = {0};  // Счётчик текущих волн для каждой ноги

// Очищаем все пиксели перед стартом
void clear_pixels() {
  for (int i = 0; i < NUM_LEGS; i++) {
    for (int j = 0; j < NUM_PIXELS; j++) {
      legs[i].setPixelColor(j, 0, 0, 0);
    }
    legs[i].show();
  }
}

// Создаёт новую волну огня на ноге
void start_new_wave(int leg_index) {
  if (wave_count[leg_index] < MAX_WAVES && random(100) < FLICKER_PROBABILITY * 100) {
    int start_pos = random(NUM_PIXELS);  // Случайный пиксель
    fire_waves[leg_index][wave_count[leg_index]] = {start_pos, 0, random(6, 18), 4};  // Начальная волна
    wave_count[leg_index]++;
  }
}

// Обновляет волны, расширяя их и уменьшая яркость
void update_waves() {
  for (int i = 0; i < NUM_LEGS; i++) {
    for (int j = 0; j < wave_count[i]; j++) {
      fire_waves[i][j].radius++;
      fire_waves[i][j].intensity = max(1, fire_waves[i][j].intensity - 1);  // Яркость уменьшается
    }

    // Удаляем завершённые волны
    int new_wave_count = 0;
    for (int j = 0; j < wave_count[i]; j++) {
      if (fire_waves[i][j].radius <= fire_waves[i][j].max_radius) {
        fire_waves[i][new_wave_count++] = fire_waves[i][j];
      }
    }
    wave_count[i] = new_wave_count;
  }
}

// Применяет огонь ко всем ногам
void apply_fire() {
  for (int i = 0; i < NUM_LEGS; i++) {
    int fire_map[NUM_PIXELS] = {0};  // Очистка карты огня

    for (int j = 0; j < wave_count[i]; j++) {
      int center = fire_waves[i][j].center;
      int radius = fire_waves[i][j].radius;
      int intensity = fire_waves[i][j].intensity;

      for (int k = 0; k < NUM_PIXELS; k++) {
        int distance = abs(k - center);
        if (distance <= radius) {
          fire_map[k] = max(fire_map[k], intensity - (distance / 2));
        }
      }
    }

    // Обновляем цвета светодиодов
    for (int j = 0; j < NUM_PIXELS; j++) {
      int level = min(fire_map[j], 4);  // Ограничиваем уровни яркости
      legs[i].setPixelColor(j, FIRE_COLORS[level][0], FIRE_COLORS[level][1], FIRE_COLORS[level][2]);
    }

    legs[i].show();
  }
}

// Основной цикл эффекта
void fire_effect() {
  while (true) {
    for (int i = 0; i < NUM_LEGS; i++) {
      start_new_wave(i);  // Добавляем новые волны
    }
    update_waves();  // Обновляем все волны
    apply_fire();  // Применяем эффект ко всем ногам
    delay(WAVE_SPEED * 1000);  // Задержка (умножаем на 1000 для миллисекунд)
}

void setup() {
  for (int i = 0; i < NUM_LEGS; i++) {
    legs[i].begin();  // Инициализация каждого нео-пикселя
  }
  clear_pixels();  // Очищаем перед началом
}

void loop() {
  fire_effect();  // Запуск эффекта огня
}

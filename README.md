# Festival Beast, Lampu2025

## MacMini M4

Центральный и единственный комп. MacOS

К нему по usb через usb-hub подключены
— kinect v1 (libfreenect) для камеры, глубинной камеры и массива микрофонов
— 7’’ hdmi тач дисплей для глаз и отладочного режима
— колонка (какой?)
— esp управляющий моторами
— esp управляющими диодными лентами

### Протокол общения с ESP32
Esp32 подлюченн по usb и подключена Serial коммуникация (какая обычно используется для отладки Arduino).

Со стороны esp32 

```
void setup() {
  Serial.begin(115200);   // initialise serial monitor port
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }  
  Serial1.begin(115200);  // initialise Serial1
  Serial.println();
  Serial.write("Arduino MKRFOX Serial1 test -  for loopback test connect pin 13 to pin 14\n");
}

void loop() {
  if (Serial1.available())        // read from Serial1 output to Serial
    Serial.write(Serial1.read());
  if (Serial.available()) {       // read from Serial outut to Serial1
    int inByte = Serial.read();
    //Serial.write(inByte);     // local echo if required
    Serial1.write(inByte);
  }
}
```

Со стороны мака

```
import serial 
arduino = serial.Serial('COM8', baudrate=115200, timeout=1.0) 
while(True): 
line = arduino.readline().decode('utf-8') 
print(line)
```

### Python script на маке 

#### Ретрансляция сообщения

* По очереди обходит оба ком порта
* Считывает все сообщения с esp32, оставляет только последнее по топику и кладет в буфер
* Отправляет все сообщения из буфера (не посланные этим esp32)

Сообщения выглядят так

```<topic>: <val1> <val2> <val3>```

#### Детект лиц и отрисовка глаз

Глаза это html страница, глаза отрисованы в svg, анимируются css
https://youtu.be/UTHgr6NLeEw?si=fQLUZeEjket0Gn6v

Скрипт считывает видео фид с кинетка, детектит лица и смотрит в сторону лиц.


## ESP32 моторы и пульт
Автономен. Считывает значения с пульта и управляет моторами

Шлет два сообщения

### Input
Что введено с джойстика

```input: <throttle -128-128> <turn -128-128> <button1 0 отжата, 1 нажата> <b2> <b3> <b4>```

Например 

```input: 0 0 0 0 0 0 (ничего не нажато)```

```input: -128 0 0 0 0 0 (полный назад)```

### Move
Статус движения робота

```move: <двигается ли 0 нет, 1 вперед, -1 назад> <скорость 0..255> <угол поворота -128..128>```

Например

```move: 0 0 0 (стоим)```

```move: 1 255 0 (полный вперед прямо)```

```Move: 1 255 84 (вперед направо 30 градусов)```


## ESP32 диоды

Каждая полунога управляется своим пином. Ленты подключены так.

ESP отрисовывает разные анимации в зависимости от состояния робота.

![ленты](https://github.com/alarin/festivalbeast/blob/master/3dmodels/leg_led_schematics.png?raw=true)

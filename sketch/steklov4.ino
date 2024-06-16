#include <FastLED.h>
#include <avr/sleep.h>  // подключаем библиотеку sleep

// Параметры адресной ленты
#define LED_PIN     11
#define NUM_LEDS    1
#define BRIGHTNESS  150
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

const int analogPin = A0;  // Аналоговый пин, к которому подключены кнопки

const int relayPin1 = 10;   // Пин первого реле
const int relayPin2 = 9;   // Пин второго реле

const unsigned long holdThreshold = 400;  // Порог удержания в миллисекундах (0.3 секунды)

unsigned long pressStartTime1 = 0;  // Время начала нажатия кнопки 1
unsigned long pressStartTime2 = 0;  // Время начала нажатия кнопки 2

int auto1=0;
int auto2=0;
int block=0;

void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  Serial.begin(9600); // Инициализация последовательной связи для вывода данных
  pinMode(relayPin1, OUTPUT);  // Настройка пина первого реле на выход
  pinMode(relayPin2, OUTPUT);  // Настройка пина второго реле на выход
  pinMode(A2, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);

  digitalWrite(relayPin1, LOW); // Устанавливаем начальное состояние реле 1 (выключенное)
  digitalWrite(relayPin2, LOW); // Устанавливаем начальное состояние реле 2 (выключенное)


  //Serial.println("****"); 
}

void loop() {
  //Serial.println(digitalRead(2));
  if (digitalRead(3)==HIGH){
    block=0;
    unlock(); 
  }

  if (digitalRead(2)==HIGH){
    block=1;
    lock();
  }


  int buttonState = analogRead(analogPin);  // Считывание состояния кнопок (значение аналогового сигнала)
  int amp = analogRead(A1);
 Serial.println(amp); // Выводим значение аналогового пина в монитор порта

  if ((auto1==1) or (auto2==1)){
      if ((amp<390) or (amp>780)){
        Serial.println("stop");
        auto1=0;
        auto2=0;
        digitalWrite(relayPin2, LOW);
        digitalWrite(relayPin1, LOW);     
        if (block==1){
          //set_sleep_mode(SLEEP_MODE_IDLE);  //  SLEEP_MODE_PWR_DOWN
          //sleep_mode();
        }  
      }
  }
 
  // Обработка кнопки 1
  if ((buttonState >= 500 && buttonState <= 600)) { // Диапазон значений для кнопки 1
    if (pressStartTime1 == 0) {
      pressStartTime1 = millis();
    }
    if (millis() - pressStartTime1 >= holdThreshold) {
      auto1=0;
      auto2=0;
      digitalWrite(relayPin2, LOW);
      digitalWrite(relayPin1, HIGH);  // Включаем реле 1 по минусу
    } else {
      auto2=0;
      auto1=1;
      digitalWrite(relayPin1, LOW); // Выключаем реле 1, если нажатие было кратким
    }
  } else {
    pressStartTime1 = 0; // Сбрасываем время начала нажатия, если кнопка не нажата
    if (auto1==1){
    auto2=0;
    digitalWrite(relayPin2, LOW);
    digitalWrite(relayPin1, HIGH);
    } else {
    digitalWrite(relayPin1, LOW); // Выключаем реле 1, если кнопка не нажата
    }
  }

  // Обработка кнопки 2
  if ((buttonState >= 680 && buttonState <= 800)) { // Диапазон значений для кнопки 2
    if (pressStartTime2 == 0) {
      pressStartTime2 = millis();
    }
    if (millis() - pressStartTime2 >= holdThreshold) {
      auto1=0;
      auto2=0;
      digitalWrite(relayPin1, LOW);
      digitalWrite(relayPin2, HIGH);  // Включаем реле 2 по минусу
    } else {
      auto1=0;
      auto2=1;
      digitalWrite(relayPin2, LOW); // Выключаем реле 2, если нажатие было кратким
    }
  } else {
    pressStartTime2 = 0; // Сбрасываем время начала нажатия, если кнопка не нажата
    if (auto2==1){
    auto1=0;
    digitalWrite(relayPin1, LOW);
    digitalWrite(relayPin2, HIGH);
    } else {
    digitalWrite(relayPin2, LOW); // Выключаем реле 2, если кнопка не нажата
    }
  }

}

void lock() {
  leds[0] = CRGB::Green;  // зажигаем светодиод зелёным
  FastLED.show();
  delay(1000);             // держим цвет 500 мс
  leds[0] = CRGB::Black;  // выключаем светодиод
  FastLED.show();

  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, LOW);

  auto1=1;
  auto2=0;
  block=1;
}

void unlock() {
  leds[0] = CRGB::Red;    // зажигаем светодиод красным
  FastLED.show();
  delay(1000);             // держим цвет 500 мс
  leds[0] = CRGB::Black;  // выключаем светодиод
  FastLED.show();
  block=0;
}

const int analogPin = A0;  // Аналоговый пин, к которому подключены кнопки

const int relayPin1 = 10;   // Пин первого реле
const int relayPin2 = 9;   // Пин второго реле

const unsigned long holdThreshold = 300;  // Порог удержания в миллисекундах (0.3 секунды)

unsigned long pressStartTime1 = 0;  // Время начала нажатия кнопки 1
unsigned long pressStartTime2 = 0;  // Время начала нажатия кнопки 2

int auto1=0;
int auto2=0;

void setup() {
  Serial.begin(115200); // Инициализация последовательной связи для вывода данных
  pinMode(relayPin1, OUTPUT);  // Настройка пина первого реле на выход
  pinMode(relayPin2, OUTPUT);  // Настройка пина второго реле на выход

  digitalWrite(relayPin1, LOW); // Устанавливаем начальное состояние реле 1 (выключенное)
  digitalWrite(relayPin2, LOW); // Устанавливаем начальное состояние реле 2 (выключенное)

  //Serial.println("****"); 
}

void loop() {
  int buttonState = analogRead(analogPin);  // Считывание состояния кнопок (значение аналогового сигнала)
  Serial.println(buttonState); // Выводим значение аналогового пина в монитор порта

  
  // Обработка кнопки 1
  if (buttonState >= 500 && buttonState <= 600) { // Диапазон значений для кнопки 1
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
  if (buttonState >= 680 && buttonState <= 800) { // Диапазон значений для кнопки 2
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

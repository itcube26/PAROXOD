#include <GyverPing.h>
/*
ПЕРЕДНИЙ ДАТЧИК:
#define HC_TRIG 38
#define HC_ECHO 39

ЛЕВЫЙ ДАТЧИК:
#define HC_TRIG 31
#define HC_ECHO 32

ПРАВЫЙ ДАТЧИК:
#define HC_TRIG 23
#define HC_ECHO 22

*/



#define HC_TRIG 23
#define HC_ECHO 22

GPingSync sonar(HC_TRIG, HC_ECHO);

void setup() {
    Serial.begin(115200);
}

void loop() {
    // сделать измерение
    sonar.ping();

    // открой плоттер для вывода графика
    Serial.print(sonar.getRaw());       // "сырое" значение
    Serial.print(',');
    Serial.print(sonar.getFiltered());  // фильтрованное от резких выбросов
    Serial.print(',');
    Serial.println(sonar.getSmooth());  // + усреднение, плавный точный сигнал

    delay(50);
}
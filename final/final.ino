#include <Vex5.h>
#include <GPingSync.h>

// ==================== ПИНЫ ДАТЧИКОВ ====================
// ПЕРЕДНИЙ ДАТЧИК
#define TRIG_F 38
#define ECHO_F 39

// ЛЕВЫЙ ДАТЧИК
#define TRIG_L 31
#define ECHO_L 32

// ПРАВЫЙ ДАТЧИК
#define TRIG_R 23
#define ECHO_R 22

// ==================== ОБЪЕКТЫ ====================
Vex5_Motor motor1; // Левый мотор
Vex5_Motor motor6; // Правый мотор

GPingSync sonarF(TRIG_F, ECHO_F);
GPingSync sonarL(TRIG_L, ECHO_L);
GPingSync sonarR(TRIG_R, ECHO_R);

// ==================== НАСТРОЙКИ ПОВЕДЕНИЯ ====================
#define BASE_SPEED 150    // Базовая скорость движения вперед
#define TARGET_DIST 20    // Желаемое расстояние до правой стены (в см)
#define FRONT_OBSTACLE 30 // Если спереди препятствие ближе этого значения (в см)
#define KP 4.0            // Коэффициент плавности поворота

void setup() {
    Vex5.begin();
    motor1.begin(VEX5_PORT_1);
    motor6.begin(VEX5_PORT_6);
    
    Serial.begin(115200);
}

void loop() {
    // 1. Опрос всех датчиков
    sonarF.ping();
    sonarL.ping();
    sonarR.ping();

    // Получаем сглаженные значения
    int distF = sonarF.getSmooth();
    int distL = sonarL.getSmooth();
    int distR = sonarR.getSmooth();

    int speed1 = 0; // Скорость левого мотора
    int speed6 = 0; // Скорость правого мотора

    // 2. Логика управления
    // ПРИОРИТЕТ 1: Если спереди стена/препятствие — поворачиваем налево
    if (distF > 0 && distF < FRONT_OBSTACLE) {
        speed1 = -BASE_SPEED; // Левый назад
        speed6 = BASE_SPEED;  // Правый вперед
    } 
    // ПРИОРИТЕТ 2: Плавное следование за ПРАВОЙ стеной
    else {
        // Считаем ошибку: насколько реальное расстояние отличается от желаемого
        float error = TARGET_DIST - distR; 
        
        // П-регулятор: вычисляем поправку к скорости
        float correction = error * KP;

        // Корректируем скорости
        speed1 = BASE_SPEED + correction;
        speed6 = BASE_SPEED - correction;
    }

    // 3. Подача сигналов на моторы
    // motor1 инвертирован (крутится в противоположную сторону)
    motor1.setSpeed(-speed1);  // Инверсия для motor1
    motor6.setSpeed(-speed6);  // Инверсия для motor6

    // 4. Вывод данных в Serial Plotter
    Serial.print(distF); Serial.print(',');
    Serial.print(distL); Serial.print(',');
    Serial.print(distR); Serial.print(',');
    Serial.print(TARGET_DIST);
    Serial.print(',');
    Serial.println(speed1);

    delay(30);
}
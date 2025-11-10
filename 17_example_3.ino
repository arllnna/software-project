#include <Servo.h>

#define PIN_IR_SENSOR A0
#define PIN_SERVO    10
#define PIN_LED      9

#define INTERVAL 20
#define ALPHA 0.1
#define DUTY_MIN 550
#define DUTY_MAX 2400

Servo myservo;

unsigned long last_loop_time = 0;
float dist_ema = 0.0;

void setup() {
  myservo.attach(PIN_SERVO);
  myservo.writeMicroseconds(DUTY_MIN);

  pinMode(PIN_LED, OUTPUT);
  Serial.begin(1000000);
}

void loop() {
  unsigned long time_curr = millis();
  if (time_curr - last_loop_time < INTERVAL) return;
  last_loop_time = time_curr;

  int a_value = analogRead(PIN_IR_SENSOR);
  float dist_raw = (6762.0 / (a_value - 9.0) - 4.0) * 10.0 - 60.0;
  dist_ema = ALPHA * dist_raw + (1.0 - ALPHA) * dist_ema;

  if (dist_ema >= 100.0 && dist_ema <= 250.0) digitalWrite(PIN_LED, HIGH);
  else digitalWrite(PIN_LED, LOW);

  float angle;
  if (dist_ema < 100.0) angle = 0.0;
  else if (dist_ema > 250.0) angle = 180.0;
  else angle = (dist_ema - 100.0) / (250.0 - 100.0) * 180.0;

  int duty = DUTY_MIN + (DUTY_MAX - DUTY_MIN) * angle / 180.0;
  myservo.writeMicroseconds(duty);

  Serial.print(a_value); Serial.print(", ");
  Serial.print(dist_raw); Serial.print(", ");
  Serial.print(dist_ema); Serial.print(", ");
  Serial.println(duty);
}

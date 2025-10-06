// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

unsigned long last_sampling_time = 0;  // unit: msec
int currentBrightness = 255;
int targetBrightness = 255;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  Serial.begin(57600);
}

void loop() { 
  if (millis() < (last_sampling_time + INTERVAL)) return;
  last_sampling_time = millis();

  float distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance
  int brightness = 255;

  if (distance >= _DIST_MIN && distance <= _DIST_MAX) {
    float duty;

    if(distance <= 200.0) {
      duty = (distance - 100.0) / 100.0;
    } else {
      duty = (300.0 - distance) / 100.0;
    }

    duty = constrain(duty, 0.0, 1.0);
    targetBrightness = (int)(255.0 * (1.0 - duty));
  }

  currentBrightness = currentBrightness + (targetBrightness - currentBrightness) * 0.1;
  analogWrite(PIN_LED, currentBrightness);

  Serial.print("Distance: ");        
  Serial.print(distance);  
  Serial.print("mm, Brightness :");       
  Serial.println(currentBrightness);

  last_sampling_time += INTERVAL;
}
  
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);

  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE;
}

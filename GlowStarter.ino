/*
 * Glow Plug Starter
 *
 * Author: Gennady Rafalovich
 * Date: 13/12/2016
 * red led on - power ok
 *
 * red led blinking - low power
 * blue len on - out power in range
 * blue led blinking - glow connected
 * buzzer on continuesly - low power
 * buzzer on/off - timeout 30sec when glow connected, 5.5min when disconnected
 *
 */

#define DEBUG
#define RED_LED_PIN 4
#define BLUE_LED_PIN 5
#define BUZZER_PIN 6

#define INPUT_VOLT A0
#define GLOW_VOLT A1
#define OUT_VOLT A2

#define MAX_POWER_ON_TIME (5L * 60 * 1000) // 5min
#define MAX_GLOW_ON_TIME (30L * 1000) // 30sec

#define MIN_BAT_VOLTAGE 9.5
#define MIN_OUT_VOLTAGE 1.2
#define MAX_OUT_VOLTAGE 2.5

unsigned long maxPowerTimesMsec;
unsigned long maxGlowTimesMsec;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(RED_LED_PIN,  OUTPUT);
  pinMode(BLUE_LED_PIN,  OUTPUT);
  pinMode(BUZZER_PIN,  OUTPUT);
}

void loop() {
  maxGlowTimesMsec = 0;
  maxPowerTimesMsec = millis() + MAX_POWER_ON_TIME;
  Serial.print("Timeout:");
  Serial.println(maxPowerTimesMsec / 1000);
  while(1)
  {
    float inVolt = analogRead(INPUT_VOLT) * 0.0183;
    float outVolt = analogRead(OUT_VOLT) * 0.0048;
    float glowVolt = analogRead(GLOW_VOLT) * 0.0048;
#ifdef DEBUG
    Serial.print("InVolt:");
    Serial.println(inVolt);
    Serial.print("OutVolt:");
    Serial.println(outVolt);
    Serial.print("GlowVolt:");
    Serial.println(glowVolt);
    Serial.print("Timeout:");
    Serial.println(((maxPowerTimesMsec - millis())) / 1000);
#endif
    if((outVolt - glowVolt) > 0.3)
    {
      digitalWrite(BLUE_LED_PIN, 0);
      if(maxGlowTimesMsec == 0)
      { // init max glow time
        maxGlowTimesMsec = millis() + MAX_GLOW_ON_TIME;
        maxPowerTimesMsec = maxGlowTimesMsec + MAX_POWER_ON_TIME;
      }
    } else
    {
      maxGlowTimesMsec = 0;
    }
    if((inVolt < MIN_BAT_VOLTAGE)
      || ((maxGlowTimesMsec > 0) && (maxGlowTimesMsec < millis()))
        || (maxPowerTimesMsec < millis()))
    {
      digitalWrite(BUZZER_PIN, 1);
    }
    digitalWrite(RED_LED_PIN, 1);

    delay(500);

    if(inVolt < MIN_BAT_VOLTAGE)
    {
      digitalWrite(RED_LED_PIN, 0);
      digitalWrite(BUZZER_PIN, 1);
    } else
    {
      digitalWrite(RED_LED_PIN, 1);
      digitalWrite(BUZZER_PIN, 0);
    }
    digitalWrite(BLUE_LED_PIN, ((outVolt > MIN_OUT_VOLTAGE) && (MAX_OUT_VOLTAGE))?1:0);
    delay(500);
  }
}

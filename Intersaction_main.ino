#include "Intersaction_TrafficLight.h"

#define UART_BAUD_RATE  (115200)

#define TRAFFIC_LIGHT_ENABLED (FALSE)
TrafficLight *pTrafficLightsArray[15] = {NULL};

#define PIZEOELECTRIC_SENSOR_ENABLED (FALSE)
#define PIEZO_SENS_INPUT_PIN  (A0) // the analog pin connected to the sensor
#define PIEZO_THRESHOLD       (400)
#define PIEZO_READ_DELAY_MILI_SEC (100)
unsigned long m_piezoPreviousMillis = 0;

#define REED_SWITCH_ENABLED (TRUE)
#define REED_SWITCH_INPUT_PIN (8)
#define REED_SWITCH_STATE_POLLING_DELAY_MILI_SEC (100)
#define REED_SWITCH_OPEN    LOW
#define REED_SWITCH_CLOSED  HIGH
#define REED_SWITCH_MOVING_CAR_DEBOUNCE_THRESHOLD 4   // 4 * 100 mSec = 4cm/Sec
uint8_t m_reedSwitchMovingCarDebounceCunter = 0;
unsigned long m_reedSwitchPreviousMillis = 0;
bool m_reedSwitchState = false;

void setup() {
  Serial.begin(UART_BAUD_RATE); // open the serial:
  delay(1000);

  initTrafficLight();

  initReadSwitch();
}


void loop() {
  // updateTraffic();
  readFromPiezoSensor();
  isReedSwitchClosed();
}


uint16_t readFromPiezoSensor()
{
#if PIZEOELECTRIC_SENSOR_ENABLED
  unsigned long currentMillis = millis();
  unsigned long tmpVal = currentMillis - m_piezoPreviousMillis;

  if (tmpVal >= PIEZO_READ_DELAY_MILI_SEC) 
  {
    m_piezoPreviousMillis = currentMillis;

    int value = analogRead(PIEZO_SENS_INPUT_PIN); 
    Serial.println(value);
  }
#endif
}


void initReadSwitch()
{
// #if REED_SWITCH_ENABLED
  pinMode(REED_SWITCH_INPUT_PIN, INPUT);
// #endif
}

bool isReedSwitchClosed()
{
// #if REED_SWITCH_ENABLED
  unsigned long currentMillis = millis();
  unsigned long tmpVal = currentMillis - m_reedSwitchPreviousMillis;

  if (tmpVal >= REED_SWITCH_STATE_POLLING_DELAY_MILI_SEC)
  {
    m_reedSwitchPreviousMillis = currentMillis;

    int state = digitalRead(REED_SWITCH_INPUT_PIN);
    if(state == REED_SWITCH_CLOSED)
    {
      m_reedSwitchMovingCarDebounceCunter += 1;
      if(m_reedSwitchMovingCarDebounceCunter >= REED_SWITCH_MOVING_CAR_DEBOUNCE_THRESHOLD)
      {
        m_reedSwitchMovingCarDebounceCunter = 0;
        m_reedSwitchState = true;
      }
    }
    else
    {
      m_reedSwitchMovingCarDebounceCunter = 0;
      m_reedSwitchState = false;
    }
    Serial.println(m_reedSwitchState);
  }
// #endif
  return m_reedSwitchState;
}

void initTrafficLight()
{
#if TRAFFIC_LIGHT_ENABLED
  TrafficLight tf_0(2, 3, 4);
  tf_0.setTlGlobalState(GLOBAL_TL_STATE_RED);
  pTrafficLightsArray[0] = &tf_0;
#endif
}


void updateTraffic()
{
#if TRAFFIC_LIGHT_ENABLED
  pTrafficLightsArray[0]->trafficLightMainLoop();
  if(Serial.available())
  {
    // char input = Serial.read();
    String command = Serial.readStringUntil('\n');
    Serial.println(command);
    if(command.equals("r"))
    {
      Serial.println("Going to GLOBAL_TL_STATE_RED");
      pTrafficLightsArray[0]->setTlGlobalState(GLOBAL_TL_STATE_RED);
    }
    else if(command.equals("g"))
    {
      Serial.println("Going to GLOBAL_TL_STATE_GREEN");
      pTrafficLightsArray[0]->setTlGlobalState(GLOBAL_TL_STATE_GREEN);
    }
    else
    {
      Serial.println("Going to GLOBAL_TL_STATE_UNDEFINED");
      pTrafficLightsArray[0]->setTlGlobalState(GLOBAL_TL_STATE_UNDEFINED);
    }  
  }
#endif
}

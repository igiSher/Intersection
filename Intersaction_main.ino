#include "Intersaction_TrafficLight.h"

#define UART_BAUD_RATE  (115200)
TrafficLight *pTrafficLightsArray[15] = {NULL};

void setup() {
  Serial.begin(UART_BAUD_RATE); // open the serial:
  delay(1000);

  TrafficLight tf_0(4, 3, 2);
  tf_0.setTlGlobalState(GLOBAL_TL_STATE_RED);
  pTrafficLightsArray[0] = &tf_0;

  TrafficLight tf_1(8, 7, 6);
  tf_1.setTlGlobalState(GLOBAL_TL_STATE_UNDEFINED);
  pTrafficLightsArray[1] = &tf_1;

  TrafficLight tf_2(11, 10, 9);
  tf_2.setTlGlobalState(GLOBAL_TL_STATE_UNDEFINED);
  pTrafficLightsArray[2] = &tf_2;
}

void loop() {
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
}

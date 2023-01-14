#ifndef _INTERSACTION_TRAFFICLIGHT_H_
#define _INTERSACTION_TRAFFICLIGHT_H_

#include <Arduino.h>

enum eTrafficLightSM {
  TL_STATE_UNDEFINED,         // Blinking yellow
  TL_STATE_CONST_GREEN,       // Constant green
  TL_STATE_CONST_RED,
  // Transition to red
  TL_STATE_BLINKING_GREEN,    //
  TL_STATE_CONST_YELLOW,      //
  // Transition to green
  TL_STATE_CONST_RED_AND_YELLOW,

  TL_STATE_MAX = 0xFF
};

enum eTrafficLightPosition {
  TL_POSITION_UNDEFINED
};


class TrafficLight {
  public:
    TrafficLight();
    TrafficLight(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin);
    TrafficLight(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin,
                 eTrafficLightPosition pos, String tlName);

    ~TrafficLight();

    setTlState(eTrafficLightSM state);
    assignColorsToGpios(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin);
    setTlPosition(eTrafficLightPosition pos);
    setNameOfTl(String name);

  private:
    eTrafficLightSM m_state;
    eTrafficLightPosition m_position;
    String m_trafficLightName;

    uint8_t m_redLight;
    uint8_t m_greenLight;
    uint8_t m_yellowLight;
};

#endif

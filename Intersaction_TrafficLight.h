#ifndef _INTERSACTION_TRAFFICLIGHT_H_
#define _INTERSACTION_TRAFFICLIGHT_H_

#include <Arduino.h>

enum eGlobalTlState {
  GLOBAL_TL_STATE_UNDEFINED,    // Blinking yellow
  GLOBAL_TL_STATE_GREEN,
  GLOBAL_TL_STATE_RED
};
enum eTrafficLightPosition {
  TL_POSITION_UNDEFINED
};

#define BLINK_PERIOD_DEFAULT_MILI_SEC           (500)
#define TRANSITION_HOLD_TIME_DEFAULT_MILI_SEC   (5000)

class TrafficLight {
  public:
    TrafficLight();
    TrafficLight(uint8_t r_pin, uint8_t y_pin, uint8_t g_pin);
    TrafficLight(uint8_t r_pin,
                 uint8_t y_pin,
                 uint8_t g_pin,
                 eTrafficLightPosition pos,
                 String tlName);

    ~TrafficLight();

    // Setters
    void setTlGlobalState(eGlobalTlState state);
    void assignColorsToGpios(uint8_t r_pin, uint8_t y_pin, uint8_t g_pin);
    void setTlPosition(eTrafficLightPosition pos);
    void setNameOfTl(String name);
    void setBlinkPeriod(unsigned long periodInMiliSec);
    void setTransitionHoldTime(unsigned long transitionTimeInMiliSec);

    ////////
    // Main loop, will be called by outer loop to handle TrafficLight
    // internal buisness logic (i.e state Machine housekeeping)
    void trafficLightMainLoop();

  private:
    enum eTrafficLightInternalFSM
    {
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

    enum eLightsState
    {
      TL_LIGHT_OFF    = 0,
      TL_LIGHT_ON     = 1,
      TL_LIGHT_BLINK  = 2
    };

    void tlFsm();
    void tlCommonInit();

    void tlFsmConstGreen();
    void tlFsmConstRed();
    void tlFsmConstYellow();
    void tlFsmConstRedAndYellow();
    void tlFsmBlinkingGreen();
    void tlFsmBlinkingYellow();

    void tlSetLightsRYG(eLightsState r, eLightsState y, eLightsState g);

    eGlobalTlState m_tlFinalstate;                     // Traffic light final state
    eTrafficLightInternalFSM m_tlInternalFsmNextState; // Internal state machine state

    eTrafficLightPosition m_position;
    String m_trafficLightName;

    uint8_t m_redLight;
    uint8_t m_greenLight;
    uint8_t m_yellowLight;
    unsigned long m_blinkPeriodDelay;
    unsigned long m_transitionHoldTime;
    uint8_t m_holdTimeCounter;
    unsigned long m_previousMillis; //variable for measuring time
};

#endif

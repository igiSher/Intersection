#include "Intersaction_TrafficLight.h"

// Constructors and distructor
TrafficLight::TrafficLight() {
  setTlGlobalState(GLOBAL_TL_STATE_UNDEFINED);
  setTlPosition(TL_POSITION_UNDEFINED);
  m_tlInternalSM = TL_STATE_UNDEFINED;
  m_trafficLightName = "";
};

TrafficLight::TrafficLight(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin) {
  assignColorsToGpios(r_pin, g_pin, y_pin);
  setTlGlobalState(GLOBAL_TL_STATE_UNDEFINED);
  setTlPosition(TL_POSITION_UNDEFINED);
  m_tlInternalSM = TL_STATE_UNDEFINED;
  m_trafficLightName = "";
}

TrafficLight::TrafficLight(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin,
                           eTrafficLightPosition pos, String tlName) {
  setTlGlobalState(GLOBAL_TL_STATE_UNDEFINED);
  assignColorsToGpios(r_pin, g_pin, y_pin);
  setTlPosition(pos);
  m_tlInternalSM = TL_STATE_UNDEFINED;
  m_trafficLightName = tlName;
}

TrafficLight::~TrafficLight() {

}

TrafficLight::trafficLightMainLoop() {
  int state = 1;
  switch(state)
  {
    case 1:
      break;
      
    default:
      break;
  }
}


// Seeters and getters
TrafficLight::setTlGlobalState(eGlobalTlState state) {
  m_tlFinalstate = state;
}

TrafficLight::assignColorsToGpios(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin) {
  m_redLight = r_pin;
  m_greenLight = g_pin;
  m_yellowLight = y_pin;
  pinMode(m_redLight, OUTPUT);
  pinMode(m_greenLight, OUTPUT);
  pinMode(m_yellowLight, OUTPUT);
}

TrafficLight::setNameOfTl(String name) {
  m_trafficLightName = name;
}

TrafficLight::setTlPosition(eTrafficLightPosition pos) {
  m_position = pos;
}

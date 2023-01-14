#include "Intersaction_TrafficLight.h"

TrafficLight::TrafficLight() {
  setTlGlobalState(GLOBAL_TF_STATE_UNDEFINED);
  setTlInternalState(TL_STATE_UNDEFINED);
  setTlPosition(TL_POSITION_UNDEFINED);
  m_trafficLightName = "";
};

TrafficLight::TrafficLight(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin) {
  assignColorsToGpios(r_pin, g_pin, y_pin);
  setTlGlobalState(GLOBAL_TF_STATE_UNDEFINED);
  setTlInternalState(TL_STATE_UNDEFINED);
  setTlPosition(TL_POSITION_UNDEFINED);
  m_trafficLightName = "";
}

TrafficLight::TrafficLight(uint8_t r_pin, uint8_t g_pin, uint8_t y_pin,
                           eTrafficLightPosition pos, String tlName) {
  setTlGlobalState(GLOBAL_TF_STATE_UNDEFINED);
  setTlInternalState(TL_STATE_UNDEFINED);
  assignColorsToGpios(r_pin, g_pin, y_pin);
  setTlPosition(pos);
  m_trafficLightName = tlName;
}

TrafficLight::~TrafficLight() {

}

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

TrafficLight::setTlInternalState(eTrafficLightInternalSM internalState) {
  m_tlInternalSM = internalState;
}

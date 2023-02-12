#include "Intersaction_TrafficLight.h"

///////////////////////////////////////////////////////////////////////////////
// Public methods
////

// Constructors and distructor
TrafficLight::TrafficLight()
{
  tlCommonInit();
  setTlPosition(TL_POSITION_UNDEFINED);
  m_trafficLightName = "";
};

TrafficLight::TrafficLight(uint8_t r_pin, uint8_t y_pin, uint8_t g_pin)
{
  tlCommonInit();
  assignColorsToGpios(r_pin, y_pin, g_pin);
  setTlPosition(TL_POSITION_UNDEFINED);
  m_trafficLightName = "";
}

TrafficLight::TrafficLight(uint8_t r_pin,
                           uint8_t y_pin,
                           uint8_t g_pin,
                           eTrafficLightPosition pos,
                           String tlName)
{
  tlCommonInit();
  assignColorsToGpios(r_pin, y_pin, g_pin);
  setTlPosition(pos);
  m_trafficLightName = tlName;
}

TrafficLight::~TrafficLight() {

}

// Traffic light main loop
// This method used as a clock input for the traffic light FSM
void TrafficLight::trafficLightMainLoop()
{
  // Basic unit for FSM transition (and blink as well) is the 
  // BLINK_PERIOD_DEFAULT_MILI_SEC
  unsigned long currentMillis = millis();
  unsigned long tmpVal = currentMillis - m_previousMillis;

  if (tmpVal >= BLINK_PERIOD_DEFAULT_MILI_SEC) {
    m_previousMillis = currentMillis;

    switch(m_tlFinalstate)
    {
      case GLOBAL_TL_STATE_GREEN:
        if(m_tlInternalFsmNextState == TL_STATE_CONST_RED)
        {
          // To avoid changing internal state during transition, set 
          // change to next state once traffic light reached CONST RED
          m_tlInternalFsmNextState = TL_STATE_CONST_RED_AND_YELLOW;
        }
        break;

      case GLOBAL_TL_STATE_RED:
        // If internalState is Undefined, switch immediately to RED
        if (m_tlInternalFsmNextState == TL_STATE_UNDEFINED)
        {
          m_tlInternalFsmNextState = TL_STATE_CONST_RED;
        }
        else if(m_tlInternalFsmNextState == TL_STATE_CONST_GREEN)
        {
          // To avoid changing internal state during transition,
          // change to the next state once traffic light reached CONST GREEN
          m_tlInternalFsmNextState = TL_STATE_BLINKING_GREEN;
        }
        break;
      
      case GLOBAL_TL_STATE_UNDEFINED:
      default:
        // In both these cases immediately switch
        // the internal FSM to TL_STATE_UNDEFINED
        m_tlInternalFsmNextState = TL_STATE_UNDEFINED;
        break;
    }
    
    // Invoke internal FSM
    tlFsm();
  }
}

// Setters and getters
void TrafficLight::setTlGlobalState(eGlobalTlState state) {
  m_tlFinalstate = state;
}

void TrafficLight::assignColorsToGpios(uint8_t r_pin, uint8_t y_pin, uint8_t g_pin) {
  m_redLight = r_pin;
  m_yellowLight = y_pin;
  m_greenLight = g_pin;

  pinMode(m_redLight, OUTPUT);
  pinMode(m_yellowLight, OUTPUT);
  pinMode(m_greenLight, OUTPUT);
}

void TrafficLight::setNameOfTl(String name) {
  m_trafficLightName = name;
}

void TrafficLight::setTlPosition(eTrafficLightPosition pos) {
  m_position = pos;
}

void TrafficLight::setBlinkPeriod(unsigned long periodInMiliSec)
{
  m_blinkPeriodDelay = periodInMiliSec;
}

///////////////////////////////////////////////////////////////////////////////
// Private methods
////

// Traffic ligh FSM
// This method implements a traffic light final state machine
void TrafficLight::tlFsm()
{
  Serial.print("TF_FSM = " );
  Serial.println(m_tlInternalFsmNextState);

  switch(m_tlInternalFsmNextState)
  {
    case TL_STATE_CONST_GREEN:
      tlFsmConstGreen();
      break;

    case TL_STATE_CONST_RED:
      tlFsmConstRed();
      break;

    case TL_STATE_CONST_YELLOW:
      tlFsmConstYellow();
      break;

    case TL_STATE_BLINKING_GREEN:
      // Transition to red
      tlFsmBlinkingGreen();
      break;

    case TL_STATE_CONST_RED_AND_YELLOW:
      // Transition to green
      tlFsmConstRedAndYellow();
      break;

    case TL_STATE_UNDEFINED:
    default:
      // Undefined = Blinking yellow
      tlFsmBlinkingYellow();
      break;
  }
}

//-------------------------------------
void TrafficLight::tlFsmConstGreen()
{
  m_tlInternalFsmNextState = TL_STATE_CONST_GREEN;
  tlSetLightsRYG(TL_LIGHT_OFF, TL_LIGHT_OFF, TL_LIGHT_ON);
}

//-------------------------------------
void TrafficLight::tlFsmConstRed()
{
  m_tlInternalFsmNextState = TL_STATE_CONST_RED;
  tlSetLightsRYG(TL_LIGHT_ON, TL_LIGHT_OFF, TL_LIGHT_OFF);
}

//-------------------------------------
void TrafficLight::tlFsmConstYellow()
{
  m_tlInternalFsmNextState = TL_STATE_CONST_YELLOW;
  tlSetLightsRYG(TL_LIGHT_OFF, TL_LIGHT_ON, TL_LIGHT_OFF);
}

//-------------------------------------
void TrafficLight::tlFsmConstRedAndYellow()
{
  if(m_holdTimeCounter < m_transitionHoldTime)
  {
    m_tlInternalFsmNextState = TL_STATE_CONST_RED_AND_YELLOW;
    m_holdTimeCounter += m_blinkPeriodDelay;
  }
  else
  {
    m_tlInternalFsmNextState = TL_STATE_CONST_GREEN;
    m_holdTimeCounter = 0;
  }
  tlSetLightsRYG(TL_LIGHT_ON, TL_LIGHT_ON, TL_LIGHT_OFF);
}

//-------------------------------------
void TrafficLight::tlFsmBlinkingGreen()
{
  if(m_holdTimeCounter < m_transitionHoldTime)
  {
    m_tlInternalFsmNextState = TL_STATE_BLINKING_GREEN;
    m_holdTimeCounter += m_blinkPeriodDelay;
  }
  else
  {
    m_tlInternalFsmNextState = TL_STATE_CONST_YELLOW;
    m_holdTimeCounter = 0;
  }
  tlSetLightsRYG(TL_LIGHT_OFF, TL_LIGHT_OFF, TL_LIGHT_BLINK);
}

//-------------------------------------
void TrafficLight::tlFsmBlinkingYellow()
{
  m_tlInternalFsmNextState = TL_STATE_UNDEFINED;
  tlSetLightsRYG(TL_LIGHT_OFF, TL_LIGHT_BLINK, TL_LIGHT_ON);
}

//-------------------------------------
void TrafficLight::tlSetLightsRYG(eLightsState r,
                                  eLightsState y,
                                  eLightsState g)
{

  // Serial.println(r);
  // Serial.println(y);
  // Serial.println(g);
  int pinVal;
  int pinsState[3] = {r, y, g};
  uint8_t pinsNum[3] = {m_redLight, m_yellowLight, m_greenLight};

  // Iterate thorugh the lights and set pin state
  for (int i=0; i<2; i++)
  {
    pinVal = pinsState[i];
    if(pinVal == TL_LIGHT_BLINK)
    {
      pinVal = !digitalRead(pinsNum[i]);
    }
    digitalWrite(pinsNum[i], pinVal);
  }
}

// This method holds common initialization
void TrafficLight::tlCommonInit()
{
  setTlGlobalState(GLOBAL_TL_STATE_UNDEFINED);
  m_tlInternalFsmNextState = TL_STATE_UNDEFINED;
  m_blinkPeriodDelay = BLINK_PERIOD_DEFAULT_MILI_SEC;
  m_transitionHoldTime = TRANSITION_HOLD_TIME_DEFAULT_MILI_SEC;
  m_holdTimeCounter = 0;
  m_previousMillis = 0;
}


//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "pwm.h"
#include <sam.h>

//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------

#define LED_R0 (PORT_PA15)
#define LED_R0_GROUP (0)
#define LED_R0_PIN (PIN_PA15%32)

#define LED_G0 (PORT_PA20)
#define LED_G0_GROUP (0)
#define LED_G0_PIN (PIN_PA20%32)

#define LED_B0 (PORT_PA07)
#define LED_B0_GROUP (0)
#define LED_B0_PIN (PIN_PA07%32)

//------------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//------------------------------------------------------------------------------

//==============================================================================
void pwm_init()
{
  // Enable Multiplexing
  PORT->Group[LED_R0_GROUP].PINCFG->bit.PMUXEN = 1;
  PORT->Group[LED_G0_GROUP].PINCFG->bit.PMUXEN = 1;
  PORT->Group[LED_B0_GROUP].PINCFG->bit.PMUXEN = 1;
  
  // Configure Multiplexing
  // R0 and G0 use Peripherial Function F (TCC0)
  // B0 uses Peripherial Function E 
  PORT->Group[LED_R0_GROUP].PMUX->bit.PMUXO = 0x5;
  PORT->Group[LED_G0_GROUP].PMUX->bit.PMUXO = 0x5;
  PORT->Group[LED_B0_GROUP].PMUX->bit.PMUXO = 0x4;
  
  
  
  PORT->Group[LED_R0_GROUP].DIRSET.reg = LED_R0;
  PORT->Group[LED_G0_GROUP].DIRSET.reg = LED_G0;
  PORT->Group[LED_B0_GROUP].DIRSET.reg = LED_B0;
  
  PORT->Group[LED_R0_GROUP].OUTCLR.reg = LED_R0;
  PORT->Group[LED_G0_GROUP].OUTSET.reg = LED_G0;
  PORT->Group[LED_B0_GROUP].OUTSET.reg = LED_B0;
}

void pwm_r0_set(uint8_t hue)
{
  
}

void pwm_g0_set(uint8_t hue)
{
  
}

void pwm_b0_set(uint8_t hue)
{
  
}

//------------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//      __                  __        __        __
//     /  `  /\  |    |    |__)  /\  /  ` |__/ /__`
//     \__, /~~\ |___ |___ |__) /~~\ \__, |  \ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//        __   __  , __
//     | /__` |__)  /__`   
//     | .__/ |  \  .__/
//
//------------------------------------------------------------------------------
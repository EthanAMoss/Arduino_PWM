//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "pwm.h"
#include <sam.h>
#include <component/pm.h>

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


// Constants for Clock multiplexers
#define GENERIC_CLOCK_MULTIPLEXER_TCC0_TCC1 (0x1A)

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
  PORT->Group[LED_R0_GROUP].PINCFG[LED_R0_PIN].bit.PMUXEN = 1;
  PORT->Group[LED_G0_GROUP].PINCFG[LED_G0_PIN].bit.PMUXEN = 1;
  PORT->Group[LED_B0_GROUP].PINCFG[LED_B0_PIN].bit.PMUXEN = 1;
  
  // Configure Multiplexing
  // R0 and G0 use Peripherial Function F (TCC0)
  // B0 uses Peripherial Function E 
  PORT->Group[LED_R0_GROUP].PMUX[LED_R0_PIN / 2].bit.PMUXO = 0x5;
  PORT->Group[LED_G0_GROUP].PMUX[LED_G0_PIN / 2].bit.PMUXE = 0x5;
  PORT->Group[LED_B0_GROUP].PMUX[LED_B0_PIN / 2].bit.PMUXO = 0x4;

  // Page 656:
  // Before the TCC is enabled, it must be configured as outlined by the following steps:
  //   Enable the TCC bus clock (CLK_TCCx_APB) first
  PM->APBCMASK.bit.TCC0_ = 1;
  
  /* ----------------------------------------------------------------------------------------------
   * Put Generic Clock Generator 1 as source for Generic Clock Multiplexer 0x1A (TCC0 and TCC1)
   */
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID( GENERIC_CLOCK_MULTIPLEXER_TCC0_TCC1 ) | // Generic Clock Multiplexer 0
                      GCLK_CLKCTRL_GEN_GCLK1 | // Generic Clock Generator 1 is source
                      GCLK_CLKCTRL_CLKEN ;
  
  //   If Capture mode is required, enable the channel in capture mode by writing a one to Capture Enable bit in Control A register (CTRLA.CAPTEN)

  // Optionally, the following configurations can be set before or after enabling TCC:
  
  
  // Select PRESCALER setting in the Control A register (CTRLA.PRESCALER)
  TCC0->CTRLA.bit.PRESCALER   = 0;        
  
  //  Select Prescaler Synchronization setting in Control A register (CTRLA.PRESCSYNC)
  TCC0->CTRLA.bit.PRESCSYNC   = 0;
  
  TCC0->CTRLA.bit.RESOLUTION  = 0;        // No dithering, please!
  
  //  If down-counting operation must be enabled, write a one to the Counter Direction bit in the Control B Set register (CTRLBSET.DIR)
  TCC0->CTRLBSET.bit.DIR      = 0;
  while ( TCC0->SYNCBUSY.bit.CTRLB )
  {
    ;
  }
  
  //   Select the Waveform Generation operation in WAVE register (WAVE.WAVEGEN)
  TCC0->WAVE.bit.WAVEGEN      = 0x2;      // Normal PWM mode
  while ( TCC0->SYNCBUSY.bit.WAVE )
  {
    ;
  }
  
  //   Select the Waveform Output Polarity in the WAVE register (WAVE.POL)
  TCC0->WAVE.vec.POL          = 0;
  while ( TCC0->SYNCBUSY.bit.WAVE )
  {
    ;
  }
  
  //   The waveform output can be inverted for the individual channels using the Waveform Output Invert Enable bit group in the Driver register (DRVCTRL.INVEN)
  TCC0->DRVCTRL.vec.INVEN     = 0;        // No inversion
  TCC0->PER.reg               = 0x0000FF; // TOP is 255
  while ( TCC0->SYNCBUSY.bit.PER )
  {
    ;
  }
  
  TCC0->WEXCTRL.bit.OTMX      = 0x1;      // Alternate CC0 and CC1 for all 8 outputs
  TCC0->CC[0].bit.CC          = 0;
  while ( TCC0->SYNCBUSY.bit.CC0 )
  {
    ;
  }
  TCC0->CC[1].bit.CC          = 0;
  while ( TCC0->SYNCBUSY.bit.CC1 )
  {
    ;
  }
  
  // The TCC is enabled by writing a one to the Enable bit in the Control A register (CTRLA.ENABLE).
  TCC0->CTRLA.bit.ENABLE      = 1;
    
  // Page 659:
  // The compare channels can be used for waveform generation on output port pins. To make the waveform visible on the connected pin, the following requirements must be fulfilled:
  //   Choose a waveform generation mode in the Waveform Control register (WAVE.WAVEGEN)
  //   Optionally invert the waveform output WO[x] by writing the corresponding Waveform Output Invert Enable bit in the Driver Control register (DRVCTRL.INVENx)
  //   Configure the PORT module to enable the peripheral function on the pin

  // Page 660:
  // For single-slope PWM generation, the period time is controlled by PER, while CCx control the duty cycle of the generated waveform output. When up-counting, WO[x] is set at start or compare match between the COUNT and TOP values, and cleared on compare match between COUNT and CCx register values.
}

void pwm_r0_set(uint8_t hue)
{
  TCC0->CC[1].bit.CC = hue;
  while ( TCC0->SYNCBUSY.bit.CC1 )
  {
    ;
  }
}

void pwm_g0_set(uint8_t hue)
{
  TCC0->CC[0].bit.CC = hue;
  while ( TCC0->SYNCBUSY.bit.CC0 )
  {
    ;
  }
}

void pwm_b0_set(uint8_t hue)
{
  ;
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

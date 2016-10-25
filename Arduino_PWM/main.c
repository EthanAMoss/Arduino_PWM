//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------

#include "sam.h"
#include "pwm.h"

//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//-----------------------------------------------------------------------------

/*
infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0484c/CHDCICDF.html
//void DelayCycles(volatile U32 n){while(n--);}
00000320 <DelayCycles>: //uses 8 cycles on each loop iteration (on ARM Cortex M0+)
320: b082 sub sp, #8
322: 9001 str r0, [sp, #4]
324: 9b01 ldr r3, [sp, #4] 2 cycles (loop start)
326: 1e5a subs r2, r3, #1 1 cycle
328: 9201 str r2, [sp, #4] 2 cycles
32a: 2b00 cmp r3, #0 1 cycles
32c: d1fa bne.n 324 <DelayCycles+0x4> 2 cycles (branch to start)
32e: b002 add sp, #8
330: 4770 bx lr
*/
#define CYCLES_IN_DLYTICKS_FUNC (8)
#define F_CPU (48000000UL)
#define MS_TO_DLYTICKS(ms) (uint32_t)(F_CPU / 1000 * ms / CYCLES_IN_DLYTICKS_FUNC)
// ((float)(F_CPU)) / 1000.0
#define DelayTicks(ticks) {volatile uint32_t n=ticks; while(n--);}//takes 8 cycles
#define DelayMs(ms) DelayTicks(MS_TO_DLYTICKS(ms))//uses 20bytes

// for fading the lights
#define LED_R0 (0)
#define LED_G0 (1)
#define LED_B0 (2)

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//
//-----------------------------------------------------------------------------

// I know this global is ugly, but we'll use it for now. Eventually, instead of
// calling DelayMs(delay), we will call a delay method which uses the timer and
// the value from the adc.
 static uint16_t fade_delay = 5; // delay per step 

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//-----------------------------------------------------------------------------

void fade_in_led(uint8_t led);
void fade_out_led(uint8_t led);

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------

//=============================================================================
int main(void)
{
  /* Initialize the SAM system */
  SystemInit();
  pwm_init();

  // note: I use 0-50% duty cycle as the range for LEDs here, just so they're
  // not so bright

  pwm_r0_set(LED_HALF_BRIGHTNESS); // start the cycle with red

  while (1) 
  {
    fade_in_led(LED_G0);  // transition to yellow
    fade_out_led(LED_R0); // transition to green
    fade_in_led(LED_B0);  // transition to cyan
    fade_out_led(LED_G0); // transition to blue
    fade_in_led(LED_R0);  // transition to magenta
    fade_out_led(LED_B0); // transition to red
  }
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------

//==============================================================================
// led is either 0 (G0), 1 (R0), or 2 (B0)
//==============================================================================
void fade_in_led(uint8_t led)
{
  int current_val = LED_OFF;
  
  // change the first value to LED_FULL_BRIGHTNESS to get the full range
  while (LED_HALF_BRIGHTNESS > current_val)
  {
    DelayMs(fade_delay);
    current_val++;
    
    if (LED_R0 == led)
    {
      pwm_r0_set(current_val);
    }
    else if (LED_G0 == led)
    {
      pwm_g0_set(current_val);
    }
    else
    {
      pwm_b0_set(current_val);
    }
  }
}

//==============================================================================
// led is either 0 (G0), 1 (R0), or 2 (B0)
//==============================================================================
void fade_out_led(uint8_t led)
{
  // change to LED_FULL_BRIGHTNESS if using the full range of brightness
  int current_val = LED_HALF_BRIGHTNESS;
  
  while (LED_OFF < current_val)
  {
    DelayMs(fade_delay);
    current_val--;
    
    if (LED_R0 == led)
    {
      pwm_r0_set(current_val);
    }
    else if (LED_G0 == led)
    {
      pwm_g0_set(current_val);
    }
    else
    {
      pwm_b0_set(current_val);
    }
  }
}

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//
//-----------------------------------------------------------------------------

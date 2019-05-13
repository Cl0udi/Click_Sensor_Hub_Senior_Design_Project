/*

  This file is part of mikroSDK.

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

----------------------------------------------------------------------------- */

#include "__t_PIC32.h"

const uint8_t _MIKROBUS_ERR_PIN         = 1;

const uint8_t _GPIO_OUTPUT              = 0;
const uint8_t _GPIO_INPUT               = 1;
                
const uint8_t _MIKROBUS_AN_PIN          = 0;
const uint8_t _MIKROBUS_RST_PIN         = 1;
const uint8_t _MIKROBUS_CS_PIN          = 2;
const uint8_t _MIKROBUS_SCK_PIN         = 3;
const uint8_t _MIKROBUS_MISO_PIN        = 4;
const uint8_t _MIKROBUS_MOSI_PIN        = 5;
const uint8_t _MIKROBUS_PWM_PIN         = 6;
const uint8_t _MIKROBUS_INT_PIN         = 7;
const uint8_t _MIKROBUS_RX_PIN          = 8;
const uint8_t _MIKROBUS_TX_PIN          = 9;
const uint8_t _MIKROBUS_SCL_PIN         = 10;
const uint8_t _MIKROBUS_SDA_PIN         = 11;

static uint8_t _getAN_1  ()       { return RB3_bit ; }
static uint8_t _getRST_1 ()       { return RD7_bit ; }
static uint8_t _getCS_1  ()       { return RG9_bit ; }
static uint8_t _getSCK_1 ()       { return RG6_bit ; }
static uint8_t _getMISO_1()       { return RG7_bit ; }
static uint8_t _getMOSI_1()       { return RG8_bit ; }
static uint8_t _getPWM_1 ()       { return RD0_bit ; }
static uint8_t _getINT_1 ()       { return RD11_bit; }
static uint8_t _getRX_1  ()       { return RB8_bit ; }
static uint8_t _getTX_1  ()       { return RB14_bit; }
static uint8_t _getSCL_1 ()       { return RF5_bit ; }
static uint8_t _getSDA_1 ()       { return RF4_bit ; }
static void _setAN_1  (uint8_t value)         { LATB3_bit  = value; }
static void _setRST_1 (uint8_t value)         { LATD7_bit  = value; }
static void _setCS_1  (uint8_t value)         { LATG9_bit  = value; }
static void _setSCK_1 (uint8_t value)         { LATG6_bit  = value; }
static void _setMISO_1(uint8_t value)         { LATG7_bit  = value; }
static void _setMOSI_1(uint8_t value)         { LATG8_bit  = value; }
static void _setPWM_1 (uint8_t value)         { LATD0_bit  = value; }
static void _setINT_1 (uint8_t value)         { LATD11_bit = value; }
static void _setRX_1  (uint8_t value)         { LATB8_bit  = value; }
static void _setTX_1  (uint8_t value)         { LATB14_bit = value; }
static void _setSCL_1 (uint8_t value)         { LATF5_bit  = value; }
static void _setSDA_1 (uint8_t value)         { LATF4_bit  = value; }

static T_mikrobus_ret _gpioInit_1(T_mikrobus_pin pin, T_gpio_dir dir)
{
    switch( pin ) 
    {
        case _MIKROBUS_AN_PIN    : TRISB3_bit   = dir; break;
        case _MIKROBUS_RST_PIN   : TRISD7_bit   = dir; break;
        case _MIKROBUS_CS_PIN    : TRISG9_bit   = dir; break;
        case _MIKROBUS_SCK_PIN   : TRISG6_bit   = dir; break;
        case _MIKROBUS_MISO_PIN  : TRISG7_bit   = dir; break;
        case _MIKROBUS_MOSI_PIN  : TRISG8_bit   = dir; break;
        case _MIKROBUS_PWM_PIN   : TRISD0_bit   = dir; break;
        case _MIKROBUS_INT_PIN   : TRISD11_bit  = dir; break;
        case _MIKROBUS_RX_PIN    : TRISB8_bit   = dir; break;
        case _MIKROBUS_TX_PIN    : TRISB14_bit  = dir; break;
        case _MIKROBUS_SCL_PIN   : TRISF5_bit   = dir; break;
        case _MIKROBUS_SDA_PIN   : TRISF4_bit   = dir; break;
        default                  : return _MIKROBUS_ERR_PIN;
    }
    return _MIKROBUS_OK;
}



/* -------------------------------------------------------------------------- */
/*

  Copyright (c) 2017, MikroElektonika - http://www.mikroe.com

  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

3. All advertising materials mentioning features or use of this software
   must display the following acknowledgement:
   This product includes software developed by the MikroElektonika.

4. Neither the name of the MikroElektonika nor the
   names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY MIKROELEKTRONIKA ''AS IS'' AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL MIKROELEKTRONIKA BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------------- */
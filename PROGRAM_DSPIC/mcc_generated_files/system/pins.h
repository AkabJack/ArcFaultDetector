/**
 * PINS Generated Driver Header File 
 * 
 * @file      pins.h
 *            
 * @defgroup  pinsdriver Pins Driver
 *            
 * @brief     The Pin Driver directs the operation and function of 
 *            the selected device pins using dsPIC MCUs.
 *
 * @skipline @version   PLIB Version 1.0.1
 *
 * @skipline  Device : dsPIC33AK128MC102
*/

/*
© [2025] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H
// Section: Includes
#include <xc.h>

// Section: Device Pin Macros

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB0 GPIO Pin which has a custom name of IO_RB0 to High
 * @pre      The RB0 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RB0_SetHigh()          (_LATB0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB0 GPIO Pin which has a custom name of IO_RB0 to Low
 * @pre      The RB0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB0_SetLow()           (_LATB0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB0 GPIO Pin which has a custom name of IO_RB0
 * @pre      The RB0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB0_Toggle()           (_LATB0 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB0 GPIO Pin which has a custom name of IO_RB0
 * @param    none
 * @return   none  
 */
#define IO_RB0_GetValue()         _RB0

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB0 GPIO Pin which has a custom name of IO_RB0 as Input
 * @param    none
 * @return   none  
 */
#define IO_RB0_SetDigitalInput()  (_TRISB0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB0 GPIO Pin which has a custom name of IO_RB0 as Output
 * @param    none
 * @return   none  
 */
#define IO_RB0_SetDigitalOutput() (_TRISB0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB2 GPIO Pin which has a custom name of IO_RB2 to High
 * @pre      The RB2 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RB2_SetHigh()          (_LATB2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB2 GPIO Pin which has a custom name of IO_RB2 to Low
 * @pre      The RB2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB2_SetLow()           (_LATB2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB2 GPIO Pin which has a custom name of IO_RB2
 * @pre      The RB2 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB2_Toggle()           (_LATB2 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB2 GPIO Pin which has a custom name of IO_RB2
 * @param    none
 * @return   none  
 */
#define IO_RB2_GetValue()         _RB2

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB2 GPIO Pin which has a custom name of IO_RB2 as Input
 * @param    none
 * @return   none  
 */
#define IO_RB2_SetDigitalInput()  (_TRISB2 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB2 GPIO Pin which has a custom name of IO_RB2 as Output
 * @param    none
 * @return   none  
 */
#define IO_RB2_SetDigitalOutput() (_TRISB2 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB3 GPIO Pin which has a custom name of IO_RB3 to High
 * @pre      The RB3 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RB3_SetHigh()          (_LATB3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB3 GPIO Pin which has a custom name of IO_RB3 to Low
 * @pre      The RB3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB3_SetLow()           (_LATB3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB3 GPIO Pin which has a custom name of IO_RB3
 * @pre      The RB3 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB3_Toggle()           (_LATB3 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB3 GPIO Pin which has a custom name of IO_RB3
 * @param    none
 * @return   none  
 */
#define IO_RB3_GetValue()         _RB3

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB3 GPIO Pin which has a custom name of IO_RB3 as Input
 * @param    none
 * @return   none  
 */
#define IO_RB3_SetDigitalInput()  (_TRISB3 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB3 GPIO Pin which has a custom name of IO_RB3 as Output
 * @param    none
 * @return   none  
 */
#define IO_RB3_SetDigitalOutput() (_TRISB3 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB4 GPIO Pin which has a custom name of IO_RB4 to High
 * @pre      The RB4 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RB4_SetHigh()          (_LATB4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RB4 GPIO Pin which has a custom name of IO_RB4 to Low
 * @pre      The RB4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB4_SetLow()           (_LATB4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RB4 GPIO Pin which has a custom name of IO_RB4
 * @pre      The RB4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RB4_Toggle()           (_LATB4 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RB4 GPIO Pin which has a custom name of IO_RB4
 * @param    none
 * @return   none  
 */
#define IO_RB4_GetValue()         _RB4

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB4 GPIO Pin which has a custom name of IO_RB4 as Input
 * @param    none
 * @return   none  
 */
#define IO_RB4_SetDigitalInput()  (_TRISB4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RB4 GPIO Pin which has a custom name of IO_RB4 as Output
 * @param    none
 * @return   none  
 */
#define IO_RB4_SetDigitalOutput() (_TRISB4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC0 GPIO Pin which has a custom name of IO_RC0 to High
 * @pre      The RC0 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RC0_SetHigh()          (_LATC0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC0 GPIO Pin which has a custom name of IO_RC0 to Low
 * @pre      The RC0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RC0_SetLow()           (_LATC0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC0 GPIO Pin which has a custom name of IO_RC0
 * @pre      The RC0 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RC0_Toggle()           (_LATC0 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC0 GPIO Pin which has a custom name of IO_RC0
 * @param    none
 * @return   none  
 */
#define IO_RC0_GetValue()         _RC0

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC0 GPIO Pin which has a custom name of IO_RC0 as Input
 * @param    none
 * @return   none  
 */
#define IO_RC0_SetDigitalInput()  (_TRISC0 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC0 GPIO Pin which has a custom name of IO_RC0 as Output
 * @param    none
 * @return   none  
 */
#define IO_RC0_SetDigitalOutput() (_TRISC0 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC1 GPIO Pin which has a custom name of IO_RC1 to High
 * @pre      The RC1 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RC1_SetHigh()          (_LATC1 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC1 GPIO Pin which has a custom name of IO_RC1 to Low
 * @pre      The RC1 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RC1_SetLow()           (_LATC1 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC1 GPIO Pin which has a custom name of IO_RC1
 * @pre      The RC1 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RC1_Toggle()           (_LATC1 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC1 GPIO Pin which has a custom name of IO_RC1
 * @param    none
 * @return   none  
 */
#define IO_RC1_GetValue()         _RC1

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC1 GPIO Pin which has a custom name of IO_RC1 as Input
 * @param    none
 * @return   none  
 */
#define IO_RC1_SetDigitalInput()  (_TRISC1 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC1 GPIO Pin which has a custom name of IO_RC1 as Output
 * @param    none
 * @return   none  
 */
#define IO_RC1_SetDigitalOutput() (_TRISC1 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC4 GPIO Pin which has a custom name of IO_RC4 to High
 * @pre      The RC4 must be set as Output Pin             
 * @param    none
 * @return   none  
 */
#define IO_RC4_SetHigh()          (_LATC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Sets the RC4 GPIO Pin which has a custom name of IO_RC4 to Low
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RC4_SetLow()           (_LATC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Toggles the RC4 GPIO Pin which has a custom name of IO_RC4
 * @pre      The RC4 must be set as Output Pin
 * @param    none
 * @return   none  
 */
#define IO_RC4_Toggle()           (_LATC4 ^= 1)

/**
 * @ingroup  pinsdriver
 * @brief    Reads the value of the RC4 GPIO Pin which has a custom name of IO_RC4
 * @param    none
 * @return   none  
 */
#define IO_RC4_GetValue()         _RC4

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of IO_RC4 as Input
 * @param    none
 * @return   none  
 */
#define IO_RC4_SetDigitalInput()  (_TRISC4 = 1)

/**
 * @ingroup  pinsdriver
 * @brief    Configures the RC4 GPIO Pin which has a custom name of IO_RC4 as Output
 * @param    none
 * @return   none  
 */
#define IO_RC4_SetDigitalOutput() (_TRISC4 = 0)

/**
 * @ingroup  pinsdriver
 * @brief    Initializes the PINS module
 * @param    none
 * @return   none  
 */
void PINS_Initialize(void);



#endif

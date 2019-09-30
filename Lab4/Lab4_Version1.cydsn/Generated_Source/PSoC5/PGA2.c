/*******************************************************************************
* File Name: PGA2.c  
* Version 2.0
*
* Description:
*  This file provides the source code to the API for the PGA 
*  User Module.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "PGA2.h"

#if (!CY_PSOC5A)
    #if (CYDEV_VARIABLE_VDDA == 1u)
        #include "CyScBoostClk.h"
    #endif /* (CYDEV_VARIABLE_VDDA == 1u) */
#endif /* (!CY_PSOC5A) */

#if (CY_PSOC5A)
    static PGA2_BACKUP_STRUCT  PGA2_P5backup;
#endif /* (CY_ PSOC5A) */

uint8 PGA2_initVar = 0u;


/*******************************************************************************   
* Function Name: PGA2_Init
********************************************************************************
*
* Summary:
*  Initialize component's parameters to the parameters set by user in the 
*  customizer of the component placed onto schematic. Usually called in 
*  PGA2_Start().
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PGA2_Init(void) 
{
    /* Set PGA mode */
    PGA2_CR0_REG = PGA2_MODE_PGA;      
    /* Set non-inverting PGA mode and reference mode */
    PGA2_CR1_REG |= PGA2_PGA_NINV;  
    /* Set default gain and ref mode */
    PGA2_CR2_REG = PGA2_VREF_MODE;
    /* Set gain and compensation */
    PGA2_SetGain(PGA2_DEFAULT_GAIN);
    /* Set power */
    PGA2_SetPower(PGA2_DEFAULT_POWER);
}


/*******************************************************************************   
* Function Name: PGA2_Enable
********************************************************************************
*
* Summary:
*  Enables the PGA block operation.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PGA2_Enable(void) 
{
    /* This is to restore the value of register CR1 and CR2 which is saved 
      in prior to the modifications in stop() API */
    #if (CY_PSOC5A)
        if(PGA2_P5backup.enableState == 1u)
        {
            PGA2_CR1_REG = PGA2_P5backup.scCR1Reg;
            PGA2_CR2_REG = PGA2_P5backup.scCR2Reg;
            PGA2_P5backup.enableState = 0u;
        }
    #endif /* CY_PSOC5A */   

    /* Enable power to the Amp in Active mode*/
    PGA2_PM_ACT_CFG_REG |= PGA2_ACT_PWR_EN;

    /* Enable power to the Amp in Alternative Active mode*/
    PGA2_PM_STBY_CFG_REG |= PGA2_STBY_PWR_EN;
    
    PGA2_PUMP_CR1_REG |= PGA2_PUMP_CR1_SC_CLKSEL;
    
    #if (!CY_PSOC5A)
        #if (CYDEV_VARIABLE_VDDA == 1u)
            if(CyScPumpEnabled == 1u)
            {
                PGA2_BSTCLK_REG &= (uint8)(~PGA2_BST_CLK_INDEX_MASK);
                PGA2_BSTCLK_REG |= PGA2_BST_CLK_EN | CyScBoostClk__INDEX;
                PGA2_SC_MISC_REG |= PGA2_PUMP_FORCE;
                CyScBoostClk_Start();
            }
            else
            {
                PGA2_BSTCLK_REG &= (uint8)(~PGA2_BST_CLK_EN);
                PGA2_SC_MISC_REG &= (uint8)(~PGA2_PUMP_FORCE);
            }
        #endif /* (CYDEV_VARIABLE_VDDA == 1u) */
    #endif /* (!CY_PSOC5A) */
}


/*******************************************************************************
* Function Name: PGA2_Start
********************************************************************************
*
* Summary:
*  The start function initializes the PGA with the default values and sets
*  the power to the given level. A power level of 0, is same as executing
*  the stop function.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PGA2_Start(void) 
{

    /* This is to restore the value of register CR1 and CR2 which is saved 
      in prior to the modification in stop() API */

    if(PGA2_initVar == 0u)
    {
        PGA2_Init();
        PGA2_initVar = 1u;
    }

    PGA2_Enable();
}


/*******************************************************************************
* Function Name: PGA2_Stop
********************************************************************************
*
* Summary:
*  Powers down amplifier to lowest power state.
*
* Parameters:
*  void
*
* Return:
*  void
*
*******************************************************************************/
void PGA2_Stop(void) 
{ 
    /* Disble power to the Amp in Active mode template */
    PGA2_PM_ACT_CFG_REG &= (uint8)(~PGA2_ACT_PWR_EN);

    /* Disble power to the Amp in Alternative Active mode template */
    PGA2_PM_STBY_CFG_REG &= (uint8)(~PGA2_STBY_PWR_EN);

    #if (!CY_PSOC5A)
        #if (CYDEV_VARIABLE_VDDA == 1u)
            PGA2_BSTCLK_REG &= (uint8)(~PGA2_BST_CLK_EN);
            /* Disable pumps only if there aren't any SC block in use */
            if ((PGA2_PM_ACT_CFG_REG & PGA2_PM_ACT_CFG_MASK) == 0u)
            {
                PGA2_SC_MISC_REG &= (uint8)(~PGA2_PUMP_FORCE);
                PGA2_PUMP_CR1_REG &= (uint8)(~PGA2_PUMP_CR1_SC_CLKSEL);
                CyScBoostClk_Stop();
            }
        #endif /* CYDEV_VARIABLE_VDDA == 1u */
    #endif /* (CY_PSOC3 || CY_PSOC5LP) */

    /* This sets PGA in zero current mode and output routes are valid */
    #if (CY_PSOC5A)
        PGA2_P5backup.scCR1Reg = PGA2_CR1_REG;
        PGA2_P5backup.scCR2Reg = PGA2_CR2_REG;
        PGA2_CR1_REG = 0x00u;
        PGA2_CR2_REG = 0x00u;
        PGA2_P5backup.enableState = 1u;
    #endif /* CY_PSOC5A */
}


/*******************************************************************************
* Function Name: PGA2_SetPower
********************************************************************************
*
* Summary:
*  Set the power of the PGA.
*
* Parameters:
*  power: Sets power level between (0) and (3) high power
*
* Return:
*  void
*
*******************************************************************************/
void PGA2_SetPower(uint8 power) 
{
    uint8 tmpCR;

    tmpCR = PGA2_CR1_REG & (uint8)(~PGA2_DRIVE_MASK);
    tmpCR |= (power & PGA2_DRIVE_MASK);
    PGA2_CR1_REG = tmpCR;  
}


/*******************************************************************************
* Function Name: PGA2_SetGain
********************************************************************************
*
* Summary:
*  This function sets values of the input and feedback resistors to set a 
*  specific gain of the amplifier.
*
* Parameters:
*  gain: Gain value of PGA (See header file for gain values.)
*
* Return:
*  void 
*
*******************************************************************************/
void PGA2_SetGain(uint8 gain) 
{
    /* Constant array for gain settings */
    const uint8 PGA2_GainArray[9] = { 
        (PGA2_RVAL_0K   | PGA2_R20_40B_40K | PGA2_BIAS_LOW), /* G=1  */
        (PGA2_RVAL_40K  | PGA2_R20_40B_40K | PGA2_BIAS_LOW), /* G=2  */
        (PGA2_RVAL_120K | PGA2_R20_40B_40K | PGA2_BIAS_LOW), /* G=4  */
        (PGA2_RVAL_280K | PGA2_R20_40B_40K | PGA2_BIAS_LOW), /* G=8  */
        (PGA2_RVAL_600K | PGA2_R20_40B_40K | PGA2_BIAS_LOW), /* G=16 */
        (PGA2_RVAL_460K | PGA2_R20_40B_40K | PGA2_BIAS_LOW), /* G=24, Sets Rin as 20k */
        (PGA2_RVAL_620K | PGA2_R20_40B_20K | PGA2_BIAS_LOW), /* G=32 */
        (PGA2_RVAL_470K | PGA2_R20_40B_20K | PGA2_BIAS_LOW), /* G=48, Sets Rin as 10k */
        (PGA2_RVAL_490K | PGA2_R20_40B_20K | PGA2_BIAS_LOW)  /* G=50, Sets Rin as 10k */
    };
    
    /* Constant array for gain compenstion settings */
    const uint8 PGA2_GainComp[9] = { 
        ( PGA2_COMP_4P35PF  | (uint8)( PGA2_REDC_00 >> 2 )), /* G=1  */
        ( PGA2_COMP_4P35PF  | (uint8)( PGA2_REDC_01 >> 2 )), /* G=2  */
        ( PGA2_COMP_3P0PF   | (uint8)( PGA2_REDC_01 >> 2 )), /* G=4  */
        ( PGA2_COMP_3P0PF   | (uint8)( PGA2_REDC_01 >> 2 )), /* G=8  */
        ( PGA2_COMP_3P6PF   | (uint8)( PGA2_REDC_01 >> 2 )), /* G=16 */
        ( PGA2_COMP_3P6PF   | (uint8)( PGA2_REDC_11 >> 2 )), /* G=24 */
        ( PGA2_COMP_3P6PF   | (uint8)( PGA2_REDC_11 >> 2 )), /* G=32 */
        ( PGA2_COMP_3P6PF   | (uint8)( PGA2_REDC_00 >> 2 )), /* G=48 */
        ( PGA2_COMP_3P6PF   | (uint8)( PGA2_REDC_00 >> 2 ))  /* G=50 */
    };
    
    /* Only set new gain if it is a valid gain */
    if( gain <= PGA2_GAIN_MAX)
    {
        /* Clear resistors, redc, and bias */
        PGA2_CR2_REG &= (uint8)(~(PGA2_RVAL_MASK | PGA2_R20_40B_MASK | 
                                PGA2_REDC_MASK | PGA2_BIAS_MASK ));

        /* Set gain value resistors, redc comp, and bias */
        PGA2_CR2_REG |= (PGA2_GainArray[gain] |
                                ((uint8)(PGA2_GainComp[gain] << 2 ) & PGA2_REDC_MASK));

        /* Clear sc_comp  */
        PGA2_CR1_REG &= (uint8)(~PGA2_COMP_MASK);
        /* Set sc_comp  */
        PGA2_CR1_REG |= ( PGA2_GainComp[gain] | PGA2_COMP_MASK );
    }
}


/* [] END OF FILE */
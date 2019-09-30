/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ==================== ====================
*/
#include "project.h"
volatile uint32 data,data_ref,distance=0;
volatile uint16 data_Average[10];
volatile uint16 resultado=0;
volatile uint8 index1;
//instance array
void Arreglo(){
if(index1<10){
    data_Average[index1]=distance;
    index1++;
 
}else{
   index1=0;
  }
}
//resultado_rela_medicion
void Get_Medida(void){
 uint16 temp=0;
   for(int i=0;i<10;i++){
     temp=temp+data_Average[i];
   }
 float value=temp/10;
 resultado=(int)(value);
}
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    LCD_Start();
    LCD_Position(0,1);
    LCD_PrintString("V[V]");
  //  PGA_1_Start();
   // PGA_2_Start();
    ADC1_Start();
    ADC2_Start();
    for(;;)
    {
        
     //  PGA_1_ 
       ADC1_StartConvert();
       ADC1_IsEndConversion(ADC1_WAIT_FOR_RESULT);
       data=ADC1_GetResult32();
       data=ADC1_CountsTo_mVolts(data);
       ADC2_StartConvert();
       ADC2_IsEndConversion(ADC2_WAIT_FOR_RESULT);
       data_ref=ADC2_GetResult16();
       data_ref=ADC2_CountsTo_mVolts(data_ref);
       distance=data-data_ref;
       distance=100*distance;
       Arreglo();
      //  if(data>1000){
         LCD_Position(1,2);
         LCD_PrintDecUint16(distance);
        //}  
      //float Number=;
      // LCD_Position(1,2);
     
     CyDelay(500);
      
        /* Place your application code here. */
    }
}

/* [] END OF FILE */

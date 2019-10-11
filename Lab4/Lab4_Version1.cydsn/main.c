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
#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "math.h"
#define Type_Gain 0
volatile uint32 data,distance=0;
volatile float32 data2,data_Average,resultado,resultado2,offset,temp,kgra2=0;
//volatile uint16 data_Average;
volatile uint16 event_c=0;
//volatile uint16 resultado=0;
volatile uint16 index1=0;
volatile uint8 show=0;
volatile int Flag_Measure=0;
  char str[16],str2[16],str3[16],srt4[16],str5[16];
//instance arrayuin 
void Get_Medida(void);
void Conversion_G100(void);
void Conversion_G1K(void);
void Conversion_G2K(void);

void delete_offset(){
     resultado2=100*(offset-resultado>0?offset-resultado:resultado-offset);
    sprintf(srt4,"%.4f",resultado2);
    LCD_Position(1,9);
    LCD_PrintString(srt4); 
    CyDelay(100);
     }
void Get_Offset(){
 temp=temp+resultado;
 event_c++;
 if(event_c==20){
   offset=(temp/20);
   event_c=0;
   temp=0;
   Flag_Measure=1;
    sprintf(str2,"%.4f",offset);
   LCD_Position(1,2);
   //sprintf(str2,"%.4f",offset);
   LCD_PrintString(str2);
 }
}
void Arreglo(){
if(index1<20){
    data_Average=data2+data_Average;
    index1++;
 
}else{
   Get_Medida(); 
   //show=1;
 //  index1=0;
  }
}
//resultado_rela_medicion
void Get_Medida(void){
   resultado=data_Average/index1;
   index1=0;
   LCD_Position(0,8);
   sprintf(str,"%.4f",resultado);
   LCD_PrintString(str);  
   show=1;
   data_Average=0;
}
//
void Conversion_G100(void){
   float32 kgra=(112.36*resultado2)-35.6;//140
    kgra2=kgra;
//    sprintf(srt4,"%.4f",kgra);
//    LCD_Position(1,9);
//    LCD_PrintString(srt4);
  // return result;
} 
void Conversion_G1K(){
 float32 kgra=(92.6*resultado2)-26.4;//hasta 540
 kgra2=kgra;
}
void Conversion_G2K(){
 float32 kgra=(90.1*resultado2)+78.11; //800
 kgra2=kgra;
}
void Conversion_G3K(){
 float32 kgra=(90.90*resultado2)-20.89;
 kgra2=kgra;
}
//
int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    LCD_Start();
    LCD_Position(0,1);
    LCD_PrintString("V[V]");
    ADC1_Start();
   
    
//    PGA1_Start();
//    PGA2_Start();
    #ifndef Type_Gain
      PGA1_SetGain(1);
      PGA2_SetGain(1);
    #else    
        int u=0;
//      PGA1_SetGain(1);
//      PGA2_SetGain(1);
    #endif
    //ADC2_Start();
    for(;;)
    {
        
     //  PGA_1_ 
       ADC1_StartConvert();
       ADC1_IsEndConversion(ADC1_WAIT_FOR_RESULT);
       data=ADC1_GetResult32();
       data2=ADC1_CountsTo_Volts(data);
       CyDelay(5);
       Arreglo();
       
    if(Flag_Measure==1){  
        delete_offset();
 
        if(resultado2<=0.55){
            Conversion_G100();            
        }else if(resultado2<=1.5){
            Conversion_G1K();
        }else if(resultado2<=5.15){
             Conversion_G2K(); 
        }else if(resultado2<=9.4){
              Conversion_G3K(); 
        }
        sprintf(str5,"%.4f",kgra2);
        LCD_Position(0,0);
        LCD_PrintString(str5);
        CyDelay(80);
  //      LCD_Position(0,0);
       }else if(show==1){
       Get_Offset();
    
    } 

     
        CyDelay(10);
        //show=0;
        /* Place your application code here. */
    }
}
/* [] END OF FILE */

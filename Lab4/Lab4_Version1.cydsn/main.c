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
#define Type_Gain 0
volatile uint32 data,data_ref,distance=0;
volatile uint16 data_Average;
volatile uint16 event_c,temp=0;
volatile uint16 resultado=0;
volatile uint16 index1;
volatile uint16 offset;
volatile int Flag_Measure=0;
//instance arrayuin 
void Get_Medida(void);
int16 Conversion_G(void);


void delete_offset(){
     resultado=(offset-resultado);
// int x1=(resultado/1000);
// if(x1!=0){
//   resultado=(resultado-x1*1000);
// }
}
void Get_Offset(){
 temp=temp+resultado;
 event_c++;
 if(event_c==100){
   offset=(temp/100);
   event_c=0;
   temp=0;
   Flag_Measure=1;
 }
}
void Arreglo(){
if(index1<=100){
    data_Average=data+data_Average;
    index1++;
 
}else{
   Get_Medida(); 
 //  index1=0;
  }
}
//resultado_rela_medicion
void Get_Medida(void){
   resultado=data_Average/index1;
   index1=0;
   data_Average=0; 
}
//
int16 Conversion_G(){
   int16 result=(-2.235*resultado)+8391;
   return result;
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
   
    
    PGA1_Start();
    PGA2_Start();
    #ifndef Type_Gain
      PGA1_SetGain(1);
      PGA2_SetGain(1);
    #else     
      PGA1_SetGain(2);
      PGA2_SetGain(50);
    #endif
    //ADC2_Start();
    for(;;)
    {
        
     //  PGA_1_ 
       ADC1_StartConvert();
       ADC1_IsEndConversion(ADC1_WAIT_FOR_RESULT);
       data=ADC1_Read32()/1;
       data=ADC1_CountsTo_mVolts(data);
         //                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 b h)
    //data=ADC1_CountsTo_mVolts(data);
       CyDelay(5);
       Arreglo();
       
    if(Flag_Measure==1){  
        delete_offset();
      //  if(data>1000){
         LCD_Position(1,2);
        // float numero2=0;
         float numero;
         numero=13.405*resultado+2.5101;
         //float numero=-2.229*resultado+8373;
         int x1=(int)(numero/1000);
         int x2=(int)(numero-x1*1000)/100;
         int x3=(int)(numero-x1*1000-x2*100)/10;
         int x4=numero-x1*1000-x2*100-x3*10;
         char Numx[4]={(char)(x1+48),(char)(x2+48),(char)(x3+48),(char)(x4+48)};
         LCD_PrintString(Numx);
    }else{
       Get_Offset();
    
    } 
        LCD_Position(0,8);
          int x11=(int)(resultado/1000);
         int x22=(int)(resultado-x11*1000)/100;
         int x33=(int)(resultado-x11*1000-x22*100)/10;
         int x44=resultado-x11*1000-x22*100-x33*10;
         char Numx2[4]={(char)(x11+48),(char)(x22+48),(char)(x33+48),(char)(x44+48)};
        // delete_offset();
         LCD_PrintString(Numx2);
        //}  
      //float Number=;
      // LCD_Position(1,2);
     
        // CyDelay(80);
      
        /* Place your application code here. */
    }
}
/* [] END OF FILE */

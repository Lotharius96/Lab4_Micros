/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"
volatile uint8 cont,cont2,cont3,cont4=0;
volatile uint8 cont5,cont6,cont7,cont8=0;
volatile uint32 fin,fin2;
volatile int listo_B,listo_N;
void conteo(uint8 cont);



//void conteo(uint8 cont){
//    
//}

CY_ISR(boton_blancas){
    cont=0;
    cont2=0;
    cont3=0;
    cont4=0;
    LCD_Position(0,8);
    LCD_PrintString("00:00");
    int flag=0;
    int operacion=0;
    int temp;
    while(boton_blancas_Read()==0){
        CyDelay(50);
        flag=1;
    }
    if (flag==1){
        operacion++;  
    }
    
    
    while(operacion==1){
        if (subir_Read()==0){
            cont++;
            CyDelay(200);  
            LCD_Position(0,8);
            LCD_PrintNumber(cont);            
        }  
        if(aceptar_Read()==0){                            
            operacion++; 
        }
    }
    CyDelay(500);
    while(operacion==2){
        if (subir_Read()==0){
            cont2++;
            CyDelay(200);  
            LCD_Position(0,9);
            LCD_PrintNumber(cont2);           
        } 
        if(aceptar_Read()==0){                          
           operacion++;              
        }
    }
    CyDelay(500);
    while(operacion==3){
        if (subir_Read()==0){
            cont3++;
            CyDelay(200);  
            LCD_Position(0,11);
            LCD_PrintNumber(cont3);           
        } 
        if(aceptar_Read()==0){                          
           operacion++;              
        }
    }
    CyDelay(500);
    while(operacion==4){
        if (subir_Read()==0){
            cont4++;
            CyDelay(200);  
            LCD_Position(0,12);
            LCD_PrintNumber(cont4);           
        } 
        if(aceptar_Read()==0){                          
           operacion++;  
           fin=(10000*cont)+(1000*cont2)+(10*cont3)+(cont4);  
           listo_B=1;
        }
    }
    
    if (operacion==5){
        boton_blancas_ClearInterrupt();
    }    
}

CY_ISR(boton_negras){
    cont5=0;
    cont6=0;
    cont7=0;
    cont8=0;
    LCD_Position(1,8);
    LCD_PrintString("00:00");
    int flag=0;
    int operacion2=0;
    int temp;
    while(boton_negras_Read()==0){
        CyDelay(50);
        flag=1;
    }
    if (flag==1){
        operacion2++;  
    }
    
    
    while(operacion2==1){
        if (subir_Read()==0){
            cont5++;
            CyDelay(200);  
            LCD_Position(1,8);
            LCD_PrintNumber(cont5);            
        }  
        if(aceptar_Read()==0){                            
            operacion2++; 
        }
    }
    CyDelay(500);
    while(operacion2==2){
        if (subir_Read()==0){
            cont6++;
            CyDelay(200);  
            LCD_Position(1,9);
            LCD_PrintNumber(cont6);           
        } 
        if(aceptar_Read()==0){                          
           operacion2++;              
        }
    }
    CyDelay(500);
    while(operacion2==3){
        if (subir_Read()==0){
            cont7++;
            CyDelay(200);  
            LCD_Position(1,11);
            LCD_PrintNumber(cont7);           
        } 
        if(aceptar_Read()==0){                          
           operacion2++;              
        }
    }
    CyDelay(500);
    while(operacion2==4){
        if (subir_Read()==0){
            cont8++;
            CyDelay(200);  
            LCD_Position(1,12);
            LCD_PrintNumber(cont8);           
        } 
        if(aceptar_Read()==0){                          
           operacion2++;  
           listo_N=1;
        }
    }
    
    if (operacion2==5){
        boton_negras_ClearInterrupt();
//        if (){
//            
//        }
    }    
}

CY_ISR(Juego_B){
    int flag=0;
        while(Juego_B_Read()==0){
        CyDelay(50);
        flag=1;
    }
    
}

CY_ISR(Juego_N){
     int flag=0;
        while(Juego_N_Read()==0){
        CyDelay(50);
        flag=1;
    }
    
}

CY_ISR(Timerr){
  uint16 total_ref=(turno==0?cont+cont2+cont3+cont4:cont5+cont6+cont7+cont8);
   //
    if(turno==0){
      visual_Type(1);
    }else{
      visual_Type(0);
    }
////  //Segundos--;
    if(cont4>0 && turno==0){
      cont4--;
    //Minutos--;
     }else if(total_ref!=0 && turno==0 && cont4==0){
     cont3--; 
     cont4=9;
    }else if(total_ref!=0 && turno==0 && cont3==0){
     cont2--;
     cont3=5;
     }else if(total_ref!=0 && turno==0 && cont2==0){
     cont--;
     cont2=9;
     } if(cont8>0 && turno==255){
      cont8--;
    //Minutos--;
     }else if(total_ref!=0 && turno==255 && cont8==0){
     cont7--; 
     cont8=9;
    }else if(total_ref!=0 && turno==255 && cont7==0){
     cont6--;
     cont7=5;
     }else if(total_ref!=0 && turno==255 && cont6==0){
     cont--;
     cont6=9;
     }
     if(total_ref==0){
        turno=~turno;
        Timer_1_Start();
        Pin_1_Write(1);
     }
   isr_Timer_ClearPending();   
    
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    LCD_Start();
    isr_boton_B_StartEx(boton_blancas);
    isr_boton_N_StartEx(boton_negras);
    isr_JB_StartEx(Juego_B);
    isr_JN_StartEx(Juego_N);
    isr_Timer_StartEx(Timerr);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
        LCD_Position(0,0);
        LCD_PrintString("Blancas:");
        LCD_Position(1,0);
        LCD_PrintString("Negras :");
    
    for(;;)
    {
       
    if (aceptar_Read()==0 && listo_B==1 && listo_N==1){
          Timerr_Start(); 
    }
    
//        fin++;
//        if(fin==8){
//            fin=0;
//        }
//        CyDelay(100);
//        LCD_Position(0,8);
//        LCD_PrintU32Number(11111);

//    
  
        LCD_Position(0,10);
        LCD_PrintInt8(cont);
//        LCD_Position(0,9);
//        LCD_PrintInt8(fin);
//        LCD_Position(0,11);
//        LCD_PrintString(":");
//        LCD_Position(0,12);
//        LCD_PrintNumber((cont3*10)+cont4);
        
        
        //el contraio al leido  se escribe en el led
        //problema rebote en el SW
        //fisica = el SW en paralelo con condensador
        //software = un retardo 100 - 200ms
        //CyDelay(250);
    }
}

/* [] END OF FILE */

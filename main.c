/*!
** @file main.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup main_module main module documentation
**  @{
*/         
/* MODULE main */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "Bit1.h"
#include "Bit2.h"
#include "PWM1.h"
#include "PWM2.h"
#include "AD1.h"
#include "Bit3.h"
#include "Bit4.h"
#include "Bit5.h"
#include "Bit6.h"
#include "TI1.h"
#include "TI2.h"
#include "EInt1.h"
#include "TI3.h"
#include "Bit7.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

char sharp;
char line;
char bobinaizq;
char bobinader;
signed char error;
signed char ajuste;
signed int sumaerror=0;
char PWM1;
char PWM2;
extern bool bandera;
char suma_bobinader=0;
char suma_bobinaizq=0;
char suma_bobinaderaux=0;
char suma_bobinaizqaux=0;
bool PIDactivo;

int fila;
int columna;
bool bandera_ln=0;
bool contador_ln;
bool a=1;
int contador=0;
int giros_izq;
int giros_der;
int b;
int xactual;
int yactual;
int xdeseada;

int ydeseada;
int recorrido_pista;
int sentido;
int obst;


void stop(void)
 {
 sumaerror=0;
 suma_bobinader=0;
 suma_bobinaizq=0;
 PWM1_SetRatio8(0);      // Rueda derecha OFF
 PWM2_SetRatio8(0);      // Rueda izquierda OFF
 Bit1_PutVal(1);         // DC GND
 Bit2_PutVal(1);
 Cpu_Delay100US(2000);
 }

void just_stop (void)
{PWM1_SetRatio8(0);      // Rueda derecha OFF
PWM2_SetRatio8(0);      // Rueda izquierda OFF
Bit1_PutVal(1);         // DC GND
Bit2_PutVal(1);
Cpu_Delay100US(2000);
}


void PID ()
{ if (bandera == 1)
{
error= suma_bobinaizq-suma_bobinader;
sumaerror=error+sumaerror;
if (sumaerror>4000)
sumaerror= 4000;
else if (sumaerror<-4000)
sumaerror=-4000;	
else 
sumaerror=sumaerror;

ajuste=(7*error)/10 + sumaerror/1000;
if (ajuste>80)
	ajuste=80;
else if (ajuste<-80)
	ajuste=-80;
else
ajuste=ajuste;

PWM1=130+ajuste;
PWM2=130-ajuste;
bandera=0;	
}

}


void avanza (void)
{

PWM1_SetRatio8(PWM1);    // Rueda derecha ON
PWM2_SetRatio8(PWM2);    // Rueda izquierda ON
Bit1_PutVal(1);         // DC GND
Bit2_PutVal(1);}

 


void calcular_sentido (void)
{if (giros_izq == 0 && giros_der == 0)
    sentido=1;
if (giros_izq == 1 || giros_der == 3)
	sentido=3;
if (giros_izq == 3 || giros_der == 1)
	sentido=2;
if (giros_izq == 2 || giros_der == 2)
    sentido=4;	
if (giros_izq == 1 && giros_der == 1)
    {sentido=1;
    giros_izq=0;
    giros_der=0;}
contador_ln=0;
a=1;
recorrido_pista=1;}





void giro_derecha(void)
{ PWM1_SetRatio8(0);      // Rueda derecha OFF
  PWM2_SetRatio8(120);      // Rueda izquierda ON
  Bit1_PutVal(1);         // DC GND
  Bit2_PutVal(1);
  Cpu_Delay100US(1500);
 while (bobinader < 90)
 {PWM1_SetRatio8(0);      // Rueda derecha OFF
  PWM2_SetRatio8(120);      // Rueda izquierda OFF
  Bit1_PutVal(1);         // DC GND
  Bit2_PutVal(1);
  }
  just_stop();
  giros_der=giros_der+1;
  calcular_sentido();
  }
  

void giro_izquierda (void)
{PWM1_SetRatio8(120);      // Rueda derecha OFF
PWM2_SetRatio8(0);      // Rueda izquierda OFF
Bit1_PutVal(1);         // DC GND
Bit2_PutVal(1);
Cpu_Delay100US(1500);
while (bobinaizq < 102)
{PWM1_SetRatio8(120);      // Rueda derecha OFF
 PWM2_SetRatio8(0);      // Rueda izquierda OFF
 Bit1_PutVal(1);         // DC GND
 Bit2_PutVal(1);
 }
 just_stop();
 giros_izq=giros_izq+1;
 calcular_sentido();
 }



void stop_decide (void)
{
if (sharp > 80)
{   
obst=0;
if (xactual < xdeseada && sentido == 1)
{giro_izquierda();
a=1;}
else if (xactual > xdeseada && sentido == 1)
{giro_derecha();
a=1;}
else if (xactual < xdeseada && sentido == 4)
{giro_derecha();
a=1;}
else if (xactual > xdeseada && sentido == 4)
{giro_izquierda();
a=1;}
else if (yactual < ydeseada && sentido == 3)
{giro_derecha();
a=1;}
else if (yactual > ydeseada && sentido == 3 )
{giro_izquierda();
a=1;}
else if (yactual < ydeseada && sentido == 2)
{giro_izquierda();
a=1;}
else if (yactual == ydeseada && xactual < xdeseada && sentido == 1)
{giro_izquierda();
a=1;}
else if (yactual == ydeseada && xactual > xdeseada && sentido == 1)
 {giro_derecha();
  a=1;}
else if (xactual == xdeseada && yactual < ydeseada && sentido == 3)
{giro_derecha();
a=1;}
else if (xactual == xdeseada && yactual > ydeseada && sentido == 3)
{giro_izquierda();
a=1;}
else if (xactual == xdeseada && yactual < ydeseada && sentido == 2)
{giro_izquierda();
a=1;}
else if (xactual == xdeseada && yactual > ydeseada && sentido == 2)
{giro_derecha();
a=1;}
else if (yactual == ydeseada && xactual > xdeseada && sentido == 4)
{giro_izquierda();
a=1;}
else if (yactual == ydeseada && xactual < xdeseada && sentido == 4)
{giro_derecha();
a=1;}

else 
{giro_derecha();
a=1;}

}

else 
{	
if (yactual == ydeseada && xactual < xdeseada && sentido == 1 && obst == 1)
{giro_izquierda();
a=1;}

else if (yactual == ydeseada && xactual > xdeseada && sentido == 4 && obst == 1)
{giro_izquierda();
a=1;}

else if (yactual == ydeseada && xactual > xdeseada && sentido == 1 && obst == 1)
 {giro_derecha();
  a=1;}

else if (xactual == xdeseada && yactual < ydeseada && sentido == 3 && obst == 1)
{giro_derecha();
a=1;}

else if (xactual == xdeseada && yactual > ydeseada && sentido == 3 && obst == 1)
{giro_izquierda();
a=1;}

else if (xactual == xdeseada && yactual < ydeseada && sentido == 2 && obst == 1)
{giro_izquierda();
a=1;}

else if (xactual == xdeseada && yactual > ydeseada && sentido == 2 && obst == 1)
{giro_derecha();
a=1;}

else if (xactual > xdeseada && yactual > ydeseada && sentido == 3 && obst == 1)
{giro_izquierda();
a=1;}

else if (xactual < xdeseada && yactual < ydeseada && sentido == 3 && obst == 1)
{giro_izquierda();
a=1;}



else 
{ suma_bobinaderaux=0;
  suma_bobinaizqaux=0;
  sumaerror=0;
  calcular_sentido();
  avanza ();
  Cpu_Delay100US(1000);
  calcular_sentido();
}
}

}





void compara (void)
{ if (xactual == xdeseada && yactual == ydeseada && sentido == 3)
	{
	 xdeseada=0;
	 ydeseada=0;
	 stop();
	 giro_izquierda();
	 }

else if (xactual == xdeseada && yactual == ydeseada && sentido == 1)
{
	 xdeseada=0;
	 ydeseada=0;
	 stop();
	 giro_derecha();
	 }

else if (sharp > 80 && xactual == xdeseada && yactual == ydeseada && sentido == 1)
{
	 xdeseada=0;
	 ydeseada=0;
	 stop ();
	 giro_derecha();
	 }
else 
stop_decide ();
}

void interseccion(void)
{   
	TI1_Enable();
	
    while (contador_ln == 0)
	{ PID ();
	 avanza ();
	
    if (line > 120 && a == 1)
     
	{bandera_ln=1;	
	 a=0;
	 Cpu_Delay100US(200);
 }
    
    else if (bandera_ln == 1 && line < 120)
 {
     bandera_ln=0;
	 contador_ln=1;
	 Cpu_Delay100US(200);	  	 	   }}
    
	PID ();
    avanza ();
 
    if (contador_ln == 1 && line > 120)
 {
 TI1_Disable();
 stop ();
 obst=1;
 
 
 switch (sentido){
 case 1:
	 yactual=yactual+1;
	 break;
 case 2:
 	 xactual=xactual-1;
 	 break;
 case 3:
 	 xactual=xactual+1;
 	 break;
 case 4:
 	 yactual=yactual-1;
 	 break;
	 
 }
 
 compara();}} 














/* User includes (#include below this line is not maintained by Processor Expert) */

void main(void)
{
  /* Write your local variable definition here */
	
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/

	



	PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/
  (void)AD1_Enable();
  (void)AD1_Start();
   PWM1_Enable();
   PWM2_Enable();
   PWM1_SetRatio8(140);
   PWM2_SetRatio8(140);
   Bit1_PutVal(1);
   Bit2_PutVal(1);
   PIDactivo=1;
   giros_izq=0;
   giros_der=0;
   xactual=0;
   yactual=0;
   xdeseada=1;
   ydeseada=2;
   recorrido_pista=1;
   sentido=1;
   obst=1;
   contador_ln=0;
   
   
  /* Write your code here */
  /* For example: for(;;) { } */

  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){
	  
  interseccion();}
  }
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
 /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/

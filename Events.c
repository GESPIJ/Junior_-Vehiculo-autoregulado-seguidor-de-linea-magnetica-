/* ###################################################################
**     Filename    : Events.c
**     Project     : JuniorMiercolesSem6
**     Processor   : MC9S08QE128CLK
**     Component   : Events
**     Version     : Driver 01.02
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2017-10-25, 16:36, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.02
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         
/* MODULE Events */


#include "Cpu.h"
#include "Events.h"

extern char line;
extern char sharp;
extern char bobinaizq;
extern char bobinader;
extern bool bandera;
extern int contador;
extern char suma_bobinader;
extern char suma_bobinaizq;
extern char suma_bobinaderaux;
extern char suma_bobinaizqaux;
extern int contador;
extern signed char error;
extern char bobinader;
extern char bobinaizq;
extern signed int sumaerror;
extern signed char ajuste;
extern char PWM1;
extern char PWM2;



int counter=0;
int ready=1;
int i=0;
int j=0;
int cont=0;
int Recibido[14];
bool ext;
int bit;


/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  AD1_OnEnd (module Events)
**
**     Component   :  AD1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void AD1_OnEnd(void)
{
  /* Write your code here ... */
	
	AD1_MeasureChan(1,0);
	AD1_GetChanValue8(0,&line);
	AD1_MeasureChan(1,1);
	AD1_GetChanValue8(1,&sharp);
	AD1_MeasureChan(1,2);
    AD1_GetChanValue8(2,&bobinader);
	AD1_MeasureChan(1,3);
	AD1_GetChanValue8(3,&bobinaizq);
	bobinaizq=bobinaizq;
	
	if (contador<100)
		{suma_bobinaderaux=suma_bobinaderaux+bobinader;
		 suma_bobinaizqaux=suma_bobinaizqaux+bobinaizq;
		contador++;}
	    else 
	    {suma_bobinader=suma_bobinaderaux/10;	
	     suma_bobinaizq=suma_bobinaizqaux/10;
	     suma_bobinaderaux=0;
	     suma_bobinaizqaux=0;
	     contador=0;}
}


/*
** ===================================================================
**     Event       :  TI1_OnInterrupt (module Events)
**
**     Component   :  TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI1_OnInterrupt(void)
{
	bandera=1;
	   
		//PWM1_SetRatio8(PWM1);    // Rueda derecha ON
		//PWM2_SetRatio8(PWM2);    // Rueda izquierda ON
		//Bit1_PutVal(1);         // DC GND
		//Bit2_PutVal(1);

}

/*
** ===================================================================
**     Event       :  TI2_OnInterrupt (module Events)
**
**     Component   :  TI2 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI2_OnInterrupt(void)
{   
    }

  

/*
** ===================================================================
**     Event       :  TI3_OnInterrupt (module Events)
**
**     Component   :  TI3 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TI3_OnInterrupt(void)
{
  /* Write your code here ... */
	ext = EInt1_GetVal();
		bit = Bit7_GetVal();
		if(bit!=0)
			bit=1;
		cont ++;
		
		if (cont==8){
				cont=4;
			}
		if(cont==4){
			j++;
			if(bit && j<15){
				Recibido[j-1]=1;
			}else{
				if (j<15)
				Recibido[j-1]=0;
			}
		}
		if(j==30){
			j=0;
			(void)EInt1_Enable();
			(void)TI3_Disable();
			
		}	
				

}

/*
** ===================================================================
**     Event       :  EInt1_OnInterrupt (module Events)
**
**     Component   :  EInt1 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void EInt1_OnInterrupt(void)
{
  /* place your EInt1 interrupt procedure body here*/
	
	(void)EInt1_Disable();
	(void)TI3_Enable();
}

/* END Events */

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

/* ###################################################################
**     Filename    : Events.c
**     Project     : expp3
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-12-28, 21:23, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Settings    :
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.c
** @version 01.00
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

#ifdef __cplusplus
extern "C" {
#endif 


/* User includes (#include below this line is not maintained by Processor Expert) */
extern volatile char key;
extern volatile char kFlag;
/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
/* ===================================================================*/
/** @brief Interrupção não utilizada. */
void Cpu_OnNMIINT(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  C3_OnInterrupt (module Events)
**
**     Component   :  C3 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/**
 * @brief
 * Esta interrupção é acionada toda vez que uma tecla da coluna 3 teclado matricial é pressionada, realizando as seguintes instruções:
 * 
 * 1. Varre as 4 linhas colocando-as alternadamente como Output de sinal digital;
 * 2. Verifica em qual das linhas o sinal foi detectado no pino estabelecido para a coluna 3;
 * 3. Define um caracter correspondente na tecla key;
 * 4. Aciona flag de tecla pressionada (kFlag);
 * 5. Coloca todas as linhas como Input de sinal digital para a próxima vez que a interrupção for acionada.
*/
void C3_OnInterrupt(void)
{
  /* Write your code here ... */
	L1_SetOutput();
	L2_SetInput();
	L3_SetInput();
	L4_SetInput();
	W_Waitms(50);
	if(!C3_GetVal()) {
		key = '3';
	} else {
		L2_SetOutput();
		W_Waitus(20);
		if(!C3_GetVal()) {
			key = '6';
		} else {
			L3_SetOutput();
			W_Waitus(20);
			if(!C3_GetVal()) {
				key = '9';
			} else {
				L4_SetOutput();
				W_Waitus(20);
				if(!C3_GetVal()) {
					key = '#';
				}
			}
		}
	}
	L1_SetOutput();
	L2_SetOutput();
	L3_SetOutput();
	L4_SetOutput();
	
	kFlag = 1;
}

/*
** ===================================================================
**     Event       :  C2_OnInterrupt (module Events)
**
**     Component   :  C2 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/**
 * @brief
 * Esta interrupção é acionada toda vez que uma tecla da coluna 2 teclado matricial é pressionada, realizando as seguintes instruções:
 * 
 * 1. Varre as 4 linhas colocando-as alternadamente como Output de sinal digital;
 * 2. Verifica em qual das linhas o sinal foi detectado no pino estabelecido para a coluna 2;
 * 3. Define um caracter correspondente na tecla key;
 * 4. Aciona flag de tecla pressionada (kFlag);
 * 5. Coloca todas as linhas como Input de sinal digital para a próxima vez que a interrupção for acionada.
*/
void C2_OnInterrupt(void)
{
  /* Write your code here ... */
	L1_SetOutput();
	L2_SetInput();
	L3_SetInput();
	L4_SetInput();
	W_Waitms(50);
	if(!C2_GetVal()) {
		key = '2';
	} else {
		L2_SetOutput();
		W_Waitus(20);
		if(!C2_GetVal()) {
			key = '5';
		} else {
			L3_SetOutput();
			W_Waitus(20);
			if(!C2_GetVal()) {
				key = '8';
			} else {
				L4_SetOutput();
				W_Waitus(20);
				if(!C2_GetVal()) {
					key = '0';
				}
			}
		}
	}
	L1_SetOutput();
	L2_SetOutput();
	L3_SetOutput();
	L4_SetOutput();
	
	kFlag = 1;
}

/*
** ===================================================================
**     Event       :  C1_OnInterrupt (module Events)
**
**     Component   :  C1 [ExtInt]
**     Description :
**         This event is called when an active signal edge/level has
**         occurred.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/**
 * @brief
 * Esta interrupção é acionada toda vez que uma tecla da coluna 1 teclado matricial é pressionada, realizando as seguintes instruções:
 * 
 * 1. Varre as 4 linhas colocando-as alternadamente como Output de sinal digital;
 * 2. Verifica em qual das linhas o sinal foi detectado no pino estabelecido para a coluna 1;
 * 3. Define um caracter correspondente na tecla key;
 * 4. Aciona flag de tecla pressionada (kFlag);
 * 5. Coloca todas as linhas como Input de sinal digital para a próxima vez que a interrupção for acionada.
*/
void C1_OnInterrupt(void)
{
  /* Write your code here ... */
	L1_SetOutput();
	L2_SetInput();
	L3_SetInput();
	L4_SetInput();
	W_Waitms(50);
	if(!C1_GetVal()) {
		key = '1';
	} else {
		L2_SetOutput();
		W_Waitus(20);
		if(!C1_GetVal()) {
			key = '4';
		} else {
			L3_SetOutput();
			W_Waitus(20);
			if(!C1_GetVal()) {
				key = '7';
			} else {
				L4_SetOutput();
				W_Waitus(20);
				if(!C1_GetVal()) {
					key = '*';
				}
			}
		}
	}
	L1_SetOutput();
	L2_SetOutput();
	L3_SetOutput();
	L4_SetOutput();
	
	kFlag = 1;
}

/*
** ===================================================================
**     Event       :  PC_OnError (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/** @brief Interrupção não utilizada. */
void PC_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  PC_OnRxChar (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void PC_OnRxChar(void)
{
  /* Write your code here ... */
	extern volatile int i;
	extern volatile char pMsg[100];
	extern volatile char pFlag;
	uint8_t c;
	
	PC_RecvChar(&c);           // Le um caracter recebido pelo pc e armazena na variavel c
	
	if(c == 0x08) {            // se o caracter for 0x08 (Backspace)...
		while(PC_SendChar(c)); // Transmite para o terminal
		i--;
	} else {                   // Senao...
		while(PC_SendChar(c)); // Transmite para o terminal
		pMsg[i] = c;           // Adiciona a string
		i++;
	}
	
	if(c == 0x0D) {     // Se o caracter for 0x0D (Carriage return)...
		pMsg[i] = 0x0A; // Adiciona 0x0A (Line feed) na string
		i++;
		pMsg[i] = 0;    // Adiciona NULL na string
		pFlag = 1;      // Ativa a flag (aciona o envio do input ao ESP em main.c)
		i = 0;
	}
}

/*
** ===================================================================
**     Event       :  PC_OnTxChar (module Events)
**
**     Component   :  PC [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/** @brief Interrupção não utilizada. */
void PC_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  ESP_OnError (module Events)
**
**     Component   :  ESP [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/** @brief Interrupção não utilizada. */
void ESP_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  ESP_OnRxChar (module Events)
**
**     Component   :  ESP [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void ESP_OnRxChar(void)
{
  /* Write your code here ... */
	extern volatile int i;
	extern volatile char eMsg[100];
	extern volatile char eFlag;
	uint8_t c;
	
	ESP_RecvChar(&c);       // Chama funcao criada pelo Processor Expert que le o caracter recebido e coloca na variavel "c"
	eMsg[i] = c;
	i++;
	if(c == 0x0A){          // Se 0x0A foi encontrado, entao...
		eMsg[i] = 0;       // ... o ultimo caracter da string o valor nulo
		i = 0;
		eFlag = 1;         // ... a flag msg sinalizada (mensagem pronta)
	}
}

/*
** ===================================================================
**     Event       :  ESP_OnTxChar (module Events)
**
**     Component   :  ESP [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/** @brief Interrupção não utilizada. */
void ESP_OnTxChar(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Event       :  SM1_OnBlockSent (module Events)
**
**     Component   :  SM1 [SPIMaster_LDD]
*/
/*!
**     @brief
**         This event is called after the last character from the
**         output buffer is moved to the transmitter. This event is
**         available only if the SendBlock method is enabled.
**     @param
**         UserDataPtr     - Pointer to the user or
**                           RTOS specific data. The pointer is passed
**                           as the parameter of Init method. 
*/
/* ===================================================================*/
/** @brief Interrupção não utilizada. */
void SM1_OnBlockSent(LDD_TUserData *UserDataPtr)
{
  /* Write your code here ... */
}

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.09]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/

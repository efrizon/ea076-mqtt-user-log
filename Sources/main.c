/* ###################################################################
**     Filename    : main.c
**     Project     : expp3
**     Processor   : MKL25Z128VLK4
**     Version     : Driver 01.01
**     Compiler    : GNU C Compiler
**     Date/Time   : 2020-12-28, 21:23, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file main.c
** @version 01.01
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
#include "MCUC1.h"
#include "SM1.h"
#include "D.h"
#include "RESpin1.h"
#include "SCEpin1.h"
#include "D_Cpin1.h"
#include "LD.h"
#include "LEDpin1.h"
#include "BitIoLdd1.h"
#include "RTC.h"
#include "ESP.h"
#include "ASerialLdd1.h"
#include "PC.h"
#include "ASerialLdd2.h"
#include "L1.h"
#include "BitIoLdd2.h"
#include "L2.h"
#include "BitIoLdd3.h"
#include "L3.h"
#include "BitIoLdd4.h"
#include "L4.h"
#include "BitIoLdd5.h"
#include "C1.h"
#include "ExtIntLdd1.h"
#include "C2.h"
#include "ExtIntLdd2.h"
#include "C3.h"
#include "ExtIntLdd3.h"
#include "W.h"
#include "LEDR.h"
#include "LEDpin2.h"
#include "BitIoLdd6.h"
#include "LEDG.h"
#include "LEDpin3.h"
#include "BitIoLdd7.h"
#include "LEDB.h"
#include "LEDpin4.h"
#include "BitIoLdd8.h"
#include "EE24.h"
#include "GI2C1.h"
#include "CLS1.h"
#include "UTIL1.h"
#include "XF1.h"
#include "CS1.h"
#include "CI2C1.h"
/* Including shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */
volatile LDD_RTC_TTime MyTime; /**<@brief Estrutura que recebe os valores do m??dulo RTC. */
volatile LDD_TDeviceData * MyRTC; /**<@brief Ponteiro utilizado na inicializa????o do m??dulo RTC e ao utilizar a fun????o RTC_GetTime(). */

volatile uint8_t i = 0; /**<@brief ??ndice para posi????es de arrays em geral. */
volatile uint8_t j = 0; /**<@brief ??ndice para posi????es de arrays em geral. */

volatile char pMsg[100] = {0}; /**<@brief Mensagem do PC a ser enviada ao m??dulo ESP, com m??ximo de 100 caracteres. */
volatile char eMsg[100] = {0}; /**<@brief Mensagem recebida pelo m??dulo ESP, com m??ximo de 100 caracteres. */
volatile char key;             /**<@brief Tecla detectada em alguma das interrup????es */

volatile char pFlag = 0; /**<@brief Flag de mensagem do PC pronta da interrup????o PC_OnRxChar(). */
volatile char eFlag = 0; /**<@brief Flag de mensagem do ESP pronta da interrup????o ESP_OnRxChar(). */
volatile char kFlag = 0; /**<@brief Flag de tecla digitada teclado matricial em alguma das interrup????es C1_OnInterrupt(), C2_OnInterrupt() e C3_OnInterrupt(). */

char cStr[20] = {0}; /**<@brief Substring de comando pertencente ?? mensagem recebida pelo ESP (Ex.: "MENSAGEM"). */
char tStr[50] = {0}; /**<@brief Substring de t??pico pertencente ?? mensagem recebida pelo ESP. */
char pStr[10] = {0}; /**<@brief Substring de par??metro pertencente ?? mensagem recebida pelo ESP. */

char aux0[40] = {0}; /**<@brief String para armazenamento de data do m??dulo RTC */
char aux1[40] = {0}; /**<@brief String para armazenamento de hora do m??dulo RTC */
char aux2[40] = {0}; /**<@brief String para prop??sitos gerais. */
char aux3[40] = {0}; /**<@brief String para prop??sitos gerais. */
char aux4[40] = {0}; /**<@brief String para prop??sitos gerais. */

char xs[7] = "grupo1"; /**<@brief String de acesso */
char id[3] = {0}; /**<@brief String em que ?? armazenada a id digitada no teclado matricial. */
char pw[5] = {0}; /**<@brief String em que ?? armazenada a senha digitada no teclado matricial. */

char block[16] = {0}; /**<@brief String em que ?? armazenada o log de usu??rios para impress??o no t??pico /log. */
char user[15] = {0}; /**<@brief String em que ?? armazenada a string do nome do usu??rio recebida no t??pico /user antes de sr armazenada em mem??ria. */
char pass[5] = {0}; /**<@brief String em que ?? armazenada a senha da mem??ria quando utilizada para compara????o com a senha digitada. */

uint16_t addr = 0; /**<@brief Endere??o atual da mem??ria */

int state = 0; /**<@brief Flag de estado, respons??vel por direcionar a entrada de dados do teclado matricial para as strings id(0) e pw(1). */

/**
 * @brief 
 * Envia a string colocada como par??metro para o terminal do PC 
 * por meio de uma varredura em todos os caracteres da string
 * e enviando-os utilizando a fun????o PC_SendChar() gerada pelo Processor Expert.
 * */
void SendP(char *s) {
	while(*s) {
		while(PC_SendChar(*s));
		s++;
	}
}

/**
 * @brief 
 * Envia a string colocada como par??metro para o m??dulo ESP 
 * por meio de uma varredura em todos os caracteres da string
 * e enviando-os utilizando a fun????o ESP_SendChar() gerada pelo Processor Expert.
 * */
void SendE(char *s) {
	while(*s) {
		while(ESP_SendChar(*s));
		s++;
	}
}

/** 
 * @brief
 * Realiza a inicializa????o autom??tica utilizando a sintaxe do protocolo MQTT, ou seja,
 * envia iterativamente os seguintes comandos ao m??dulo ESP na sequ??ncia abaixo:
 * 
 * 1. Conecta-se ao Wi-Fi;
 * 2. Conecta-se ao MQTT;
 * 3. Inscreve-se no t??pico /data;
 * 4. Inscreve-se no t??pico /hora;
 * 5. Inscreve-se no t??pico /user;
 * 6. Inscreve-se no t??pico /log;
 * 7. Inscreve-se no t??pico /apaga;
 * 8. Inscreve-se no t??pico /resposta;
 * 
 * Em seguida, envia para o terminal do PC uma mensagem de configura????es prontas.
 * Caso aconte??a falha em algum dos passos acima listados, uma tentativa ?? feita
 * novamente desde o in??cio a partir da pr??xima itera????o.
 * */
void AutoInit(void) {
	int a = 0;
	while(1) {
		if(a == 0) {
			SendE("CONNWIFI \"wifi-network\",\"wifi-password\"\r\n");
		} else if(a == 1) {
			SendE("CONNMQTT \"host-name\",1883,\"mac-address\"\r\n");
		} else if(a == 2) {
			SendE("SUBSCRIBE \"EA076/166974/data\"\r\n");
		} else if(a == 3) {
			SendE("SUBSCRIBE \"EA076/166974/hora\"\r\n");
		} else if(a == 4) {
			SendE("SUBSCRIBE \"EA076/166974/user\"\r\n");
		} else if(a == 5) {
			SendE("SUBSCRIBE \"EA076/166974/log\"\r\n");
		} else if(a == 6) {
			SendE("SUBSCRIBE \"EA076/166974/apaga\"\r\n");
		} else if(a == 7) {
			SendE("SUBSCRIBE \"EA076/166974/resposta\"\r\n");
		} else if(a == 8) {
			SendP("SETTINGS READY\r\n");
			break;
		}
		
		while(!eFlag);
		eFlag = 0;
		SendP(eMsg);
		
		if(!strcmp(eMsg, "CONNECT WIFI\r\n") || !strcmp(eMsg, "CONNECT MQTT\r\n") || !strcmp(eMsg, "OK SUBSCRIBE\r\n")) {
			a++;
		} else if(!strcmp(eMsg, "ERROR WIFI\r\n") || !strcmp(eMsg, "NO WIFI\r\n") || !strcmp(eMsg, "NOT CONNECTED\r\n")) {
			a = 0;
		}
	}
}

/*lint -save  -e970 Disable MISRA rule (6.3) checking. */
/**
 * @brief
 * A fun????o main realiza as seguintes fun????es:
 * 
 * 1. Inicializa os componentes do Processor Expert;
 * 2. Inicializa o m??dulo RTC;
 * 3. Inicializa automaticamente a conex??o MQTT (Fun????o AutoInit());
 * 4. Inicializa o Display e seus LEDs de ilumina????o traseira.
 * 4. Entra em um loop infinito, com as seguintes fun????es:
 * 	- Extrai o hor??rio atualizado do m??dulo RTC e os converte em strings com informa????o de data (aux0) e hora (aux1) e as imprime no display;
 * 	- Publica a mensagem "Digite a ID" e faz uma verifica????o constante se alguma tecla no teclado matricial foi pressionada (Interrup????es C1_OnInterrupt(), C2_OnInterrupt() e C3_OnInterrupt());
 * 		- Se a tecla digitada for uma tecla num??rica, envia para o display;
 * 		- Se a tecla digitada for "*", confirma o ID inserido;
 * 		- Se a tecla digitada for "#", apaga o ID inserido;
 * 	- Ap??s a entrada do ID, publica a mensagem "Digite a senha" e faz uma verifica????o constante se alguma tecla no teclado matricial foi pressionada (Interrup????es C1_OnInterrupt(), C2_OnInterrupt() e C3_OnInterrupt());
 * 		- Se a tecla digitada for uma tecla num??rica, envia para o display;
 * 		- Se a tecla digitada for "*", confirma o ID inserido;
 * 		- Se a tecla digitada for "#", apaga o ID inserido;
 * 	- Se uma mensagem for recebida pelo m??dulo ESP, faz a separa????o entre strings de t??pico e par??metro;
 * 	- Em seguida, faz a separa????o das substrings da string de par??metro e verifica a senha de acesso;
 * 	- Se a senha de acesso estiver correta, atua conforme a fun????o especificada a cada t??pico:
 * 		- T??pico /data: Alterar data;
 * 		- T??pico /hora: Alterar hora;
 * 		- T??pico /user: Cadastrar usu??rio (se o ID estiver dison??vel);
 * 		- T??pico /log: Imprimir o log de entradas;
 * 		- T??pico /apaga: Apagar log de entradas ou todos os cadastros de usu??rio;
 * 		- T??pico /resposta: Imprimir um feedback sobre o ??ltimo comando enviado.    
 */
int main(void)
/*lint -restore Enable MISRA rule (6.3) checking. */
{
  /* Write your local variable definition here */

  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
    PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
  /* For example: for(;;) { } */
	MyRTC = RTC_Init(NULL, FALSE);
	
	AutoInit();
	
	D_SetContrast(60);
	D_SetMode(FALSE);
	LD_On();
	
	while(1) {
		LEDB_On();
		
		RTC_GetTime(MyRTC, &MyTime);
				
		aux0[0] = (MyTime.Day)/10 + 0x30;
		aux0[1] = (MyTime.Day)%10 + 0x30;
		aux0[2] = '/';
		aux0[3] = (MyTime.Month)/10 + 0x30;
		aux0[4] = (MyTime.Month)%10 + 0x30;
		aux0[5] = '/';
		aux0[6] = (MyTime.Year)/1000 + 0x30;
		aux0[7] = ((MyTime.Year)%1000)/100 + 0x30;
		aux0[8] = ((MyTime.Year)%100)/10 + 0x30;
		aux0[9] = (MyTime.Year)%10 + 0x30;
		aux0[10] = 0;
		
		D_WriteLineStr(0, aux0);
		
		aux1[0] = (MyTime.Hour)/10 + 0x30;
		aux1[1] = (MyTime.Hour)%10 + 0x30;
		aux1[2] = ':';
		aux1[3] = (MyTime.Minute)/10 + 0x30;
		aux1[4] = (MyTime.Minute)%10 + 0x30;
		aux1[5] = ':';
		aux1[6] = (MyTime.Second)/10 + 0x30;
		aux1[7] = (MyTime.Second)%10 + 0x30;
		aux1[8] = 0;
		
		D_WriteLineStr(1, aux1);
		
		D_WriteLineStr(2, "Digite a ID");

		if(kFlag) {
			kFlag = 0;
			
			if(!state && i <= 2) {
				if(key == '*' && i == 0) {
				} else if(key == '*') {
					id[i] = 0;
					state = 1;
					i = 0;
				} else if(key == '#') {
					strcpy(id, "  ");
					i = 0;
				} else if(i < 2) {
					id[i] = key;
					i++;
				}
				D_WriteLineStr(3, id);
			}
			
			if(state && i <= 4) {
				D_WriteLineStr(4, "Digite a senha");
				if(key == '*' && i == 0) {
				} else if(key == '*' && i != 0) {
					
					D_Clear();
					LEDB_Off();
					
					addr = ((aux3[0] - 0x30) * 10 + (aux3[1] - 0x30)) * 16;
					EE24_ReadBlock(addr, user, 12);
					W_Waitms(5);
					addr = addr + 12;
					EE24_ReadBlock(addr, pass, 4);
					W_Waitms(5);
					
					if (!strcmp(pw, pass)) {
						addr = 0x200;
						EE24_ReadBlock(addr, block, 8);
						W_Waitms(5);
						while(block[0] != 0) {
							addr = addr + 8;
						  	EE24_ReadBlock(addr, block, 8);
							W_Waitms(5);
						}
						block[0] = (id[0] - 0x30) * 10 + (id[1] - 0x30);
						block[1] = (aux0[0] - 0x30) * 10 + (aux0[1] - 0x30);
						block[2] = (aux0[3] - 0x30) * 10 + (aux0[4] - 0x30);
						block[3] = (aux0[6] - 0x30) * 10 + (aux0[7] - 0x30);
						block[4] = (aux0[8] - 0x30) * 10 + (aux0[9] - 0x30);
						block[5] = (aux1[0] - 0x30) * 10 + (aux1[1] - 0x30);
						block[6] = (aux1[3] - 0x30) * 10 + (aux1[4] - 0x30);
						block[7] = (aux1[6] - 0x30) * 10 + (aux1[7] - 0x30);
						
						EE24_WriteBlock(addr, block, 8);
						W_Waitms(5);
						
						D_WriteLineStr(2, "Bem vindo,");
						D_WriteLineStr(3, user);
						LEDG_On();
					} else {
						D_WriteLineStr(2, "ACESSO NEGADO");
						LEDR_On();
					}
					
					W_Waitms(5000);
					LEDR_Off();
					LEDG_Off();
					D_Clear();
					strcpy(id, "  ");
					strcpy(pw, "    ");
					state = 0;
					i = 0;
					
				} else if(key == '#') {
					strcpy(pw, "    ");
					D_WriteLineStr(5, pw);
					i = 0;
				} else if(i < 4) {
					pw[i] = key;
					
					if(i == 0) {
						D_WriteLineStr(5, "* ");
					} else if(i == 1) {
						D_WriteLineStr(5, "**");
					} else if(i == 2) {
						D_WriteLineStr(5, "***");
					} else if(i == 3) {
						D_WriteLineStr(5, "****");
					}
					i++;
				}
			}
		}
		
		if(eFlag) {
			eFlag = 0;
			SendP(eMsg);
			
			i = 0;
			j = 0;
			while(eMsg[i] != ' ') {
				cStr[j] = eMsg[i]; 
				i++;
				j++;
			}
			cStr[j] = 0;
			i = i + 2;
			j = 0;
			while(eMsg[i] != ']') {
				tStr[j] = eMsg[i];
				i++;
				j++;
			}
			tStr[j] = 0;
			i = i + 3;
			j = 0;
			while(eMsg[i] != ']') {
				pStr[j] = eMsg[i];
				i++;
				j++;
			}
			pStr[j] = 0;
			i = 0;
			j = 0;
			
			if(strcmp(cStr, "MESSAGE")) {
			} else if(!strcmp(tStr, "EA076/166974/data")) {
				
				i = 1;
				j = 0;
				while(pStr[i] != '\"') {
					aux2[j] = pStr[i];
					i++;
					j++;
				}
				aux2[j] = 0;
				i = i + 3;
				j = 0;
				while(pStr[i] != '\"') {
					aux3[j] = pStr[i];
					i++;
					j++;
				}
				aux3[j] = 0;
				i = 0;
				j = 0;

				if(!strcmp(aux2, xs)) {
					
					MyTime.Day = (aux3[0] - 0x30) * 10 + (aux3[1] - 0x30);
					MyTime.Month = (aux3[3] - 0x30) * 10 + (aux3[4] - 0x30);
					MyTime.Year = (aux3[6] - 0x30) * 1000 + (aux3[7] - 0x30) * 100 + (aux3[8] - 0x30) * 10 + (aux3[9] - 0x30);
					
					MyTime.Hour = MyTime.Hour;
					MyTime.Minute = MyTime.Minute;
					MyTime.Second = MyTime.Second;
					
					RTC_SetTime(MyRTC, &MyTime);
					SendE("PUBLISH \"EA076/166974/resposta\",\"Data alterada com sucesso.\"\r\n");
					
				} else {
					SendE("PUBLISH \"EA076/166974/resposta\",\"String de acesso incorreta.\"\r\n");
				}
				
			} else if(!strcmp(tStr, "EA076/166974/hora")) {
				
				i = 1;
				j = 0;
				while(pStr[i] != '\"') {
					aux2[j] = pStr[i];
					i++;
					j++;
				}
				aux2[j] = 0;
				i = i + 3;
				j = 0;
				while(pStr[i] != '\"') {
					aux3[j] = pStr[i];
					i++;
					j++;
				}
				aux3[j] = 0;
				i = 0;
				j = 0;
				
				if(!strcmp(aux2, xs)) {
				
					MyTime.Day = MyTime.Day;
					MyTime.Month = MyTime.Month;
					MyTime.Year = MyTime.Year;
					
					MyTime.Hour = (aux3[0] - 0x30) * 10 + (aux3[1] - 0x30);
					MyTime.Minute = (aux3[3] - 0x30) * 10 + (aux3[4] - 0x30);
					MyTime.Second = (aux3[6] - 0x30) * 10 + (aux3[7] - 0x30);
					
					RTC_SetTime(MyRTC, &MyTime);
					
					SendE("PUBLISH \"EA076/166974/resposta\",\"Horario alterado com sucesso.\"\r\n");
				} else {
					SendE("PUBLISH \"EA076/166974/resposta\",\"String de acesso incorreta.\"\r\n");
				}
				
			} else if(!strcmp(tStr, "EA076/166974/user")) {
				i = 1;
				j = 0;
				while(pStr[i] != '\"') {
					aux2[j] = pStr[i];
					i++;
					j++;
				}
				aux2[j] = 0;
				
				if(!strcmp(aux2, xs)) {
					i = i + 3;
					j = 0;
					while(pStr[i] != '\"') {
						aux2[j] = pStr[i];
						i++;
						j++;
					}
					aux2[j] = 0;
					i = i + 3;
					j = 0;
					while(pStr[i] != '\"') {
						aux3[j] = pStr[i];
						i++;
						j++;
					}
					aux3[j] = 0;
					i = i + 3;
					j = 0;
					while(pStr[i] != '\"') {
						aux4[j] = pStr[i];
						i++;
						j++;
					}
					aux4[j] = 0;
					i = 0;
					j = 0;
					
					addr = ((aux3[0] - 0x30) * 10 + (aux3[1] - 0x30)) * 16;
					EE24_ReadBlock(addr, block, 16);
					W_Waitms(5);
					
					if(block[0] == 0 && addr <= 0x1F0) {
						EE24_WriteBlock(addr, aux2, 12);
						W_Waitms(5);
						addr = addr + 12;
						EE24_WriteBlock(addr, aux4, 4);
						W_Waitms(5);
						SendE("PUBLISH \"EA076/166974/resposta\",\"ID cadastrada com sucesso.\"\r\n");
					} else {
						SendE("PUBLISH \"EA076/166974/resposta\",\"ID indisponivel.\"\r\n");
					}
				} else {
					i = 0;
					j = 0;
					SendE("PUBLISH \"EA076/166974/resposta\",\"String de acesso incorreta.\"\r\n");
				}
			} else if(!strcmp(tStr, "EA076/166974/log")) {
				i = 1;
				j = 0;
				while(pStr[i] != '\"') {
					aux2[j] = pStr[i];
					i++;
					j++;
				}
				aux2[j] = 0;
				i = 0;
				j = 0;
				
				if(!strcmp(aux2, xs)) {
					addr = 0x1F8;
					block[0] = 1;
					strcpy(pMsg, "PUBLISH \"EA076/166974/log\",\"  @  /  /       :  :  \"\r\n");
					while(block[0] != 0) {
						addr = addr + 8;
					  	EE24_ReadBlock(addr, block, 8);
						W_Waitms(5);
						
						msg[28] = block[0]/10 + 0x30;
						msg[29] = block[0]%10 + 0x30;
						msg[31] = block[1]/10 + 0x30;
						msg[32] = block[1]%10 + 0x30;
						msg[34] = block[2]/10 + 0x30;
						msg[35] = block[2]%10 + 0x30;
						msg[37] = block[3]/10 + 0x30;
						msg[38] = block[3]%10 + 0x30;
						msg[39] = block[4]/10 + 0x30;
						msg[40] = block[4]%10 + 0x30;
						msg[42] = block[5]/10 + 0x30;
						msg[43] = block[5]%10 + 0x30;
						msg[45] = block[6]/10 + 0x30;
						msg[46] = block[6]%10 + 0x30;
						msg[48] = block[7]/10 + 0x30;
						msg[49] = block[7]%10 + 0x30;
						
						if(strcmp(pMsg, "PUBLISH \"EA076/166974/log\",\"00@00/00/0000 00:00:00\"\r\n")) {
							SendE(pMsg);
						}
					}					
				} //else {
					//SendE("PUBLISH \"EA076/166974/resposta\",\"String de acesso incorreta.\"\r\n");
				//}
			} else if(!strcmp(tStr, "EA076/166974/apaga")) {
				i = 0;
				j = 0;
				while(pStr[i] != ' ') {
					aux2[j] = pStr[i];
					i++;
					j++;
				}
				aux2[j] = 0;
				i = i + 2;
				j = 0;
				while(pStr[i] != '\"') {
					aux3[j] = pStr[i];
					i++;
					j++;
				}
				aux3[j] = 0;
				i = 0;
				j = 0;
				
				if(!strcmp(aux3, xs)) {
					if(!strcmp(aux2, "USER")) {
						addr = 0;
						while(addr < 0x200) {
							EE24_WriteByte(addr, 0);
							W_Waitms(5);
							addr++;
						}
						SendE("PUBLISH \"EA076/166974/resposta\",\"Registro de usuarios apagado.\"\r\n");
					} else if(!strcmp(aux2, "LOG")) {
						addr = 0x200;
						while(addr < 0x400) {
							EE24_WriteByte(addr, 0);
							W_Waitms(5);
							addr++;
						}
						SendE("PUBLISH \"EA076/166974/resposta\",\"Registro de entradas apagado.\"\r\n");
					}
				} else {
					SendE("PUBLISH \"EA076/166974/resposta\",\"String de acesso incorreta.\"\r\n");
				}
			}
		}
	}
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
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

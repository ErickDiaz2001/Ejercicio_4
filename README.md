INTRODUCCIÓN

Este informe describe un sistema de control que utiliza máquinas de estados con debounce para dos teclas. 
El sistema envía información sobre el estado de las teclas a través de la UART y enciende un LED correspondiente cuando se presiona una tecla.

CONFIGURACION

El system clock del microcontrolador se configuro a 72Mhz.

Se utilizara el timer 2, para obtener una interrupción cada 1 milisegundo se configuro un prescaler a 7200-1 y el counter period a 10. Ademas de habilitar la interrupcion. 
Para la comunicaion se utilizo el usart_3, se habilito la interrupcion para la resepcion de datos.

![image](https://github.com/ErickDiaz2001/Ejercicio_3/assets/169405943/cd9eade3-dc9e-49f4-9afd-71f11d27777c)


MÁQUINAS DE ESTADOS CON DEBOUNCE

![image](https://github.com/ErickDiaz2001/Ejercicio_4/assets/169405943/f791805e-a26c-49cf-807e-db8f2ac320b6)

Para evitar el rebote de las teclas, se utilizan máquinas de estados con debounce. Cada tecla tiene su propia máquina de estado, que consta de los siguientes estados:

•	Tecla Inactiva

•	Tecla Presionada

•	Tecla Liberada

Las máquinas de estado realizan las siguientes transiciones:

•	Tecla Inactiva a Tecla Presionada: 

Si se detecta un pulso de la tecla, guarda el valor_swich en valor_swich_anterior y pasa a Tecla Presionada.

•	Tecla Presionada: 

Inicia de un tiempo de debounce (10 ms).

•	Tecla Presionada a Tecla Liberada: 

Si pasaron los 10ms y el valor_swich es igual a valor_swich_anterior pasa al siquiente estado, sino vuelve a Tecla Inactiva.

•	Tecla Inactiva: 

Se detecta un estado bajo de la tecla.

ENVÍO DE INFORMACIÓN POR UART

Cada vez que una tecla se presiona o se suelta, el microcontrolador envía un mensaje a través de la UART. El mensaje contiene el identificador de la tecla (tecla 1 o tecla 2) 

y el nuevo estado de la tecla (presionada o liberada).

CONTROL DE LEDS POR UART

El sistema permite controlar el estado de cuatro LEDs mediante comandos enviados por UART. Los comandos deben tener el siguiente formato:


/* USER CODE BEGIN PTD */
typedef enum commandsEnum {
	COMMAND_LED1_TOGGLE = 'a',
	COMMAND_LED2_TOGGLE = 's',
	COMMAND_LED3_TOGGLE = 'd',
	COMMAND_LED4_TOGGLE = 'f'
} commands_LED_t;

typedef enum BUTTON_STATE {
	TECLA_INACTIVA,
	TECLA_PRESIONADA,
	TECLA_LIBERADA
} button_state_t;

/* USER CODE END PTD */

void debounce(uint8_t pin_tecla, uint8_t i)
{
	switch (estado_boton[i])
	{
	case TECLA_INACTIVA:
		valor_swich[i]= HAL_GPIO_ReadPin(GPIOA, pin_tecla);//se lee el estado del boton
		if (valor_swich[i] == 1)
		{
			valor_swich_anterior[i] = valor_swich[i];
			estado_boton[i] = TECLA_PRESIONADA;
		}
		break;
	case TECLA_PRESIONADA:
		valor_swich[i] = HAL_GPIO_ReadPin(GPIOA, pin_tecla);//se lee el estado del boton
		if (valor_swich[i] == valor_swich_anterior[i]|| contador[i] > TIEMPO_DEBOUNCE)//comparamos valor anterior con el valor actual
		{
			printf("Tecla apretada: TEC%d\n\r", i);
			valor_swich_anterior[i] = 0;
			estado_boton[i] = TECLA_LIBERADA;
		}
		else
		{
			estado_boton[i] = TECLA_INACTIVA;
		}
		contador[i] = contador[i] + 1;
		break;
	case TECLA_LIBERADA:
		valor_swich[i] = HAL_GPIO_ReadPin(GPIOA, pin_tecla);//se lee el estado del boton
		if (valor_swich[i] == 0)
		{
			printf("Tecla liberada: TEC%d\n\r", i);
			estado_boton[i] = TECLA_INACTIVA;
		}
		break;
	}
}


ANÁLISIS DE RESULTADOS 


CONCLUSIÓN

El sistema de control descrito en este informe utiliza máquinas de estados con debounce para dos teclas de manera efectiva.
Esto permite detectar de manera precisa la presión y liberación de las teclas, incluso en presencia de rebote.
El sistema también envía información sobre el estado de las teclas a través de la UART y enciende un LED correspondiente cuando se presiona una tecla.

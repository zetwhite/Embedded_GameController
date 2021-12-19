#include <stdio.h>
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#include "lcd.h"

/* function prototype */
void RCC_Configure(void);
void GPIO_Configure(void);
void ADC_Configure(void);
void USART1_2_Init(void);
void NVIC_Configure(void);

void Delay(void);

void sendDataUART1(uint16_t data);
void sendDataUART2(uint16_t data);

// global variables
uint16_t getx;

uint16_t xSlope;
uint16_t ySlope;

// xSlope (down ~ up) 2200 ~ 2300
// base 2255
uint16_t UP_THRESHOLD = 2300;
uint16_t DOWN_THRESHOLD = 2200;

// ySlope (right ~ left) 2145 ~ 2680
// base 2420
uint16_t RIGHT_THRESHOLD = 2200;
uint16_t LEFT_THRESHOLD = 2600;

int color[12] = {
  WHITE,        // 0
  CYAN,         // 1
  BLUE,         // 2
  RED,          // 3
  MAGENTA,      // 4
  LGRAY,        // 5
  GREEN,        // 6
  YELLOW,       // 7
  BROWN,        // 8
  BRRED,        // 9
  GRAY          // 10
};


void RCC_Configure(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //for bluetooth
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //for joystick 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //for joystick, for buttons
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); //for button

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // for tilt sensor
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // for ADC

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // for serial
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); // for bluetooth
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void GPIO_Configure(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //JoyStick Setting
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_9); 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOB, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    
    //For Button 
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_2 | GPIO_Pin_3); 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOE, &GPIO_InitStructure); 
    
    // For Tilt Sensor PC0:X, PC1:Y
    GPIO_InitStructure.GPIO_Pin = (GPIO_Pin_0 | GPIO_Pin_1);
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* UART1 pin setting (PC - board serial communication) */
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /* UART2 pin setting (bluetooth communication)*/
    //TX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //RX
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ADC_Configure(void)
{
    ADC_InitTypeDef ADC_InitStructure;
  
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;
    
    ADC_Init(ADC1, &ADC_InitStructure);
    
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);
    
    ADC_Cmd(ADC1, ENABLE);
    
    ADC_ResetCalibration(ADC1);
    
    while( ADC_GetResetCalibrationStatus(ADC1) )
      ;

    ADC_StartCalibration(ADC1);
    
    while( ADC_GetCalibrationStatus(ADC1) )
      ;

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void USART1_Init(void)
{
    USART_InitTypeDef USART_InitStructure;
    
    USART_Cmd(USART1, ENABLE);
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_Mode = (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);
    USART_InitStructure.USART_WordLength = 0;
    USART_InitStructure.USART_StopBits = 0;
    USART_InitStructure.USART_Parity = 0;
    USART_InitStructure.USART_HardwareFlowControl = 0;
    USART_Init(USART1, &USART_InitStructure);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART2_Init(void)
{
    USART_InitTypeDef USART_InitStructure;

    USART_Cmd(USART2, ENABLE);
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_Mode = (USART_CR1_RE | USART_CR1_TE | USART_CR1_UE);
    USART_InitStructure.USART_WordLength = 0;
    USART_InitStructure.USART_StopBits = 0;
    USART_InitStructure.USART_Parity = 0;
    USART_InitStructure.USART_HardwareFlowControl = 0;
    USART_Init(USART2, &USART_InitStructure);
        
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void EXTI_Configure(void){
    EXTI_InitTypeDef EXTI_InitStructure; 
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0);
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
 
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource2);
    EXTI_InitStructure.EXTI_Line = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource3);
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource5);
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);
    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource9);
    EXTI_InitStructure.EXTI_Line = EXTI_Line9;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

void NVIC_Configure(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // UART1
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    // UART2
    NVIC_EnableIRQ(USART2_IRQn);
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   
    // Joystick Down, Left, Right
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // Joystick Up
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // Button 1
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // Button 2
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ADC
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

// Joystick Up
void EXTI0_IRQHandler(void) {
        
	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) {
                  USART_SendData(USART2, '0');
                  printf("JoyStick_4(PB0)\n"); 
	}
        EXTI_ClearITPendingBit(EXTI_Line0);
}

// Button 1
void EXTI2_IRQHandler(void) {

	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)) {
                  USART_SendData(USART2, '2');
                  printf("Button1_4(PE2)\n"); 
	}
        EXTI_ClearITPendingBit(EXTI_Line2);
}

// Button 2
void EXTI3_IRQHandler(void) {

	if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)) {
                  USART_SendData(USART2, '3');
                  printf("Button2_4(PE3)\n"); 
	}
        EXTI_ClearITPendingBit(EXTI_Line3);
}

void EXTI9_5_IRQHandler(void) {

    // Joystick Down
    if (EXTI_GetITStatus(EXTI_Line6) != RESET) {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)) {
                  USART_SendData(USART2, '6');
                  printf("JoyStick_1(PB6)\n"); 
          }
          EXTI_ClearITPendingBit(EXTI_Line6);
    }
    // Joystick Left
    else if (EXTI_GetITStatus(EXTI_Line5) != RESET) {
          if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)) {
                  USART_SendData(USART2, '5');
                  printf("JoyStick_2(PB5)\n"); 

          }
          EXTI_ClearITPendingBit(EXTI_Line5);
    }
    // Joystick Right
    else if (EXTI_GetITStatus(EXTI_Line9) != RESET) {
          if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)) {
                  USART_SendData(USART2, '9');
                  printf("JoyStick_3(PB9)\n"); 
          }
          EXTI_ClearITPendingBit(EXTI_Line9);
    }
}

void USART1_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART1,USART_IT_RXNE)!= RESET){
    	// the most recent received data by the USART1 peripheral
        word = USART_ReceiveData(USART1);

        sendDataUART2(word);
        
    	USART_ClearITPendingBit(USART1,USART_IT_RXNE);
    }
} 

void USART2_IRQHandler() {
    uint16_t word;
    if(USART_GetITStatus(USART2,USART_IT_RXNE)!= RESET){
    	// the most recent received data by the USART2 peripheral
        word = USART_ReceiveData(USART2);

        // TODO implement
        sendDataUART1(word);
        // clear 'Read data register not empty' flag
    	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
    }
}

void ADC1_2_IRQHandler(void){
  //Check for end of conversion
  if(ADC_GetITStatus(ADC1, ADC_IT_EOC)){
    if( getx == 1 ) {
      xSlope = ADC_GetConversionValue(ADC1);
    }
    else {
      ySlope = ADC_GetConversionValue(ADC1);
    }
    //Clear interrupt bit
    ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
  }
}

void Delay(void) {
	int i;

	for (i = 0; i < 2000000; i++) {}
}

// send to PC serial port for bluetooth configuration
void sendDataUART1(uint16_t data) {
	/* Wait till TC is set */
	USART_SendData(USART1, data);
}

// send to bluetooth module
void sendDataUART2(uint16_t data) {
	/* Wait till TC is set */
	USART_SendData(USART2, data);
}

int main(void)
{
    SystemInit();
    RCC_Configure();
    GPIO_Configure();
    ADC_Configure();
    USART1_Init();
    USART2_Init(); 
    EXTI_Configure();   
    NVIC_Configure();
    /*
    LCD & printf is for debug

    LCD_Init();
    LCD_Clear(WHITE);

    LCD_ShowString(0x10, 0x10, "DEBUG", color[3], color[0]);
    LCD_Fill(0x20, 0x80, 0x30, 0x90,BROWN );
    */

    while (1) {
      // get xSlope from ADC1
      if( getx == 1 ) {
        // Change ADC Channel for get xSlope from PC0 port
        ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_28Cycles5);
        
        // enable interrupt only when slope value need
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
        
        // if xSlope value exceeds threshold, send 'U' to bluetooth module
        if( xSlope > UP_THRESHOLD ) {
          sendDataUART2('U');
          printf("UP\n");
          //LCD_Fill(0x20, 0x70, 0x30, 0x80, RED);
        }
        else {
          //LCD_Fill(0x20, 0x70, 0x30, 0x80, WHITE);
        }
        
        // if xSlope value is less than threshold, send 'D' to bluetooth module
        if( xSlope < DOWN_THRESHOLD ) {
          sendDataUART2('D'); 
          printf("DOWN\n");
          //LCD_Fill(0x20, 0x90, 0x30, 0xA0, YELLOW);
        }
        else {
          //LCD_Fill(0x20, 0x90, 0x30, 0xA0, WHITE);
        }
        //LCD_ShowNum(0x10, 0x40, xSlope, 10, color[3], color[0]);
        
        // done getting slope value, disable interrupt
        ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
        getx = 0;
      }
      
      // get ySlope from ADC1
      else {
        // Change ADC Channel for get ySlope from PC1 port
        ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_28Cycles5);
        ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
        
        // if ySlope value exceeds threshold, send 'L' to bluetooth module
        if( ySlope > LEFT_THRESHOLD ) {
          sendDataUART2('L');
          printf("LEFT\n");
          //LCD_Fill(0x10, 0x80, 0x20, 0x90, GREEN);
        }
        else {
          //LCD_Fill(0x10, 0x80, 0x20, 0x90, WHITE);
        }
        
        // if ySlope value is less than threshold, send 'R' to bluetooth module
        if( ySlope < RIGHT_THRESHOLD ) {
          sendDataUART2('R');
          printf("RIGHT\n");
          //LCD_Fill(0x30, 0x80, 0x40, 0x90, BLUE);
        }
        else {
          //LCD_Fill(0x30, 0x80, 0x40, 0x90, WHITE);
        }
        //LCD_ShowNum(0x10, 0x60, ySlope, 10, color[3], color[0]);
        ADC_ITConfig(ADC1, ADC_IT_EOC, DISABLE);
        getx = 1;
      }  
      Delay();
    }
    return 0;
}
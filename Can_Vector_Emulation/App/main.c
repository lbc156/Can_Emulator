/**
  ******************************************************************************
  * @file    GPIO/IOToggle/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "HMACSHA1.h"
#include "stm32f10x_it.h"
#include "isotp.h"
#include "isotp_defines.h"
/** @addtogroup Base on: STM32F10x_StdPeriph_Examples GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ISOTP_BUFSIZE 8
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef 		GPIO_InitStructure;
CAN_InitTypeDef        	CAN_InitStructure;
CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
RCC_ClocksTypeDef		RCC_Clock;
CanTxMsg 				TxMessage;
CanTxMsg 				ResponseTxMessage;

volatile uint32_t Timingdelay;
uint8_t string_sha1_src[4];
unsigned char string_dest[20] = {0};
uint8_t OEM_key[8] = {0x0A, 0xBA, 0x22, 0x10, 0x69, 0x38, 0xB4, 0xC7};
uint8_t ret;
/* Alloc IsoTpLink statically in RAM */
static IsoTpLink g_link;

/* Alloc send and receive buffer statically in RAM */
static uint8_t g_isotpRecvBuf[24];
static uint8_t g_isotpSendBuf[24];
//							  ={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};

const uint8_t payload[24]={0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04,0x05,0x05,0x06,0x06,0x07,0x07,0x08,0x08,0x01,0x01,0x02,0x02,0x03,0x03,0x04,0x04};
uint16_t payload_size = sizeof(payload);
//uint_8 SecureCom_Oem_Key_Raw[8];
/* Private function prototypes -----------------------------------------------*/
void NVIC_Config(void);
void CAN_Config(void);
void VCU_First_Message();
void Delay_ms_sys(uint16_t time);
void VCU_Response_Message();
int  isotp_user_send_can(const uint32_t arbitration_id, const uint8_t* data, const uint8_t size);
uint32_t isotp_user_get_ms(void);
uint16_t* out_size;
uint8_t u8count =0;
uint8_t data_receive[8]={0x30,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/* NVIC configuration */
	NVIC_Config();

	// cau hinh systick ngat moi 1ms
	SysTick_Config(SystemCoreClock/1000);

	  /* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PC13 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* CAN configuration */
	CAN_Config();
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

	/* Initialize link, 0x100 is the CAN ID you send with */

	isotp_init_link(&g_link, 0x100, g_isotpSendBuf, sizeof(g_isotpSendBuf), g_isotpRecvBuf, sizeof(g_isotpRecvBuf));
	ITStatus a =RESET;
    while(1) {
    	 //
    	 //a = CAN_GetITStatus(CAN1, CAN_IT_FMP0);
    	 Delay_ms_sys(2000);
    	 a = CAN_GetITStatus(CAN1, CAN_IT_FMP1);
    	 if(a == SET){

    	 }
    	 isotp_on_can_message(&g_link, data_receive, 0x08);
         /* Poll link to handle multiple frame transmition */
         isotp_poll(&g_link);

         /* You can receive message with isotp_receive.
            payload is upper layer message buffer, usually UDS;
            payload_size is payload buffer size;
            out_size is the actuall read size;
            */
         ret = isotp_receive(&g_link, payload, payload_size, &out_size);
         if (ISOTP_RET_OK == ret) {
             /* Handle received message */
         }

         /* And send message with isotp_send */
         ret = isotp_send(&g_link, payload, payload_size);
         if (ISOTP_RET_OK == ret) {
             /* Send ok */
         } else {
             /* An error occured */
         }

         /* In case you want to send data w/ functional addressing, use isotp_send_with_id */
         ret = isotp_send_with_id(&g_link, 0x7df, payload, payload_size);
         if (ISOTP_RET_OK == ret) {
             /* Send ok */
         } else {
             /* Error occur */
         }
     }
	/*
	VCU_First_Message();
	Delay_ms_sys(100);
	while(Flag_indicate_Rx)
	{
		//Delay_ms_sys(7000);
		VCU_Response_Message();
		Flag_indicate_Rx = 0u;
	}

	Delay_ms_sys(100);
	VCU_First_Message();
	Delay_ms_sys(100);
	while(Flag_indicate_Rx)
	{
		//Delay_ms_sys(7000);
		VCU_Response_Message();
		Flag_indicate_Rx = 0u;
	}

	Delay_ms_sys(100);
	VCU_First_Message();
	Delay_ms_sys(100);
	while(Flag_indicate_Rx)
	{
		//Delay_ms_sys(7000);
		VCU_Response_Message();
		Flag_indicate_Rx = 0u;
	}

	Delay_ms_sys(100);
	VCU_First_Message();
	*/
}

/**
  * @brief  Transmit First CAN Message to BMS 071A 2701 FFFF FFFF
  * @param  None
  * @retval None
  */
void VCU_First_Message()
{
	TxMessage.Data[0] = 0x07;
	TxMessage.Data[1] = 0x1A;
	TxMessage.Data[2] = 0x27;
	TxMessage.Data[3] = 0x01;
	TxMessage.Data[4] = 0xFF;
	TxMessage.Data[5] = 0xFF;
	TxMessage.Data[6] = 0xFF;
	TxMessage.Data[7] = 0xFF;
	CAN_Transmit(CAN1, &TxMessage);
}

/**
  * @brief  Transmit Response CAN Message to BMS 071A 2702 hmac hmac hmac hmac
  * @param  None
  * @retval None
  */
void VCU_Response_Message()
{
	HMACSHA1(OEM_key, &(Check_data[4]), 4, string_sha1_src, 4);
	TxMessage.Data[0] = 0x07;
	TxMessage.Data[1] = 0x1A;
	TxMessage.Data[2] = 0x27;
	TxMessage.Data[3] = 0x02;
	TxMessage.Data[4] = string_sha1_src[3];
	TxMessage.Data[5] = string_sha1_src[2];
	TxMessage.Data[6] = string_sha1_src[1];
	TxMessage.Data[7] = string_sha1_src[0];
	CAN_Transmit(CAN1, &TxMessage);
}

/**
  * @brief  Configures the NVIC for CAN.
  * @param  None
  * @retval None
  */
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the CAN.
  * @param  None
  * @retval None
  */
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* GPIO clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

  /* Configure CAN pin: RX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);

  /* CANx Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);


  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

  /* CAN Baudrate = 1MBps*/
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_15tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_Init(CAN1, &CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* Transmit */
//  ResponseTxMessage.StdId = 0x74A;
//  ResponseTxMessage.ExtId = 0x01;
//  ResponseTxMessage.RTR = CAN_RTR_DATA;
//  ResponseTxMessage.IDE = CAN_ID_STD;
//  ResponseTxMessage.DLC = 8;
}

/**
  * @brief  Delay
  * @param  None
  * @retval None
  */
void Delay_ms_sys(uint16_t time)
{
	Timingdelay = time;						// gan bien dem bang tham so truyen vao
	while(Timingdelay!=0);				// ham cho doi bien dem bang 0.
}

/* required, this must send a single CAN message with the given arbitration
    * ID (i.e. the CAN message ID) and data. The size will never be more than 8
    * bytes. */
int  isotp_user_send_can(const uint32_t arbitration_id,
                             const uint8_t* data, const uint8_t size) {
	TxMessage.StdId = arbitration_id;
	TxMessage.ExtId = 0x01;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = size;

	for(int i=0;i<8;i++){
		TxMessage.Data[i] = data[i];
	}
	CAN_Transmit(CAN1, &TxMessage);
}

/* required, return system tick, unit is millisecond */
uint32_t isotp_user_get_ms(void) {
        // ...
}

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

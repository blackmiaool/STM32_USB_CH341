#include "usb_ch341.h"
#include "usb_lib.h"

void delay_ms(u16 nms);
void delay_us(u32 nus);
u16 ch341_baud;

void USB_CH341_Init()
{
    Set_USBClock();
	USB_Interrupts_Config();	
	USB_Init();	
    delay_ms(1000);
    delay_ms(1000);
}

void USB_Cable_Config(FunctionalState NewState)
{
  if (NewState != DISABLE)
  {
    GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
  else
  {
    GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
  }
}

void USB_send(u8 *buf,u8 lenth)
{
    if(lenth>32)//max package size=32
    {
        lenth-=32;
        USB_send(buf,lenth);
        buf+=lenth;
        lenth=32;
    }
        
    UserToPMABufferCopy(buf, ENDP2_TXADDR, lenth);
    SetEPTxCount(ENDP2, lenth);
    SetEPTxValid(ENDP2);
    while(GetEPTxStatus(2)!=32);//wait for send finished
}
void USB_receive(const u8 *buf,u8 lenth)
{
  //add your own code and remove this 
  USB_send((u8 *)buf,lenth);
}
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

//³õÊ¼»¯USB

void USB_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

    GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);
    

	pInformation = &Device_Info;
	pInformation->ControlState = 2;
	pProperty = &Device_Property;
	pUser_Standard_Requests = &User_Standard_Requests;
	/* Initialize devices one by one */
	pProperty->Init();
}
/*******************************************************************************
* Function Name  : SetCNTR.
* Description    : Set the CNTR register value.
* Input          : wRegValue: new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetCNTR(u16 wRegValue)
{
  _SetCNTR(wRegValue);
}
/*******************************************************************************
* Function Name  : GetCNTR.
* Description    : returns the CNTR register value.
* Input          : None.
* Output         : None.
* Return         : CNTR register Value.
*******************************************************************************/
u16 GetCNTR(void)
{
  return(_GetCNTR());
}

/*******************************************************************************
* Function Name  : SetISTR.
* Description    : Set the ISTR register value.
* Input          : wRegValue: new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetISTR(u16 wRegValue)
{
  _SetISTR(wRegValue);
}

/*******************************************************************************
* Function Name  : GetISTR
* Description    : Returns the ISTR register value.
* Input          : None.
* Output         : None.
* Return         : ISTR register Value
*******************************************************************************/
u16 GetISTR(void)
{
  return(_GetISTR());
}

/*******************************************************************************
* Function Name  : GetFNR
* Description    : Returns the FNR register value.
* Input          : None.
* Output         : None.
* Return         : FNR register Value
*******************************************************************************/
u16 GetFNR(void)
{
  return(_GetFNR());
}

/*******************************************************************************
* Function Name  : SetDADDR
* Description    : Set the DADDR register value.
* Input          : wRegValue: new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetDADDR(u16 wRegValue)
{
  _SetDADDR(wRegValue);
}

/*******************************************************************************
* Function Name  : GetDADDR
* Description    : Returns the DADDR register value.
* Input          : None.
* Output         : None.
* Return         : DADDR register Value
*******************************************************************************/
u16 GetDADDR(void)
{
  return(_GetDADDR());
}

/*******************************************************************************
* Function Name  : SetBTABLE
* Description    : Set the BTABLE.
* Input          : wRegValue: New register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetBTABLE(u16 wRegValue)
{
  _SetBTABLE(wRegValue);
}

/*******************************************************************************
* Function Name  : GetBTABLE.
* Description    : Returns the BTABLE register value.
* Input          : None. 
* Output         : None.
* Return         : BTABLE address.
*******************************************************************************/
u16 GetBTABLE(void)
{
  return(_GetBTABLE());
}

/*******************************************************************************
* Function Name  : SetENDPOINT
* Description    : Setthe Endpoint register value.
* Input          : bEpNum: Endpoint Number. 
*                  wRegValue.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetENDPOINT(u8 bEpNum, u16 wRegValue)
{
  _SetENDPOINT(bEpNum, wRegValue);
}

/*******************************************************************************
* Function Name  : GetENDPOINT
* Description    : Return the Endpoint register value.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint register value.
*******************************************************************************/
u16 GetENDPOINT(u8 bEpNum)
{
  return(_GetENDPOINT(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPType
* Description    : sets the type in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
*                  wType: type definition.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPType(u8 bEpNum, u16 wType)
{
  _SetEPType(bEpNum, wType);
}

/*******************************************************************************
* Function Name  : GetEPType
* Description    : Returns the endpoint type.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint Type
*******************************************************************************/
u16 GetEPType(u8 bEpNum)
{
  return(_GetEPType(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPTxStatus
* Description    : Set the status of Tx endpoint.
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxStatus(u8 bEpNum, u16 wState)
{
  _SetEPTxStatus(bEpNum, wState);
}

/*******************************************************************************
* Function Name  : SetEPRxStatus
* Description    : Set the status of Rx endpoint.
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxStatus(u8 bEpNum, u16 wState)
{
  _SetEPRxStatus(bEpNum, wState);
}

/*******************************************************************************
* Function Name  : SetDouBleBuffEPStall
* Description    : sets the status for Double Buffer Endpoint to STALL
* Input          : bEpNum: Endpoint Number. 
*                  bDir: Endpoint direction.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetDouBleBuffEPStall(u8 bEpNum, u8 bDir)
{
  u16 Endpoint_DTOG_Status;
  Endpoint_DTOG_Status = GetENDPOINT(bEpNum);
  if (bDir == EP_DBUF_OUT)
  { /* OUT double buffered endpoint */
    _SetENDPOINT(bEpNum, Endpoint_DTOG_Status & ~EPRX_DTOG1);
  }
  else if (bDir == EP_DBUF_IN)
  { /* IN double buffered endpoint */
    _SetENDPOINT(bEpNum, Endpoint_DTOG_Status & ~EPTX_DTOG1);
  }
}

/*******************************************************************************
* Function Name  : GetEPTxStatus
* Description    : Returns the endpoint Tx status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint TX Status
*******************************************************************************/
u16 GetEPTxStatus(u8 bEpNum)
{
  return(_GetEPTxStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : GetEPRxStatus
* Description    : Returns the endpoint Rx status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint RX Status
*******************************************************************************/
u16 GetEPRxStatus(u8 bEpNum)
{
  return(_GetEPRxStatus(bEpNum));
}

/*******************************************************************************
* Function Name  : SetEPTxValid
* Description    : Valid the endpoint Tx Status.
* Input          : bEpNum: Endpoint Number.  
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxValid(u8 bEpNum)
{
  _SetEPTxStatus(bEpNum, EP_TX_VALID);
}

/*******************************************************************************
* Function Name  : SetEPRxValid
* Description    : Valid the endpoint Rx Status.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxValid(u8 bEpNum)
{
  _SetEPRxStatus(bEpNum, EP_RX_VALID);
}

/*******************************************************************************
* Function Name  : SetEP_KIND
* Description    : Clear the EP_KIND bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEP_KIND(u8 bEpNum)
{
  _SetEP_KIND(bEpNum);
}

/*******************************************************************************
* Function Name  : ClearEP_KIND
* Description    : set the  EP_KIND bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEP_KIND(u8 bEpNum)
{
  _ClearEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : Clear_Status_Out
* Description    : Clear the Status Out of the related Endpoint
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void Clear_Status_Out(u8 bEpNum)
{
  _ClearEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : Set_Status_Out
* Description    : Set the Status Out of the related Endpoint
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_Status_Out(u8 bEpNum)
{
  _SetEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : SetEPDoubleBuff
* Description    : Enable the double buffer feature for the endpoint. 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDoubleBuff(u8 bEpNum)
{
  _SetEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearEPDoubleBuff
* Description    : Disable the double buffer feature for the endpoint. 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEPDoubleBuff(u8 bEpNum)
{
  _ClearEP_KIND(bEpNum);
}
/*******************************************************************************
* Function Name  : GetTxStallStatus
* Description    : Returns the Stall status of the Tx endpoint.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Tx Stall status.
*******************************************************************************/
u16 GetTxStallStatus(u8 bEpNum)
{
  return(_GetTxStallStatus(bEpNum));
}
/*******************************************************************************
* Function Name  : GetRxStallStatus
* Description    : Returns the Stall status of the Rx endpoint. 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx Stall status.
*******************************************************************************/
u16 GetRxStallStatus(u8 bEpNum)
{
  return(_GetRxStallStatus(bEpNum));
}
/*******************************************************************************
* Function Name  : ClearEP_CTR_RX
* Description    : Clear the CTR_RX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEP_CTR_RX(u8 bEpNum)
{
  _ClearEP_CTR_RX(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearEP_CTR_TX
* Description    : Clear the CTR_TX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearEP_CTR_TX(u8 bEpNum)
{
  _ClearEP_CTR_TX(bEpNum);
}
/*******************************************************************************
* Function Name  : ToggleDTOG_RX
* Description    : Toggle the DTOG_RX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ToggleDTOG_RX(u8 bEpNum)
{
  _ToggleDTOG_RX(bEpNum);
}
/*******************************************************************************
* Function Name  : ToggleDTOG_TX
* Description    : Toggle the DTOG_TX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ToggleDTOG_TX(u8 bEpNum)
{
  _ToggleDTOG_TX(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearDTOG_RX.
* Description    : Clear the DTOG_RX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearDTOG_RX(u8 bEpNum)
{
  _ClearDTOG_RX(bEpNum);
}
/*******************************************************************************
* Function Name  : ClearDTOG_TX.
* Description    : Clear the DTOG_TX bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void ClearDTOG_TX(u8 bEpNum)
{
  _ClearDTOG_TX(bEpNum);
}
/*******************************************************************************
* Function Name  : SetEPAddress
* Description    : Set the endpoint address.
* Input          : bEpNum: Endpoint Number.
*                  bAddr: New endpoint address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPAddress(u8 bEpNum, u8 bAddr)
{
  _SetEPAddress(bEpNum, bAddr);
}
/*******************************************************************************
* Function Name  : GetEPAddress
* Description    : Get the endpoint address.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint address.
*******************************************************************************/
u8 GetEPAddress(u8 bEpNum)
{
  return(_GetEPAddress(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPTxAddr
* Description    : Set the endpoint Tx buffer address.
* Input          : bEpNum: Endpoint Number.
*                  wAddr: new address. 
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxAddr(u8 bEpNum, u16 wAddr)
{
  _SetEPTxAddr(bEpNum, wAddr);
}
/*******************************************************************************
* Function Name  : SetEPRxAddr
* Description    : Set the endpoint Rx buffer address.
* Input          : bEpNum: Endpoint Number.
*                  wAddr: new address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxAddr(u8 bEpNum, u16 wAddr)
{
  _SetEPRxAddr(bEpNum, wAddr);
}
/*******************************************************************************
* Function Name  : GetEPTxAddr
* Description    : Returns the endpoint Tx buffer address.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx buffer address. 
*******************************************************************************/
u16 GetEPTxAddr(u8 bEpNum)
{
  return(_GetEPTxAddr(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPRxAddr.
* Description    : Returns the endpoint Rx buffer address.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx buffer address.
*******************************************************************************/
u16 GetEPRxAddr(u8 bEpNum)
{
  return(_GetEPRxAddr(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPTxCount.
* Description    : Set the Tx count.
* Input          : bEpNum: Endpoint Number.
*                  wCount: new count value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPTxCount(u8 bEpNum, u16 wCount)
{
  _SetEPTxCount(bEpNum, wCount);
}
/*******************************************************************************
* Function Name  : SetEPCountRxReg.
* Description    : Set the Count Rx Register value.
* Input          : *pdwReg: point to the register.
*                  wCount: the new register value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPCountRxReg(u32 *pdwReg, u16 wCount)
{
  _SetEPCountRxReg(dwReg, wCount);
}
/*******************************************************************************
* Function Name  : SetEPRxCount
* Description    : Set the Rx count.
* Input          : bEpNum: Endpoint Number. 
*                  wCount: the new count value.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPRxCount(u8 bEpNum, u16 wCount)
{
  _SetEPRxCount(bEpNum, wCount);
}
/*******************************************************************************
* Function Name  : GetEPTxCount
* Description    : Get the Tx count.
* Input          : bEpNum: Endpoint Number. 
* Output         : None
* Return         : Tx count value.
*******************************************************************************/
u16 GetEPTxCount(u8 bEpNum)
{
  return(_GetEPTxCount(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPRxCount
* Description    : Get the Rx count.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Rx count value.
*******************************************************************************/
u16 GetEPRxCount(u8 bEpNum)
{
  return(_GetEPRxCount(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPDblBuffAddr
* Description    : Set the addresses of the buffer 0 and 1.
* Input          : bEpNum: Endpoint Number.  
*                  wBuf0Addr: new address of buffer 0. 
*                  wBuf1Addr: new address of buffer 1.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuffAddr(u8 bEpNum, u16 wBuf0Addr, u16 wBuf1Addr)
{
  _SetEPDblBuffAddr(bEpNum, wBuf0Addr, wBuf1Addr);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf0Addr
* Description    : Set the Buffer 1 address.
* Input          : bEpNum: Endpoint Number
*                  wBuf0Addr: new address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf0Addr(u8 bEpNum, u16 wBuf0Addr)
{
  _SetEPDblBuf0Addr(bEpNum, wBuf0Addr);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf1Addr
* Description    : Set the Buffer 1 address.
* Input          : bEpNum: Endpoint Number
*                  wBuf1Addr: new address.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf1Addr(u8 bEpNum, u16 wBuf1Addr)
{
  _SetEPDblBuf1Addr(bEpNum, wBuf1Addr);
}
/*******************************************************************************
* Function Name  : GetEPDblBuf0Addr
* Description    : Returns the address of the Buffer 0.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : None.
*******************************************************************************/
u16 GetEPDblBuf0Addr(u8 bEpNum)
{
  return(_GetEPDblBuf0Addr(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPDblBuf1Addr
* Description    : Returns the address of the Buffer 1.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : Address of the Buffer 1.
*******************************************************************************/
u16 GetEPDblBuf1Addr(u8 bEpNum)
{
  return(_GetEPDblBuf1Addr(bEpNum));
}
/*******************************************************************************
* Function Name  : SetEPDblBuffCount
* Description    : Set the number of bytes for a double Buffer 
*                  endpoint.
* Input          : bEpNum,bDir, wCount
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuffCount(u8 bEpNum, u8 bDir, u16 wCount)
{
  _SetEPDblBuffCount(bEpNum, bDir, wCount);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf0Count
* Description    : Set the number of bytes in the buffer 0 of a double Buffer 
*                  endpoint.
* Input          : bEpNum, bDir,  wCount
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf0Count(u8 bEpNum, u8 bDir, u16 wCount)
{
  _SetEPDblBuf0Count(bEpNum, bDir, wCount);
}
/*******************************************************************************
* Function Name  : SetEPDblBuf1Count
* Description    : Set the number of bytes in the buffer 0 of a double Buffer 
*                  endpoint.
* Input          : bEpNum,  bDir,  wCount
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetEPDblBuf1Count(u8 bEpNum, u8 bDir, u16 wCount)
{
  _SetEPDblBuf1Count(bEpNum, bDir, wCount);
}
/*******************************************************************************
* Function Name  : GetEPDblBuf0Count
* Description    : Returns the number of byte received in the buffer 0 of a double
*                  Buffer endpoint.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : Endpoint Buffer 0 count
*******************************************************************************/
u16 GetEPDblBuf0Count(u8 bEpNum)
{
  return(_GetEPDblBuf0Count(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPDblBuf1Count
* Description    : Returns the number of data received in the buffer 1 of a double
*                  Buffer endpoint.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : Endpoint Buffer 1 count.
*******************************************************************************/
u16 GetEPDblBuf1Count(u8 bEpNum)
{
  return(_GetEPDblBuf1Count(bEpNum));
}
/*******************************************************************************
* Function Name  : GetEPDblBufDir
* Description    : gets direction of the double buffered endpoint
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : EP_DBUF_OUT, EP_DBUF_IN,
*                  EP_DBUF_ERR if the endpoint counter not yet programmed.
*******************************************************************************/
EP_DBUF_DIR GetEPDblBufDir(u8 bEpNum)
{
  if ((u16)(*_pEPRxCount(bEpNum) & 0xFC00) != 0)
    return(EP_DBUF_OUT);
  else if (((u16)(*_pEPTxCount(bEpNum)) & 0x03FF) != 0)
    return(EP_DBUF_IN);
  else
    return(EP_DBUF_ERR);
}
/*******************************************************************************
* Function Name  : FreeUserBuffer
* Description    : free buffer used from the application realizing it to the line
                   toggles bit SW_BUF in the double buffered endpoint register
* Input          : bEpNum, bDir
* Output         : None.
* Return         : None.
*******************************************************************************/
void FreeUserBuffer(u8 bEpNum, u8 bDir)
{
  if (bDir == EP_DBUF_OUT)
  { /* OUT double buffered endpoint */
    _ToggleDTOG_TX(bEpNum);
  }
  else if (bDir == EP_DBUF_IN)
  { /* IN double buffered endpoint */
    _ToggleDTOG_RX(bEpNum);
  }
}

/*******************************************************************************
* Function Name  : ToWord
* Description    : merge two byte in a word.
* Input          : bh: byte high, bl: bytes low.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
u16 ToWord(u8 bh, u8 bl)
{
  u16 wRet;
  wRet = (u16)bl | ((u16)bh << 8);
  return(wRet);
}
/*******************************************************************************
* Function Name  : ByteSwap
* Description    : Swap two byte in a word.
* Input          : wSwW: word to Swap.
* Output         : None.
* Return         : resulted word.
*******************************************************************************/
u16 ByteSwap(u16 wSwW)
{
  u8 bTemp;
  u16 wRet;
  bTemp = (u8)(wSwW & 0xff);
  wRet =  (wSwW >> 8) | ((u16)bTemp << 8);
  return(wRet);
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_pwr.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Connection/disconnection & power management
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu32 bDeviceState = UNCONNECTED; /* USB device status */
volatile bool fSuspendEnabled = TRUE;  /* True when suspend is possible */

struct
{
  volatile RESUME_STATE eState;
  volatile u8 bESOFcnt;
}
ResumeS;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : PowerOn
* Description    : USB IP power ON Routine.
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
RESULT PowerOn(void)
{
  u16 wRegVal;

  /*** cable plugged-in ? ***/
  USB_Cable_Config(ENABLE);

  /*** CNTR_PWDN = 0 ***/
  wRegVal = CNTR_FRES;
  _SetCNTR(wRegVal);

  /*** CNTR_FRES = 0 ***/
  wInterrupt_Mask = 0;
  _SetCNTR(wInterrupt_Mask);
  /*** Clear pending interrupts ***/
  _SetISTR(0);
  /*** Set interrupt mask ***/
  wInterrupt_Mask = CNTR_RESETM | CNTR_SUSPM | CNTR_WKUPM;
  _SetCNTR(wInterrupt_Mask);

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : PowerOff
* Description    : Handles switch-off conditions
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
RESULT PowerOff()
{
  /* disable all ints and force USB reset */
  _SetCNTR(CNTR_FRES);
  /* clear interrupt status register */
  _SetISTR(0);
  /* Disable the Pull-Up*/
  USB_Cable_Config(DISABLE);
  /* switch-off device */
  _SetCNTR(CNTR_FRES + CNTR_PDWN);
  /* sw variables reset */
  /* ... */

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Suspend
* Description    : sets suspend mode operating conditions
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
void Suspend(void)
{
  u16 wCNTR;
  /* suspend preparation */
  /* ... */

  /* macrocell enters suspend mode */
  wCNTR = _GetCNTR();
  wCNTR |= CNTR_FSUSP;
  _SetCNTR(wCNTR);

  /* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
  /* power reduction */
  /* ... on connected devices */


  /* force low-power mode in the macrocell */
  wCNTR = _GetCNTR();
  wCNTR |= CNTR_LPMODE;
  _SetCNTR(wCNTR);

  /* switch-off the clocks */
  /* ... */
  Enter_LowPowerMode();

}

/*******************************************************************************
* Function Name  : Resume_Init
* Description    : Handles wake-up restoring normal operations
* Input          : None.
* Output         : None.
* Return         : USB_SUCCESS.
*******************************************************************************/
void Resume_Init(void)
{
  u16 wCNTR;
  /* ------------------ ONLY WITH BUS-POWERED DEVICES ---------------------- */
  /* restart the clocks */
  /* ...  */

  /* CNTR_LPMODE = 0 */
  wCNTR = _GetCNTR();
  wCNTR &= (~CNTR_LPMODE);
  _SetCNTR(wCNTR);

  /* restore full power */
  /* ... on connected devices */
  Leave_LowPowerMode();

  /* reset FSUSP bit */
  _SetCNTR(IMR_MSK);

  /* reverse suspend preparation */
  /* ... */

}

/*******************************************************************************
* Function Name  : Resume
* Description    : This is the state machine handling resume operations and
*                 timing sequence. The control is based on the Resume structure
*                 variables and on the ESOF interrupt calling this subroutine
*                 without changing machine state.
* Input          : a state machine value (RESUME_STATE)
*                  RESUME_ESOF doesn't change ResumeS.eState allowing
*                  decrementing of the ESOF counter in different states.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Resume(RESUME_STATE eResumeSetVal)
{
  u16 wCNTR;

  if (eResumeSetVal != RESUME_ESOF)
    ResumeS.eState = eResumeSetVal;

  switch (ResumeS.eState)
  {
    case RESUME_EXTERNAL:
      Resume_Init();
      ResumeS.eState = RESUME_OFF;
      break;
    case RESUME_INTERNAL:
      Resume_Init();
      ResumeS.eState = RESUME_START;
      break;
    case RESUME_LATER:
      ResumeS.bESOFcnt = 2;
      ResumeS.eState = RESUME_WAIT;
      break;
    case RESUME_WAIT:
      ResumeS.bESOFcnt--;
      if (ResumeS.bESOFcnt == 0)
        ResumeS.eState = RESUME_START;
      break;
    case RESUME_START:
      wCNTR = _GetCNTR();
      wCNTR |= CNTR_RESUME;
      _SetCNTR(wCNTR);
      ResumeS.eState = RESUME_ON;
      ResumeS.bESOFcnt = 10;
      break;
    case RESUME_ON:
      ResumeS.bESOFcnt--;
      if (ResumeS.bESOFcnt == 0)
      {
        wCNTR = _GetCNTR();
        wCNTR &= (~CNTR_RESUME);
        _SetCNTR(wCNTR);
        ResumeS.eState = RESUME_OFF;
      }
      break;
    case RESUME_OFF:
    case RESUME_ESOF:
    default:
      ResumeS.eState = RESUME_OFF;
      break;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_prop.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : All processing related to Virtual Com Port Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

u32 ch341_state=0xeeff;
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 Request = 0;
void Enter_LowPowerMode()
{
    bDeviceState = SUSPENDED;
}
void Leave_LowPowerMode()
{
  DEVICE_INFO *pInfo = &Device_Info;

  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}
void Get_SerialNum(void)
{
  u32 Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(vu32*)(0x1FFFF7E8);
  Device_Serial1 = *(vu32*)(0x1FFFF7EC);
  Device_Serial2 = *(vu32*)(0x1FFFF7F0);

  if (Device_Serial0 != 0)
  {
    Virtual_Com_Port_StringSerial[2] = (u8)(Device_Serial0 & 0x000000FF);
    Virtual_Com_Port_StringSerial[4] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[6] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[8] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

    Virtual_Com_Port_StringSerial[10] = (u8)(Device_Serial1 & 0x000000FF);
    Virtual_Com_Port_StringSerial[12] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[14] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[16] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

    Virtual_Com_Port_StringSerial[18] = (u8)(Device_Serial2 & 0x000000FF);
    Virtual_Com_Port_StringSerial[20] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
    Virtual_Com_Port_StringSerial[22] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
    Virtual_Com_Port_StringSerial[24] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
  }
}
//void USART_Config_Default(void)
//{

//}

LINE_CODING linecoding =
  {
    115200, /* baud rate*/
    0x00,   /* stop bits-1*/
    0x00,   /* parity - none*/
    0x08    /* no. of bits 8*/
  };
//bool USART_Config(void)
//{

//  /* set the Stop bit*/
//  switch (linecoding.format)
//  {
//    case 0:
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    default :
//    {
//      USART_Config_Default();
//      return (FALSE);
//    }
//  }

//  /* set the parity bit*/
//  switch (linecoding.paritytype)
//  {
//    case 0:
//      break;
//    case 1:
//      break;
//    case 2:
//      break;
//    default :
//    {
//      USART_Config_Default();
//      return (FALSE);
//    }
//  }

//  /*set the data type : only 8bits and 9bits is supported */
//  switch (linecoding.datatype)
//  {
//    case 0x07:
//      break;
//    case 0x08:
//      break;
//    default :
//    {
//      USART_Config_Default();
//      return (FALSE);
//    }
//  }
//  return (TRUE);
//}




/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    Virtual_Com_Port_init,
    Virtual_Com_Port_Reset,
    Virtual_Com_Port_Status_In,
    Virtual_Com_Port_Status_Out,
    Virtual_Com_Port_Data_Setup,
    Virtual_Com_Port_NoData_Setup,
    Virtual_Com_Port_Get_Interface_Setting,
    Virtual_Com_Port_GetDeviceDescriptor,
    Virtual_Com_Port_GetConfigDescriptor,
    Virtual_Com_Port_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };

USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Virtual_Com_Port_GetConfiguration,
    Virtual_Com_Port_SetConfiguration,
    Virtual_Com_Port_GetInterface,
    Virtual_Com_Port_SetInterface,
    Virtual_Com_Port_GetStatus,
    Virtual_Com_Port_ClearFeature,
    Virtual_Com_Port_SetEndPointFeature,
    Virtual_Com_Port_SetDeviceFeature,
    Virtual_Com_Port_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (u8*)Virtual_Com_Port_DeviceDescriptor,
    VIRTUAL_COM_PORT_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (u8*)Virtual_Com_Port_ConfigDescriptor,
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(u8*)Virtual_Com_Port_StringLangID, VIRTUAL_COM_PORT_SIZ_STRING_LANGID},
    {(u8*)Virtual_Com_Port_StringVendor, VIRTUAL_COM_PORT_SIZ_STRING_VENDOR},
    {(u8*)Virtual_Com_Port_StringProduct, VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT},
    {(u8*)Virtual_Com_Port_StringSerial, VIRTUAL_COM_PORT_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : Virtual_Com_Port_init.
* Description    : Virtual COM Port Mouse init routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_init(void)
{

  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  pInformation->Current_Configuration = 0;

  /* Connect the device */
  PowerOn();
  /* USB interrupts initialization */
  /* clear pending interrupts */
  _SetISTR(0);
  wInterrupt_Mask = IMR_MSK;
  /* set interrupts mask */
  _SetCNTR(wInterrupt_Mask);

  /* configure the USART 1 to the default settings */
  //USART_Config_Default();

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Reset
* Description    : Virtual_Com_Port Mouse reset routine
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Reset(void)
{
  /* Set Virtual_Com_Port DEVICE as not configured */
  pInformation->Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = Virtual_Com_Port_ConfigDescriptor[7];

  /* Set Virtual_Com_Port DEVICE with the default Interface*/
  pInformation->Current_Interface = 0;
  SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxStatus(ENDP1, EP_TX_NAK);
  SetEPRxStatus(ENDP1, EP_RX_DIS);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_BULK);
  SetEPTxAddr(ENDP2, ENDP2_TXADDR);
  SetEPTxStatus(ENDP2, EP_TX_NAK);
  SetEPRxStatus(ENDP2, EP_RX_VALID);

  /* Initialize Endpoint 2 */
  SetEPType(ENDP2, EP_BULK);
  SetEPRxAddr(ENDP2, ENDP2_RXADDR);
  SetEPRxCount(ENDP2, VIRTUAL_COM_PORT_DATA_SIZE);

  /* Set this device to response on default address */
  SetDeviceAddress(0);

  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to configured.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetConfiguration.
* Description    : Udpade the device state to addressed.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_In.
* Description    : Virtual COM Port Status In Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_In(void)
{
  if (Request == SET_LINE_CODING)
  {
    //USART_Config();
    Request = 0;
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Status_Out
* Description    : Virtual COM Port Status OUT Routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Virtual_Com_Port_Status_Out(void)
{}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Data_Setup
* Description    : handle the data class specific requests
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/
static u8 vender_request;

u8 *Vender_Handle_CH341(u16 Length)
{
    u16 wValue = pInformation->USBwValues.w;
    u16 wIndex = pInformation->USBwIndexs.w;
    static u8 buf1[2]={0x30,0};
    static u8 buf2[2]={0xc3,0};
    change_byte(wValue);
    change_byte(wIndex);
    if (Length == 0 && (vender_request==0x5f||vender_request==0x95))
    {
        pInformation->Ctrl_Info.Usb_wLength=2;
        return 0;
    }
    switch(vender_request)
    {
    case 0x5f:
            return buf1;
    case 0x95:
            if(wValue==0x2518)
                return buf2;
            else if(wValue==0x0706)
                return (u8 *)&ch341_state;     
    }
    return 0;
}
RESULT Virtual_Com_Port_Data_Setup(u8 RequestNo)
{
  u8    *(*CopyRoutine)(u16);

  CopyRoutine = NULL;

  if (RequestNo == GET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = Virtual_Com_Port_GetLineCoding;
    }
  }
  else if (RequestNo == SET_LINE_CODING)
  {
    if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
    {
      CopyRoutine = Virtual_Com_Port_SetLineCoding;
    }
    Request = SET_LINE_CODING;
  }
  else
  {
    vender_request=RequestNo;
    CopyRoutine = Vender_Handle_CH341; 
  }
  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_NoData_Setup.
* Description    : handle the no data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_UNSUPPORT or USB_SUCCESS.
*******************************************************************************/

#define CH341_BAUDBASE_FACTOR 1532620800L
RESULT Virtual_Com_Port_NoData_Setup(u8 RequestNo)
{
  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
  {
    if (RequestNo == SET_COMM_FEATURE)
    {
      return USB_SUCCESS;
    }
    else if (RequestNo == SET_CONTROL_LINE_STATE)
    {
      return USB_SUCCESS;
    }
  }
  else//CH341 specific
  {
    u16 wValue = pInformation->USBwValues.w;
    u16 wIndex = pInformation->USBwIndexs.w;
    static u32 baud_factor;
    static u8 divisor;
    change_byte(wValue);
    change_byte(wIndex);
    switch(RequestNo)
    {
    case 0XA1:
        break;
    case CH341_REQ_WRITE_REG:
        switch(wValue)
        {
        case 0x1312:
            
            baud_factor&=0x00ff;
            baud_factor|=(wIndex&0xff00);
            divisor=wIndex&0x03;
            ch341_baud=CH341_BAUDBASE_FACTOR/(((~baud_factor)&0xffff)<<((3-divisor)*3));
            break; 
        case 0x0f2c:
            baud_factor&=0xff00;
            baud_factor|=wIndex;
            ch341_baud=CH341_BAUDBASE_FACTOR/(((~baud_factor)&0xffff)<<((3-divisor)*3));
            break;
        }  
        break; 
    case 0xA4: 
        ch341_state=0xee9f;
        break;
        
    }
    return USB_SUCCESS;           
  }
  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetDeviceDescriptor.
* Description    : Gets the device descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the device descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetDeviceDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetConfigDescriptor.
* Description    : get the configuration descriptor.
* Input          : Length.
* Output         : None.
* Return         : The address of the configuration descriptor.
*******************************************************************************/
u8 *Virtual_Com_Port_GetConfigDescriptor(u16 Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetStringDescriptor
* Description    : Gets the string descriptors according to the needed index
* Input          : Length.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
u8 *Virtual_Com_Port_GetStringDescriptor(u16 Length)
{
    
  u8 wValue0 = pInformation->USBwValue0;
  if (wValue0 > 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_Get_Interface_Setting.
* Description    : test the interface and the alternate setting according to the
*                  supported one.
* Input1         : u8: Interface : interface number.
* Input2         : u8: AlternateSetting : Alternate Setting number.
* Output         : None.
* Return         : The address of the string descriptors.
*******************************************************************************/
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 1)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_GetLineCoding.
* Description    : send the linecoding structure to the PC host.
* Input          : Length.
* Output         : None.
* Return         : Inecoding structure base address.
*******************************************************************************/
u8 *Virtual_Com_Port_GetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(u8 *)&linecoding;
}

/*******************************************************************************
* Function Name  : Virtual_Com_Port_SetLineCoding.
* Description    : Set the linecoding structure fields.
* Input          : Length.
* Output         : None.
* Return         : Linecoding structure base address.
*******************************************************************************/
u8 *Virtual_Com_Port_SetLineCoding(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = sizeof(linecoding);
    return NULL;
  }
  return(u8 *)&linecoding;
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_mem.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Utility functions for memory transfers to/from PMA
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : UserToPMABufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf: pointer to user memory area.
*                  - wPMABufAddr: address into PMA.
*                  - wNBytes: no. of bytes to be copied.
* Output         : None.
* Return         : None	.
*******************************************************************************/
void UserToPMABufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes)
{
  u32 n = (wNBytes + 1) >> 1;   /* n = (wNBytes + 1) / 2 */
  u32 i, temp1, temp2;
  u16 *pdwVal;
  pdwVal = (u16 *)(wPMABufAddr * 2 + PMAAddr);
  for (i = n; i != 0; i--)
  {
    temp1 = (u16) * pbUsrBuf;
    pbUsrBuf++;
    temp2 = temp1 | (u16) * pbUsrBuf << 8;
    *pdwVal++ = temp2;
    pdwVal++;
    pbUsrBuf++;
  }
}
/*******************************************************************************
* Function Name  : PMAToUserBufferCopy
* Description    : Copy a buffer from user memory area to packet memory area (PMA)
* Input          : - pbUsrBuf    = pointer to user memory area.
*                  - wPMABufAddr = address into PMA.
*                  - wNBytes     = no. of bytes to be copied.
* Output         : None.
* Return         : None.
*******************************************************************************/
void PMAToUserBufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes)
{
  u32 n = (wNBytes + 1) >> 1;/* /2*/
  u32 i;
  u32 *pdwVal;

  
  pdwVal = (u32 *)(wPMABufAddr * 2 + PMAAddr);
  for (i = n; i != 0; i--)
  {
    *(u16*)pbUsrBuf++ = *pdwVal++;
    pbUsrBuf++;
  }
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_istr.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : ISTR events interrupt service routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile u16 wIstr;  /* ISTR register last read value */
volatile u8 bIntPackSOF = 0;  /* SOFs received between 2 consecutive packets */

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* function pointers to non-control endpoints service routines */
void (*pEpInt_IN[7])(void) =
  {
    EP1_IN_Callback,
    EP2_IN_Callback,
    EP3_IN_Callback,
    EP4_IN_Callback,
    EP5_IN_Callback,
    EP6_IN_Callback,
    EP7_IN_Callback,
  };

void (*pEpInt_OUT[7])(void) =
  {
    EP1_OUT_Callback,
    EP2_OUT_Callback,
    EP3_OUT_Callback,
    EP4_OUT_Callback,
    EP5_OUT_Callback,
    EP6_OUT_Callback,
    EP7_OUT_Callback,
  };


/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler.
* Description    : ISTR events interrupt service routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{

  wIstr = _GetISTR();

#if (IMR_MSK & ISTR_RESET)
  if (wIstr & ISTR_RESET & wInterrupt_Mask)
  {
    _SetISTR((u16)CLR_RESET);
    Device_Property.Reset();
#ifdef RESET_CALLBACK
    RESET_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_DOVR)
  if (wIstr & ISTR_DOVR & wInterrupt_Mask)
  {
    _SetISTR((u16)CLR_DOVR);
#ifdef DOVR_CALLBACK
    DOVR_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_ERR)
  if (wIstr & ISTR_ERR & wInterrupt_Mask)
  {
    _SetISTR((u16)CLR_ERR);
#ifdef ERR_CALLBACK
    ERR_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_WKUP)
  if (wIstr & ISTR_WKUP & wInterrupt_Mask)
  {
    _SetISTR((u16)CLR_WKUP);
    Resume(RESUME_EXTERNAL);
#ifdef WKUP_CALLBACK
    WKUP_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_SUSP)
  if (wIstr & ISTR_SUSP & wInterrupt_Mask)
  {

    /* check if SUSPEND is possible */
    if (fSuspendEnabled)
    {
      Suspend();
    }
    else
    {
      /* if not possible then resume after xx ms */
      Resume(RESUME_LATER);
    }
    /* clear of the ISTR bit must be done after setting of CNTR_FSUSP */
    _SetISTR((u16)CLR_SUSP);
#ifdef SUSP_CALLBACK
    SUSP_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_SOF)
  if (wIstr & ISTR_SOF & wInterrupt_Mask)
  {
    _SetISTR((u16)CLR_SOF);
    bIntPackSOF++;

#ifdef SOF_CALLBACK
    SOF_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_ESOF)
  if (wIstr & ISTR_ESOF & wInterrupt_Mask)
  {
    _SetISTR((u16)CLR_ESOF);
    /* resume handling timing is made with ESOFs */
    Resume(RESUME_ESOF); /* request without change of the machine state */

#ifdef ESOF_CALLBACK
    ESOF_Callback();
#endif
  }
#endif
  /*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*/
#if (IMR_MSK & ISTR_CTR)
  if (wIstr & ISTR_CTR & wInterrupt_Mask)
  {
    /* servicing of the endpoint correct transfer interrupt */
    /* clear of the CTR flag into the sub */
    CTR_LP();
#ifdef CTR_CALLBACK
    CTR_Callback();
#endif
  }
#endif
} /* USB_Istr */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_int.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Endpoint CTR (Low and High) interrupt's service routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16 SaveRState;
u16 SaveTState;

/* Extern variables ----------------------------------------------------------*/
extern void (*pEpInt_IN[7])(void);    /*  Handles IN  interrupts   */
extern void (*pEpInt_OUT[7])(void);   /*  Handles OUT interrupts   */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : CTR_LP.
* Description    : Low priority Endpoint Correct Transfer interrupt's service
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_LP(void)
{
  u32 wEPVal = 0;
  /* stay in loop while pending ints */
  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {
    _SetISTR((u16)CLR_CTR); /* clear CTR flag */
    /* extract highest priority endpoint number */
    EPindex = (u8)(wIstr & ISTR_EP_ID);
    if (EPindex == 0)
    {
      /* Decode and service control endpoint interrupt */
      /* calling related service routine */
      /* (Setup0_Process, In0_Process, Out0_Process) */

      /* save RX & TX status */
      /* and set both to NAK */
      SaveRState = _GetEPRxStatus(ENDP0);
      SaveTState = _GetEPTxStatus(ENDP0);
      _SetEPRxStatus(ENDP0, EP_RX_NAK);
      _SetEPTxStatus(ENDP0, EP_TX_NAK);


      /* DIR bit = origin of the interrupt */

      if ((wIstr & ISTR_DIR) == 0)//IN
      {
        /* DIR = 0      => IN  int */
        /* DIR = 0 implies that (EP_CTR_TX = 1) always  */
        _ClearEP_CTR_TX(ENDP0);
        
        In0_Process();

           /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
      }
      else//
      {
          
        /* DIR = 1 */

        /* DIR = 1 & CTR_RX       => SETUP or OUT int */
        /* DIR = 1 & (CTR_TX | CTR_RX) => 2 int pending */

        wEPVal = _GetENDPOINT(ENDP0);
        if ((wEPVal & EP_CTR_TX) != 0)
        {
          _ClearEP_CTR_TX(ENDP0);
          In0_Process();
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }
        else if ((wEPVal &EP_SETUP) != 0)
        {
          _ClearEP_CTR_RX(ENDP0); /* SETUP bit kept frozen while CTR_RX = 1 */
          Setup0_Process();
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }

        else if ((wEPVal & EP_CTR_RX) != 0)
        {
          _ClearEP_CTR_RX(ENDP0);
          Out0_Process();
          /* before terminate set Tx & Rx status */
          _SetEPRxStatus(ENDP0, SaveRState);
          _SetEPTxStatus(ENDP0, SaveTState);
          return;
        }
      }
    }/* if(EPindex == 0) */
    else
    {
      /* Decode and service non control endpoints interrupt  */

      /* process related endpoint register */
      wEPVal = _GetENDPOINT(EPindex);
      if ((wEPVal & EP_CTR_RX) != 0)
      {
        /* clear int flag */
        _ClearEP_CTR_RX(EPindex);

        /* call OUT service function */
        (*pEpInt_OUT[EPindex-1])();

      } /* if((wEPVal & EP_CTR_RX) */

      if ((wEPVal & EP_CTR_TX) != 0)
      {
        /* clear int flag */
        _ClearEP_CTR_TX(EPindex);

        /* call IN service function */
        (*pEpInt_IN[EPindex-1])();
      } /* if((wEPVal & EP_CTR_TX) != 0) */

    }/* if(EPindex == 0) else */

  }/* while(...) */
}

/*******************************************************************************
* Function Name  : CTR_HP.
* Description    : High Priority Endpoint Correct Transfer interrupt's service 
*                  routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void CTR_HP(void)
{
  u32 wEPVal = 0;

  while (((wIstr = _GetISTR()) & ISTR_CTR) != 0)
  {
    _SetISTR((u16)CLR_CTR); /* clear CTR flag */
    /* extract highest priority endpoint number */
    EPindex = (u8)(wIstr & ISTR_EP_ID);
    /* process related endpoint register */
    wEPVal = _GetENDPOINT(EPindex);
    if ((wEPVal & EP_CTR_RX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_RX(EPindex);

      /* call OUT service function */
      (*pEpInt_OUT[EPindex-1])();

    } /* if((wEPVal & EP_CTR_RX) */
    else if ((wEPVal & EP_CTR_TX) != 0)
    {
      /* clear int flag */
      _ClearEP_CTR_TX(EPindex);

      /* call IN service function */
      (*pEpInt_IN[EPindex-1])();


    } /* if((wEPVal & EP_CTR_TX) != 0) */

  }/* while(...) */
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_init.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Initialization routines & global variables
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/*  The number of current endpoint, it will be used to specify an endpoint */
 u8	EPindex;
/*  The number of current device, it is an index to the Device_Table */
/* u8	Device_no; */
/*  Points to the DEVICE_INFO structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_INFO *pInformation;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */
DEVICE_PROP *pProperty;
/*  Temporary save the state of Rx & Tx status. */
/*  Whenever the Rx or Tx state is changed, its value is saved */
/*  in this variable first and will be set to the EPRB or EPRA */
/*  at the end of interrupt process */
u16	SaveState ;
u16  wInterrupt_Mask;
DEVICE_INFO	Device_Info;
USER_STANDARD_REQUESTS  *pUser_Standard_Requests;


void Set_USBClock(void)
{
  /* USBCLK = PLLCLK / 1.5 */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable USART1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 buffer_out[VIRTUAL_COM_PORT_DATA_SIZE];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP3_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_IN_Callback(void)
{

}
void EP2_OUT_Callback(void)
{
    u8 count_out;
    count_out = GetEPRxCount(ENDP2);
    PMAToUserBufferCopy(buffer_out, ENDP2_RXADDR, count_out);
    USB_receive(buffer_out,count_out);
    SetEPRxValid(ENDP2);
    
}
/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{

}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Descriptors for Virtual Com Port Demo
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* USB Standard Device Descriptor */
const u8 Virtual_Com_Port_DeviceDescriptor[] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x10,
    0x01,   /* bcdUSB = 1.10 */
    0xff,   /* bDeviceClass: CDC */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 */
    0x86,
    0x1a,   /* idVendor = 0x0483 */
    0x23,
    0x75,   /* idProduct = 0x7540 */
    0x54,
    0x02,   /* bcdDevice = 2.00 */
    1,              /* Index of string descriptor describing manufacturer */
    2,              /* Index of string descriptor describing product */
    1,              /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

const u8 Virtual_Com_Port_ConfigDescriptor[] =
  {
    /*Configuation Descriptor*/
    0x09,   /* bLength: Configuation Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0x80,   /* bmAttributes: self powered */
    0x30,   /* MaxPower 0 mA */
    /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x03,   /* bNumEndpoints: One endpoints used */
    0xff,   /* bInterfaceClass: Communication Interface Class */
    0x01,   /* bInterfaceSubClass: Abstract Control Model */
    0x02,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */
    
    /*Endpoint 2in Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x82,   /* bEndpointAddress: (IN2) */
    0x02,   /* bmAttributes: bulk */
    0x20,      
    0x00,   /* wMaxPacketSize: */
    0x00,   /* bInterval: */
    
    /*Endpoint 2out Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x02,   /* bEndpointAddress: (out2) */
    0x02,   /* bmAttributes: bulk */
    0x20,      
    0x00,   /* wMaxPacketSize: */
    0x00,   /* bInterval: */
    
    /*Endpoint 1in Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x81,   /* bEndpointAddress: (IN1) */
    0x03,   /* bmAttributes: Interrupt */
    0x08,      /* wMaxPacketSize: */
    0x00,
    0x01,   /* bInterval: */
    
  };

/* USB String Descriptors */
const u8 Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const u8 Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
    'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
    'c', 0, 's', 0
  };

const u8 Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "STM32 Virtual COM Port" */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, 'S', 0, 'i', 0, 'm', 0,
    'u', 0, 'l', 0, 'a', 0, 't', 0, 'e', 0, ' ', 0, 'C', 0, 'H', 0,
    '3', 0, '4', 0, '1', 0, '~', 0, 'r', 0, 't', 0, ' ', 0, ' ', 0
  };

u8 Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  }
  ;/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_core.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Standard protocol processing (USB v2.0)
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ValBit(VAR,Place)    (VAR & (1 << Place))
#define SetBit(VAR,Place)    (VAR |= (1 << Place))
#define ClrBit(VAR,Place)    (VAR &= ((1 << Place) ^ 255))

#define Send0LengthData() { _SetEPTxCount(ENDP0, 0); \
    vSetEPTxStatus(EP_TX_VALID); \
  }

#define vSetEPRxStatus(st) (SaveRState = st)
#define vSetEPTxStatus(st) (SaveTState = st)

#define USB_StatusIn() Send0LengthData()
#define USB_StatusOut() vSetEPRxStatus(EP_RX_VALID)

#define StatusInfo0 StatusInfo.bw.bb1 /* Reverse bb0 & bb1 */
#define StatusInfo1 StatusInfo.bw.bb0

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u16_u8 StatusInfo;
bool Data_Mul_MaxPacketSize = FALSE;
/* Private function prototypes -----------------------------------------------*/
static void DataStageOut(void);
static void DataStageIn(void);
static void NoData_Setup0(void);
static void Data_Setup0(void);
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Standard_GetConfiguration.
* Description    : Return the current configuration variable address.
* Input          : Length - How many bytes are needed.
* Output         : None.
* Return         : Return 1 , if the request is invalid when "Length" is 0.
*                  Return "Buffer" if the "Length" is not 0.
*******************************************************************************/
u8 *Standard_GetConfiguration(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength =
      sizeof(pInformation->Current_Configuration);
    return 0;
  }
  pUser_Standard_Requests->User_GetConfiguration();
  return (u8 *)&pInformation->Current_Configuration;
}

/*******************************************************************************
* Function Name  : Standard_SetConfiguration.
* Description    : This routine is called to set the configuration value
*                  Then each class should configure device themself.
* Input          : None.
* Output         : None.
* Return         : Return USB_SUCCESS, if the request is performed.
*                  Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetConfiguration(void)
{
  if ((pInformation->USBwValue0 <=
      Device_Table.Total_Configuration) && (pInformation->USBwValue1 == 0)
      && (pInformation->USBwIndex == 0)) /*call Back usb spec 2.0*/
  {
    pInformation->Current_Configuration = pInformation->USBwValue0;
    pUser_Standard_Requests->User_SetConfiguration();
    return USB_SUCCESS;
  }
  else
  {
    return USB_UNSUPPORT;
  }
}

/*******************************************************************************
* Function Name  : Standard_GetInterface.
* Description    : Return the Alternate Setting of the current interface.
* Input          : Length - How many bytes are needed.
* Output         : None.
* Return         : Return 0, if the request is invalid when "Length" is 0.
*                  Return "Buffer" if the "Length" is not 0.
*******************************************************************************/
u8 *Standard_GetInterface(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength =
      sizeof(pInformation->Current_AlternateSetting);
    return 0;
  }
  pUser_Standard_Requests->User_GetInterface();
  return (u8 *)&pInformation->Current_AlternateSetting;
}

/*******************************************************************************
* Function Name  : Standard_SetInterface.
* Description    : This routine is called to set the interface.
*                  Then each class should configure the interface them self.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetInterface(void)
{
  RESULT Re;
  /*Test if the specified Interface and Alternate Setting are supported by
    the application Firmware*/
  Re = (*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, pInformation->USBwValue0);

  if (pInformation->Current_Configuration != 0)
  {
    if ((Re != USB_SUCCESS) || (pInformation->USBwIndex1 != 0)
        || (pInformation->USBwValue1 != 0))
    {
      return  USB_UNSUPPORT;
    }
    else if (Re == USB_SUCCESS)
    {
      pUser_Standard_Requests->User_SetInterface();
      pInformation->Current_Interface = pInformation->USBwIndex0;
      pInformation->Current_AlternateSetting = pInformation->USBwValue0;
      return USB_SUCCESS;
    }

  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Standard_GetStatus.
* Description    : Copy the device request data to "StatusInfo buffer".
* Input          : - Length - How many bytes are needed.
* Output         : None.
* Return         : Return 0, if the request is at end of data block,
*                  or is invalid when "Length" is 0.
*******************************************************************************/
u8 *Standard_GetStatus(u16 Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 2;
    return 0;
  }

  StatusInfo.w = 0;
  /* Reset Status Information */

  if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {
    /*Get Device Status */
    u8 Feature = pInformation->Current_Feature;

    /* Remote Wakeup enabled */
    if (ValBit(Feature, 5))
    {
      SetBit(StatusInfo0, 1);
    }

    /* Bus-powered */
    if (ValBit(Feature, 6))
    {
      ClrBit(StatusInfo0, 0);
    }
    else /* Self-powered */
    {
      SetBit(StatusInfo0, 0);
    }
  }
  /*Interface Status*/
  else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
  {
    return (u8 *)&StatusInfo;
  }
  /*Get EndPoint Status*/
  else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {
    u8 Related_Endpoint;
    u8 wIndex0 = pInformation->USBwIndex0;

    Related_Endpoint = (wIndex0 & 0x0f);
    if (ValBit(wIndex0, 7))
    {
      /* IN endpoint */
      if (_GetTxStallStatus(Related_Endpoint))
      {
        SetBit(StatusInfo0, 0); /* IN Endpoint stalled */
      }
    }
    else
    {
      /* OUT endpoint */
      if (_GetRxStallStatus(Related_Endpoint))
      {
        SetBit(StatusInfo0, 0); /* OUT Endpoint stalled */
      }
    }

  }
  else
  {
    return NULL;
  }
  pUser_Standard_Requests->User_GetStatus();
  return (u8 *)&StatusInfo;
}

/*******************************************************************************
* Function Name  : Standard_ClearFeature.
* Description    : Clear or disable a specific feature.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/

RESULT Standard_ClearFeature(void)
{
  u32     Type_Rec = Type_Recipient;
  u32     Status;


  if (Type_Rec == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {/*Device Clear Feature*/
    ClrBit(pInformation->Current_Feature, 5);
    return USB_SUCCESS;
  }
  else if (Type_Rec == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {/*EndPoint Clear Feature*/
    DEVICE* pDev;
    u32 Related_Endpoint;
    u32 wIndex0;
    u32 rEP;

    if ((pInformation->USBwValue != ENDPOINT_STALL)
        || (pInformation->USBwIndex1 != 0))
    {
      return USB_UNSUPPORT;
    }

    pDev = &Device_Table;
    wIndex0 = pInformation->USBwIndex0;
    rEP = wIndex0 & ~0x80;
    Related_Endpoint = ENDP0 + rEP;

    if (ValBit(pInformation->USBwIndex0, 7))
    {
      /*Get Status of endpoint & stall the request if the related_ENdpoint
      is Disabled*/
      Status = _GetEPTxStatus(Related_Endpoint);
    }
    else
    {
      Status = _GetEPRxStatus(Related_Endpoint);
    }

    if ((rEP >= pDev->Total_Endpoint) || (Status == 0)
        || (pInformation->Current_Configuration == 0))
    {
      return USB_UNSUPPORT;
    }


    if (wIndex0 & 0x80)
    {
      /* IN endpoint */
      if (_GetTxStallStatus(Related_Endpoint ))
      {
        ClearDTOG_TX(Related_Endpoint);
        SetEPTxStatus(Related_Endpoint, EP_TX_VALID);
      }
    }
    else
    {
      /* OUT endpoint */
      if (_GetRxStallStatus(Related_Endpoint))
      {
        if (Related_Endpoint == ENDP0)
        {
          /* After clear the STALL, enable the default endpoint receiver */
          SetEPRxCount(Related_Endpoint, Device_Property.MaxPacketSize);
          _SetEPRxStatus(Related_Endpoint, EP_RX_VALID);
        }
        else
        {
          ClearDTOG_RX(Related_Endpoint);
          _SetEPRxStatus(Related_Endpoint, EP_RX_VALID);
        }
      }
    }
    pUser_Standard_Requests->User_ClearFeature();
    return USB_SUCCESS;
  }

  return USB_UNSUPPORT;
}

/*******************************************************************************
* Function Name  : Standard_SetEndPointFeature
* Description    : Set or enable a specific feature of EndPoint
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetEndPointFeature(void)
{
  u32    wIndex0;
  u32    Related_Endpoint;
  u32    rEP;
  u32   Status;

  wIndex0 = pInformation->USBwIndex0;
  rEP = wIndex0 & ~0x80;
  Related_Endpoint = ENDP0 + rEP;

  if (ValBit(pInformation->USBwIndex0, 7))
  {
    /* get Status of endpoint & stall the request if the related_ENdpoint
    is Disabled*/
    Status = _GetEPTxStatus(Related_Endpoint);
  }
  else
  {
    Status = _GetEPRxStatus(Related_Endpoint);
  }

  if (Related_Endpoint >= Device_Table.Total_Endpoint
      || pInformation->USBwValue != 0 || Status == 0
      || pInformation->Current_Configuration == 0)
  {
    return USB_UNSUPPORT;
  }
  else
  {
    if (wIndex0 & 0x80)
    {
      /* IN endpoint */
      _SetEPTxStatus(Related_Endpoint, EP_TX_STALL);
    }

    else
    {
      /* OUT endpoint */
      _SetEPRxStatus(Related_Endpoint, EP_RX_STALL);
    }
  }
  pUser_Standard_Requests->User_SetEndPointFeature();
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Standard_SetDeviceFeature.
* Description    : Set or enable a specific feature of Device.
* Input          : None.
* Output         : None.
* Return         : - Return USB_SUCCESS, if the request is performed.
*                  - Return USB_UNSUPPORT, if the request is invalid.
*******************************************************************************/
RESULT Standard_SetDeviceFeature(void)
{
  SetBit(pInformation->Current_Feature, 5);
  pUser_Standard_Requests->User_SetDeviceFeature();
  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : Standard_GetDescriptorData.
* Description    : Standard_GetDescriptorData is used for descriptors transfer.
*                : This routine is used for the descriptors resident in Flash
*                  or RAM
*                  pDesc can be in either Flash or RAM
*                  The purpose of this routine is to have a versatile way to
*                  response descriptors request. It allows user to generate
*                  certain descriptors with software or read descriptors from
*                  external storage part by part.
* Input          : - Length - Length of the data in this transfer.
*                  - pDesc - A pointer points to descriptor struct.
*                  The structure gives the initial address of the descriptor and
*                  its original size.
* Output         : None.
* Return         : Address of a part of the descriptor pointed by the Usb_
*                  wOffset The buffer pointed by this address contains at least
*                  Length bytes.
*******************************************************************************/
u8 *Standard_GetDescriptorData(u16 Length, ONE_DESCRIPTOR *pDesc)
{
  u32  wOffset;
    
  wOffset = pInformation->Ctrl_Info.Usb_wOffset;
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = pDesc->Descriptor_Size - wOffset;
    return 0;
  }

  return pDesc->Descriptor + wOffset;
}

/*******************************************************************************
* Function Name  : DataStageOut.
* Description    : Data stage of a Control Write Transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DataStageOut(void)
{
  ENDPOINT_INFO *pEPinfo = &pInformation->Ctrl_Info;
  u32 save_rLength;

  save_rLength = pEPinfo->Usb_rLength;

  if (pEPinfo->CopyData && save_rLength)
  {
    u8 *Buffer;
    u32 Length;

    Length = pEPinfo->PacketSize;
    if (Length > save_rLength)
    {
      Length = save_rLength;
    }

    
    Buffer = (*pEPinfo->CopyData)(Length);
    pEPinfo->Usb_rLength -= Length;
    pEPinfo->Usb_rOffset += Length;

    PMAToUserBufferCopy(Buffer, GetEPRxAddr(ENDP0), Length);

  }

  if (pEPinfo->Usb_rLength != 0)
  {
    vSetEPRxStatus(EP_RX_VALID);/* re-enable for next data reception */
    SetEPTxCount(ENDP0, 0);
    vSetEPTxStatus(EP_TX_VALID);/* Expect the host to abort the data OUT stage */
  }
  /* Set the next State*/
  if (pEPinfo->Usb_rLength >= pEPinfo->PacketSize)
  {
    pInformation->ControlState = OUT_DATA;
  }
  else
  {
    if (pEPinfo->Usb_rLength > 0)
    {
      pInformation->ControlState = LAST_OUT_DATA;
    }
    else if (pEPinfo->Usb_rLength == 0)
    {
      pInformation->ControlState = WAIT_STATUS_IN;
      USB_StatusIn();
    }
  }
  
  
}

/*******************************************************************************
* Function Name  : DataStageIn.
* Description    : Data stage of a Control Read Transfer.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DataStageIn(void)
{
  ENDPOINT_INFO *pEPinfo = &pInformation->Ctrl_Info;
  u32 save_wLength = pEPinfo->Usb_wLength;
  u32 ControlState = pInformation->ControlState;

  u8 *DataBuffer;
  u32 Length;

  if ((save_wLength == 0) && (ControlState == LAST_IN_DATA))
  {
    if(Data_Mul_MaxPacketSize == TRUE)
    {
      /* No more data to send and empty packet */
      Send0LengthData();
      ControlState = LAST_IN_DATA;
      Data_Mul_MaxPacketSize = FALSE;
    }
    else 
    {
      /* No more data to send so STALL the TX Status*/
      ControlState = WAIT_STATUS_OUT;
      vSetEPTxStatus(EP_TX_STALL);
    }
    
    goto Expect_Status_Out;
  }

  Length = pEPinfo->PacketSize;
  ControlState = (save_wLength <= Length) ? LAST_IN_DATA : IN_DATA;

  if (Length > save_wLength)
  {
    Length = save_wLength;
  }

  DataBuffer = (*pEPinfo->CopyData)(Length);

  UserToPMABufferCopy(DataBuffer, GetEPTxAddr(ENDP0), Length);

  SetEPTxCount(ENDP0, Length);

  pEPinfo->Usb_wLength -= Length;
  pEPinfo->Usb_wOffset += Length;
  vSetEPTxStatus(EP_TX_VALID);

  USB_StatusOut();/* Expect the host to abort the data IN stage */

Expect_Status_Out:
  pInformation->ControlState = ControlState;
}

/*******************************************************************************
* Function Name  : NoData_Setup0.
* Description    : Proceed the processing of setup request without data stage.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void NoData_Setup0(void)
{
  RESULT Result = USB_UNSUPPORT;
  u32 RequestNo = pInformation->USBbRequest;
  u32 ControlState;

  if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
  {
    /* Device Request*/
    /* SET_CONFIGURATION*/
    if (RequestNo == SET_CONFIGURATION)
    {
      Result = Standard_SetConfiguration();
    }

    /*SET ADDRESS*/
    else if (RequestNo == SET_ADDRESS)
    {
      if ((pInformation->USBwValue0 > 127) || (pInformation->USBwValue1 != 0)
          || (pInformation->USBwIndex != 0)
          || (pInformation->Current_Configuration != 0))
        /* Device Address should be 127 or less*/
      {
        ControlState = STALLED;
        goto exit_NoData_Setup0;
      }
      else
      {
        Result = USB_SUCCESS;
      }
    }
    /*SET FEATURE for Device*/
    else if (RequestNo == SET_FEATURE)
    {
      if ((pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP)
          && (pInformation->USBwIndex == 0)
          && (ValBit(pInformation->Current_Feature, 5)))
      {
        Result = Standard_SetDeviceFeature();
      }
      else
      {
        Result = USB_UNSUPPORT;
      }
    }
    /*Clear FEATURE for Device */
    else if (RequestNo == CLEAR_FEATURE)
    {
      if (pInformation->USBwValue0 == DEVICE_REMOTE_WAKEUP
          && pInformation->USBwIndex == 0
          && ValBit(pInformation->Current_Feature, 5))
      {
        Result = Standard_ClearFeature();
      }
      else
      {
        Result = USB_UNSUPPORT;
      }
    }

  }

  /* Interface Request*/
  else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
  {
    /*SET INTERFACE*/
    if (RequestNo == SET_INTERFACE)
    {
      Result = Standard_SetInterface();
    }
  }

  /* EndPoint Request*/
  else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
  {
    /*CLEAR FEATURE for EndPoint*/
    if (RequestNo == CLEAR_FEATURE)
    {
      Result = Standard_ClearFeature();
    }
    /* SET FEATURE for EndPoint*/
    else if (RequestNo == SET_FEATURE)
    {
      Result = Standard_SetEndPointFeature();
    }
  }
  else
  {
    Result = USB_UNSUPPORT;
  }


  if (Result != USB_SUCCESS)
  {
    Result = (*pProperty->Class_NoData_Setup)(RequestNo);
    if (Result == USB_NOT_READY)
    {
      ControlState = PAUSE;
      goto exit_NoData_Setup0;
    }
  }

  if (Result != USB_SUCCESS)
  {
    ControlState = STALLED;
    goto exit_NoData_Setup0;
  }

  ControlState = WAIT_STATUS_IN;/* After no data stage SETUP */

  USB_StatusIn();

exit_NoData_Setup0:
  pInformation->ControlState = ControlState;
  return;
}

/*******************************************************************************
* Function Name  : Data_Setup0.
* Description    : Proceed the processing of setup request with data stage.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Data_Setup0(void)
{
  u8 *(*CopyRoutine)(u16);
  RESULT Result;
  u32 Request_No = pInformation->USBbRequest;

  u32 Related_Endpoint, Reserved;
  u32 wOffset, Status;



  CopyRoutine = NULL;
  wOffset = 0;

  if (Request_No == GET_DESCRIPTOR)
  {
    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
    {
      u8 wValue1 = pInformation->USBwValue1;
      if (wValue1 == DEVICE_DESCRIPTOR)
      {
        CopyRoutine = pProperty->GetDeviceDescriptor;
      }
      else if (wValue1 == CONFIG_DESCRIPTOR)
      {
        CopyRoutine = pProperty->GetConfigDescriptor;
      }
      else if (wValue1 == STRING_DESCRIPTOR)
      {
        CopyRoutine = pProperty->GetStringDescriptor;
      }  /* End of GET_DESCRIPTOR */
    }
  }

  /*GET STATUS*/
  else if ((Request_No == GET_STATUS) && (pInformation->USBwValue == 0)
           && (pInformation->USBwLength == 0x0002)
           && (pInformation->USBwIndex1 == 0))
  {
    /* GET STATUS for Device*/
    if ((Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
        && (pInformation->USBwIndex == 0))
    {
      CopyRoutine = Standard_GetStatus;
    }

    /* GET STATUS for Interface*/
    else if (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
    {
      if (((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS)
          && (pInformation->Current_Configuration != 0))
      {
        CopyRoutine = Standard_GetStatus;
      }
    }

    /* GET STATUS for EndPoint*/
    else if (Type_Recipient == (STANDARD_REQUEST | ENDPOINT_RECIPIENT))
    {
      Related_Endpoint = (pInformation->USBwIndex0 & 0x0f);
      Reserved = pInformation->USBwIndex0 & 0x70;

      if (ValBit(pInformation->USBwIndex0, 7))
      {
        /*Get Status of endpoint & stall the request if the related_ENdpoint
        is Disabled*/
        Status = _GetEPTxStatus(Related_Endpoint);
      }
      else
      {
        Status = _GetEPRxStatus(Related_Endpoint);
      }

      if ((Related_Endpoint < Device_Table.Total_Endpoint) && (Reserved == 0)
          && (Status != 0))
      {
        CopyRoutine = Standard_GetStatus;
      }
    }

  }

  /*GET CONFIGURATION*/
  else if (Request_No == GET_CONFIGURATION)
  {
    if (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT))
    {
      CopyRoutine = Standard_GetConfiguration;
    }
  }
  /*GET INTERFACE*/
  else if (Request_No == GET_INTERFACE)
  {
    if ((Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
        && (pInformation->Current_Configuration != 0) && (pInformation->USBwValue == 0)
        && (pInformation->USBwIndex1 == 0) && (pInformation->USBwLength == 0x0001)
        && ((*pProperty->Class_Get_Interface_Setting)(pInformation->USBwIndex0, 0) == USB_SUCCESS))
    {
      CopyRoutine = Standard_GetInterface;
    }

  }
  if (CopyRoutine)
  {
    pInformation->Ctrl_Info.Usb_wOffset = wOffset;
    pInformation->Ctrl_Info.CopyData = CopyRoutine;
    /* sb in the original the cast to word was directly */
    /* now the cast is made step by step */
    (*CopyRoutine)(0);
    Result = USB_SUCCESS;
  }
  else
  {
    Result = (*pProperty->Class_Data_Setup)(pInformation->USBbRequest);
    if (Result == USB_NOT_READY)
    {
      pInformation->ControlState = PAUSE;
      return;
    }
  }

  if (pInformation->Ctrl_Info.Usb_wLength == 0xFFFF)
  {
    /* Data is not ready, wait it */
    pInformation->ControlState = PAUSE;
    return;
  }
  if ((Result == USB_UNSUPPORT) || (pInformation->Ctrl_Info.Usb_wLength == 0))
  {
    /* Unsupported request */
    pInformation->ControlState = STALLED;
    return;
  }


  if (ValBit(pInformation->USBbmRequestType, 7))
  {
    /* Device ==> Host */
    vu32 wLength = pInformation->USBwLength;
     
    /* Restrict the data length to be the one host asks */
    if (pInformation->Ctrl_Info.Usb_wLength > wLength)
    {
      pInformation->Ctrl_Info.Usb_wLength = wLength;
    }
    
    else if (pInformation->Ctrl_Info.Usb_wLength < pInformation->USBwLength)
    {
      if (pInformation->Ctrl_Info.Usb_wLength < pProperty->MaxPacketSize)
      {
        Data_Mul_MaxPacketSize = FALSE;
      }
      else if ((pInformation->Ctrl_Info.Usb_wLength % pProperty->MaxPacketSize) == 0)
      {
        Data_Mul_MaxPacketSize = TRUE;
      }
    }   

    pInformation->Ctrl_Info.PacketSize = pProperty->MaxPacketSize;
    DataStageIn();
  }
  else
  {
    pInformation->ControlState = OUT_DATA;
    vSetEPRxStatus(EP_RX_VALID); /* enable for next data reception */
  }

  return;
}

/*******************************************************************************
* Function Name  : Setup0_Process
* Description    : Get the device request data and dispatch to individual process.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
u8 Setup0_Process(void)
{

  union
  {
    u8* b;
    u16* w;
  } pBuf;

  pBuf.b = PMAAddr + (u8 *)(_GetEPRxAddr(ENDP0) * 2); /* *2 for 32 bits addr */

  if (pInformation->ControlState != PAUSE)
  {
    pInformation->USBbmRequestType = *pBuf.b++; /* bmRequestType */
    pInformation->USBbRequest = *pBuf.b++; /* bRequest */
    pBuf.w++;  /* word not accessed because of 32 bits addressing */
    pInformation->USBwValue = ByteSwap(*pBuf.w++); /* wValue */
    pBuf.w++;  /* word not accessed because of 32 bits addressing */
    pInformation->USBwIndex  = ByteSwap(*pBuf.w++); /* wIndex */
    pBuf.w++;  /* word not accessed because of 32 bits addressing */
    pInformation->USBwLength = *pBuf.w; /* wLength */
  }

  pInformation->ControlState = SETTING_UP;
  if (pInformation->USBwLength == 0)
  {
    /* Setup with no data stage */
    NoData_Setup0();
  }
  else
  {
    /* Setup with data stage */
    Data_Setup0();
  }
  return Post0_Process();
}

/*******************************************************************************
* Function Name  : In0_Process
* Description    : Process the IN token on all default endpoint.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
u8 In0_Process(void)
{
  u32 ControlState = pInformation->ControlState;

  if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA))
  {
    DataStageIn();
    /* ControlState may be changed outside the function */
    ControlState = pInformation->ControlState;
  }

  else if (ControlState == WAIT_STATUS_IN)
  {
    if ((pInformation->USBbRequest == SET_ADDRESS) &&
        (Type_Recipient == (STANDARD_REQUEST | DEVICE_RECIPIENT)))
    {
      SetDeviceAddress(pInformation->USBwValue0);
      pUser_Standard_Requests->User_SetDeviceAddress();
    }
    (*pProperty->Process_Status_IN)();
    ControlState = STALLED;
  }

  else
  {
    ControlState = STALLED;
  }

  pInformation->ControlState = ControlState;
  return Post0_Process();
}

/*******************************************************************************
* Function Name  : Out0_Process
* Description    : Process the OUT token on all default endpoint.
* Input          : None.
* Output         : None.
* Return         : Post0_Process.
*******************************************************************************/
u8 Out0_Process(void)
{
    
  u32 ControlState = pInformation->ControlState;

  if ((ControlState == OUT_DATA) || (ControlState == LAST_OUT_DATA))
  {
    DataStageOut();
    ControlState = pInformation->ControlState; /* may be changed outside the function */
  }
  else if (ControlState == WAIT_STATUS_OUT)
  {
    (*pProperty->Process_Status_OUT)();
    ControlState = STALLED;
  }

  else if ((ControlState == IN_DATA) || (ControlState == LAST_IN_DATA))
  {
    /* host aborts the transfer before finish */
    ControlState = STALLED;
  }

  /* Unexpect state, STALL the endpoint */
  else
  {
    ControlState = STALLED;
  }

  pInformation->ControlState = ControlState;

  return Post0_Process();
}

/*******************************************************************************
* Function Name  : Post0_Process
* Description    : Stall the Endpoint 0 in case of error.
* Input          : None.
* Output         : None.
* Return         : - 0 if the control State is in PAUSE
*                  - 1 if not.
*******************************************************************************/
u8 Post0_Process(void)
{
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);

  if (pInformation->ControlState == STALLED)
  {
    vSetEPRxStatus(EP_RX_STALL);
    vSetEPTxStatus(EP_TX_STALL);
  }

  return (pInformation->ControlState == PAUSE);
}

/*******************************************************************************
* Function Name  : SetDeviceAddress.
* Description    : Set the device and all the used Endpoints addresses.
* Input          : - Val: device adress.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SetDeviceAddress(u8 Val)
{
  u32 i;
  u32 nEP = Device_Table.Total_Endpoint;

  /* set address in every used endpoint */
  for (i = 0; i < nEP; i++)
  {
    _SetEPAddress((u8)i, (u8)i);
  } /* for */
  _SetDADDR(Val | DADDR_EF); /* set device address and enable function */
}

/*******************************************************************************
* Function Name  : NOP_Process
* Description    : No operation function.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void NOP_Process(void)
{
}

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/



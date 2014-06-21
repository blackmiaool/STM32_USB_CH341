/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_lib.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : USB library include files
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_LIB_H
#define __USB_LIB_H

/* Includes ------------------------------------------------------------------*/
#ifndef __STM32F10x_TYPE_H

typedef signed long      s32;
typedef signed short     s16;
typedef signed char      s8;

typedef volatile signed long      vs32;
typedef volatile signed short     vs16;
typedef volatile signed char      vs8;

typedef unsigned long       u32;
typedef unsigned short      u16;
typedef unsigned char       u8;

typedef unsigned long  const    uc32;  /* Read Only */
typedef unsigned short const    uc16;  /* Read Only */
typedef unsigned char  const    uc8;   /* Read Only */

typedef volatile unsigned long      vu32;
typedef volatile unsigned short     vu16;
typedef volatile unsigned char      vu8;

typedef volatile unsigned long  const    vuc32;  /* Read Only */
typedef volatile unsigned short const    vuc16;  /* Read Only */
typedef volatile unsigned char  const    vuc8;   /* Read Only */


typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;

typedef enum { RESET = 0, SET   = !RESET } FlagStatus, ITStatus;

typedef enum { DISABLE = 0, ENABLE  = !DISABLE} FunctionalState;

typedef enum { ERROR = 0, SUCCESS  = !ERROR} ErrorStatus;
#endif
#define change_byte(a) do{u8 lb=a;a=(lb<<8)+(a>>8);}while(0)
typedef enum _CONTROL_STATE
{
  WAIT_SETUP,       /* 0 */
  SETTING_UP,       /* 1 */
  IN_DATA,          /* 2 */
  OUT_DATA,         /* 3 */
  LAST_IN_DATA,     /* 4 */
  LAST_OUT_DATA,    /* 5 */
  WAIT_STATUS_IN,   /* 7 */
  WAIT_STATUS_OUT,  /* 8 */
  STALLED,          /* 9 */
  PAUSE             /* 10 */
} CONTROL_STATE;    /* The state machine states of a control pipe */

typedef struct OneDescriptor
{
  u8 *Descriptor;
  u16 Descriptor_Size;
}
ONE_DESCRIPTOR, *PONE_DESCRIPTOR;
/* All the request process routines return a value of this type
   If the return value is not SUCCESS or NOT_READY,
   the software will STALL the correspond endpoint */
typedef enum _RESULT
{
  USB_SUCCESS = 0,    /* Process sucessfully */
  USB_ERROR,
  USB_UNSUPPORT,
  USB_NOT_READY       /* The process has not been finished, endpoint will be
                         NAK to further rquest */
} RESULT;


/*-*-*-*-*-*-*-*-*-*-* Definitions for endpoint level -*-*-*-*-*-*-*-*-*-*-*-*/
typedef struct _ENDPOINT_INFO
{
  /* When send data out of the device,
   CopyData() is used to get data buffer 'Length' bytes data
   if Length is 0,
    CopyData() returns the total length of the data
    if the request is not supported, returns 0
    (NEW Feature )
     if CopyData() returns -1, the calling routine should not proceed
     further and will resume the SETUP process by the class device
   if Length is not 0,
    CopyData() returns a pointer to indicate the data location
   Usb_wLength is the data remain to be sent,
   Usb_wOffset is the Offset of original data
  When receive data from the host,
   CopyData() is used to get user data buffer which is capable
   of Length bytes data to copy data from the endpoint buffer.
   if Length is 0,
    CopyData() returns the available data length,
   if Length is not 0,
    CopyData() returns user buffer address
   Usb_rLength is the data remain to be received,
   Usb_rPointer is the Offset of data buffer
  */
  u16  Usb_wLength;
  u16  Usb_wOffset;
  u16  PacketSize;
  u8   *(*CopyData)(u16 Length);
}ENDPOINT_INFO;

/*-*-*-*-*-*-*-*-*-*-*-* Definitions for device level -*-*-*-*-*-*-*-*-*-*-*-*/

typedef struct _DEVICE
{
  u8 Total_Endpoint;     /* Number of endpoints that are used */
  u8 Total_Configuration;/* Number of configuration available */
}
DEVICE;

typedef union
{
  u16 w;
  struct BW
  {
    u8 bb1;
    u8 bb0;
  }
  bw;
} u16_u8;

typedef struct _DEVICE_INFO
{
  u8 USBbmRequestType;       /* bmRequestType */
  u8 USBbRequest;            /* bRequest */
  u16_u8 USBwValues;         /* wValue */
  u16_u8 USBwIndexs;         /* wIndex */
  u16_u8 USBwLengths;        /* wLength */

  u8 ControlState;           /* of type CONTROL_STATE */
  u8 Current_Feature;
  u8 Current_Configuration;   /* Selected configuration */
  u8 Current_Interface;       /* Selected interface of current configuration */
  u8 Current_AlternateSetting;/* Selected Alternate Setting of current
                                     interface*/

  ENDPOINT_INFO Ctrl_Info;
}DEVICE_INFO;

typedef struct _DEVICE_PROP
{
  void (*Init)(void);        /* Initialize the device */
  void (*Reset)(void);       /* Reset routine of this device */

  /* Device dependent process after the status stage */
  void (*Process_Status_IN)(void);
  void (*Process_Status_OUT)(void);

  /* Procedure of process on setup stage of a class specified request with data stage */
  /* All class specified requests with data stage are processed in Class_Data_Setup
   Class_Data_Setup()
    responses to check all special requests and fills ENDPOINT_INFO
    according to the request
    If IN tokens are expected, then wLength & wOffset will be filled
    with the total transferring bytes and the starting position
    If OUT tokens are expected, then rLength & rOffset will be filled
    with the total expected bytes and the starting position in the buffer

    If the request is valid, Class_Data_Setup returns SUCCESS, else UNSUPPORT

   CAUTION:
    Since GET_CONFIGURATION & GET_INTERFACE are highly related to
    the individual classes, they will be checked and processed here.
  */
  RESULT (*Class_Data_Setup)(u8 RequestNo);

  /* Procedure of process on setup stage of a class specified request without data stage */
  /* All class specified requests without data stage are processed in Class_NoData_Setup
   Class_NoData_Setup
    responses to check all special requests and perform the request

   CAUTION:
    Since SET_CONFIGURATION & SET_INTERFACE are highly related to
    the individual classes, they will be checked and processed here.
  */
  RESULT (*Class_NoData_Setup)(u8 RequestNo);

  /*Class_Get_Interface_Setting
   This function is used by the file usb_core.c to test if the selected Interface
   and Alternate Setting (u8 Interface, u8 AlternateSetting) are supported by
   the application.
   This function is writing by user. It should return "SUCCESS" if the Interface
   and Alternate Setting are supported by the application or "UNSUPPORT" if they
   are not supported. */

  RESULT  (*Class_Get_Interface_Setting)(u8 Interface, u8 AlternateSetting);

  u8* (*GetDeviceDescriptor)(u16 Length);
  u8* (*GetConfigDescriptor)(u16 Length);
  u8* (*GetStringDescriptor)(u16 Length);

  u8* RxEP_buffer;
  u8 MaxPacketSize;

}DEVICE_PROP;

typedef struct _USER_STANDARD_REQUESTS
{
  void (*User_GetConfiguration)(void);       /* Get Configuration */
  void (*User_SetConfiguration)(void);       /* Set Configuration */
  void (*User_GetInterface)(void);           /* Get Interface */
  void (*User_SetInterface)(void);           /* Set Interface */
  void (*User_GetStatus)(void);              /* Get Status */
  void (*User_ClearFeature)(void);           /* Clear Feature */
  void (*User_SetEndPointFeature)(void);     /* Set Endpoint Feature */
  void (*User_SetDeviceFeature)(void);       /* Set Device Feature */
  void (*User_SetDeviceAddress)(void);       /* Set Device Address */
}
USER_STANDARD_REQUESTS;

/* Exported constants --------------------------------------------------------*/
#define Type_Recipient (pInformation->USBbmRequestType & (REQUEST_TYPE | RECIPIENT))

#define Usb_rLength Usb_wLength
#define Usb_rOffset Usb_wOffset

#define USBwValue USBwValues.w
#define USBwValue0 USBwValues.bw.bb0
#define USBwValue1 USBwValues.bw.bb1
#define USBwIndex USBwIndexs.w
#define USBwIndex0 USBwIndexs.bw.bb0
#define USBwIndex1 USBwIndexs.bw.bb1
#define USBwLength USBwLengths.w
#define USBwLength0 USBwLengths.bw.bb0
#define USBwLength1 USBwLengths.bw.bb1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
u8 Setup0_Process(void);
u8 Post0_Process(void);
u8 Out0_Process(void);
u8 In0_Process(void);

RESULT Standard_SetEndPointFeature(void);
RESULT Standard_SetDeviceFeature(void);

u8 *Standard_GetConfiguration(u16 Length);
RESULT Standard_SetConfiguration(void);
u8 *Standard_GetInterface(u16 Length);
RESULT Standard_SetInterface(void);
u8 *Standard_GetDescriptorData(u16 Length, PONE_DESCRIPTOR pDesc);

u8 *Standard_GetStatus(u16 Length);
RESULT Standard_ClearFeature(void);
void SetDeviceAddress(u8);
void NOP_Process(void);

extern DEVICE_PROP Device_Property;
extern  USER_STANDARD_REQUESTS User_Standard_Requests;
extern  DEVICE  Device_Table;
extern DEVICE_INFO Device_Info;

/* cells saving status during interrupt servicing */
extern u16 SaveRState;
extern u16 SaveTState;
typedef struct
{
  u32 bitrate;
  u8 format;
  u8 paritytype;
  u8 datatype;
}LINE_CODING;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

#define Virtual_Com_Port_GetConfiguration          NOP_Process
//#define Virtual_Com_Port_SetConfiguration          NOP_Process
#define Virtual_Com_Port_GetInterface              NOP_Process
#define Virtual_Com_Port_SetInterface              NOP_Process
#define Virtual_Com_Port_GetStatus                 NOP_Process
#define Virtual_Com_Port_ClearFeature              NOP_Process
#define Virtual_Com_Port_SetEndPointFeature        NOP_Process
#define Virtual_Com_Port_SetDeviceFeature          NOP_Process
//#define Virtual_Com_Port_SetDeviceAddress          NOP_Process

#define SEND_ENCAPSULATED_COMMAND   0x00
#define GET_ENCAPSULATED_RESPONSE   0x01
#define SET_COMM_FEATURE            0x02
#define GET_COMM_FEATURE            0x03
#define CLEAR_COMM_FEATURE          0x04
#define SET_LINE_CODING             0x20
#define GET_LINE_CODING             0x21
#define SET_CONTROL_LINE_STATE      0x22
#define SEND_BREAK                  0x23

////CH341 reg
#define CH341_REQ_WRITE_REG    0x9A
#define CH341_REQ_READ_REG     0x95
#define CH341_REG_BREAK1       0x05
#define CH341_REG_BREAK2       0x18
#define CH341_NBREAK_BITS_REG1 0x01
#define CH341_NBREAK_BITS_REG2 0x40
/* Exported functions ------------------------------------------------------- */

void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void Virtual_Com_Port_init(void);
void Virtual_Com_Port_Reset(void);
void Virtual_Com_Port_SetConfiguration(void);
void Virtual_Com_Port_SetDeviceAddress (void);
void Virtual_Com_Port_Status_In (void);
void Virtual_Com_Port_Status_Out (void);
void Get_SerialNum(void);
//void USART_Config_Default(void);
//bool USART_Config(void);
RESULT Virtual_Com_Port_Data_Setup(u8);
RESULT Virtual_Com_Port_NoData_Setup(u8);
RESULT Virtual_Com_Port_Get_Interface_Setting(u8 Interface, u8 AlternateSetting);
u8 *Virtual_Com_Port_GetDeviceDescriptor(u16 );
u8 *Virtual_Com_Port_GetConfigDescriptor(u16);
u8 *Virtual_Com_Port_GetStringDescriptor(u16);

u8 *Virtual_Com_Port_GetLineCoding(u16 Length);
u8 *Virtual_Com_Port_SetLineCoding(u16 Length);
extern u32 ch341_state;
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/
void USB_Init(void);
void Set_USBClock(void);
void USB_Interrupts_Config(void);
/* External variables --------------------------------------------------------*/
/*  The number of current endpoint, it will be used to specify an endpoint */
extern u8	EPindex;
/*  The number of current device, it is an index to the Device_Table */
/*extern u8	Device_no; */
/*  Points to the DEVICE_INFO structure of current device */
/*  The purpose of this register is to speed up the execution */
extern DEVICE_INFO*	pInformation;
/*  Points to the DEVICE_PROP structure of current device */
/*  The purpose of this register is to speed up the execution */
extern DEVICE_PROP*	pProperty;
/*  Temporary save the state of Rx & Tx status. */
/*  Whenever the Rx or Tx state is changed, its value is saved */
/*  in this variable first and will be set to the EPRB or EPRA */
/*  at the end of interrupt process */
extern USER_STANDARD_REQUESTS *pUser_Standard_Requests;

extern u16	SaveState ;
extern u16 wInterrupt_Mask;
void CTR_LP(void);
void CTR_HP(void);
typedef enum _RESUME_STATE
{
  RESUME_EXTERNAL,
  RESUME_INTERNAL,
  RESUME_LATER,
  RESUME_WAIT,
  RESUME_START,
  RESUME_ON,
  RESUME_OFF,
  RESUME_ESOF
} RESUME_STATE;

typedef enum _DEVICE_STATE
{
  UNCONNECTED,
  ATTACHED,
  POWERED,
  SUSPENDED,
  ADDRESSED,
  CONFIGURED
} DEVICE_STATE;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Suspend(void);
void Resume_Init(void);
void Resume(RESUME_STATE eResumeSetVal);
RESULT PowerOn(void);
RESULT PowerOff(void);

/* External variables --------------------------------------------------------*/
extern  vu32 bDeviceState; /* USB device status */
extern volatile bool fSuspendEnabled;  /* true when suspend is possible */
void UserToPMABufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes);
void PMAToUserBufferCopy(u8 *pbUsrBuf, u16 wPMABufAddr, u16 wNBytes);

typedef enum _EP_DBUF_DIR
{
  /* double buffered endpoint direction */
  EP_DBUF_ERR,
  EP_DBUF_OUT,
  EP_DBUF_IN
}EP_DBUF_DIR;

/* endpoint buffer number */
enum EP_BUF_NUM
{
  EP_NOBUF,
  EP_BUF0,
  EP_BUF1
};

/* Exported constants --------------------------------------------------------*/
#define RegBase  (0x40005C00L)  /* USB_IP Peripheral Registers base address */
#define PMAAddr  (0x40006000L)  /* USB_IP Packet Memory Area base address   */

/******************************************************************************/
/*                         General registers                                  */
/******************************************************************************/

/* Control register */
#define CNTR    ((volatile unsigned *)(RegBase + 0x40))
/* Interrupt status register */
#define ISTR    ((volatile unsigned *)(RegBase + 0x44))
/* Frame number register */
#define FNR     ((volatile unsigned *)(RegBase + 0x48))
/* Device address register */
#define DADDR   ((volatile unsigned *)(RegBase + 0x4C))
/* Buffer Table address register */
#define BTABLE  ((volatile unsigned *)(RegBase + 0x50))
/******************************************************************************/
/*                         Endpoint registers                                 */
/******************************************************************************/
#define EP0REG  ((volatile unsigned *)(RegBase)) /* endpoint 0 register address */

/* endpoints enumeration */
#define ENDP0   ((u8)0)
#define ENDP1   ((u8)1)
#define ENDP2   ((u8)2)
#define ENDP3   ((u8)3)
#define ENDP4   ((u8)4)
#define ENDP5   ((u8)5)
#define ENDP6   ((u8)6)
#define ENDP7   ((u8)7)
/******************************************************************************/
/*                       ISTR interrupt events                                */
/******************************************************************************/
#define ISTR_CTR    (0x8000) /* Correct TRansfer (clear-only bit) */
#define ISTR_DOVR   (0x4000) /* DMA OVeR/underrun (clear-only bit) */
#define ISTR_ERR    (0x2000) /* ERRor (clear-only bit) */
#define ISTR_WKUP   (0x1000) /* WaKe UP (clear-only bit) */
#define ISTR_SUSP   (0x0800) /* SUSPend (clear-only bit) */
#define ISTR_RESET  (0x0400) /* RESET (clear-only bit) */
#define ISTR_SOF    (0x0200) /* Start Of Frame (clear-only bit) */
#define ISTR_ESOF   (0x0100) /* Expected Start Of Frame (clear-only bit) */


#define ISTR_DIR    (0x0010)  /* DIRection of transaction (read-only bit)  */
#define ISTR_EP_ID  (0x000F)  /* EndPoint IDentifier (read-only bit)  */

#define CLR_CTR    (~ISTR_CTR)   /* clear Correct TRansfer bit */
#define CLR_DOVR   (~ISTR_DOVR)  /* clear DMA OVeR/underrun bit*/
#define CLR_ERR    (~ISTR_ERR)   /* clear ERRor bit */
#define CLR_WKUP   (~ISTR_WKUP)  /* clear WaKe UP bit     */
#define CLR_SUSP   (~ISTR_SUSP)  /* clear SUSPend bit     */
#define CLR_RESET  (~ISTR_RESET) /* clear RESET bit      */
#define CLR_SOF    (~ISTR_SOF)   /* clear Start Of Frame bit   */
#define CLR_ESOF   (~ISTR_ESOF)  /* clear Expected Start Of Frame bit */

/******************************************************************************/
/*             CNTR control register bits definitions                         */
/******************************************************************************/
#define CNTR_CTRM   (0x8000) /* Correct TRansfer Mask */
#define CNTR_DOVRM  (0x4000) /* DMA OVeR/underrun Mask */
#define CNTR_ERRM   (0x2000) /* ERRor Mask */
#define CNTR_WKUPM  (0x1000) /* WaKe UP Mask */
#define CNTR_SUSPM  (0x0800) /* SUSPend Mask */
#define CNTR_RESETM (0x0400) /* RESET Mask   */
#define CNTR_SOFM   (0x0200) /* Start Of Frame Mask */
#define CNTR_ESOFM  (0x0100) /* Expected Start Of Frame Mask */


#define CNTR_RESUME (0x0010) /* RESUME request */
#define CNTR_FSUSP  (0x0008) /* Force SUSPend */
#define CNTR_LPMODE (0x0004) /* Low-power MODE */
#define CNTR_PDWN   (0x0002) /* Power DoWN */
#define CNTR_FRES   (0x0001) /* Force USB RESet */

/******************************************************************************/
/*                FNR Frame Number Register bit definitions                   */
/******************************************************************************/
#define FNR_RXDP (0x8000) /* status of D+ data line */
#define FNR_RXDM (0x4000) /* status of D- data line */
#define FNR_LCK  (0x2000) /* LoCKed */
#define FNR_LSOF (0x1800) /* Lost SOF */
#define FNR_FN  (0x07FF) /* Frame Number */
/******************************************************************************/
/*               DADDR Device ADDRess bit definitions                         */
/******************************************************************************/
#define DADDR_EF (0x80)
#define DADDR_ADD (0x7F)
/******************************************************************************/
/*                            Endpoint register                               */
/******************************************************************************/
/* bit positions */
#define EP_CTR_RX      (0x8000) /* EndPoint Correct TRansfer RX */
#define EP_DTOG_RX     (0x4000) /* EndPoint Data TOGGLE RX */
#define EPRX_STAT      (0x3000) /* EndPoint RX STATus bit field */
#define EP_SETUP       (0x0800) /* EndPoint SETUP */
#define EP_T_FIELD     (0x0600) /* EndPoint TYPE */
#define EP_KIND        (0x0100) /* EndPoint KIND */
#define EP_CTR_TX      (0x0080) /* EndPoint Correct TRansfer TX */
#define EP_DTOG_TX     (0x0040) /* EndPoint Data TOGGLE TX */
#define EPTX_STAT      (0x0030) /* EndPoint TX STATus bit field */
#define EPADDR_FIELD   (0x000F) /* EndPoint ADDRess FIELD */

/* EndPoint REGister MASK (no toggle fields) */
#define EPREG_MASK     (EP_CTR_RX|EP_SETUP|EP_T_FIELD|EP_KIND|EP_CTR_TX|EPADDR_FIELD)

/* EP_TYPE[1:0] EndPoint TYPE */
#define EP_TYPE_MASK   (0x0600) /* EndPoint TYPE Mask */
#define EP_BULK        (0x0000) /* EndPoint BULK */
#define EP_CONTROL     (0x0200) /* EndPoint CONTROL */
#define EP_ISOCHRONOUS (0x0400) /* EndPoint ISOCHRONOUS */
#define EP_INTERRUPT   (0x0600) /* EndPoint INTERRUPT */
#define EP_T_MASK      (~EP_T_FIELD & EPREG_MASK)


/* EP_KIND EndPoint KIND */
#define EPKIND_MASK    (~EP_KIND & EPREG_MASK)

/* STAT_TX[1:0] STATus for TX transfer */
#define EP_TX_DIS      (0x0000) /* EndPoint TX DISabled */
#define EP_TX_STALL    (0x0010) /* EndPoint TX STALLed */
#define EP_TX_NAK      (0x0020) /* EndPoint TX NAKed */
#define EP_TX_VALID    (0x0030) /* EndPoint TX VALID */
#define EPTX_DTOG1     (0x0010) /* EndPoint TX Data TOGgle bit1 */
#define EPTX_DTOG2     (0x0020) /* EndPoint TX Data TOGgle bit2 */
#define EPTX_DTOGMASK  (EPTX_STAT|EPREG_MASK)

/* STAT_RX[1:0] STATus for RX transfer */
#define EP_RX_DIS      (0x0000) /* EndPoint RX DISabled */
#define EP_RX_STALL    (0x1000) /* EndPoint RX STALLed */
#define EP_RX_NAK      (0x2000) /* EndPoint RX NAKed */
#define EP_RX_VALID    (0x3000) /* EndPoint RX VALID */
#define EPRX_DTOG1     (0x1000) /* EndPoint RX Data TOGgle bit1 */
#define EPRX_DTOG2     (0x2000) /* EndPoint RX Data TOGgle bit1 */
#define EPRX_DTOGMASK  (EPRX_STAT|EPREG_MASK)
/* Exported macro ------------------------------------------------------------*/
/* SetCNTR */
#define _SetCNTR(wRegValue)  (*CNTR   = (u16)wRegValue)

/* SetISTR */
#define _SetISTR(wRegValue)  (*ISTR   = (u16)wRegValue)

/* SetDADDR */
#define _SetDADDR(wRegValue) (*DADDR  = (u16)wRegValue)

/* SetBTABLE */
#define _SetBTABLE(wRegValue)(*BTABLE = (u16)(wRegValue & 0xFFF8))

/* GetCNTR */
#define _GetCNTR()   ((u16) *CNTR)

/* GetISTR */
#define _GetISTR()   ((u16) *ISTR)

/* GetFNR */
#define _GetFNR()    ((u16) *FNR)

/* GetDADDR */
#define _GetDADDR()  ((u16) *DADDR)

/* GetBTABLE */
#define _GetBTABLE() ((u16) *BTABLE)

/* SetENDPOINT */
#define _SetENDPOINT(bEpNum,wRegValue)  (*(EP0REG + bEpNum)= \
    (u16)wRegValue)

/* GetENDPOINT */
#define _GetENDPOINT(bEpNum)        ((u16)(*(EP0REG + bEpNum)))

/*******************************************************************************
* Macro Name     : SetEPType
* Description    : sets the type in the endpoint register(bits EP_TYPE[1:0])
* Input          : bEpNum: Endpoint Number. 
*                  wType
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPType(bEpNum,wType) (_SetENDPOINT(bEpNum,\
                                  ((_GetENDPOINT(bEpNum) & EP_T_MASK) | wType)))

/*******************************************************************************
* Macro Name     : GetEPType
* Description    : gets the type in the endpoint register(bits EP_TYPE[1:0]) 
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : Endpoint Type
*******************************************************************************/
#define _GetEPType(bEpNum) (_GetENDPOINT(bEpNum) & EP_T_FIELD)

/*******************************************************************************
* Macro Name     : SetEPTxStatus
* Description    : sets the status for tx transfer (bits STAT_TX[1:0]).
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxStatus(bEpNum,wState) {\
    register u16 _wRegVal;       \
    _wRegVal = _GetENDPOINT(bEpNum) & EPTX_DTOGMASK;\
    /* toggle first bit ? */     \
    if((EPTX_DTOG1 & wState)!= 0)      \
      _wRegVal ^= EPTX_DTOG1;        \
    /* toggle second bit ?  */         \
    if((EPTX_DTOG2 & wState)!= 0)      \
      _wRegVal ^= EPTX_DTOG2;        \
    _SetENDPOINT(bEpNum, _wRegVal);    \
  } /* _SetEPTxStatus */

/*******************************************************************************
* Macro Name     : SetEPRxStatus
* Description    : sets the status for rx transfer (bits STAT_TX[1:0])
* Input          : bEpNum: Endpoint Number. 
*                  wState: new state.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPRxStatus(bEpNum,wState) {\
    register u16 _wRegVal;   \
    \
    _wRegVal = _GetENDPOINT(bEpNum) & EPRX_DTOGMASK;\
    /* toggle first bit ? */  \
    if((EPRX_DTOG1 & wState)!= 0) \
      _wRegVal ^= EPRX_DTOG1;  \
    /* toggle second bit ? */  \
    if((EPRX_DTOG2 & wState)!= 0) \
      _wRegVal ^= EPRX_DTOG2;  \
    _SetENDPOINT(bEpNum, _wRegVal); \
  } /* _SetEPRxStatus */
/*******************************************************************************
* Macro Name     : GetEPTxStatus / GetEPRxStatus 
* Description    : gets the status for tx/rx transfer (bits STAT_TX[1:0]
*                  /STAT_RX[1:0])
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : status .
*******************************************************************************/
#define _GetEPTxStatus(bEpNum) ((u16)_GetENDPOINT(bEpNum) & EPTX_STAT)

#define _GetEPRxStatus(bEpNum) ((u16)_GetENDPOINT(bEpNum) & EPRX_STAT)

/*******************************************************************************
* Macro Name     : SetEPTxValid / SetEPRxValid 
* Description    : sets directly the VALID tx/rx-status into the enpoint register
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxValid(bEpNum)     (_SetEPTxStatus(bEpNum, EP_TX_VALID))

#define _SetEPRxValid(bEpNum)     (_SetEPRxStatus(bEpNum, EP_RX_VALID))

/*******************************************************************************
* Macro Name     : GetTxStallStatus / GetRxStallStatus.
* Description    : checks stall condition in an endpoint.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : TRUE = endpoint in stall condition.
*******************************************************************************/
#define _GetTxStallStatus(bEpNum) (_GetEPTxStatus(bEpNum) \
                                   == EP_TX_STALL)
#define _GetRxStallStatus(bEpNum) (_GetEPRxStatus(bEpNum) \
                                   == EP_RX_STALL)

/*******************************************************************************
* Macro Name     : SetEP_KIND / ClearEP_KIND.
* Description    : set & clear EP_KIND bit.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEP_KIND(bEpNum)    (_SetENDPOINT(bEpNum, \
                                (_GetENDPOINT(bEpNum) | EP_KIND) & EPREG_MASK))
#define _ClearEP_KIND(bEpNum)  (_SetENDPOINT(bEpNum, \
                                (_GetENDPOINT(bEpNum) & EPKIND_MASK)))

/*******************************************************************************
* Macro Name     : Set_Status_Out / Clear_Status_Out.
* Description    : Sets/clears directly STATUS_OUT bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _Set_Status_Out(bEpNum)    _SetEP_KIND(bEpNum)
#define _Clear_Status_Out(bEpNum)  _ClearEP_KIND(bEpNum)

/*******************************************************************************
* Macro Name     : SetEPDoubleBuff / ClearEPDoubleBuff.
* Description    : Sets/clears directly EP_KIND bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDoubleBuff(bEpNum)   _SetEP_KIND(bEpNum)
#define _ClearEPDoubleBuff(bEpNum) _ClearEP_KIND(bEpNum)

/*******************************************************************************
* Macro Name     : ClearEP_CTR_RX / ClearEP_CTR_TX.
* Description    : Clears bit CTR_RX / CTR_TX in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ClearEP_CTR_RX(bEpNum)   (_SetENDPOINT(bEpNum,\
                                   _GetENDPOINT(bEpNum) & 0x7FFF & EPREG_MASK))
#define _ClearEP_CTR_TX(bEpNum)   (_SetENDPOINT(bEpNum,\
                                   _GetENDPOINT(bEpNum) & 0xFF7F & EPREG_MASK))

/*******************************************************************************
* Macro Name     : ToggleDTOG_RX / ToggleDTOG_TX .
* Description    : Toggles DTOG_RX / DTOG_TX bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ToggleDTOG_RX(bEpNum)    (_SetENDPOINT(bEpNum, \
                                   EP_DTOG_RX | _GetENDPOINT(bEpNum) & EPREG_MASK))
#define _ToggleDTOG_TX(bEpNum)    (_SetENDPOINT(bEpNum, \
                                   EP_DTOG_TX | _GetENDPOINT(bEpNum) & EPREG_MASK))

/*******************************************************************************
* Macro Name     : ClearDTOG_RX / ClearDTOG_TX.
* Description    : Clears DTOG_RX / DTOG_TX bit in the endpoint register.
* Input          : bEpNum: Endpoint Number. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _ClearDTOG_RX(bEpNum)  if((_GetENDPOINT(bEpNum) & EP_DTOG_RX) != 0)\
    _ToggleDTOG_RX(bEpNum)
#define _ClearDTOG_TX(bEpNum)  if((_GetENDPOINT(bEpNum) & EP_DTOG_TX) != 0)\
    _ToggleDTOG_TX(bEpNum)
/*******************************************************************************
* Macro Name     : SetEPAddress.
* Description    : Sets address in an endpoint register.
* Input          : bEpNum: Endpoint Number.
*                  bAddr: Address. 
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPAddress(bEpNum,bAddr) _SetENDPOINT(bEpNum,\
    _GetENDPOINT(bEpNum) & EPREG_MASK | bAddr)

/*******************************************************************************
* Macro Name     : GetEPAddress.
* Description    : Gets address in an endpoint register.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPAddress(bEpNum) ((u8)(_GetENDPOINT(bEpNum) & EPADDR_FIELD))

#define _pEPTxAddr(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8  )*2 + PMAAddr))
#define _pEPTxCount(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8+2)*2 + PMAAddr))
#define _pEPRxAddr(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8+4)*2 + PMAAddr))
#define _pEPRxCount(bEpNum) ((u32 *)((_GetBTABLE()+bEpNum*8+6)*2 + PMAAddr))

/*******************************************************************************
* Macro Name     : SetEPTxAddr / SetEPRxAddr.
* Description    : sets address of the tx/rx buffer.
* Input          : bEpNum: Endpoint Number.
*                  wAddr: address to be set (must be word aligned).
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxAddr(bEpNum,wAddr) (*_pEPTxAddr(bEpNum) = ((wAddr >> 1) << 1))
#define _SetEPRxAddr(bEpNum,wAddr) (*_pEPRxAddr(bEpNum) = ((wAddr >> 1) << 1))

/*******************************************************************************
* Macro Name     : GetEPTxAddr / GetEPRxAddr.
* Description    : Gets address of the tx/rx buffer.
* Input          : bEpNum: Endpoint Number.
* Output         : None.
* Return         : address of the buffer.
*******************************************************************************/
#define _GetEPTxAddr(bEpNum) ((u16)*_pEPTxAddr(bEpNum))
#define _GetEPRxAddr(bEpNum) ((u16)*_pEPRxAddr(bEpNum))

/*******************************************************************************
* Macro Name     : SetEPCountRxReg.
* Description    : Sets counter of rx buffer with no. of blocks.
* Input          : pdwReg: pointer to counter.
*                  wCount: Counter.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _BlocksOf32(dwReg,wCount,wNBlocks) {\
    wNBlocks = wCount >> 5;\
    if((wCount & 0x1f) == 0)\
      wNBlocks--;\
    *pdwReg = (u32)((wNBlocks << 10) | 0x8000);\
  }/* _BlocksOf32 */

#define _BlocksOf2(dwReg,wCount,wNBlocks) {\
    wNBlocks = wCount >> 1;\
    if((wCount & 0x1) != 0)\
      wNBlocks++;\
    *pdwReg = (u32)(wNBlocks << 10);\
  }/* _BlocksOf2 */

#define _SetEPCountRxReg(dwReg,wCount)  {\
    u16 wNBlocks;\
    if(wCount > 62){_BlocksOf32(dwReg,wCount,wNBlocks);}\
    else {_BlocksOf2(dwReg,wCount,wNBlocks);}\
  }/* _SetEPCountRxReg */



#define _SetEPRxDblBuf0Count(bEpNum,wCount) {\
    u32 *pdwReg = _pEPTxCount(bEpNum); \
    _SetEPCountRxReg(pdwReg, wCount);\
  }
/*******************************************************************************
* Macro Name     : SetEPTxCount / SetEPRxCount.
* Description    : sets counter for the tx/rx buffer.
* Input          : bEpNum: endpoint number.
*                  wCount: Counter value.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPTxCount(bEpNum,wCount) (*_pEPTxCount(bEpNum) = wCount)
#define _SetEPRxCount(bEpNum,wCount) {\
    u32 *pdwReg = _pEPRxCount(bEpNum); \
    _SetEPCountRxReg(pdwReg, wCount);\
  }
/*******************************************************************************
* Macro Name     : GetEPTxCount / GetEPRxCount.
* Description    : gets counter of the tx buffer.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : Counter value.
*******************************************************************************/
#define _GetEPTxCount(bEpNum)((u16)(*_pEPTxCount(bEpNum)) & 0x3ff)
#define _GetEPRxCount(bEpNum)((u16)(*_pEPRxCount(bEpNum)) & 0x3ff)

/*******************************************************************************
* Macro Name     : SetEPDblBuf0Addr / SetEPDblBuf1Addr.
* Description    : Sets buffer 0/1 address in a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : wBuf0Addr: buffer 0 address.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuf0Addr(bEpNum,wBuf0Addr) {_SetEPTxAddr(bEpNum, wBuf0Addr);}
#define _SetEPDblBuf1Addr(bEpNum,wBuf1Addr) {_SetEPRxAddr(bEpNum, wBuf1Addr);}

/*******************************************************************************
* Macro Name     : SetEPDblBuffAddr.
* Description    : Sets addresses in a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : wBuf0Addr: buffer 0 address.
*                : wBuf1Addr = buffer 1 address.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuffAddr(bEpNum,wBuf0Addr,wBuf1Addr) { \
    _SetEPDblBuf0Addr(bEpNum, wBuf0Addr);\
    _SetEPDblBuf1Addr(bEpNum, wBuf1Addr);\
  } /* _SetEPDblBuffAddr */

/*******************************************************************************
* Macro Name     : GetEPDblBuf0Addr / GetEPDblBuf1Addr.
* Description    : Gets buffer 0/1 address of a double buffer endpoint.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPDblBuf0Addr(bEpNum) (_GetEPTxAddr(bEpNum))
#define _GetEPDblBuf1Addr(bEpNum) (_GetEPRxAddr(bEpNum))

/*******************************************************************************
* Macro Name     : SetEPDblBuffCount / SetEPDblBuf0Count / SetEPDblBuf1Count.
* Description    : Gets buffer 0/1 address of a double buffer endpoint.
* Input          : bEpNum: endpoint number.
*                : bDir: endpoint dir  EP_DBUF_OUT = OUT 
*                                      EP_DBUF_IN  = IN 
*                : wCount: Counter value    
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _SetEPDblBuf0Count(bEpNum, bDir, wCount)  { \
    if(bDir == EP_DBUF_OUT)\
      /* OUT endpoint */ \
    {_SetEPRxDblBuf0Count(bEpNum,wCount);} \
    else if(bDir == EP_DBUF_IN)\
      /* IN endpoint */ \
      *_pEPTxCount(bEpNum) = (u32)wCount;  \
  } /* SetEPDblBuf0Count*/

#define _SetEPDblBuf1Count(bEpNum, bDir, wCount)  { \
    if(bDir == EP_DBUF_OUT)\
      /* OUT endpoint */ \
    {_SetEPRxCount(bEpNum,wCount);}\
    else if(bDir == EP_DBUF_IN)\
      /* IN endpoint */\
      *_pEPRxCount(bEpNum) = (u32)wCount; \
  } /* SetEPDblBuf1Count */

#define _SetEPDblBuffCount(bEpNum, bDir, wCount) {\
    _SetEPDblBuf0Count(bEpNum, bDir, wCount); \
    _SetEPDblBuf1Count(bEpNum, bDir, wCount); \
  } /* _SetEPDblBuffCount  */

/*******************************************************************************
* Macro Name     : GetEPDblBuf0Count / GetEPDblBuf1Count.
* Description    : Gets buffer 0/1 rx/tx counter for double buffering.
* Input          : bEpNum: endpoint number.
* Output         : None.
* Return         : None.
*******************************************************************************/
#define _GetEPDblBuf0Count(bEpNum) (_GetEPTxCount(bEpNum))
#define _GetEPDblBuf1Count(bEpNum) (_GetEPRxCount(bEpNum))


/* External variables --------------------------------------------------------*/
extern volatile u16 wIstr;  /* ISTR register last read value */

/* Exported functions ------------------------------------------------------- */
void SetCNTR(u16 /*wRegValue*/);
void SetISTR(u16 /*wRegValue*/);
void SetDADDR(u16 /*wRegValue*/);
void SetBTABLE(u16 /*wRegValue*/);
void SetBTABLE(u16 /*wRegValue*/);
u16 GetCNTR(void);
u16 GetISTR(void);
u16 GetFNR(void);
u16 GetDADDR(void);
u16 GetBTABLE(void);
void SetENDPOINT(u8 /*bEpNum*/, u16 /*wRegValue*/);
u16 GetENDPOINT(u8 /*bEpNum*/);
void SetEPType(u8 /*bEpNum*/, u16 /*wType*/);
u16 GetEPType(u8 /*bEpNum*/);
void SetEPTxStatus(u8 /*bEpNum*/, u16 /*wState*/);
void SetEPRxStatus(u8 /*bEpNum*/, u16 /*wState*/);
void SetDouBleBuffEPStall(u8 /*bEpNum*/, u8 bDir);
u16 GetEPTxStatus(u8 /*bEpNum*/);
u16 GetEPRxStatus(u8 /*bEpNum*/);
void SetEPTxValid(u8 /*bEpNum*/);
void SetEPRxValid(u8 /*bEpNum*/);
u16 GetTxStallStatus(u8 /*bEpNum*/);
u16 GetRxStallStatus(u8 /*bEpNum*/);
void SetEP_KIND(u8 /*bEpNum*/);
void ClearEP_KIND(u8 /*bEpNum*/);
void Set_Status_Out(u8 /*bEpNum*/);
void Clear_Status_Out(u8 /*bEpNum*/);
void SetEPDoubleBuff(u8 /*bEpNum*/);
void ClearEPDoubleBuff(u8 /*bEpNum*/);
void ClearEP_CTR_RX(u8 /*bEpNum*/);
void ClearEP_CTR_TX(u8 /*bEpNum*/);
void ToggleDTOG_RX(u8 /*bEpNum*/);
void ToggleDTOG_TX(u8 /*bEpNum*/);
void ClearDTOG_RX(u8 /*bEpNum*/);
void ClearDTOG_TX(u8 /*bEpNum*/);
void SetEPAddress(u8 /*bEpNum*/, u8 /*bAddr*/);
u8 GetEPAddress(u8 /*bEpNum*/);
void SetEPTxAddr(u8 /*bEpNum*/, u16 /*wAddr*/);
void SetEPRxAddr(u8 /*bEpNum*/, u16 /*wAddr*/);
u16 GetEPTxAddr(u8 /*bEpNum*/);
u16 GetEPRxAddr(u8 /*bEpNum*/);
void SetEPCountRxReg(u32 * /*pdwReg*/, u16 /*wCount*/);
void SetEPTxCount(u8 /*bEpNum*/, u16 /*wCount*/);
void SetEPRxCount(u8 /*bEpNum*/, u16 /*wCount*/);
u16 GetEPTxCount(u8 /*bEpNum*/);
u16 GetEPRxCount(u8 /*bEpNum*/);
void SetEPDblBuf0Addr(u8 /*bEpNum*/, u16 /*wBuf0Addr*/);
void SetEPDblBuf1Addr(u8 /*bEpNum*/, u16 /*wBuf1Addr*/);
void SetEPDblBuffAddr(u8 /*bEpNum*/, u16 /*wBuf0Addr*/, u16 /*wBuf1Addr*/);
u16 GetEPDblBuf0Addr(u8 /*bEpNum*/);
u16 GetEPDblBuf1Addr(u8 /*bEpNum*/);
void SetEPDblBuffCount(u8 /*bEpNum*/, u8 /*bDir*/, u16 /*wCount*/);
void SetEPDblBuf0Count(u8 /*bEpNum*/, u8 /*bDir*/, u16 /*wCount*/);
void SetEPDblBuf1Count(u8 /*bEpNum*/, u8 /*bDir*/, u16 /*wCount*/);
u16 GetEPDblBuf0Count(u8 /*bEpNum*/);
u16 GetEPDblBuf1Count(u8 /*bEpNum*/);
EP_DBUF_DIR GetEPDblBufDir(u8 /*bEpNum*/);
void FreeUserBuffer(u8 bEpNum/*bEpNum*/, u8 bDir);
u16 ToWord(u8, u8);
u16 ByteSwap(u16);

/*-------------------------------------------------------------*/
/* EP_NUM */
/* defines how many endpoints are used by the device */
/*-------------------------------------------------------------*/

#define EP_NUM                          (4)

/*-------------------------------------------------------------*/
/* --------------   Buffer Description Table  -----------------*/
/*-------------------------------------------------------------*/
/* buffer table base address */
/* buffer table base address */
#define BTABLE_ADDRESS      (0x00)

/* EP0  */
/* rx/tx buffer base address */
#define ENDP0_RXADDR        (0x40)
#define ENDP0_TXADDR        (0x80)

/* EP1  */
/* tx buffer base address */
#define ENDP1_TXADDR        (0xc0)
#define ENDP2_TXADDR        (ENDP1_TXADDR+0x40)
#define ENDP2_RXADDR        (ENDP2_TXADDR+0X40)
#ifndef NULL
#define NULL ((void *)0)
#endif
void USB_Istr(void);

/* function prototypes Automatically built defining related macros */

#ifdef CTR_CALLBACK
  void CTR_Callback(void);
#endif

#ifdef DOVR_CALLBACK
  void DOVR_Callback(void);
#endif

#ifdef ERR_CALLBACK
  void ERR_Callback(void);
#endif

#ifdef WKUP_CALLBACK
  void WKUP_Callback(void);
#endif

#ifdef SUSP_CALLBACK
  void SUSP_Callback(void);
#endif

#ifdef RESET_CALLBACK
  void RESET_Callback(void);
#endif

#ifdef SOF_CALLBACK
  void SOF_Callback(void);
#endif

#ifdef ESOF_CALLBACK
  void ESOF_Callback(void);
#endif

void EP1_IN_Callback(void);
void EP2_IN_Callback(void);
void EP3_IN_Callback(void);
void EP4_IN_Callback(void);
void EP5_IN_Callback(void);
void EP6_IN_Callback(void);
void EP7_IN_Callback(void);

void EP1_OUT_Callback(void);
void EP2_OUT_Callback(void);
void EP3_OUT_Callback(void);
void EP4_OUT_Callback(void);
void EP5_OUT_Callback(void);
void EP6_OUT_Callback(void);
void EP7_OUT_Callback(void);
#define IMR_MSK (CNTR_CTRM  | CNTR_SOFM  | CNTR_RESETM )

/*#define CTR_CALLBACK*/
/*#define DOVR_CALLBACK*/
/*#define ERR_CALLBACK*/
/*#define WKUP_CALLBACK*/
/*#define SUSP_CALLBACK*/
/*#define RESET_CALLBACK*/
/*#define SOF_CALLBACK*/
/*#define ESOF_CALLBACK*/

/* CTR service routines */
/* associated to defined endpoints */
/*#define  EP1_IN_Callback   NOP_Process*/
//#define  EP2_IN_Callback   NOP_Process
#define  EP3_IN_Callback   NOP_Process
#define  EP4_IN_Callback   NOP_Process
#define  EP5_IN_Callback   NOP_Process
#define  EP6_IN_Callback   NOP_Process
#define  EP7_IN_Callback   NOP_Process

#define  EP1_OUT_Callback   NOP_Process
//#define  EP2_OUT_Callback   NOP_Process
#define  EP3_OUT_Callback   NOP_Process
#define  EP4_OUT_Callback   NOP_Process
#define  EP5_OUT_Callback   NOP_Process
#define  EP6_OUT_Callback   NOP_Process
#define  EP7_OUT_Callback   NOP_Process
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define VIRTUAL_COM_PORT_DATA_SIZE              32
#define VIRTUAL_COM_PORT_INT_SIZE               8

#define VIRTUAL_COM_PORT_SIZ_DEVICE_DESC        18
#define VIRTUAL_COM_PORT_SIZ_CONFIG_DESC        0x27
#define VIRTUAL_COM_PORT_SIZ_STRING_LANGID      4
#define VIRTUAL_COM_PORT_SIZ_STRING_VENDOR      38
#define VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT     50
#define VIRTUAL_COM_PORT_SIZ_STRING_SERIAL      26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

/* Exported functions ------------------------------------------------------- */
extern const u8 Virtual_Com_Port_DeviceDescriptor[VIRTUAL_COM_PORT_SIZ_DEVICE_DESC];
extern const u8 Virtual_Com_Port_ConfigDescriptor[VIRTUAL_COM_PORT_SIZ_CONFIG_DESC];

extern const u8 Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID];
extern const u8 Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR];
extern const u8 Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT];
extern u8 Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL];
typedef enum _RECIPIENT_TYPE
{
  DEVICE_RECIPIENT,     /* Recipient device */
  INTERFACE_RECIPIENT,  /* Recipient interface */
  ENDPOINT_RECIPIENT,   /* Recipient endpoint */
  OTHER_RECIPIENT
} RECIPIENT_TYPE;


typedef enum _STANDARD_REQUESTS
{
  GET_STATUS = 0,
  CLEAR_FEATURE,
  RESERVED1,
  SET_FEATURE,
  RESERVED2,
  SET_ADDRESS,
  GET_DESCRIPTOR,
  SET_DESCRIPTOR,
  GET_CONFIGURATION,
  SET_CONFIGURATION,
  GET_INTERFACE,
  SET_INTERFACE,
  TOTAL_sREQUEST,  /* Total number of Standard request */
  SYNCH_FRAME = 12
} STANDARD_REQUESTS;

/* Definition of "USBwValue" */
typedef enum _DESCRIPTOR_TYPE
{
  DEVICE_DESCRIPTOR = 1,
  CONFIG_DESCRIPTOR,
  STRING_DESCRIPTOR,
  INTERFACE_DESCRIPTOR,
  ENDPOINT_DESCRIPTOR
} DESCRIPTOR_TYPE;

/* Feature selector of a SET_FEATURE or CLEAR_FEATURE */
typedef enum _FEATURE_SELECTOR
{
  ENDPOINT_STALL,
  DEVICE_REMOTE_WAKEUP
} FEATURE_SELECTOR;

/* Exported constants --------------------------------------------------------*/
/* Definition of "USBbmRequestType" */
#define REQUEST_TYPE      0x60  /* Mask to get request type */
#define STANDARD_REQUEST  0x00  /* Standard request */
#define CLASS_REQUEST     0x20  /* Class request */
#define VENDOR_REQUEST    0x40  /* Vendor request */

#define RECIPIENT         0x1F  /* Mask to get recipient */

#endif /* __USB_LIB_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

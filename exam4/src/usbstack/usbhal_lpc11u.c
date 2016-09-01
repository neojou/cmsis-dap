
//
// Ref : https://developer.mbed.org/users/mbed_official/code/USBDevice/file/01321bd6ff89/USBDevice/USBHAL_LPC11U.cpp
//

#include <stdint.h>
#include "compiler.h"

#include "LPC11Uxx.h"
#include "gpio.h"

#include "usbhal.h"


// endpoint

/* SETUP packet size */
#define SETUP_PACKET_SIZE (8)

/* Options flags for configuring endpoints */
#define DEFAULT_OPTIONS     (0)
#define SINGLE_BUFFERED     (1U << 0)
#define ISOCHRONOUS         (1U << 1)
#define RATE_FEEDBACK_MODE  (1U << 2) /* Interrupt endpoints only */

/* Endpoint transfer status, for endpoints > 0 */
typedef enum {
    EP_COMPLETED,   /* Transfer completed */
    EP_PENDING,     /* Transfer in progress */
    EP_INVALID,     /* Invalid parameter */
    EP_STALLED,     /* Endpoint stalled */
} EP_STATUS;

#define NUMBER_OF_LOGICAL_ENDPOINTS (5)
#define NUMBER_OF_PHYSICAL_ENDPOINTS (NUMBER_OF_LOGICAL_ENDPOINTS * 2)

/* Define physical endpoint numbers */

/*      Endpoint    No.     Type(s)       MaxPacket   DoubleBuffer  */
/*      ----------------    ------------  ----------  ---           */
#define EP0OUT      (0)  /* Control       64          No            */
#define EP0IN       (1)  /* Control       64          No            */
#define EP1OUT      (2)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP1IN       (3)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP2OUT      (4)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP2IN       (5)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP3OUT      (6)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP3IN       (7)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP4OUT      (8)  /* Int/Bulk/Iso  64/64/1023  Yes           */
#define EP4IN       (9)  /* Int/Bulk/Iso  64/64/1023  Yes           */

/* Maximum Packet sizes */

#define MAX_PACKET_SIZE_EP0 (64)
#define MAX_PACKET_SIZE_EP1 (64) /* Int/Bulk */
#define MAX_PACKET_SIZE_EP2 (64) /* Int/Bulk */
#define MAX_PACKET_SIZE_EP3 (64) /* Int/Bulk */
#define MAX_PACKET_SIZE_EP4 (64) /* Int/Bulk */

#define MAX_PACKET_SIZE_EP1_ISO (1023) /* Isochronous */
#define MAX_PACKET_SIZE_EP2_ISO (1023) /* Isochronous */
#define MAX_PACKET_SIZE_EP3_ISO (1023) /* Isochronous */
#define MAX_PACKET_SIZE_EP4_ISO (1023) /* Isochronous */

/* Generic endpoints - intended to be portable accross devices */
/* and be suitable for simple USB devices. */

/* Bulk endpoint */
#define EPBULK_OUT  (EP2OUT)
#define EPBULK_IN   (EP2IN)
#define EPBULK_OUT_callback   EP2_OUT_callback
#define EPBULK_IN_callback    EP2_IN_callback
/* Interrupt endpoint */
#define EPINT_OUT   (EP1OUT)
#define EPINT_IN    (EP1IN)
#define EPINT_OUT_callback    EP1_OUT_callback
#define EPINT_IN_callback     EP1_IN_callback
/* Isochronous endpoint */
#define EPISO_OUT   (EP3OUT)
#define EPISO_IN    (EP3IN)
#define EPISO_OUT_callback    EP3_OUT_callback
#define EPISO_IN_callback     EP3_IN_callback

#define MAX_PACKET_SIZE_EPBULK  (MAX_PACKET_SIZE_EP2)
#define MAX_PACKET_SIZE_EPINT   (MAX_PACKET_SIZE_EP1)
#define MAX_PACKET_SIZE_EPISO   (MAX_PACKET_SIZE_EP3_ISO)

// Covert physical endpoint number to register bit
#define EP(endpoint) (1UL<<endpoint)

// Get endpoint direction
#define IN_EP(endpoint)		((endpoint) & 1U ? true : false)
#define OUT_EP(endpoint)	((endpoint) & 1U ? false : true)


//

// SYSAHBCLKCTRL
#define CLK_USB 	(1UL<<14)
#define CLK_USBRAM	(1UL<<27)

#define EP_LIST_BASE       0x20004400
#define EP_BUF_BASE        (uint32_t)(EP_LIST_BASE + 0x100)

// USB Information register
#define FRAME_NR(a)			((a) & 0x7ff)			// Frame number

// USB Device Command/Status register
#define DEV_ADDR_MASK		(0x7f)					// Device address
#define DEV_ADDR(a)			((a) & DEV_ADDR_MASK)
#define DEV_EN				(1UL<<7)				// Device Enable
#define SETUP				(1UL<<8)				// SETUP token received
#define PLL_ON				(1UL<<9)				// PLL enabled in suspend
#define DCON				(1UL<<16)				// Device status - connect
#define DSUS				(1UL<<17)				// Device status - suspend
#define DCON_C				(1UL<<24)				// Connect change
#define DSUS_C				(1UL<<25)				// Suspend change
#define DRES_C				(1UL<<26)				// Reset change
#define VBUSDEBOUNCED		(1UL<<28)				// Vbus detected


// Endpoint Command/Status list
#define CMDSTS_A					(1UL<<31)			// Active
#define CMDSTS_D					(1UL<<30)			// Disable
#define CMDSTS_S					(1UL<<29)			// Stall
#define CMDSTS_TR					(1UL<<28)			// Toggle Reset
#define CMDSTS_RF					(1UL<<27)			// Rate Feedback mode
#define CMDSTS_TV					(1UL<<27)			// Toggle Value
#define CMDSTS_T					(1UL<<26)			// Endpoint Type
#define CMDSTS_NBYTES(n)			(((n)&0x3ff)<<16)	// Number of bytes
#define CMDSTS_ADDRESS_OFFSET(a) 	(((a)>>6)&0xFFFF)	// Buffer start address

#define BYTES_REMAINING(s)		(((s)>>16)&0x3FF)		// Bytes remaining after transfer


// USB Non-endpoint interrupt sources
#define FRAME_INT	(1UL<<30)
#define DEV_INT		(1UL<<31)

static volatile int epComplete = 0;

// One entry for a double-buffered logical endpoint in the endpoint
// command/status list. Endpoint 0 is single buffered, out[1] is used
// for the SETUP packet and in[1] is not used
typedef struct {
	uint32_t out[2];
	uint32_t in[2];
} __packed EP_COMMAND_STATUS;

typedef struct {
	uint8_t out[MAX_PACKET_SIZE_EP0];
	uint8_t in[MAX_PACKET_SIZE_EP0];
	uint8_t setup[SETUP_PACKET_SIZE];
} __packed CONTROL_TRANSFER ;

typedef struct {
	uint32_t maxPacket;
	uint32_t buffer[2];
	uint32_t options;
} __packed EP_STATE;

// Pointer to the endpoint command/status list
static EP_COMMAND_STATUS *ep;

// Pointer to endpoint 0 data (IN/OUT and SETUP)
static CONTROL_TRANSFER *ct;

// Shadow DEVCMDSTAT register to avoid accidentally clearing flags or
// initiating a remote wakeup event;
static volatile uint32_t devCmdStat;


static void delay_1ms(volatile int msec)
{
	volatile int i;

	while (msec--) {
		i = 7990;
		while(i--);
	}
}

static void dap_led_fast_blink(int times)
{
	int i;
	for (i=times*2; i>0; i--) {
		gpio_set_dap_led(i&0x1);
		delay_1ms(100);
	}
}

/*
 *  USB Device Initialize Function
 *   Called by the User to initialize USB Device
 *    Return Value:    None
 */

void usbhal_init (void)
{
  NVIC_DisableIRQ(USB_IRQn);

  LPC_USB->DEVCMDSTAT  |=   (1UL << 9); /* PLL ON                             */

  // USB_VBUS input with pull-down
  LPC_IOCON->PIO0_3    &=  ~(0x1F    );
  LPC_IOCON->PIO0_3    |=   (1UL << 3)| /* pull-down                          */
                            (1UL << 0); /* Secondary function VBUS            */

  // nUSB_CONNECT output
  LPC_IOCON->PIO0_6    &=   ~7;
  LPC_IOCON->PIO0_6    |=   (1UL << 0); /* Secondary function USB CON         */


  LPC_SYSCON->SYSAHBCLKCTRL |= (1UL <<  6);

  // Enable clocks (USB registers, USB RAM)
  LPC_SYSCON->SYSAHBCLKCTRL |= CLK_USB | CLK_USBRAM;

  // Ensure device disconnected (DCON not set)
  LPC_USB->DEVCMDSTAT = 0;

  // to ensure that the USB host sees the device as
  // disconnected if the target CPU is reset
  // wait(0.3);

  // Reserve space in USB RAM for endpoint command/status list
  // Must be 256 byte aligned
  ep = (EP_COMMAND_STATUS *)EP_LIST_BASE;
  LPC_USB->EPLISTSTART  = EP_LIST_BASE;

  // Reserve space in USB RAM for Endpoint 0
  // Must be 64 byte aligned
  ct = (CONTROL_TRANSFER *)EP_BUF_BASE;
  LPC_USB->DATABUFSTART = EP_BUF_BASE & 0xFFC00000;

  // Setup command/status list for EP0
  ep[0].out[0] = 0;
  ep[0].in[0] = 0;
  ep[0].out[1] = CMDSTS_ADDRESS_OFFSET((uint32_t)ct->setup);

  // Route all interrupts to IRQ, some can be routed to
  // USB_FIQ if you wish
  LPC_USB->INTROUTING = 0;

  // Set device address 0, enable USB device, no remote wakeup
  devCmdStat = DEV_ADDR(0) | DEV_EN |DSUS;
  LPC_USB->DEVCMDSTAT = devCmdStat;

  // Enable interrupts for device events and EP0
  LPC_USB->INTEN  = DEV_INT | EP(EP0IN) | EP(EP0OUT) | FRAME_INT;

  //LPC_SYSCON->DRUNCFG &= ~((1UL << 8)| /* USB PLL powered                    */
  //                          (1UL <<10));/* USB transceiver powered            */

  //LPC_USB->INTSTAT     = 0x2FC;              /* clear EP interrupt flags      */

  //NVIC_SetVector(USB_IRQn, (uint32_t)&usbhal_irqhandler);
}

/*
 *  USB Device Connect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */
void usbhal_connect(int con) {

  if ( con ) {
	  NVIC_EnableIRQ(USB_IRQn);
	  devCmdStat |=  DCON;/* Set device connect status          */
  } else {
	  NVIC_DisableIRQ(USB_IRQn);
      devCmdStat &= ~DCON;/* Clear device connect status        */
  }

  LPC_USB->DEVCMDSTAT = devCmdStat;

  return;
}


/*
 *  USB Device Interrupt Service Routine
 */
void usbhal_irqhandler(void)
{
	if ( LPC_USB->INTSTAT & FRAME_INT ) {
		// write 1 to clear SOF interrupt
		LPC_USB->INTSTAT = FRAME_INT;

		// SOF event, read frame number
		//SOF(Frame_NR(LPC_USB->INFO));
	}

	if ( LPC_USB->INTSTAT & DEV_INT ) {
		LPC_USB->INTSTAT = DEV_INT;

		// TODO : Suspend
		// TODO : Bus Reset
	}

	// Endpoint 0
	// EP0 out
	if ( LPC_USB->INTSTAT & EP(EP0OUT) ) {
		// Clear EP0OUT/SETUP interrupt
		LPC_USB->INTSTAT = EP(EP0OUT);

		// Check if SETUP
		if ( LPC_USB->DEVCMDSTAT & SETUP ) {
			// Clear Active and Stall bits for EP0
			// Documentation does not make it clear if we must use the
			// EPSKIP register to achieve this. Fig. 16 and NXP reference
			// code suggests we can just clear the Active bits - check with
			// NXP to be sure.
			ep[0].in[0] = 0;
			ep[0].out[0] = 0;

			// Clear EP0IN interrupt
			LPC_USB->INTSTAT = EP(EP0IN);

			// Clear SETUP (and INTONNAK_CI/O) in device status register
			LPC_USB->DEVCMDSTAT = devCmdStat | SETUP;

			// EP0 SETUP event (SETUP data received)
			// TODO :
			dap_led_fast_blink(7);
			//EP0SetupCallbak();
		} else {
			//EP0out();
		}
	}

	// EP0 in
	if ( LPC_USB->INTSTAT & EP(EP0IN) ) {
		// Clear EP0IN interrupt
		LPC_USB->INTSTAT = EP(EP0IN);

		// TODO
		// EP0IN ACK event (IN data sent)
		//EP0IN();
	}

	for (uint8_t num = 2; num < 5*2; num++) {
		if (LPC_USB->INTSTAT & EP(num)) {
			LPC_USB->INTSTAT = EP(num);

			// TODO
			//epComplete |= EP(num);
			//if ((*(epCallback[num - 2]))()) {
			//	epComplete &= ~EP(num);
			//}
		}
	}
}

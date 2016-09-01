

#include "usb_device.h"

int USBD_Configuration;

/*
 *  Init USB Device Core and Hardware
 *    Parameters:      None
 *    Return Value:    None
 */

void usbd_init (void)
{
  // init upper class
  //usbd_class_init();
  //USBD_RTX_TaskInit();

  // init hardware
  usbhal_init();

}


/*
 *  USB Device Configured Function
 *   Called by the User to check id USB Device is configured
 *    Parameters:
 *    Return Value:    Configurated state (FALSE = unconfigured, TRUE = configured)
 */
int usbd_configured(void)
{

  if (USBD_Configuration)
    return 1;

  return 0;
}

/*
 *  USB Device Connect/Disconnect Function
 *   Called by the User to Connect/Disconnect USB Device
 *    Parameters:      con:   Connect/Disconnect
 *    Return Value:    None
 */
void usbd_connect(int con)
{
  usbhal_connect(con);
}

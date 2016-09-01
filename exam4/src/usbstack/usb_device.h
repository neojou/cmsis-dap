#ifndef __USB_DEVICE_H__
#define __USB_DEVICE_H__

#if defined ( __GNUC__ )
#define __packed __attribute__((__packed__))
#endif

#ifndef BOOL
#define BOOL unsigned int
#endif

void usbd_init (void);

int usbd_configured(void);


#endif

include ../config.mk

# Build all libraries

all:
	$(MAKE) -C StdPeriph FLOAT_TYPE=$(FLOAT_TYPE)
	$(MAKE) -C USB_OTG FLOAT_TYPE=$(FLOAT_TYPE)
	$(MAKE) -C USB_Host/Core FLOAT_TYPE=$(FLOAT_TYPE)
	$(MAKE) -C USB_Host/Class/CDC FLOAT_TYPE=$(FLOAT_TYPE)
	$(MAKE) -C FreeRTOS FLOAT_TYPE=$(FLOAT_TYPE)

clean:
	$(MAKE) clean -C StdPeriph
	$(MAKE) clean -C USB_OTG
	$(MAKE) clean -C USB_Host/Core
	$(MAKE) clean -C USB_Host/Class/CDC
	$(MAKE) clean -C FreeRTOS FLOAT_TYPE=$(FLOAT_TYPE)

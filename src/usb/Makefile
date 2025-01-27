#tmp...
CFLAGS += -Wno-unused-parameter 

#debug flags; DU = Debug USB
#CFLAGS += -D _DU_OHCI_F #fire @ u/h/ohci.c
CFLAGS += -D _DU_OHCI_F_HALT #halted @ u/h/ohci.c
#CFLAGS += -D _DU_OHCI_Q #enqueue @ u/h/ohci.c
#CFLAGS += -D _DU_OHCI_RH #roothub @ u/h/ohci.c
CFLAGS += -D _DU_CORE #@ u/c/core.c
#CFLAGS += -D _DU_CORE_ADD #@ u/c/core.c
CFLAGS += -D _DU_USB #@ u/c/usb.c

OBJS += src/usb/host/ohci.o src/usb/core/core.o src/usb/core/usb.o \
		src/usb/lib/list.o \
		src/usb/drivers/class/hid.o

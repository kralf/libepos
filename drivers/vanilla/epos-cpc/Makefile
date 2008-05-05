CFLAGS = -I../cpc-usb
LDFLAGS = -L../cpc-usb

libepos-cpc.so: epos.c epos_cpc.c
	$(CC) $(CFLAGS) $(LDFLAGS) -Wall -shared -o libepos-cpc.so epos.c \
	epos_cpc.c -lcpc
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so

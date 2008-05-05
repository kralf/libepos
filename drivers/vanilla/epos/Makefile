CFLAGS = -I../cpc-usb

libepos-cpc.so: epos.c epos_cpc.c
	$(CC) $(CFLAGS) -Wall -shared -o libepos-cpc.so epos.c epos_cpc.c
clean:
	rm -f *.o
	rm -f *.a
	rm -f *.so

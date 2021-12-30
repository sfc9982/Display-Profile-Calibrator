DPC_VERSION = 0.10
CFLAGS = -O2

all: win_dpc
	

win_dpc: dpc.c
	$(CC) $(CFLAGS) -c dpc.c -DDPC_VERSION=\"$(DPC_VERSION)\" -DWIN32GDI
	windres.exe resource.rc resource.o
	$(CC) $(CFLAGS) -mwindows -lm resource.o -o dpc dpc.o

clean:
	rm -f dpc.o
	rm -f resource.o
	rm -f dpc
	rm -f dpc.exe


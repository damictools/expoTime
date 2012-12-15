CFITSIO = $(FITSIOROOT)
CPP = g++
CC = gcc
CFLAGS = -Wall -I$(CFITSIO)
LIBS = -L$(CFITSIO) -lcfitsio -lm
GLIBS = 
GLIBS += 
OBJECTS = printExpoTime.o 
HEADERS = 

ALL : printExpoTime.exe
	echo "Listo!"

printExpoTime.exe : $(OBJECTS)
	$(CPP) $(OBJECTS) -o printExpoTime.exe $(LIBS) $(GLIBS) $(CFLAGS)

printExpoTime.o : printExpoTime.cc $(HEADERS)
	$(CPP) -c printExpoTime.cc -o printExpoTime.o $(CFLAGS)

clean:
	rm -f *~ *.o *.exe

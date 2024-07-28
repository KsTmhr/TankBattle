#
# Makefile
#

# final result
TARGET = tankbattle.exe

# list of source file(*.c)
SRCS = main.c display.c others.c keyboard.c sprites.c enemy.c

# icon
ICON = ./imgs/tankbattle_icon.ico
RC = tankbattle_icon.rc
COFF = ${RC:.rc=.o}

# list of object file(*.o)
OBJS = ${SRCS:.c=.o}

# list of header file(*.h)
HEADERS = header.h parameters.h

WINDRES = i686-pc-cygwin-windres

# setting of compiler and linker
CC = i686-pc-cygwin-gcc
CCFLAGS = -fexec-charset=CP932 -Wall -I/usr/include/opengl
LD = $(CC)
LDFLAGS =
LIBS = -lm -lglpng -lglut32 -lglu32 -lopengl32

# how to generate TARGET from OBJS
$(TARGET) : $(OBJS) ${COFF}
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS) ${COFF}

# how to generate *.o from *.c
.c.o:
	$(CC) $(CCFLAGS) -c $<

${COFF} : ${RC}
	${WINDRES} -i ${RC} -o ${COFF}

# dependance
$(OBJS) : $(HEADERS) Makefile
$(COFF) : $(ICON) Makefile

# execute when "make clean"
clean :
	rm -f $(TARGET) $(OBJS) core *~

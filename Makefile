#
# Makefile
#

# final result
TARGET = a.exe

# list of source file(*.c)
SRCS = main.c display.c others.c keyboard.c sprites.c enemy.c

# list of object file(*.o)
OBJS = ${SRCS:.c=.o}

# list of header file(*.h)
HEADERS = header.h parameters.h

# setting of compiler and linker
CC = i686-pc-cygwin-gcc
CCFLAGS = -fexec-charset=CP932 -Wall -I/usr/include/opengl
LD = $(CC)
LDFLAGS =
LIBS = -lm -lglpng -lglut32 -lglu32 -lopengl32

# how to generate TARGET from OBJS
$(TARGET) : $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

# how to generate *.o from *.c
.c.o:
	$(CC) $(CCFLAGS) -c $<

# *.o depends on HEADERS and Makefile(restruct when these are rewrite)
$(OBJS) : $(HEADERS) Makefile

# execute when "make clean"
clean :
	rm -f $(TARGET) $(OBJS) core *~

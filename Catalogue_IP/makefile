# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -Werror

# Include directories
INCLUDES = -I/usr/include/SDL2

# Libraries
LIBS = -lSDL2 -lSDL2_ttf

# Source files
SRCS = interface.c

# Object files
OBJS = $(SRCS:.c=.o)

# Output executable
OUTPUT = mon_programme

# Main target
all: $(OUTPUT)

# Build executable
$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUT) $(OBJS) $(LIBS)

# Build object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

# Clean the project
clean:
	rm -f $(OBJS) $(OUTPUT)

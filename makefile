# Compiler
CC := gcc

# Compiler flags
CFLAGS := -Wall -Werror -g

# Directories
INCDIR := include
SRCDIR := src
OBJDIR := obj

# Sources
SOURCES := $(wildcard $(SRCDIR)/*.c)

# Object files
OBJECTS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCES))

# Executable
EXEC := final

# Default target
all: $(OBJDIR) $(EXEC)

# Linking
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -I$(INCDIR) -c -o $@ $<

# Create obj directory if it doesn't exist
$(OBJDIR):
	mkdir $(OBJDIR)

# Clean
clean:
	rmdir /s /q $(OBJDIR)
	del $(EXEC).exe

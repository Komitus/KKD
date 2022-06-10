.PHONY: all clean remove 

TARGET   = main
CC       = g++
# compiling flags here
CFLAGS   = -std=c++17 -Wall -O3 --pedantic 

LINKER   = g++
# linking flags here
LFLAGS   = -Wall -lm


SRCDIR   = src
OBJDIR   = obj
BINDIR   = bin
SOURCES  := $(wildcard $(SRCDIR)/*.cpp)
INCLUDES := $(wildcard $(SRCDIR)/*.hpp)
OBJECTS  := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
rm       = rm -f

all: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	@$(LINKER) $(LFLAGS) $(OBJECTS) -o $@
	@echo "Linking complete!"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo $<" compiled successfully!"

clean:
	@$(rm) $(wildcard $(OBJDIR)/*.o)
	@echo "Cleanup complete!"

remove: clean
	@$(rm) $(BINDIR)/$(TARGET)
	@echo "Executable removed!"

clean_test:
	@$(rm) $(BINDIR)/tests/decoded/*
	@$(rm) $(BINDIR)/tests/encoded/*
	@echo "Test files cleaned!"

CC := g++
SRCDIR := src
BUILDDIR := build
TARGET := libifabot.so
INSTALL_DIR := /usr/local

SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))
CFLAGS := -g -Wall -O3
LIBS := -lboost_system -lboost_thread -lboost_chrono
INCDIR := include

$(TARGET): $(OBJECTS)
	@echo "Creating shared library: $(TARGET)"
	@echo " $(CC) -shared -o $(TARGET) $(OBJECTS)" ; $(CC) -shared -o $(TARGET) $(OBJECTS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) -fPIC $(CFLAGS) -I $(INCDIR) -c -o $@ $<"; $(CC) -fPIC $(CFLAGS) -I $(INCDIR) -c -o $@ $<

install: all
	@echo " cp $(TARGET) $(INSTALL_DIR)/lib"; cp $(TARGET) $(INSTALL_DIR)/lib
	@echo " cp include $(INSTALL_DIR)/include"; cp include/* $(INSTALL_DIR)/include

all: $(TARGET)

clean:
	@echo "Cleaning..."; 
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(TARGET)
	@echo " $(RM) $(TARGET)"; $(RM) $(TARGET)

.PHONY: clean

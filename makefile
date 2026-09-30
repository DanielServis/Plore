CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
LDLIBS   := -lGLEW -lglfw -lGL

TARGET    := explore
PREFIX    ?= /usr/local
BINDIR    := $(PREFIX)/bin
SHADERDIR := $(PREFIX)/share/$(TARGET)

SRCS := fops.cpp input.cpp main.cpp models.cpp objects.cpp
OBJS := $(SRCS:.cpp=.o)

# Makes SHADER_DIR available in the C++ code as a string literal
CXXFLAGS += -DSHADER_DIR='"$(SHADERDIR)"'

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

-include $(OBJS:.o=.d)

install: $(TARGET)
	install -d $(BINDIR) $(SHADERDIR)
	install -m 755 $(TARGET) $(BINDIR)/$(TARGET)
	install -m 644 shader_vertex.glsl shader_fragment.glsl $(SHADERDIR)

uninstall:
	rm -f $(BINDIR)/$(TARGET)
	rm -rf $(SHADERDIR)

clean:
	rm -f $(OBJS) $(OBJS:.o=.d) $(TARGET)

.PHONY: all install uninstall clean

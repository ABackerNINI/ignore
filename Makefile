# BUILD DETAILS

CXX = g++
CXXFLAGS = -W -Wall -g
LFLAGS = -lm
BD = build

# BUILD EXECUTABLE

bin = ignore

all: $(bin)

# EXECUTABLE 1

obj = ignore.o
objbd = $(obj:%=$(BD)/%)

$(bin): $(objbd)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LFLAGS)

# CLEAN UP

clean:
	rm -rf "$(bin)" "$(BD)"

# DEPENDENCIES

SRC = $(wildcard *.h *.hpp *.c *.cpp *.cc)

$(BD)/depend.mk: $(SRC)
	@mkdir -p "$(BD)"
	@rm -f "$(BD)/depend.mk"
	@$(CXX) -MM *.cpp | sed 's/^\(.*\).o:/$$(BD)\/\1.o:/' >> $@

include $(BD)/depend.mk

# COMPILE TO OBJECTS

$(BD)/%.o: %.cpp
	@mkdir -p "$(BD)"
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# PHONY

.PHONY: all clean

.PHONY := info clean

MAJOR:=1
MINOR:=0
NAME:=logger
VERSION:=$(MAJOR).$(MINOR)

# Travis CI.
ifeq ($(CXX),)
	CXX=g++
endif

CXXFLAGS=-fPIC -O3 -Wall -ggdb --std=c++14
LDFLAGS=-lpthread
MAKEFLAGS+=--no-builtin-rules


SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)

all: info $(NAME) test

info:
	@echo "============== Compilation Info =============="
	@echo "Compiler:" $(CXX)
	@echo "CXX Flags:" $(CXXFLAGS)
	@echo "LD Flags:" $(LDFLAGS)
	@echo "Sources:" $(SRCS)
	@echo "Objects:" $(OBJS)
	@echo "Lib:" $(NAME)
	@echo "Version:" $(VERSION)
	@echo "=============================================="

$(NAME): lib$(NAME).so.$(VERSION)
	@echo "====== Creating Symbolic Link ======"
	ldconfig -n .
	ln -s lib$(NAME).so.$(MAJOR) lib$(NAME).so

lib$(NAME).so.$(VERSION): $(OBJS)
	@echo "====== Linking Objects ======"
	$(CXX) -shared -Wl,-soname,lib$(NAME).so.$(MAJOR) -o $@ $(OBJS)

%.o: %.d
	@echo "====== Creating Object:" $@ "======"
	$(CXX) $(CXXFLAGS) -c $(@:.o=.cpp) -o $@

%.d:
	@echo "====== Creating Dependacy:" $@ "======"
	@$(CXX) $(CXXFLAGS) $(@:.d=.cpp) -MM
	$(CXX) $(CXXFLAGS) $(@:.d=.cpp) -MM -MF $@

test: $(NAME)
	@echo "====== Compiling Test Application ======"
	$(CXX) $(CXXFLAGS) test/$(NAME)_test.cpp -o logger_$@ -I. -L. -l$(NAME) $(LDFLAGS)

clean:
	@echo "====== Cleaning Project ======"
	-rm -r src/*.o *.d *.ii *.s *.so*
	-rm logger logger_test

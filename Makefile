.PHONY: info clean install unistall test

MAJOR:=1
MINOR:=0
NAME:=logger
MODE:=release
VERSION:=$(MAJOR).$(MINOR)

# Travis CI.
ifeq ($(CXX),)
	CXX=g++
endif

# Install prefix.
ifeq ($(PREFIX),)
	PREFIX := /usr
endif

CXXFLAGS=-fPIC -O3 -Wall -Werror --std=c++14
LDFLAGS=-lpthread
MAKEFLAGS+=--no-builtin-rules

SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)

all: info lib$(NAME).so.$(VERSION) $(NAME)_test

debug: CXXFLAGS=-fPIC -O3 -Wall -Werror -ggdb --std=c++14
debug: MODE:=debug
debug: info lib$(NAME).so.$(VERSION) $(NAME)_test

info:
	@echo "============================== Compilation Info ==============================="
	@echo "Compiler:" $(CXX)
	@echo "CXX Flags:" $(CXXFLAGS)
	@echo "LD Flags:" $(LDFLAGS)
	@echo "Sources:" $(SRCS)
	@echo "Objects:" $(OBJS)
	@echo "Lib:" $(NAME)
	@echo "Version:" $(VERSION)
	@echo "Mode:" $(MODE)
	@echo "Prefix:" $(PREFIX)
	@echo "==============================================================================="

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

$(NAME)_test: lib$(NAME).so.$(VERSION)
	@echo "====== Compiling Test Application ======"
	$(CXX) $(CXXFLAGS) test/$(NAME)_test.cpp -o $@ -I. -L. -l$(NAME) $(LDFLAGS)

test:
	@echo "====== Running Test Application ======"
	./$(NAME)_test

install: lib$(NAME).so.$(VERSION)
	@echo "====== Installing Application ======"
	@echo "Installing at" $(PREFIX)/ "the files:"
	@echo $(PREFIX)/lib/lib$(NAME).so
	@echo $(PREFIX)/lib/lib$(NAME).so.$(MAJOR)
	@echo $(PREFIX)/lib/lib$(NAME).so.$(VERSION)
	install -d $(PREFIX)/include/
	install -m 644 src/logger.h $(PREFIX)/include/
	install -d $(PREFIX)/lib/
	install -m 755 lib$(NAME).so.$(MAJOR).$(MINOR) $(PREFIX)/lib/
	ldconfig -n $(PREFIX)/lib/
	-ln -s $(PREFIX)/lib/lib$(NAME).so.$(MAJOR) $(PREFIX)/lib/lib$(NAME).so
	ldconfig

unistall:
	@echo "====== Uninstalling Application ======"
	@echo "Uninstalling the files:"
	rm -f $(PREFIX)/lib/lib$(NAME).so
	rm -f $(PREFIX)/lib/lib$(NAME).so.$(MAJOR)
	rm -f $(PREFIX)/lib/lib$(NAME).so.$(VERSION)  
	rm -f $(PREFIX)/include/logger.h  

clean:
	@echo "====== Cleaning Project ======"
	-rm -r src/*.o *.d *.ii *.s *.so*
	-rm $(NAME) $(NAME)_test

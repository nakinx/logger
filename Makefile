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

# Install path.
ifeq ($(DESTDIR),)
	DESTDIR := /usr
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
	@echo "Destination Directory:" $(DESTDIR)
	@echo "==============================================================================="

lib$(NAME).so.$(VERSION): $(OBJS)
	@echo "====== Linking Objects ======"
	$(CXX) -shared -Wl,-soname,lib$(NAME).so.$(MAJOR) -o $@ $(OBJS)
	ldconfig -n .
	ln -s lib$(NAME).so.$(MAJOR) lib$(NAME).so

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
	@echo "Installing at" $(DESTDIR)/ "the files:"
	@echo $(DESTDIR)/lib/lib$(NAME).so
	@echo $(DESTDIR)/lib/lib$(NAME).so.$(MAJOR)
	@echo $(DESTDIR)/lib/lib$(NAME).so.$(VERSION)
	install -d $(DESTDIR)/include/
	install -m 644 src/logger.h $(DESTDIR)/include/
	install -d $(DESTDIR)/lib/
	install -m 755 lib$(NAME).so.$(MAJOR).$(MINOR) $(DESTDIR)/lib/
	ldconfig -n $(DESTDIR)/lib/
	-ln -s $(DESTDIR)/lib/lib$(NAME).so.$(MAJOR) $(DESTDIR)/lib/lib$(NAME).so
	ldconfig

unistall:
	@echo "====== Uninstalling Application ======"
	@echo "Uninstalling the files:"
	rm -f $(DESTDIR)/lib/lib$(NAME).so
	rm -f $(DESTDIR)/lib/lib$(NAME).so.$(MAJOR)
	rm -f $(DESTDIR)/lib/lib$(NAME).so.$(VERSION)  
	rm -f $(DESTDIR)/include/logger.h  

clean:
	@echo "====== Cleaning Project ======"
	-rm -r src/*.o *.d *.ii *.s *.so*
	-rm $(NAME) $(NAME)_test

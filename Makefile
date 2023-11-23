# This toplevel Makefile compiles the library in the lib subdirectory.
# If you want to see how to integrate the library in your own projects, check
# out the sub-directories examples-api-use/ and utils/

CXXFLAGS=-Wall -O3 -g
OBJECTS=CA.o
BINARIES=CA

RGB_LIB_DISTRIBUTION=matrix
RGB_INCDIR=matrix/include
RGB_LIBDIR=matrix/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

# Some language bindings.
PYTHON_LIB_DIR=bindings/python
CSHARP_LIB_DIR=bindings/c\#

all : $(RGB_LIBRARY)

$(RGB_LIBRARY): FORCE
	$(MAKE) -C $(RGB_LIBDIR)

CA : $(OBJECTS) $(RGB_LIBRARY)
    $(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(LDFLAGS)

CA.o : CA.cc

%.o : %.cc
	$(CXX) -I$(RGB_INCDIR) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) $(BINARIES)
	$(MAKE) -C $(RGB_LIBDIR) clean
	$(MAKE) -C lib clean
	$(MAKE) -C utils clean
	$(MAKE) -C examples-api-use clean
	$(MAKE) -C $(PYTHON_LIB_DIR) clean

build-csharp:
	$(MAKE) -C $(CSHARP_LIB_DIR) nuget
	$(MAKE) -C $(CSHARP_LIB_DIR) build

build-python: $(RGB_LIBRARY)
	$(MAKE) -C $(PYTHON_LIB_DIR) build

install-python: build-python
	$(MAKE) -C $(PYTHON_LIB_DIR) install

FORCE:
.PHONY: FORCE

export CXXFLAGS=-m32
export CFLAGS=-m32
export LDFLAGS=-m32
qibuild clean -f
qibuild configure
qibuild make

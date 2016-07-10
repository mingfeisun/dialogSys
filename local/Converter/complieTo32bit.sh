# export CXXFLAGS='-m32'
# export CFLAGS='-m32'
# export LDFLAGS='-m32 -l:/lib32/libdl.so.2'
# export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/lib32
# export LIBS='-ldl'
qibuild clean -f
qibuild configure
qibuild make
# echo $LIBRARY_PATH
# echo $LD_LIBRARY_PATH

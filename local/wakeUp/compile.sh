qibuild config --wizard
qibuild clean -f
qibuild configure
qibuild make
scp ./build-naoqisdk32/sdk/lib/naoqi/libwakeUp.so nao@192.168.1.102:~/mingfei
echo 'ok'

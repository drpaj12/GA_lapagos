file ../GA_lapagos 
set args -c ./config.xml -t ./test_out.test -m 1 
#set args -c ./config.xml -t ./test_out.test -m 2 -g hardware_out.txt
#set args -c ./config.xml -t ./test_out.test
#set args -c ../SCRIPTS/SANDBOX/mkp_testing_SINGLE_POINT_SUS_DAT_FORMAT_PENALTY.xml -t ./test_out.test
b main
r

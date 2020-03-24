#!/bin/bash

#valgrind --vgdb=yes --vgdb-error=0  ../GA_lapagos -c config.xml
#valgrind --track-origins=yes --keep-stacktraces=alloc-and-free --vgdb=yes --vgdb-error=0  ../GA_lapagos -c config.xml
valgrind --leak-check=yes --track-origins=yes --keep-stacktraces=alloc-and-free --vgdb=yes --vgdb-error=0  ../GA_lapagos -c config.xml -t ./test_out.test -m 0 


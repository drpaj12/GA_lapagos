GA_lapagos was created by Peter Jamieson in 2019 and is released under the MIT license for open source files.  
/*
Copyright (c) 2019 Peter Andrew Jamieson (jamieson.peter@gmail.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

This file provides some rudimentary ideas on how to use GA-lapagos and the associated tools

1 ----------------------------------------------------------
BEFORE you start dependencies
------------------------------------------------------------
 // Before compiling - Needed other things installed
- libxml2 in LIBS directory (NOTE - we are including libxml2.9 in github for now)
    - download libxml2 - for example - http://xmlsoft.org/downloads.html and go to DOWNLOADS -> xmlsoft.org/libxml2 and download LATEST_LIBXML2
    - put in LIBS directory 
    - untar with "tar -xzvf {LATEST_LIBXML2}"
    - config with your directory for example "./configure --prefix=/home/jamiespa/H_DRIVE/RESEARCH/SUPERVISION_AND_PROJECTS/PROGRAMS_PROJECTS_DEVELOPMENT/GA_LAPAGOS_/GA_lapagos/GA_lapagos/LIBS/libxml2-2.9.9/LOCAL --without-python"
    - "make" (that LOCAL dir NOTE=use your own local)
    - "make install"
    - update CMakeLists.txt with proper version in HINTS (find_library(LIBXML2 NAMES libxml2 xml2 HINTS LIBS/libxml2-2.9.9/LOCAL/lib PATH_SUFFIXES libxml2) and in libxml2 include dir ("include_directories(LIBS/libxml2-2.9.10/LOCAL/include/libxml2")

- FIXES PETER DID in UBUNTU (Note not sure if needed) to get automake updated - "sudo apt-get autoremove automake" "sudo apt-get install automake autoconf libtool" "autoreconf -i" 

- python3 for scripts

2 -----------------------------------------------------------
Basic Build of GA_lapagos
-------------------------------------------------------------
// Builds the make files.  Make has to be done in the root of GA_lapagos (the one with the CMakeLists.txt).
- "cmake ."
OR for Debugging
- "cmake -DCMAKE_BUILD_TYPE=Debug ."

// Then to compile GA_lapagos executable:
- "make"

3 -----------------------------------------------------------
Basic Testing
-------------------------------------------------------------
// Then to test
- "make test"
OR to see more outputs from python script
- "ctest -VV"

4 -----------------------------------------------------------
Advanced Debug and Tools
-------------------------------------------------------------
// Valgrind - compile with -g (DEBUG)
1. run script in SANDBOX to start up valgrind (see the script there)
2. run gdb in another process and attach

// PRofile
- cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-pg . 
1. make
2. run program
3. gprof -i GA_lapagos > profile_file.out

// utrace
- cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS=-pg . 
1. make
NOTE - uftrace needs to be on a linux drive (if you're using a Virtual Box) for mkfifo to work so from
~/TMP/
2. ../H_DRIVE/RESEARCH/SUPERVISION_AND_PROJECTS/PROGRAMS_PROJECTS_DEVELOPMENT/VIRTUAL_GOOP/uftrace-git/uftrace/bin/bin/uftrace record ../H_DRIVE/RESEARCH/SUPERVISION_AND_PROJECTS/PROGRAMS_PROJECTS_DEVELOPMENT/VIRTUAL_GOOP/virtual_goop/GA_lapagos -c config.xml -t test.out 
3. ../H_DRIVE/RESEARCH/SUPERVISION_AND_PROJECTS/PROGRAMS_PROJECTS_DEVELOPMENT/VIRTUAL_GOOP/uftrace-git/uftrace/bin/bin/uftrace graph

// call trace
- cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS=-finstrument-functions . 

5 -----------------------------------------------------------
GA_lapagos Notes
-------------------------------------------------------------

// NOTES
- There are 3 flows in GA_lapagos
    - Sequential GAs
    - Multithreaded GAs (TSP only)
        - "..._threaded.*" files are part of this and thread_utils.cpp and .h
    - HW generator and code implementation for HLS (Legup4.0) to target FPGAs
        - "..._hw.*" files are hard coded implementations for hw that removes floats and function pointers to make
        the creation of a flat .c file (see TSP_HLS_EXAMPLE/GA_lapagos_hw.c) and the associated README on how this file
        is created and used with Legup4.0


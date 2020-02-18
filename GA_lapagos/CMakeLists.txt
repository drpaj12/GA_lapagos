cmake_minimum_required(VERSION 2.8.9)
project("GA_lapagos")
 
# We require c++14 support
#
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) #No compiler specific extensions

# options
option(GA_LAPAGOS_ENABLE_PROFILING "Enable performance profiler (gprof)" OFF)
option(GA_LAPAGOS_ENABLE_COVERAGE "Enable code coverage tracking (gcov)" OFF)
option(GA_LAPAGOS_ENABLE_DEBUG_LOGGING "Enable debug logging" OFF)

#
# Warning flags
#
set(WARN_FLAGS "") #The actual warning flags to be applied

set(WARN_FLAGS_TO_CHECK  #The flags to check if the compiler supports
        #GCC-like
        "-Wall"                         #Most warnings, typically good
        "-Wextra"                       #Extra warning, usually good
        "-Wpedantic"                    #Ensure ISO compliance (i.e. no non-standard extensions)
        "-Wcast-qual"                   #Warn if cast removes qualifier (e.g. const char* -> char*)
        "-Wcast-align"                  #Warn if a cast causes memory alignment changes
        "-Wshadow"                      #Warn if local variable shadows another variable
        "-Wformat=2"                    #Sanity checks for printf-like formatting
        "-Wno-format-nonliteral"        # But don't worry about non-literal formtting (i.e. run-time printf format strings)
        "-Wlogical-op"                  #Checks for logical op when bit-wise expected
        "-Wmissing-declarations"        #Warn if a global function is defined with no declaration
        "-Wmissing-include-dirs"        #Warn if a user include directory is missing
        "-Wredundant-decls"             #Warn if there are overlapping declarations
        "-Wswitch-default"              #Warn if a switch has no default
        "-Wundef"                       #Warn if #if() preprocessor refers to an undefined directive
        "-Wunused"                      #Warn about unused variables/parameters
        "-Wunused-variable"             #Warn about variables that are not used
        "-Wunused-parameter"            #Warn about function parameters which are unused
        "-Wdisabled-optimization"       #Warn when optimizations are skipped (usually due to large/complex code)
        "-Wnoexcept"                    #Warn when functions should be noexcept (i.e. compiler know it doesn't throw)
        "-Woverloaded-virtual"          #Warn when a function declaration overrides a virtual method
        "-Wctor-dtor-privacy"           #Warn about inaccessible constructors/destructors
        "-Wnon-virtual-dtor"            #Warn about missing virtual destructors
        "-Wduplicated-cond"             #Warn about identical conditions in if-else chains
        "-Wduplicated-branches"         #Warn when different branches of an if-else chain are equivalent
        "-Wnull-dereference"            #Warn about null pointer dereference execution paths
        "-Wuninitialized"               #Warn about unitialized values
        "-Winit-self"                   #Warn about self-initialization
        "-Wcatch-value=3"               #Warn when catch statements don't catch by reference
        "-Wextra-semi"                  #Warn about redudnant semicolons
        #GCC-like optional
        #"-Wsuggest-final-types"         #Suggest where 'final' would help if specified on a type methods
        #"-Wsuggest-final-methods"       #Suggest where 'final' would help if specified on methods
        #"-Wsuggest-override"            #Suggest where 'override' should be specified
        #"-Wold-style-cast"              #Warn about using c-style casts
        #"-Wconversion"                  #Warn when type conversions may change value
        #"-Wsign-conversion"             #Warn if a conversion may change the sign
        #"-Wpadded"                      #Will warn if additional padding is introduced to a struct/class. Turn on if optimizing class memory layouts
        #"-Wstrict-overflow=2"           #Warn if the compiler optimizes assuming signed overflow does not occur
        #"-Wfloat-equal"                 #Warn about using direct floating point equality
        #"-Wunsafe-loop-optimizations"   #Warn when loops can't be optimized
        #"-Wswitch-enum"                 #Warn about uncovered enumeration values in a switch (even if there is a default)
        #"-Wsign-promo"                  #Warn when overload resolution converts an unsigned type to signed when an unsigned overload exists
        #"-Wdouble-promotion"            #Warn when float is implicitly propted to double
        #"-Wuseless-cast"                #Warn about casts to the same type
        #"-Wzero-as-null-pointer-constant" #Warn about using '0' instead of nullptr
        )

#
# Profiling flags
#

#Support for gprof
set(PROFILING_FLAGS "")
if(GA_LAPAGOS_ENABLE_PROFILING)
    #Enable gprof
    set(PROFILING_FLAGS "-g -pg")
    message(STATUS "Profiling Flags: ${PROFILING_FLAGS}")
endif()

#
# Code coverage flags
#

#Support for gcov
set(COVERAGE_FLAGS "")
if(GA_LAPAGOS_ENABLE_COVERAGE)
    #Enable gcov
    set(COVERAGE_FLAGS "-g -ftest-coverage -fprofile-arcs")
    message(STATUS "Coverage Flags: ${COVERAGE_FLAGS}")
endif()

#
# Debug logging
#
set(LOGGING_FLAGS "")
if(GA_LAPAGOS_ENABLE_DEBUG_LOGGING)
    #Enable gcov
    set(LOGGING_FLAGS "-DGA_LAPAGOS_ENABLE_DEBUG_LOGGING")
    message(STATUS "Logging Flags: ${LOGGING_FLAGS}")
endif()

#
# Set final flags
#
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WARN_FLAGS} ${SANITIZE_FLAGS} ${PROFILING_FLAGS} ${COVERAGE_FLAGS} ${LOGGING_FLAGS} ${COLORED_COMPILE}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

#Add the various sub-projects
add_subdirectory(LIBS)

#Bring the headers
include_directories(SRC)
include_directories(SRC/tsp)
include_directories(SRC/mkp)
include_directories(SRC/psns)
include_directories(LIBS/libargparse/src)
include_directories(LIBS/libxml2-2.9.9/LOCAL/include/libxml2)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB_RECURSE SOURCES 	SRC/*.cpp 
				SRC/tsp/*.cpp
				SRC/mkp/*.cpp
				SRC/psns/*.cpp
)
file(GLOB_RECURSE HEADERS 	SRC/INCLUDE/*.h
				SRC/tsp/*.h
				SRC/mkp/*.h
				SRC/psns/*.h
)
 
# Executable 
add_executable(GA_lapagos ${SOURCES} ${HEADERS} ${LIB_HEADERS})

# Libraries needed
target_link_libraries(GA_lapagos libargparse)
find_library(LIBXML2 NAMES libxml2 xml2 HINTS LIBS/libxml2-2.9.9/LOCAL/lib PATH_SUFFIXES libxml2)
target_link_libraries(GA_lapagos ${LIBXML2})
find_package (Threads)
target_link_libraries (GA_lapagos ${CMAKE_THREAD_LIBS_INIT})

# Add a top-level "tags" target which includes all files in both
# the build and source versions of src/*.
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_target(tags
    COMMAND ctags -R --c++-kinds=+p --fields=+iaS --extra=+q
        ${SOURCES} ${HEADERS} ${LIB_HEADERS}
)

# ...but only make it a dependency of the project if the ctags program
# is available, else it will fail to build on Windows.
find_program(CTAGS_PATH ctags)
if(CTAGS_PATH)
    message(STATUS "Found ctags: ${CTAGS_PATH}")
    add_dependencies(GA_lapagos tags)
else()
    message(STATUS "No ctags")
endif(CTAGS_PATH)

# make test component
enable_testing()
add_test(NAME test COMMAND python3 ../SCRIPTS/test_and_compare_to_golden.py master_list_of_tests.txt ./ ../GA_lapagos ./ ./GOLDEN/ WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/TESTS)

# clean up cmake files by a "make clean"
add_custom_target(clean-cmake-files
   COMMAND ${CMAKE_COMMAND} -P clean-all.cmake
)

# clean-all.cmake
set(cmake_generated ${CMAKE_BINARY_DIR}/CMakeCache.txt
                    ${CMAKE_BINARY_DIR}/cmake_install.cmake
                    ${CMAKE_BINARY_DIR}/Makefile
                    ${CMAKE_BINARY_DIR}/CMakeFiles
)

foreach(file ${cmake_generated})

  if (EXISTS ${file})
     file(REMOVE_RECURSE ${file})
  endif()

endforeach(file)

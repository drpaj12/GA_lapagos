Steps to create GOLDEN
1. you need: the .dat files for the config_file creationg example.dat 
    i.e. 
    python3 ../SCRIPTS/write_config_benchmark.py mkp_ga_data_for_configs_xml_data.dat 
2. (if you need to recreate the Golden outputs) you then run the executables to create the golden output and put in a GOLDEN directory.  Run from TESTS directory the following commands
    i.e. 
    - python3 ../SCRIPTS/execute_list_of_files.py mkp_testing_list_config_files.txt ./ ../GA_lapagos ./
    - python3 ../SCRIPTS/execute_list_of_files.py tsp_testing_list_config_files.txt ./ ../GA_lapagos ./
    - python3 ../SCRIPTS/execute_list_of_files.py psns_testing_list_config_files.txt ./ ../GA_lapagos ./
    - mv *.test_out GOLDEN

Step to test current versus GOLDEN
3. you then run the final script to compare executes against GOLDEN, which is run in the top directory with "make tests"
    i.e. python3 ../SCRIPTS/test_and_compare_to_golden.py mkp_testing_list_config_files.txt ./ ../../GA_lapagos ./ ./GOLDEN/
    NOTE: the master list in CMakeLists.txt (that eventually becomes "make test" is a master list of all the config files

The file "LastTest.log holds the info for the "make test" results and you should read it.

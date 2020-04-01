Steps to create EXPERIMENTS
1. you need: the .dat files for the config_file creation experiments1.dat 
    i.e. 
	python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_permutation.dat
	python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_lehmer_random_keys.dat
2. to execute
    i.e. 
    - python3 execute_benchmarks.py tsp_list_config_files.txt ./ ../../GA_lapagos ./
    - python3 execute_benchmarks.py tsp_l_rk_list_config_files.txt ./ ../../GA_lapagos ./



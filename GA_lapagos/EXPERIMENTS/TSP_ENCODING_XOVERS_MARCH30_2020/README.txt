Steps to create EXPERIMENTS
1. you need: the .dat files for the config_file creation experiments1.dat 
i.e. 
	- python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_permutation.dat
	- python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_lehmer.dat
	- python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_random_keys.dat

	- one line - python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_permutation.dat && python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_lehmer.dat && python3 ../../SCRIPTS/write_config_benchmark_for_experiments.py experiments_tsp_random_keys.dat

2. to execute
i.e. 
	- python3 execute_benchmarks.py tsp_list_config_files.txt ./ ../../GA_lapagos ./
	- python3 execute_benchmarks.py tsp_l_list_config_files.txt ./ ../../GA_lapagos ./
	- python3 execute_benchmarks.py tsp_rk_list_config_files.txt ./ ../../GA_lapagos ./

	-one line -  python3 execute_benchmarks.py tsp_list_config_files.txt ./ ../../GA_lapagos ./ && python3 execute_benchmarks.py tsp_l_list_config_files.txt ./ ../../GA_lapagos ./ && python3 execute_benchmarks.py tsp_rk_list_config_files.txt ./ ../../GA_lapagos ./

3. data for bar plots
        - Sample: python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*bench*0*test_out' 7 config.dat results.out
4. comparing rates of improvement
         - Sample: python3 create_ga_line_graph_of_quality_vs_time.py 7 'GENERATION*bench*0*test_out' 12


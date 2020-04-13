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

3. Make Hamming Graphs:
	- sample: python3 create_hamming_lines.py 10 'GENERATIONS*0*test_out' 12
	- one line - python3 create_hamming_lines.py 10 'GENERATIONS*0*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*1*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*2*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*3*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*4*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*5*test_out' 12
4. Make EBI Graphs
	- samble: python3 create_EBI_lines.py 10 'GENERATIONS*0*test_out' 12
	- one line - python3 create_EBI_lines.py 10 'GENERATIONS*0*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*1*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*2*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*3*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*4*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*5*test_out' 12
5. Make bin graphs
	- sample per benchmark for all xovers: python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*0*test_out" 12
	- one file - python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*0*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*1*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*2*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*3*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*4*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*5*test_out" 12
6. Do it all: 
python3 create_hamming_lines.py 10 'GENERATIONS*0*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*1*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*2*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*3*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*4*test_out' 12 && python3 create_hamming_lines.py 10 'GENERATIONS*5*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*0*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*1*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*2*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*3*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*4*test_out' 12 && python3 create_EBI_lines.py 10 'GENERATIONS*5*test_out' 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*0*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*1*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*2*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*3*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*4*test_out" 12 && python3 create_bin_graph_per_bmark.py 10 "*GENERATIONS*5*test_out" 12
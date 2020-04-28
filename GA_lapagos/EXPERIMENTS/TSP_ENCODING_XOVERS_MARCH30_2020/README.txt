Steps to create EXPERIMENTS
0. Compile GA_lapagos with the timing and best switches on (types.h)
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
	-one line -  python3 execute_benchmarks.py tsp_list_config_files.txt ./ ../../GA_lapagos_timing ./ && python3 execute_benchmarks.py tsp_l_list_config_files.txt ./ ../../GA_lapagos_timing ./ && python3 execute_benchmarks.py tsp_rk_list_config_files.txt ./ ../../GA_lapagos_timing ./

3. data for bar plots
        - Sample: python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*0*test_out' 7 config.dat results.out
	- one line - python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*0*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*1*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*2*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*3*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*4*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*5*test_out' 7 config.dat results.out
	- one line - python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*0*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*1*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*2*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*3*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*4*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*5*test_out' 7 config.dat results.out

4. comparing rates of improvement
         - Sample: python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*0*test_out' 12
         - one line - python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*0*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*1*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*2*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*3*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*4*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*5*test_out' 12

5. all as one
	- python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*0*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*1*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*2*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*3*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*4*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'FIVE*5*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*0*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*1*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*2*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*3*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*4*test_out' 7 config.dat results.out && python3 create_data_for_quality_runtime_bar_plot.py 'GENERATIONS*5*test_out' 7 config.dat results.out && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*0*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*1*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*2*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*3*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*4*test_out' 12 && python3 create_ga_line_graph_of_quality_vs_time.py 7 'FIVE*5*test_out' 12
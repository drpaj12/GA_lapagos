import sys
import os
import subprocess
from pathlib import Path
import glob
import pandas as pd
import re

import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt

#matplotlib.use("pgf")
#matplotlib.rcParams.update({
#    "pgf.texsystem": "pdflatex",
#    'font.family': 'serif',
#    'text.usetex': True,
#    'pgf.rcfonts': False,
#})

# 0_script 
# 1_extension_to_look_for 
# 2_number_of_rows_in_benchmark_outputs 
# 3_output_file_name
#
# SAMPLE: create_spread_sheet.py "files*files" 7 out.csv
print ("Number of arguments: %d" %  len(sys.argv))
print ("Argument List: %s" % str(sys.argv))

# get all the files to analyze
file_list = glob.glob('*'+sys.argv[1].rstrip()+'*')

print (file_list)

list_of_dfs = []
list_of_crossover_bm = []
column_header_list =  [str(x) for x in range(int(sys.argv[2]))]
print(column_header_list)

# read through the file and create a data frame for each benchmark
for log_file in file_list:
    frame = pd.read_csv(log_file, names=column_header_list)
    #print(frame)
    # make a list of each frame
    list_of_dfs.append(frame)
    name = re.search('tsp_(.+?)_ROULETTE', log_file).group(1)
    list_of_crossover_bm.append(name)
    #input("Press Enter to continue...")

list_of_time_wall = []
list_of_time_mutation = []
list_of_time_crossbreed = []
list_of_time_cost_function = []
list_of_time_keep = []
list_of_time_random_new = []
list_of_best_cost = []

# goes through the data frames read previously and search for specific values
for tdf in list_of_dfs:
    if tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].any():
        #print(tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0])
        time_wall = tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0]
        list_of_time_wall.append(time_wall)

    match_string = 'Wall_clock_time_mutation'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_mutation = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_mutation.append(time_mutation)
        #print(time_mutation)
        val = time_mutation / time_wall * 100
        #print("time_mutation:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_crossbreed'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_crossbreed = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_crossbreed.append(time_crossbreed)
        #print(time_crossbreed)
        val = time_crossbreed / time_wall * 100
        #print("time_crossbreed:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_cost_function'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_cost_function = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_cost_function.append(time_cost_function)
        #print(time_cost_function)
        val = time_cost_function / time_wall * 100
        #print("time_cost_function:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_keep'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_keep = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_keep.append(time_keep)
        #print(time_keep)
        val = time_keep / time_wall * 100
        #print("time_keep:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_random_new'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_random_new = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_random_new.append(time_random_new)
        #print(time_random_new)
        val = time_random_new / time_wall * 100
        #print("time_random_new:"+"{:.1f}".format(val) + "%")

    match_string = 'Best_cost'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        best_cost = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_best_cost.append(best_cost)

    #input("Press Enter to continue...")

columns_names = ["crossover", "time (s)"]
plot_df = pd.DataFrame({'crossover':list_of_crossover_bm,
                        'time (s)':list_of_time_wall})
print(plot_df)

plot_df.plot(kind='bar', x='crossover', legend=False, width=.9)
plt.ylabel('time (s)')
plt.tight_layout()
plt.savefig('run_time_bm.png', dpi=400)

columns_names = ["crossover", "quality"]
plot_df = pd.DataFrame({'crossover':list_of_crossover_bm,
                        'quality':list_of_best_cost})
print(plot_df)

plot_df.plot(kind='bar', x='crossover', legend=False, width=.9)
plt.ylabel('quality of result')
plt.tight_layout()
plt.savefig('quality_bm.png', dpi=400)

import sys
import os
import subprocess
from pathlib import Path
import glob
import pandas as pd

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
column_header_list =  [str(x) for x in range(int(sys.argv[2]))]
print(column_header_list)

# read through the file and create a data frame for each benchmark
for log_file in file_list:
    frame = pd.read_csv(log_file, names=column_header_list)
    #print(frame)
    # make a list of each frame
    list_of_dfs.append(frame)
    #input("Press Enter to continue...")

# goes through the data frames read previously and search for specific values
for tdf in list_of_dfs:
    if tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].any():
        print(tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0])
        wall_time = tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0]

    match_string = 'Wall_clock_time_mutation'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_mutation = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        print(time_mutation)
        val = time_mutation / wall_time * 100
        print("time_mutation:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_crossbreed'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_crossbreed = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        print(time_crossbreed)
        val = time_crossbreed / wall_time * 100
        print("time_crossbreed:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_cost_function'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_cost_function = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        print(time_cost_function)
        val = time_cost_function / wall_time * 100
        print("time_cost_function:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_keep'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_time_keep = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        print(time_time_keep)
        val = time_time_keep / wall_time * 100
        print("time_time_keep:"+"{:.1f}".format(val) + "%")

    match_string = 'Wall_clock_time_random_new'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_time_random_new = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        print(time_time_random_new)
        val = time_time_random_new / wall_time * 100
        print("time_time_random_new:"+"{:.1f}".format(val) + "%")



    input("Press Enter to continue...")


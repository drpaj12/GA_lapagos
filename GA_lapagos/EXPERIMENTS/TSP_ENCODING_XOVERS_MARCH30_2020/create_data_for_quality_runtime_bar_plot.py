import sys
import os
import subprocess
from pathlib import Path
import glob
import pandas as pd
import numpy as np
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
list_of_dfs_permutation = []
list_of_dfs_lehmer = []
list_of_dfs_random_keys = []
list_of_crossover_bm = []
list_of_crossover_bm_permutation = []
list_of_crossover_bm_lehmer = []
list_of_crossover_bm_random_keys = []
column_header_list =  [str(x) for x in range(int(sys.argv[2]))]
print(column_header_list)

# read through the file and create a data frame for each benchmark
for log_file in file_list:
    frame = pd.read_csv(log_file, names=column_header_list)
    #print(frame)
    # make a list of each frame
    exit_name = re.search('_ES(.+?)EE', log_file).group(1)
    bench_name = re.search('_BS(.+?)BE', log_file).group(1)
    list_of_dfs.append(frame)
    name = re.search('XS(.+?)XE', log_file).group(1)
    list_of_crossover_bm.append(name)
    problem_type_name = re.search('PSADJACENCY_(.+?)PE', log_file).group(1)

    if problem_type_name == "PERMUTATION":
        list_of_crossover_bm_permutation.append(name)
        list_of_dfs_permutation.append(frame)
    elif problem_type_name == "LEHMER":
        list_of_crossover_bm_lehmer.append(name)
        list_of_dfs_lehmer.append(frame)
    elif problem_type_name == "RANDOM_KEYS":
        list_of_crossover_bm_random_keys.append(name)
        list_of_dfs_random_keys.append(frame)
    #input("Press Enter to continue...")

list_of_time_wall = []
list_of_time_wall_permutation = []
list_of_time_wall_lehmer = []
list_of_time_wall_random_keys = []
list_of_time_mutation = []
list_of_time_mutation_permutation = []
list_of_time_mutation_lehmer = []
list_of_time_mutation_random_keys = []
list_of_time_crossbreed = []
list_of_time_crossbreed_permutation = []
list_of_time_crossbreed_lehmer = []
list_of_time_crossbreed_random_keys = []
list_of_time_cost_function = []
list_of_time_cost_function_permutation = []
list_of_time_cost_function_lehmer = []
list_of_time_cost_function_random_keys = []
list_of_time_keep = []
list_of_time_random_new = []
list_of_best_cost = []
list_of_best_cost_permutation = []
list_of_best_cost_lehmer = []
list_of_best_cost_random_keys = []
list_of_time_other_permutation = []
list_of_time_other_lehmer = []
list_of_time_other_random_keys = []

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

for tdf in list_of_dfs_permutation:
    if tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].any():
        #print(tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0])
        time_wall = tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0]
        list_of_time_wall_permutation.append(time_wall)

    match_string = 'Wall_clock_time_mutation'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_mutation = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_mutation_permutation.append(time_mutation)

    match_string = 'Wall_clock_time_crossbreed'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_crossbreed = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_crossbreed_permutation.append(time_crossbreed)

    match_string = 'Wall_clock_time_cost_function'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_cost_function = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_cost_function_permutation.append(time_cost_function)

    list_of_time_other_permutation.append(time_wall - (time_mutation + time_crossbreed +
        time_cost_function))

    match_string = 'Best_cost'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        best_cost = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_best_cost_permutation.append(best_cost)

for tdf in list_of_dfs_lehmer:
    if tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].any():
        #print(tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0])
        time_wall = tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0]
        list_of_time_wall_lehmer.append(time_wall)

    match_string = 'Wall_clock_time_mutation'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_mutation = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_mutation_lehmer.append(time_mutation)

    match_string = 'Wall_clock_time_crossbreed'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_crossbreed = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_crossbreed_lehmer.append(time_crossbreed)

    match_string = 'Wall_clock_time_cost_function'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_cost_function = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_cost_function_lehmer.append(time_cost_function)

    list_of_time_other_lehmer.append(time_wall - (time_mutation + time_crossbreed +
        time_cost_function))

    match_string = 'Best_cost'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        best_cost = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_best_cost_lehmer.append(best_cost)

for tdf in list_of_dfs_random_keys:
    if tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].any():
        #print(tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0])
        time_wall = tdf.loc[tdf['1'].str.contains('Wall_clock_time')]['2'].values[0]
        list_of_time_wall_random_keys.append(time_wall)

    match_string = 'Wall_clock_time_mutation'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_mutation = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_mutation_random_keys.append(time_mutation)

    match_string = 'Wall_clock_time_crossbreed'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_crossbreed = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_crossbreed_random_keys.append(time_crossbreed)

    match_string = 'Wall_clock_time_cost_function'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        time_cost_function = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_time_cost_function_random_keys.append(time_cost_function)

    list_of_time_other_random_keys.append(time_wall - (time_mutation + time_crossbreed +
        time_cost_function))

    match_string = 'Best_cost'
    if tdf.loc[tdf['1'].str.contains(match_string)]['2'].any():
        best_cost = tdf.loc[tdf['1'].str.contains(match_string)]['2'].values[0]
        list_of_best_cost_random_keys.append(best_cost)

# PLOT bargraph for all permuations
#print(list_of_crossover_bm)
#print(list_of_time_wall)
#plot_df = pd.DataFrame({'crossover':list_of_crossover_bm,
#                        'time (s)':list_of_time_wall})
#print(plot_df)
#plot_df.plot(kind='bar', x='crossover', legend=False, width=.9)
#plt.ylabel('time (s)')
#plt.tight_layout()
#plt.savefig('run_time_bm.png', dpi=400)
#
#plot_df = pd.DataFrame({'crossover':list_of_crossover_bm,
#                        'quality':list_of_best_cost})
#print(plot_df)
#plot_df.plot(kind='bar', x='crossover', legend=False, width=.9)
#plt.ylabel('quality of result')
#plt.tight_layout()
#plt.savefig('quality_bm.png', dpi=400)

# PLOTS BAR plot with three groupings for time
#width = 1
#groupgap=1
#y1=list_of_time_wall_permutation
#y2=list_of_time_wall_lehmer
#y3=list_of_time_wall_random_keys
#x1 = np.arange(len(y1))
#x2 = np.arange(len(y2))+groupgap+len(y1)
#x3 = np.arange(len(y3))+groupgap+len(y2)+groupgap+len(y1)
#ind = np.concatenate((x1,x2,x3))
#fig, ax = plt.subplots()
#rects1 = ax.bar(x1, y1, width, color='r',  edgecolor= "black",label="Permuatation")
#rects2 = ax.bar(x2, y2, width, color='b',  edgecolor= "black",label="Lehmer")
#rects3 = ax.bar(x3, y3, width, color='g',  edgecolor= "black",label="Random Keys")
#ax.set_ylabel('time(s)',fontsize=14)
#ax.set_xlabel('crossover',fontsize=14)
#ax.set_xticks(ind)
#ax.set_xticklabels((list_of_crossover_bm_permutation + list_of_crossover_bm_lehmer + list_of_crossover_bm_random_keys), fontsize=7)
#plt.legend(loc='center left')
#plt.tight_layout()
#plt.savefig('group_times_exit'+exit_name+'_bm'+bench_name+'.png', dpi=400)

# PLOTS BAR plot with three groupings for time
width = 1
groupgap=1
y1a=list_of_time_mutation_permutation
y1b=list_of_time_crossbreed_permutation
y1c=list_of_time_cost_function_permutation
y1d=list_of_time_other_permutation
y2a=list_of_time_mutation_lehmer
y2b=list_of_time_crossbreed_lehmer
y2c=list_of_time_cost_function_lehmer
y2d=list_of_time_other_lehmer
y3a=list_of_time_mutation_random_keys
y3b=list_of_time_crossbreed_random_keys
y3c=list_of_time_cost_function_random_keys
y3d=list_of_time_other_random_keys
x1 = np.arange(len(y1a))
x2 = np.arange(len(y2a))+groupgap+len(y1a)
x3 = np.arange(len(y3a))+groupgap+len(y2a)+groupgap+len(y1a)
ind = np.concatenate((x1,x2,x3))
fig, ax = plt.subplots()
rects1a = ax.bar(x1, y1b, width, color='w',  edgecolor= "r",label="Crossbreed")
rects1b = ax.bar(x1, y1a, width, bottom=y1b, color='purple',  edgecolor= "r",label="Mutation")
rects1c = ax.bar(x1, y1c, width, bottom=y1a, color='black',  edgecolor= "r",label="Cost Function")
rects1d = ax.bar(x1, y1d, width, bottom=y1c, color='orange',  edgecolor= "r",label="Other")
rects2a = ax.bar(x2, y2b, width, color='w',  edgecolor= "g",label="Lehmer Crossbreed")
rects2b = ax.bar(x2, y2a, width, bottom=y2b, color='purple',  edgecolor= "g",label="Lehmer Mutation")
rects2c = ax.bar(x2, y2c, width, bottom=y2a, color='black',  edgecolor= "g",label="Lehmer Cost Function")
rects2d = ax.bar(x2, y2d, width, bottom=y2c, color='orange',  edgecolor= "g",label="Lehmer Other")
rects3a = ax.bar(x3, y3b, width, color='w',  edgecolor= "b",label="RanKeys Crossbreed")
rects3b = ax.bar(x3, y3a, width, bottom=y3b, color='purple',  edgecolor= "b",label="RanKeys Mutation")
rects3c = ax.bar(x3, y3c, width, bottom=y3a, color='black',  edgecolor= "b",label="RanKeys Cost Function")
rects3d = ax.bar(x3, y3d, width, bottom=y3c, color='orange',  edgecolor= "b",label="RanKeys Other")
ax.set_ylabel('time(s)',fontsize=14)
ax.set_xlabel('crossover',fontsize=14)
ax.set_xticks(ind)
ax.set_xticklabels((list_of_crossover_bm_permutation + list_of_crossover_bm_lehmer + list_of_crossover_bm_random_keys), fontsize=7)
plt.legend(loc='center left')
plt.tight_layout()
plt.savefig('group_times_exit'+exit_name+'_bm'+bench_name+'.png', dpi=400)


# PLOTS BAR plot with three groupings for time
width = 1
groupgap=1
y1=list_of_time_wall_permutation
y2=list_of_time_wall_random_keys
x1 = np.arange(len(y1))
x2 = np.arange(len(y2))+groupgap+len(y1)
ind = np.concatenate((x1,x2))
fig, ax = plt.subplots()
rects1 = ax.bar(x1, y1, width, color='r',  edgecolor= "black",label="Permuatation")
rects3 = ax.bar(x2, y2, width, color='g',  edgecolor= "black",label="Random Keys")
ax.set_ylabel('time(s)',fontsize=14)
ax.set_xlabel('crossover',fontsize=14)
ax.set_xticks(ind)
ax.set_xticklabels((list_of_crossover_bm_permutation + list_of_crossover_bm_random_keys), fontsize=7)
plt.legend(loc='center left')
plt.tight_layout()
plt.savefig('group_times_only_perm_and_rand_keys_exit'+exit_name+'_bm'+bench_name+'.png', dpi=400)

# PLOTS BAR plot with three groupings for time
width = 1
groupgap=1
y1=list_of_best_cost_permutation
y2=list_of_best_cost_lehmer
y3=list_of_best_cost_random_keys
x1 = np.arange(len(y1))
x2 = np.arange(len(y2))+groupgap+len(y1)
x3 = np.arange(len(y3))+groupgap+len(y2)+groupgap+len(y1)
ind = np.concatenate((x1,x2,x3))
fig, ax = plt.subplots()
rects1 = ax.bar(x1, y1, width, color='r',  edgecolor= "black",label="Permuatation")
rects2 = ax.bar(x2, y2, width, color='b',  edgecolor= "black",label="Lehmer")
rects3 = ax.bar(x3, y3, width, color='g',  edgecolor= "black",label="Random Keys")
ax.set_ylabel('quality',fontsize=14)
ax.set_xlabel('crossover',fontsize=14)
ax.set_xticks(ind)
ax.set_xticklabels((list_of_crossover_bm_permutation + list_of_crossover_bm_lehmer + list_of_crossover_bm_random_keys), fontsize=7)
plt.legend(loc='center left')
plt.tight_layout()
plt.savefig('group_quality_exit'+exit_name+'_bm'+bench_name+'.png', dpi=400)

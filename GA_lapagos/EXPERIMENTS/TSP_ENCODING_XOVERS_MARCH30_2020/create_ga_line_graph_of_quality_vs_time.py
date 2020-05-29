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
# 1_witdh of the columns {7}
# 2_regex_of_files
# 3_number_of_colors
# SAMPLE: create_spread_sheet.py "files*files" 7 out.csv
print ("Number of arguments: %d" %  len(sys.argv))
print ("Argument List: %s" % str(sys.argv))

NUM_COLORS = int(sys.argv[3])
line_style=['-','--','-.',':']
# start the plot
f, ax = plt.subplots()
# set up a color cylcle based on NUM_COLORS
cm = plt.get_cmap('gist_rainbow')
ax.set_prop_cycle(color=[cm(1.*i/NUM_COLORS) for i in range(NUM_COLORS)])

# argument 1 is max number of columns which we use
column_header_list =  [str(x) for x in range(int(sys.argv[1]))]

list_legend = []

# get all the files to analyze
file_list = glob.glob('*'+sys.argv[2].rstrip()+'*')

print (file_list)

current_x = 0;
count = 0
# read through the file and create a data frame for each benchmark
for log_file in file_list:
    count = count + 1
    frame = pd.read_csv(log_file, names=column_header_list)
    # extract details
    cross_name = re.search('XS(.+?)XE', log_file).group(1)
    problem_type_name = re.search('PSADJACENCY_(.+?)PE', log_file).group(1)
    exit_name = re.search('_ES(.+?)EE', log_file).group(1)
    bench_name = re.search('_BS(.+?)BE', log_file).group(1)
    # add to the list that will be the legend
    list_legend.append(cross_name + ' ' + problem_type_name)

    # do a step plot and set to x = 0 as start
    ax.step(frame.loc[frame['1'].str.contains('generation')]['2'],
            frame.loc[frame['1'].str.contains('generation')]['6'],
            linestyle=line_style[count % 4])
    ax.set_xlim(0)
    p = frame.loc[frame['1'].str.contains('generation')]['2'];

    # adjust for the biggest x value
    # -2 since there is a Generation term in their that is the exit
    if (p.iloc[-2] > current_x):
        current_x = p.iloc[-2]
        ax.set_xbound(0,current_x)

if bench_name == '0':
    plt.legend(list_legend, loc='lower left', framealpha=0.5)

plt.xlabel('Generations')
plt.ylabel('Quality')
plt.tight_layout()
plt.savefig('bmark_quality_vs_generation_'+exit_name+'_'+bench_name+'.png', dpi=400)

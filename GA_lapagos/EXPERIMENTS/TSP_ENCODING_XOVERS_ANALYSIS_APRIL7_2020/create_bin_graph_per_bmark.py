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

# get all the files to analyze
file_list = glob.glob('*'+sys.argv[2].rstrip()+'*')

print (file_list)

# argument 1 is max number of columns which we use
column_header_list =  [str(x) for x in range(int(sys.argv[1]))]
list_legend = ['bin 1', 'bin 2', 'bin 3', 'bin 4', 'bin 5']

count = 0
# read through the file and create a data frame for each benchmark
for log_file in file_list:
    count = count + 1
    frame = pd.read_csv(log_file, names=column_header_list)

    # start the plot
    f, ax = plt.subplots()

    # extract details
    cross_name = re.search('XS(.+?)XE', log_file).group(1)
    problem_type_name = re.search('PSADJACENCY_(.+?)PE', log_file).group(1)
    exit_name = re.search('_ES(.+?)EE', log_file).group(1)
    bench_name = re.search('_BS(.+?)BE', log_file).group(1)
    # add to the list that will be the legend

    y = np.vstack([ frame.loc[frame['1'].str.contains('Marks Generation')]['7'],
                    frame.loc[frame['1'].str.contains('Marks Generation')]['6'],
                    frame.loc[frame['1'].str.contains('Marks Generation')]['5'],
                    frame.loc[frame['1'].str.contains('Marks Generation')]['4'],
                    frame.loc[frame['1'].str.contains('Marks Generation')]['3']])
    # do a step plot and set to x = 0 as start
    ax.stackplot(frame.loc[frame['1'].str.contains('Marks Generation')]['2'],
            y,
            labels=list_legend)
    ax.set_xlim(0,1000)
    ax.set_ylim(0,500)

    plt.legend(list_legend)

    plt.xlabel('Generations')
    plt.ylabel('Population')
    plt.tight_layout()
    plt.savefig('bmark_marks_'+cross_name+'_'+problem_type_name+'_'+exit_name+'_'+bench_name+'.png', dpi=400)



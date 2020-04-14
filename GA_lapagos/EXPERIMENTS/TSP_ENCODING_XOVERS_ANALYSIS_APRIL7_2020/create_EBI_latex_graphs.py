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

# argument 1 is max number of columns which we use
column_header_list =  [str(x) for x in range(int(sys.argv[1]))]

list_legend = []
list_means = []
list_std = []

# get all the files to analyze
file_list = glob.glob('*'+sys.argv[2].rstrip()+'*')

print (file_list)

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
    # add to the lists
    list_legend.append(cross_name)
    list_means.append(frame.loc[frame['1'].str.contains('EBI_avg generation')]['3'].mean())
    list_std.append(frame.loc[frame['1'].str.contains('EBI_avg generation')]['3'].std())

df = pd.DataFrame({'Mean':list_means,'Standard Dev':list_std}, index = list_legend)
# two decimal places
df = df.round(2)
print(df.to_latex())



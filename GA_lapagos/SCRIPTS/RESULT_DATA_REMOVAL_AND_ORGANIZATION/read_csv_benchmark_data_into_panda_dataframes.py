import sys
import os
import subprocess
from pathlib import Path
import glob
import pandas as pd

# 0_script 
# 1_extension_to_look_for 
# 2_number_of_rows_in_benchmark_outputs 
# 3_output_file_name [NOT USED]
#
# SAMPLE: read_csv_benchmark_data_into_panda_dataframes.py "files*files" 7 out.csv
print ("Number of arguments: %d" %  len(sys.argv))
print ("Argument List: %s" % str(sys.argv))

# get all the files to analyze via the regex in arg1
file_list = glob.glob('*'+sys.argv[1].rstrip()+'*')
print (file_list)

# create the names as top of file for width of data
list_of = []
column_header_list =  [str(x) for x in range(int(sys.argv[2]))]
print(column_header_list)

# read through the file and create a data frame for each benchmark
for log_file in file_list:
    frame = pd.read_csv(log_file, names=column_header_list)
    print(frame)
    # add dataframe to a list 
    list_of.append(frame)
    input("Press Enter to continue...")


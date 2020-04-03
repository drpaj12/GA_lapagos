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
    # creates a boolean in column '1' that contains 'Exit'
    print(tdf['1'].str.contains('Exit'))
    # gets the rows that containt 'Exit'
    print(tdf[tdf['1'].str.contains('Exiting_gen')])
    # gets the row 'Exit' column '2'
    print(tdf.loc[tdf['1'].str.contains('Exiting_gen')]['2'])
    # checks if there is a row 'Exit' column '2' and then gets the actual value
    if tdf.loc[tdf['1'].str.contains('Exiting_gen')]['2'].any():
        # if multiple values found, then 0 is first, 1 is second and so on
        print(tdf.loc[tdf['1'].str.contains('Exiting_gen')]['2'].values[0])
    input("Press Enter to continue...")


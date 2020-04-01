import sys
print(sys.version_info)

import os
import subprocess
from pathlib import Path
import glob
#sys.path.insert(1, '/opt/anaconda3/lib/python3.7/site-packages')
import pandas as pd

# 0_script 1_extension_to_look_for 2_config_file_of_data_to_organize 3_output_file_name
# SAMPLE: create_spread_sheet.py test_out file.txt 
print ("Number of arguments: %d" %  len(sys.argv))
print ("Argument List: %s" % str(sys.argv))

# get all the files to analyze
file_list = glob.glob('*'+sys.argv[1].rstrip()+'*')

# read through the file and run them 
for log_file in file_list:
    df = pd.read_csv(log_file)
    print (df)
    input("Press Enter to continue...")

    print ("Reading: "+log_file)
    log_file_details = open(log_file,"r")

    # read file into array
    file_details = log_file_details.readlines()

    close(log_file_details)





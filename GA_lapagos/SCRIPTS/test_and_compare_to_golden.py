import sys
import os
from pathlib import Path
import filecmp
import subprocess

# 0_script 1_list_of_files_of_configs 2_location_of_config_files 3_executable_location 4_output_logs_directory 5_output_logs_golden_directory
# SAMPLE: execute_list_of_files.py mkp_testing_list_config_files.txt ./ ./ 
print ("Number of arguments: %d" %  len(sys.argv))
print ("Argument List: %s" % str(sys.argv))

failed_a_test = False

log_file = open("execute.log", "w")

# read through the file and run them 
with open(sys.argv[1],"r") as f:
    for line in f:
        filename, file_extension = os.path.splitext(line)
        execution = sys.argv[3] + " -c " + sys.argv[2] + line.rstrip() + " -t " + sys.argv[4] + filename + ".test_out" 

        print(execution)
        p = subprocess.call([sys.argv[3], "-c",  sys.argv[2]+line.rstrip(),  "-t", sys.argv[4]+filename+".test_out"], stdout=log_file, stderr=log_file)

        file_compare1 = sys.argv[4] + filename + ".test_out"
        file_compare2 = sys.argv[5] + filename + ".test_out"
        if filecmp.cmp(file_compare1, file_compare2):
            print("PASSED: " + filename)
        else:
            print("FAILED: " + filename)
            failed_a_test = True

        p = subprocess.call(["rm", "-f", sys.argv[4]+filename+".test_out"])

f.close()
log_file.close()

if failed_a_test:
    print ("TESTs FAILED\n")
else:
    print ("TESTs PASSED\n")
    subprocess.call(["rm", "-f", "execute.log"])
    subprocess.call(["rm", "-f", "default_out.txt"])
    subprocess.call(["rm", "-f", "default_test_out.txt"])



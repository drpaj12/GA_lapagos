import sys

print ("Number of arguments: %d" %  len(sys.argv))
print ("Argument List: %s" % str(sys.argv))

f1 = open(sys.argv[1],"r")

# read file into array
file_details = f1.readlines()
# top line is for comments
running_parser = 1

directory = file_details[running_parser]
running_parser += 1

bname = file_details[running_parser]
running_parser += 1

ga_type = file_details[running_parser]
running_parser += 1

num_alphabetic = int(file_details[running_parser])
running_parser += 1

num_crossovers = int(file_details[running_parser])
index_crossovers = running_parser+1
running_parser += num_crossovers+1

num_selectors = int(file_details[running_parser])
index_selectors = running_parser+1
running_parser += num_selectors+1

num_file_formats = int(file_details[running_parser])
index_file_formats = running_parser+1
running_parser += num_file_formats+1

num_benchmarks = int(file_details[running_parser])
index_benchmarks = running_parser+1
running_parser += num_benchmarks+1

num_exits = int(file_details[running_parser])
index_exits = running_parser+1
running_parser += num_exits+1

num_population = file_details[running_parser]
running_parser += 1

percent_to_keep = file_details[running_parser]
running_parser += 1

percent_from_mutate = file_details[running_parser]
running_parser += 1

percent_from_breed = file_details[running_parser]
running_parser += 1

percent_from_breed_and_mutate = file_details[running_parser]
running_parser += 1

percent_to_random = file_details[running_parser]
running_parser += 1

top_percent_to_breed_from = file_details[running_parser]
running_parser += 1

top_percent_to_mutate_from = file_details[running_parser]
running_parser += 1

top_percent_to_breed_mutate_from = file_details[running_parser]
running_parser += 1

percent_of_genome_mutations = file_details[running_parser]
running_parser += 1

tournament_size = file_details[running_parser]
running_parser += 1

f1.close()

# open file to write
file_name = directory.rstrip() + "/" + bname.rstrip() + "_list_config_files.txt"
f3 = open(file_name, "w+")

for i in range(num_crossovers):
    for j in range(num_selectors):
        for k in range(num_file_formats): 
            for l in range(num_benchmarks):
                for m in range(num_exits):
                    if num_alphabetic < 10:
                        file_name = "00" + str(num_alphabetic) + "_" + bname.rstrip() + "_XS_" + file_details[index_crossovers+i].rstrip() + "_XE_SS_" + file_details[index_selectors+j].rstrip() + "_SE_PS_" + file_details[index_file_formats+k].rstrip() + "_PE_ES_" + file_details[index_exits+m].rstrip() + "_EE_bench_BS_" + str(l) + "_BE.xml"
                    else:
                        file_name = "0" + str(num_alphabetic) + "_" + bname.rstrip() + "_XS_" + file_details[index_crossovers+i].rstrip() + "_XE_SS_" + file_details[index_selectors+j].rstrip() + "_SE_PS_" + file_details[index_file_formats+k].rstrip() + "_PE_ES_" + file_details[index_exits+m].rstrip() + "_EE_bench_BS_" + str(l) + "_BE.xml"

                    file_name_w_directory = directory.rstrip() + "/" + file_name
        
                    f3.write("%s\n" % file_name)
        
                    f2 = open(file_name_w_directory, "w+")
        
                    f2.write("<ga_config>\n")
                    f2.write("\t<ga_type>%s</ga_type>\n" % ga_type.rstrip())
                    f2.write("\t<crossover_type>%s</crossover_type>\n" % file_details[index_crossovers+i].rstrip())
                    f2.write("\t<selector_type>%s</selector_type>\n" % file_details[index_selectors+j].rstrip())
                    f2.write("\t<rand_seed>1</rand_seed>\n")
                    f2.write("\t<problem_type>%s</problem_type>\n" % file_details[index_file_formats+k].rstrip())
                    f2.write("\t<benchmark_file_name>%s</benchmark_file_name>\n" % file_details[index_benchmarks+l].rstrip())
                    f2.write("\t<threads>1</threads>\n")
                    f2.write("\t<exit_type>%s</exit_type>\n" % file_details[index_exits+m].rstrip())
                    f2.write("\t<ga_params>\n")
                    f2.write("\t\t<num_population>%s</num_population>\n" % num_population.rstrip())
                    f2.write("\t\t<!-- following parameters must add to 1.0 -->\n")
                    f2.write("\t\t<percent_to_keep>%s</percent_to_keep>\n" % percent_to_keep.rstrip())
                    f2.write("\t\t<percent_from_mutate>%s</percent_from_mutate>\n" % percent_from_mutate.rstrip())
                    f2.write("\t\t<percent_from_breed>%s</percent_from_breed>\n" % percent_from_breed.rstrip())
                    f2.write("\t\t<percent_from_breed_and_mutate>%s</percent_from_breed_and_mutate>\n" % percent_from_breed_and_mutate.rstrip())
                    f2.write("\t\t<percent_to_random>%s</percent_to_random>\n" % percent_to_random.rstrip())
                    f2.write("\t\t<!-- the above parameters must add to 1.0 -->\n")
                    f2.write("\t\t<top_percent_to_breed_from>%s</top_percent_to_breed_from>\n" % top_percent_to_breed_from.rstrip())
                    f2.write("\t\t<top_percent_to_mutate_from>%s</top_percent_to_mutate_from>\n" % top_percent_to_mutate_from.rstrip())
                    f2.write("\t\t<top_percent_to_breed_mutate_from>%s</top_percent_to_breed_mutate_from>\n" % top_percent_to_breed_mutate_from.rstrip())
                    f2.write("\t\t<percent_of_genome_mutations>%s</percent_of_genome_mutations>\n" % percent_of_genome_mutations.rstrip())
                    f2.write("\t\t<!-- if tournament selection -->\n")
                    f2.write("\t\t<tournament_size>%s</tournament_size>\n" % tournament_size.rstrip())
                    f2.write("\t</ga_params>\n")
                    f2.write("</ga_config>\n")
                                
                    f2.close()

    num_alphabetic = num_alphabetic + 1
        
f3.close()

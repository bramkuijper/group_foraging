#!/usr/bin/env python3
import datetime as dt

base_name = "sim_group_forage_"
current_time = dt.datetime.now()
base_name += current_time.strftime("%Y%m%d_%H%M%S")

nrep = 3

sim_counter = 0

mu_a_resource = [0.01]
mu_b_resource = [0.01]
p_high_quality = [0.5]

exe = "./group_foraging_ibm.exe"

for i in range(0, nrep):
    for mu_a_resource_i in mu_a_resource:
        for mu_b_resource_i in mu_b_resource:
            for p_high_quality_i in p_high_quality:
                sim_counter += 1
                output_file = base_name + "_" + str(sim_counter)

                print(f"{exe} " +
                      f"{output_file} " +
                      f"{mu_a_resource_i} " + 
                      f"{mu_b_resource_i} " + 
                      f"{p_high_quality_i} ") 


    

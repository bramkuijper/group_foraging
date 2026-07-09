#!/usr/bin/env python3
import datetime as dt

base_name = "sim_group_forage_"
current_time = dt.datetime.now()
base_name += current_time.strftime("%Y%m%d_%H%M%S")

nrep = 6

sim_counter = 0

# resources at the start of the season
init_resources = 10
nest_pred_baseline = 0.0
nest_pred_scale = [0.005,0.0]

mu_a_resource = [0.05]
mu_b_resource = [0.05]
mu_a_t = [0]
mu_b_t = [0]
mu_a_qown = [0]
mu_b_qown = [0]
mu_a_qother = [0]
mu_b_qother = [0]
mu_a_action_other = [0]
mu_b_action_other = [0]

p_high_quality = [0.5]
max_gen = 50000

exe = "./group_foraging_ibm.exe"

for i in range(0, nrep):
    for mu_a_resource_i in mu_a_resource:
        for mu_b_resource_i in mu_b_resource:
            for mu_a_t_i in mu_a_t:
                for mu_b_t_i in mu_b_t:
                    for mu_a_qown_i in mu_a_qown:
                        for mu_b_qown_i in mu_b_qown:
                            for mu_a_qother_i in mu_a_qother:
                                for mu_b_qother_i in mu_b_qother:
                                    for mu_a_action_other_i in mu_a_action_other:
                                        for mu_b_action_other_i in mu_b_action_other:
                                            for p_high_quality_i in p_high_quality:
                                                for nest_pred_scale_i in nest_pred_scale:
                                                    sim_counter += 1
                                                    output_file = base_name + "_" + str(sim_counter)

                                                    print(f"{exe} " +
                                                          f"{output_file} " +
                                                          f"{mu_a_resource_i} " + 
                                                          f"{mu_b_resource_i} " + 
                                                          f"{mu_a_t_i} " + 
                                                          f"{mu_b_t_i} " + 
                                                          f"{mu_a_qown_i} " + 
                                                          f"{mu_b_qown_i} " + 
                                                          f"{mu_a_qother_i} " + 
                                                          f"{mu_b_qother_i} " + 
                                                          f"{mu_a_action_other_i} " + 
                                                          f"{mu_b_action_other_i} " + 
                                                          f"{p_high_quality_i} " + 
                                                          f"{init_resources} " + 
                                                          f"{nest_pred_baseline} " + 
                                                          f"{nest_pred_scale_i} " + 
                                                          f"{max_gen} ") 


                

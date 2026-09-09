#!/usr/bin/env python3
import datetime as dt
import numpy as np

base_name = "sim_group_forage_"
current_time = dt.datetime.now()
base_name += current_time.strftime("%Y%m%d_%H%M%S")

nrep = 10

sim_counter = 0

# resources at the start of the season
init_resources = 10
nest_pred_baseline = 0.0
nest_pred_scale = [0.05,0.1]

# the different cue combinations
# order is: resources, seasonal time (t), own quality (qown), other's quality (qother), action other
cues = [
        [ 0, 0, 0, 0, 0], 
        [1,0,0,0,0], 
        [0,1,0,0,0],
        [1,1,0,0,0], 
        [0,0,1,1,1], 
        [1,1,1,1,1]
        ]


mu_a_resource = [0.05]
mu_b_resource = [0.05]
mu_a_t = [0.0]
mu_b_t = [0.0]
mu_a_qown = [0.0]
mu_b_qown = [0.0]
mu_a_qother = [0.0]
mu_b_qother = [0.0]
mu_a_action_other = [0.0]
mu_b_action_other = [0.0]

p_high_quality = [0.5]
max_gen = 50000

forage_individually = [0,1]

epsilon = 0.75

exe = "./group_foraging_ibm.exe"

for i in range(0, nrep):
    for cue_combn_i in cues:
        mu_a_resource_i = 0.0
        mu_b_resource_i = 0.0
        mu_a_t_i = 0.0
        mu_b_t_i = 0.0
        mu_a_qown_i = 0.0
        mu_b_qown_i = 0.0
        mu_a_qother_i = 0.0
        mu_b_qother_i = 0.0
        mu_a_action_other_i = 0.0
        mu_b_action_other_i = 0.0

        if cue_combn_i[0] > 0:
            mu_a_resource_i = 0.05
            mu_b_resource_i = 0.05

        if cue_combn_i[1] > 0:
            mu_a_t_i = 0.05
            mu_b_t_i = 0.05
        
        if cue_combn_i[2] > 0:
            mu_a_qown_i = 0.05
            mu_b_qown_i = 0.05
        
        if cue_combn_i[3] > 0:
            mu_a_qother_i = 0.05
            mu_b_qother_i = 0.05

        if cue_combn_i[4] > 0:
            mu_a_action_other_i = 0.05
            mu_b_action_other_i = 0.05

        for p_high_quality_i in p_high_quality:
            for nest_pred_scale_i in nest_pred_scale:
                for forage_individually_i in forage_individually:
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
                          f"{epsilon} " + 
                          f"{forage_individually_i} " + 
                          f"{max_gen} ") 

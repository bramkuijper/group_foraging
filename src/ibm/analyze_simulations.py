#!/usr/bin/env python3

import os, re, sys
import pandas as pd

first = True
init = False
num_semicol_header = 0

# analyze parameters at the end of the file
def analyze_parameters(lines,first=False):

    pars = {}

    for line in lines:
        mobj = line.split(";")

        if len(mobj) > 1:
            pars[mobj[0]] = mobj[1]

    return(pars)

def analyze_data(lines):

    data = [ [ float(celli) ] for celli in lines[0].split(";")[0:-1] ]

    # loop through the lines to collect the data
    for line in lines[1:]:
        splitline = line.split(";")[0:-1]

        for i in range(0,len(splitline)):
            data[i].append(float(splitline[i]))

    # now take averages

    avgs = []
    for i in range(0,len(data)):
        avgs.append(sum(data[i])/len(data[i]))

    return(avgs)

# processes the first line headers
# when making line headers for initial values
def process_first_line(line):

    # get the column names and split them into a list
    line_cols = line.strip().split(";")

    new_cols = ""

    for colname in line_cols:
        if not colname:
            continue

        new_cols += colname + "_t_0;" 

    return(new_cols)

def column_join_func(join_tpl):
    
    if join_tpl[1] == "":
        return(join_tpl[0])

    return(join_tpl[0] + "_" + join_tpl[1])
    

# obtain the listing of parameters
def get_parameters_again(filename):

    with open(filename) as f:
        fl = f.readlines()

    fl_idx = -1

    for fl_idx in range(0,len(fl)):
        if fl[fl_idx].strip() == "":
            break

    params_df = pd.read_csv(filepath_or_buffer = filename, 
                            sep=";", 
                            header = None,
                            skiprows= fl_idx)

    params_df = params_df.rename(
            columns = {0:'name',1:'value'})

    return(params_df)

# calculate variation in group resources across different
# time points. 
# What we want is a table with time step, group size, variance resources, intercept resources
def group_resources_over_time(filename, only_survivors = True):

    dynamics_filename = filename + "_dynamics"

    dynamics_data = pd.read_csv(filepath_or_buffer = dynamics_filename,
                    sep=";")

    # select only t, group size and resources
    dynamics_data = dynamics_data[["t","group_idx","group_size","group_resources_pc","group_resources"]]

    # if we are only looking at groups which survive until
    # the end, get their indices from last time step and use
    # those to subset the data
    if only_survivors:
        survivors_ids = dynamics_data.loc[(dynamics_data["t"] == max(dynamics_data["t"])), "group_idx"].unique()

        dynamics_data = dynamics_data.loc[dynamics_data["group_idx"].isin(survivors_ids)]

    dynamics_data = dynamics_data.loc[:,dynamics_data.columns != "group_idx"]

    # then aggregate the data on ecological time step t 
    # and group size
    result = dynamics_data.groupby(["t","group_size"], 
                                   as_index = False).aggregate(["mean","std"])

    result.columns = [column_join_func(col) for col in result.columns]

    nrow = result.shape[0]

    params_df = get_parameters_again(filename)

    for index, row in params_df.iterrows():
        param_name = row["name"]
        param_value = row["value"]

        result.reset_index(drop=True, inplace=True)

        result[param_name] = pd.Series(param_value for i in range(0, nrow))

    result["file"] = filename

    return(result)

def analyze_file(filename):

    global first;
    global num_semicol_header;

    global init;

    # indicator variable whether we are at first line
    # of the file to be read
    firstline = True

    flhead = ""

    lc = 0

    # indicator variable whether we are at the part
    # involving parameters
    parameter_part = False

    parameter_lines = []

    # the line where the parameter output
    # starts
    parline = -1

    # store the last line of data
    last_data_line = ""

    # store the first line of data
    # in case we need initial values too
    first_data_line =""

    # the header of the resulting data file
    flhead = ""

    # open the file and read the stuff
    with open(filename) as infile:
        for line in infile:

            # see whether we also have to store the initial values
            if init:
                if firstline:
                    flhead += process_first_line(line)

            # update line count
            lc += 1

            # get the first line of data
            if lc == 2:
                first_data_line = line.strip()

            # if this is the first line store
            # the header
            if firstline:
                flhead += line.strip()
                firstline = False

            # if this is any other line starting
            # with a numerical value store the line
            # as it might be potentially the last one
            elif re.match(r"^\d",line):
                last_data_line = line

            # hold this until we have the parameter file
            if not parameter_part:
                if re.match("^\n",line) is not None:
                    parline = lc
                    parameter_part = True
                    parameter_lines += [line.strip()]
            elif parameter_part:
                parameter_lines += [line.strip()]

    if parline < 1:
        return

    parameters = analyze_parameters(parameter_lines)

    # prepare the data to be printed
    data = ""

    if init:

        # do error checking in terms of the csv values
        count_semicol = len(re.findall(";", first_data_line))
        count_semicol_data = len(re.findall(";", last_data_line))

        assert(count_semicol == count_semicol_data)

        data += first_data_line.strip()

    data += last_data_line.strip()

    if first:

        header_line = ";".join(parameters.keys()) + ";" + flhead.strip() + "file"

        # count number of occurrences of semicolon for error checking
        num_semicol_header = len(re.findall(";",header_line))

        print(header_line)

        first = False

    data_line = ";".join(parameters.values()) + ";" + data +  filename

    # count number of occurrences of semicolon for error checking
    num_semicol_data = len(re.findall(";",data_line))

    assert(num_semicol_header == num_semicol_data)
    print(data_line)


if len(sys.argv) > 2:

    # get initial values from the files as well
    init = True


list_time_dfs = []


# run the function on the indicated dir
for root, dir, files in os.walk(sys.argv[1]):

    for file in files:
        if re.match(r"(output|competition_output|sim|iter).*\d(.csv|.txt)?$",file) != None:

            the_actual_file = os.path.join(root, file)
            analyze_file(the_actual_file)
            
            df_time = group_resources_over_time(the_actual_file)
            list_time_dfs.append(df_time)


total = pd.concat(list_time_dfs)

print(total.head())

total.to_csv(path_or_buf = "summary_time_data.csv", sep=";")



#!/usr/bin/env Rscript
library("tidyverse")
library("lme4")
# small script to extract means variances
# at the end as well temporal stuff over time

# go over each dynamics file and extract temporal variance
dynamics_files <- list.files(
    path = ".", 
    pattern = ".*sim_group.*_dynamics$")

# calculate temporal variance, how to do this:
# we have tonnes of females and males each
# with particular ID's nested within groups

the.data <- data.frame(file = dynamics_files)

the.data$sd_single <- NA
the.data$sd_double <- NA

for (idx in 1:length(dynamics_files))
{
    print(idx)
    
    file <- as.character(the.data[idx,"file"])
    fx <- read.table(file = file,
                     header = T,
                     sep =";")
    
    fx_single <- fx %>% filter(group_size == 1)
    fx_double <- fx %>% filter(group_size > 1)
    
    sd_single <- NA
    sd_double <- NA
    
    if (nrow(fx_single) >0)
    {
        # variance in resource levels over time for singles
        obj <- lmer(formula = group_resources ~ t + (t || group_idx),
                data = fx %>% filter(group_size == 1))
        
        sd_single <- as.numeric(attr(VarCorr(obj, comp="stddev")$group_idx.1, "stddev"))
        
    }
    
    if (nrow(fx_double) > 0)
    {
        # variance in resource levels over time for paired individuals
        obj <- lmer(formula = group_resources ~ t + (t || group_idx),
                data = fx %>% filter(group_size > 1))
        
        sd_double <- as.numeric(attr(VarCorr(obj, comp="stddev")$group_idx.1, "stddev"))
    }
    
    the.data[idx,c("sd_single","sd_double")] <- c(sd_single,sd_double)
}

ggplot(data = the.data,
       mapping = aes(x = sd_single, y = sd_double)) +
    geom_point() +
    theme_classic()

ggsave(filename="plot_single_double.pdf")

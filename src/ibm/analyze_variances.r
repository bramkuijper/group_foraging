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

file <- dynamics_files[1]

fx <- read.table(file = file,
                 header = T,
                 sep =";")

ggplot(data = fx, 
       mapping = aes(x = t, y = group_resources)) +
    geom_line(mapping = aes(group = group_idx),alpha = 0.1) +
    geom_jitter(width = 0.2, alpha = 0.1,height=0.05)

ggsave(filename=paste0(file,".pdf"))

# variance in resource levels over time
obj <- lmer(formula = group_resources ~ 1 + (1|group_idx),
        data = fx)

print(obj)


## calculate numbers of individuals over time
#fx_summary <- fx %>% group_by(t) %>%
#    summarise(total = n())
#
#ggplot(data = fx_summary,
#       mapping = aes(x = t, y = total)) +
#    geom_line()


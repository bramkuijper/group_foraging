#!/usr/bin/env Rscript

library("tidyverse")
library("patchwork")


if (!exists("dyn_file"))
{
    dyn_file <- commandArgs(trailingOnly = T)
}

find_params <- function(filename) {

    f <- readLines(filename)

    seq.rev <- rev(seq(1,length(f),1))

    for (line_i in seq.rev)
    {
        if (length(grep("^\\d",f[[line_i]])) > 0)
        {
            return(line_i)
        }
    }
} 


main_file <- gsub(x = dyn_file,
        pattern = "_dynamics$", 
        replacement="")

param_main_file_line <- find_params(main_file)

# get the parameters
params <- read_delim(file=main_file
        ,delim=";"
        ,skip=param_main_file_line
        ,col_names=c("name","value")
        )

mutation_params <- params %>% 
    filter(str_detect(name, "mu_b.*")) 

mutation_params <- mutation_params %>%
    unite(., labels, -X3, sep="=")

title <- paste(mutation_params$labels, collapse=" ")

output_file_name = paste0("graph_",basename(dyn_file),".pdf")

dyn_data <- read_delim(file = dyn_file
                       ,delim = ";") %>% 
    mutate(
    group_size_f = as_factor(n_per_group),
    group_idx = as.integer(group_idx),
    group_idx_f = as_factor(group_idx),
    individual_idx_f = as_factor(individual_idx),
    t_new = if_else(group_size_f == 1, t - 0.1, t)
)

dyn_data_alive <- dyn_data %>% 
    filter(group_dead == 0) 

p1 <- ggplot(data = dyn_data_alive,
       mapping = aes(x = t, y = cumul_resources)) +
    geom_line(mapping = aes(group = group_idx_f, colour = group_size_f), 
              alpha = 0.1) +
    theme_classic() + 
    facet_grid(~group_size_f) +
    scale_colour_brewer(palette="Set1")

(p1)

## next: distribution of resources
p2 <- ggplot(data = dyn_data_alive %>% filter(t == max(t)),
            mapping = aes(x = cumul_resources)) +
                geom_density(mapping = aes(colour = group_size_f)) +
    theme_classic() +
    scale_colour_brewer(palette="Set1")



## now the foraging dynamics
p3 <- ggplot(data = dyn_data,
       mapping = aes(x = t, y = foraging_current)) +
    geom_line(mapping = aes(group = individual_idx_f, colour = group_size_f), 
              alpha = 0.1) +
    theme_classic() + 
    facet_grid(~group_size_f) +
    scale_colour_brewer(palette="Set1")


p4 <- ggplot(data = dyn_data %>% filter(t == max(t)),
       mapping = aes(x = foraging_previous, y = foraging_current)) +
    geom_jitter(mapping = aes(group = individual_idx_f, colour = group_size_f), 
                alpha = 0.1, width = 0.1, height = 0.1) +
    theme_classic() + 
    facet_grid(~group_size_f) +
    scale_colour_brewer(palette="Set1")



#### fraction of dead individuals ####

dat1 <- dyn_data %>% group_by(t) %>% 
    summarise(frac_dead = mean(group_dead))

p5 <- ggplot(data = dat1,
       mapping = aes(x = t, y = frac_dead)) +
    theme_classic() + 
    geom_line() +
    scale_colour_brewer(palette="Set1")

# summarize dynamical data to know a bit more
# about group numberts
group_numbers_per_t <- dyn_data %>% 
    filter(generation == max(generation)) %>%
    group_by(t) %>%
    summarise(total = n())


# actually this is not what I want, I want a histogram
# over time of group size distributions of groups that
# that are alive at that moment
the.summary <- dyn_data %>% 
    filter(group_dead == 0) %>%
    group_by(t, group_idx) %>%
    summarise(n_total_per_group = n()) %>%  
    group_by(t, n_total_per_group) %>%
    summarize(group_hist = n(), .groups = "drop") %>%
    complete(t = 0:max(dyn_data$t),
             n_total_per_group = 0:5,
             fill = list(group_hist = 0)
             )

p6 <- ggplot(data = the.summary,
        mapping = aes(x = t, y = n_total_per_group)) +
    geom_tile(mapping = aes(fill = group_hist))

     ## number of groups as the season progresses
     #p6 <- ggplot(data = group_numbers_per_t,
     #             mapping = aes(x = t, y = total)) +
     #    geom_line() +
     #    theme_classic() +
     #    ylim(0, max(group_numbers_per_t$total))

dyn_data <- dyn_data %>% mutate(
            tf = as_factor(t)
        )

p7 <- ggplot(data = dyn_data %>% filter(group_dead == 0 & t %in% c(19,35,50)), 
       mapping = aes(x = cumul_resources)) +
    geom_density(mapping = aes(colour = tf), alpha = 0.5,linewidth = 0.25) +
    facet_grid(~n_per_group) +
    theme_classic()

# plot the number of inactive workers
dat_inactive <- dyn_data %>% mutate(
        n_per_group_f = as_factor(n_per_group)
    ) %>%
    group_by(t, n_per_group_f) %>%
    summarise(inactive_fraction = mean(currently_inactive))


p8 <- ggplot(data = dat_inactive, 
        mapping = aes(x = t, y = inactive_fraction)) +
    geom_line(mapping = aes(colour = n_per_group_f)) +
    theme_classic()

patchwork <- p1 / p2 / p3 / p4 / p5 / p6 / p7 / p8

(patchwork + plot_annotation(title = title))

ggsave(filename = output_file_name, height = 15)

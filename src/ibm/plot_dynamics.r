#!/usr/bin/env Rscript

library("tidyverse")
library("patchwork")


if (!exists("dyn_file"))
{
    dyn_file <- commandArgs(trailingOnly = T)
}

output_file_name = paste0("graph_",basename(dyn_file),".pdf")

dyn_data <- read_delim(file = dyn_file
                       ,delim = ";") %>% mutate(
    group_size_f = as_factor(n_per_group),
    group_idx = as.integer(group_idx),
    group_idx_f = as_factor(group_idx),
    individual_idx_f = as_factor(individual_idx),
    t_new = if_else(group_size_f == 1, t - 0.1, t)
)

p1 <- ggplot(data = dyn_data,
       mapping = aes(x = t, y = cumul_resources)) +
    geom_line(mapping = aes(group = group_idx_f, colour = group_size_f), 
              alpha = 0.1) +
    theme_classic() + 
    facet_grid(~group_size_f) +
    scale_colour_brewer(palette="Set1")

(p1)

## next: distribution of resources
p2 <- ggplot(data = dyn_data %>% filter(t == max(t)),
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

p5 <- ggplot(data = dyn_data %>% filter(t == max(t)),
       mapping = aes(x = t, y = group_dead)) +
    geom_jitter(mapping = aes(group = individual_idx_f, colour = group_size_f), 
                alpha = 0.1, width = 0.1, height = 0.1) +
    theme_classic() + 
    facet_grid(~group_size_f) +
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

(p1 / p2 / p3 / p4 / p5 / p6)

ggsave(filename = output_file_name, height = 12)

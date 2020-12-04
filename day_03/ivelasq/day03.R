# Advent of Code - Day 03

# Libraries ---------------------------------------------------------------

library(tidyverse)

# Data --------------------------------------------------------------------

dat <-
  as.matrix(read.fwf(here::here("day_03", "trees.txt"),
                     widths = rep(1, 31),
                     comment.char = ""))

# Functions ---------------------------------------------------------------

# this is to create a matrix that accommodates all the movement that
# you have to go through to get to the bottom right

get_encounters <- function(matrix, num_right, num_down){
  
  repeats <- ceiling(num_right * nrow(matrix) / ncol(matrix))
  
  new_mat <- do.call(cbind, replicate(repeats, matrix, simplify = F))

  encounters <- c()
  
  n <- ceiling(nrow(matrix) / num_down) - 1
  
  for(i in 1:n){
    
    encounters[i] <- new_mat[num_down * i + 1, num_right * i + 1]
      
  }
  
  print(sum(as.numeric(str_count(encounters, "#")))) # avoid integer overflow later
  
}

get_encounters_again <- function(matrix, slopes){
  
  n_trees <- c()
  
  for(i in 1:nrow(slopes)){
    
    n_trees[i] <- get_encounters(matrix, slopes[i, 1], slopes[i, 2])
  }
  
  print(prod(n_trees))
  
}

# Calculations ------------------------------------------------------------

# Part 01

get_encounters(dat, 3, 1)

# Part 02

entries <-
  matrix(c(1, 1,
           3, 1,
           5, 1,
           7, 1,
           1, 2),
         nrow = 5,
         byrow = T)

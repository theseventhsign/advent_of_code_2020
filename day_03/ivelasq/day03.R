# Advent of Code - Day 03

# Data --------------------------------------------------------------------

dat <-
  as.matrix(read.fwf(
    here::here("day_03", "trees.txt"),
    widths = rep(1, 31),
    comment.char = ""
  ))

# Functions ---------------------------------------------------------------

# this is to create a mat that accommodates all the movement that
# you have to go through to get to the bottom right

trees_get <- function(mat, n_right, n_down){
  
  repeats <- ceiling(n_right * nrow(mat) / ncol(mat))
  
  new_mat <- do.call(cbind, replicate(repeats, mat, simplify = F))

  encounters <- c()
  
  n <- ceiling(nrow(mat) / n_down) - 1
  
  for(i in 1:n){
    
    encounters[i] <- new_mat[n_down * i + 1, n_right * i + 1]
      
  }
  
  print(sum(as.numeric(stringr::str_count(encounters, "#")))) # avoid integer overflow later
  
}

prod_trees <- function(mat, slopes){
  
  n_trees <- c()
  
  for(i in 1:nrow(slopes)){
    
    n_trees[i] <- trees_get(mat, slopes[i, 1], slopes[i, 2])
  }
  
  print(prod(n_trees))
  
}

# Calculations ------------------------------------------------------------

# Part 01

trees_get(dat, 3, 1)

# Part 02

entries <-
  matrix(c(1, 1,
           3, 1,
           5, 1,
           7, 1,
           1, 2),
         nrow = 5,
         byrow = T)

prod_trees(dat, entries)

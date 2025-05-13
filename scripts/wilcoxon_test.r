# Load the data
data <- read.csv("./results/Wilcoxon_Test_Input.csv")

# Separate the data by job size
sizes <- unique(data$job_size)

cat("Results of the Wilcoxon test for each instance size\n\n")

for (size in sizes) {
  subset <- subset(data, job_size == size)
  
  cat(paste("instance size :", size, "jobs\n"))
  result <- wilcox.test(subset$avg_deviation_memetic, subset$avg_deviation_tabu, paired=TRUE)
  print(result)
  cat("\n")
}

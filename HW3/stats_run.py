# Import statistics package
import statistics
import math

# Problem 1
# Data
print("Problem 1")
print("")
program = [1, 2, 3, 4, 5]
s1_exec_time = [33.4, 19.9, 6.5, 84.3, 101.1]
s1_exec_mean = statistics.mean(s1_exec_time)
s2_exec_time = [28.8, 22.1, 5.3, 75.8, 99.4]
s2_exec_mean = statistics.mean(s2_exec_time)
s3_exec_time = [28.3, 25.3, 4.7, 80.1, 7.2]
s3_exec_mean = statistics.mean(s3_exec_time)
instr_count = [1.45e10, 7.97e9, 3.11e9, 3.77e10, 4.56e10]

prog_times = []
for prog in program:
    prog_times.append([s1_exec_time[prog-1], s2_exec_time[prog-1],s3_exec_time[prog-1]])
    
print(" ")
print("Mean for the 3 different systems S1, S2, S3");
print("S1 Mean =  %f" % s1_exec_mean)
print("S2 Mean = %f" % s2_exec_mean)
print("S3 Mean = %f" % s3_exec_mean)

print(" ")
print("Average across the 3 systems of the MIPS rate for each of Program")
prog_cnt = 1
for s in prog_times:
    print("Progam %d | RunTime Mean: %f(sec)\tMIPS Mean: %f(MIPS)" % (prog_cnt , statistics.mean(s), instr_count[prog_cnt-1]/(statistics.mean(s)*1e6)))
    prog_cnt = prog_cnt + 1


print(" ")
print("Average speedup for S1 and S2 with S3 as baseline")
print("S1 Average Speed up = %f%%" % ( (s3_exec_mean-s1_exec_mean)*100/s3_exec_mean) )
print("S2 Average Speed up = %f%%" % ( (s3_exec_mean-s2_exec_mean)*100/s3_exec_mean) )

print(" ")
print("Coefficient of variation of the execution times")
print("S1 Coef. of Variation =  %f" % ( statistics.stdev(s1_exec_time)/s1_exec_mean) )
print("S2 Coef. of Variation =  %f" % ( statistics.stdev(s2_exec_time)/s2_exec_mean) )
print("S3 Coef. of Variation =  %f" % ( statistics.stdev(s3_exec_time)/s3_exec_mean) )

# Problem 2
# Data
print("")
print("Problem 2")
print("")
write_time = [8.0, 7.0, 5.0, 9.0, 9.5, 11.3, 5.2, 8.5]

print("8 sample t-score = 1.895 (from df = 7 and A = 0.05)")
print("90%% confidence interval for mean time(c1, c2) = (%f, %f)" % 
    ( ( statistics.mean(write_time) - (1.895 * statistics.stdev(write_time)/(math.sqrt(8)))), 
    ( ( statistics.mean(write_time) + (1.895 * statistics.stdev(write_time)/(math.sqrt(8))))) )
)

print("")
print("Measurements for 90% confidence with 7% error")
print("n = %d" % ( math.ceil( ( 1.895 * statistics.stdev(write_time)/((0.07/2)*statistics.mean(write_time))))))

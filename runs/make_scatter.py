# This script makes scatter plots from a cvs
# Please use python3, and either pipe in 
# stdin or save the csv and supply the filename
# as an argument
import matplotlib
import matplotlib.pyplot as plot
import numpy as np
import pandas as pd
import sys

# Lets check if an argument is given, if not, assume
# that we are piping an stdin into the script
if len(sys.argv) == 1:
    f_input = sys.stdin
else:
    f_input = sys.argv[1]

# Read in the data as a pandas dataframe
df = pd.read_csv(f_input, sep=',', header=None)
# sanity check
print("Sanity Check (read_cvs)")
print(df)
print(df.dtypes)

# Calculate the total number of cores for the y axis, append
# as last column
n_columns = df.shape[1]
df.insert(n_columns, n_columns, df[2] * df[3], True)

# Now we calculate the Speedup and append it again as the last column
df.insert(n_columns+1, n_columns+1, max(df[4]) / df[4], True)

# Now lets scatter
df.plot.scatter(x=n_columns, y=n_columns+1, title="Speedup vs. Number of Threads")

# Now lets set some meta data
plot.ylabel('Speedup')
plot.xlabel('Number of Threads')

plot.show(block=True)



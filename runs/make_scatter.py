# This script makes scatter plots from a cvs
# Please use python3, and either pipe in 
# stdin or save the csv and supply the filename
# as an argument
import matplotlib
import matplotlib.pyplot as plot
import matplotlib.lines as mlines
import matplotlib.transforms as mtransforms
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
df.insert(n_columns+1, n_columns+1, max(df[17]) / df[17], True)
# print(df)

# plot f(x) = x
line = np.linspace(0, 48, 100)
x_line = np.linspace(0, 48, 100)

fig, ax = plot.subplots()

# Now lets scatter
df.plot(kind='scatter', x=n_columns, y=n_columns+1, 
        title="do_qmp2::compute() speedup", grid=True, ax=ax, 
        label='measured')
ax.plot(line, x_line, zorder=-1, color='green', label='opt.')

# Now lets set some meta data
plot.ylabel('speedup')
plot.xlabel('number of threads')

plot.legend()

plot.show()



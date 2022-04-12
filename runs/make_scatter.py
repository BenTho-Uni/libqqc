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
df.insert(n_columns+1, n_columns+1, max(df[18]+df[19]) / (df[18]+df[19]), True)
#print(df)

# plot f(x) = x
line = np.linspace(0, 1000, 100)
x_line = np.linspace(0, 1000, 100)

fig, ax = plot.subplots()

def thr2node(x):
    x = x / 48
    return x

def node2thr(x):
    x = x * 48
    return x

sec_ax = ax.secondary_xaxis('top', functions=(thr2node, node2thr))
sec_ax.set_xlabel('number of nodes')

# Now lets scatter
df.plot(kind='scatter', x=n_columns, y=n_columns+1, 
        title="do_qmp2::compute() speedup", grid=True, ax=ax, 
        label='measured')
ax.plot(line, x_line, zorder=-1, color='green', label='opt.')
plot.ylabel('speedup')
plot.xlabel('number of threads')
plot.legend()

X_detail = np.linspace(0,48,100)
Y_detail = np.linspace(0,48,100)
sub_axes = plot.axes([.6,.2,.25,.25])
sub_axes.set_xlim([0,50])
sub_axes.set_ylim([0,50])
sub_axes.plot(X_detail, Y_detail, c='green')
df.plot(kind='scatter', x=n_columns, y=n_columns+1, title="one node scaling", grid=True, ax=sub_axes)
plot.ylabel('')
plot.xlabel('')


# Now lets set some meta data


plot.show()



import matplotlib
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import seaborn as sns
import sys

# First we create an empty list for the dataframes
df_list = []

# Then we loop over all system arguments
for x in range(0, len(sys.argv)):
    f_input = sys.argv[x] # set file input name
    print(f_input)
    # on first iteration (no input files)
    if x == 0:
        # take sys stdin as input if no input given
        if len(sys.argv) == 1:
            f_input = sys.stdin
        # otherwise skip first iteration
        else:
            continue
    # read in csv input file 
    df = pd.read_csv(f_input, sep=',', header=None)
    # Calculate the total number of cores for the y axis, append
    # as last column
    n_columns = df.shape[1]
    df.insert(n_columns, n_columns, df[2] * df[3], True)

    # Now we calculate the Speedup and append it again as the last column
    df.insert(n_columns+1, n_columns+1, max(df[19]+df[20]) / (df[19]+df[20]), True)
    # Make another column with the file input name
    if "anthracene" in f_input:
        df[n_columns + 2] = "anthracene"
    elif "h2o" in f_input:
        df[n_columns + 2] = "water"
    elif "h3coh" in f_input:
        df[n_columns + 2] = "methanol"
    elif "porphyin" in f_input:
        df[n_columns + 2] = "porphyrin"
    else:
        df[n_columns + 2] = f_input

    # Then append a dataframe consisting of number of cores, speedup and filename 
    # to the dataframe list
    df_list.append(df[[n_columns, n_columns + 1, n_columns + 2]])

# Concat the dataframe list and reset the index
df_all = pd.concat(df_list).reset_index()
# We now have a dataframe of all points
# Let's clean it up. We replace inf and -inf with NaN
df_all.replace([np.inf, -np.inf], np.nan, inplace=True)
# end drop all rows with NA
df_all = df_all.dropna()

# plot f(x) = x
line = np.linspace(0, 1000, 100)
x_line = np.linspace(0, 1000, 100)

# Setting up plots and subplots
fig, ax = plt.subplots(figsize = (6.5,6.5))

def thr2node(x):
    x = x / 48
    return x

def node2thr(x):
    x = x * 48
    return x

# Secondary upper axes depixting the number of nodes
sec_ax = ax.secondary_xaxis('top', functions=(thr2node, node2thr))
sec_ax.set_xlabel('number of nodes')

# Plotting the linear function
ax.plot(line, x_line, zorder=-1, color='grey', label='optimal', linestyle='dashed')
# Using sns to scatterplot the data grouped by one column with the names
sns.scatterplot(x = 23, y = 24, hue = 25, data = df_all, palette='muted')

# Adding some labeling
#plt.title('Parallel Scaling of Hybrid Variant')
plt.ylabel('speedup')
plt.xlabel('number of threads')
#plt.grid()
plt.legend()

# Linear line for the smaller plot
X_detail = np.linspace(0,48,100)
Y_detail = np.linspace(0,48,100)

# Setting up the smaller plot
sub_axes = plt.axes([.2,.45,.23,.23])
sub_axes.set_xlim([0,50])
sub_axes.set_ylim([0,50])
# Plotting linear 
sub_axes.plot(X_detail, Y_detail, c='grey', linestyle='dashed')
# Plotting data again
sns.scatterplot(x = 23, y = 24, hue = 25, data = df_all, legend=None, palette='muted')
# Disabling labels
plt.ylabel(None)
plt.xlabel(None)

#plt.savefig("scaling_mpionly_eigen.png")
plt.show()


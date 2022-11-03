import sys
import matplotlib.pyplot as plt

if len(sys.argv) < 3:
    print("Error: too few arguments!")
    print(
        "Usage: ./plot.py input_data_filename output_filename [plot_title] [y_axis_label1] [y_axis_label2]"
    )
    exit()

# Read command line arguments
datafile = sys.argv[1]
outfile = sys.argv[2]

plottitle = ""
ylabel1 = ""
ylabel2 = ""
if len(sys.argv) >= 4:
    plottitle = sys.argv[3]
    if len(sys.argv) >= 5:
        ylabel1 = sys.argv[4]
        ylabel2 = sys.argv[5]

# Read input data file
labels = []
data = []

with open(datafile) as f:
    content = f.read().splitlines()
    # First row is assumed to contain labels for each column
    labels = [l.strip() for l in content[0].split("\t")]

    # Use the number of labels to determine the number of data columns
    data = [[] for _ in range(len(labels))]
    for line in content[1:]:
        i = 0
        for num in line.split():
            data[i].append(float(num))
            i += 1

if ylabel1 == "" and len(labels) == 2:
    ylabel1 = labels[1]
if ylabel2 == "" and len(labels) == 3:
    ylabel2 = labels[2]

# Initialize plot
fig, (ax1, ax2) = plt.subplots(2, 1, sharex=True)

# Plot each data set, using first column as x-axis data
# for i in range(1, len(labels)):
ax1.plot(data[0], data[1])
ax2.plot(data[0], data[2])

# Plot's aesthetics
ax1.set_title(plottitle)
ax1.set_ylabel(ylabel1)
ax2.set_xlabel(labels[0])
ax2.set_ylabel(ylabel2)

# Save figure
fig.savefig(outfile, dpi=600)
plt.close("all")

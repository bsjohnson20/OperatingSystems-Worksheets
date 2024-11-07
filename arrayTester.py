import subprocess as sp
import matplotlib.pyplot as plt
# open file output/task1 and run it, pass input to it automatically
import re
output = []
# print("\n",x.stdout)



"""
File to test task2's array sum. Creates a plot with formula, a plot with 
the output and a plot with difference, 
and prints the difference between the two plots.

Ideally it should be a brown line, with a blue line at a constant 0 difference.
"""


def disabled_task1(): # to be run if task1 was jmp'd over and jumped to array task
    output = []
    for i in range(50, 101):
        x = sp.run(['output/task2'], input=f'{i}\n', text=True, capture_output=True,timeout=5)
        output.append(x.stdout.split(')')[1].strip())
    return output
        
def task1():
    # compiled regex to find (1-100)num
    p = re.compile(r'\(1-100\)\d+')
    output = []
    for i in range(50, 101):
        x = sp.run(['output/task2'], input=f'{i}\n{i}\nluna\n53\n{i}', text=True, capture_output=True)
        # print(x.stdout);
        # print(x.stderr)
        # print(x.returncode)
        num = p.findall(x.stdout)[0].split(')')[1].strip()
    
        # input()
        output.append(num)
    return output

if input("Was task1 disabled? (y/n): ") == 'y':
    output = disabled_task1()
else:
    output = task1()
# print(x.stdout)

inc = 49
def num(x):
    global inc
    inc+=1
    return [int(inc), int(x)] 

# add plot of n(n+1)/2
import math
out2 = [[x, (x*(x+1))/2] for x in range(50, 101)]
plt.plot([x[0] for x in out2],[x[1] for x in out2],color='green',alpha=0.8)

# Brown line = overlay of both plots and therefore correct. 
# Blue line denotes difference between the two plots, should be constant 0

# Code for output from array task
out = [num(x) for x in output]
plt.plot([x[0] for x in out],[x[1] for x in out],color='red',alpha=0.5)



# Plot differences between plot 2 and plot 1
plt.plot([x[0] for x in out],[abs(out2[i][1] - out[i][1]) for i in range(len(out))],color='blue',alpha=0.5)


# calc average shifted between lists
def calc_avg(a, b):
    return sum([abs(a[i][1] - b[i][1]) for i in range(len(a))]) / len(a)

print("Average difference between the two lists: ", calc_avg(out, out2))

# print output
for i in range(len(out)):
    print(f"Expected: {out2[i][1]} Got: {out[i][1]}. Difference: {out2[i][1] - out[i][1]}")

# save plot to html
plt.savefig('arrayTesting/test.webp')

print("Plot saved to test.webp")

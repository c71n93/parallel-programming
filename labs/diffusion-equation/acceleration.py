import os
import subprocess
from termcolor import colored
import matplotlib.pyplot as plt
import numpy as np

def print_cyan(str):
    print(colored(str, "cyan"))
os.system("cmake . -B build/")

size_x = str(5000)
size_t = str(3000)
os.chdir("build")
print_cyan("make")
os.system("make")
accelerations = []
efficiencies = []
sizes = []
threads = 1

cmd = "mpirun -np " + str(threads) + " --oversubscribe ./parc " + size_t + " " + size_x
result = subprocess.check_output(cmd, shell=True, text=True)
sizes.append(threads)
accelerations.append(1.0)
efficiencies.append(1.0)
seq = float(result)
threads = 2
while threads < 16:
    cmd = "mpirun -np " + str(threads) + " --oversubscribe ./parc " + size_t + " " + size_x
    result = subprocess.check_output(cmd, shell=True, text=True)
    print("n_proc = %s, time = %ss" % (str(threads), result))
    sizes.append(threads)
    accelerations.append(seq / float(result))
    efficiencies.append(seq / (float(result) * threads))
    threads += 1

plt.figure(figsize=[12, 5], dpi=100)
plt.plot(list(sizes), accelerations, '-o', markersize=4, linewidth=2, label='y1', color = np.random.rand(3))

plt.xlabel("n_proc")
plt.ylabel("t_seq / t_par")
plt.title("Ускорение")
plt.minorticks_on()
plt.grid()
os.chdir("../")
if(not os.path.exists("graphs")):
    os.mkdir("graphs")
os.chdir("graphs")
current_dir = os.getcwd()
print_cyan(f"Saving graph to {current_dir}/acceleration.png")
plt.savefig('acceleration.png')

plt.figure(figsize=[12, 5], dpi=100)
plt.plot(list(sizes), efficiencies, '-o', markersize=4, linewidth=2, label='y1', color = np.random.rand(3))
plt.xlabel("n_proc")
plt.ylabel("t_seq / (t_par * n_proc)")
plt.title("Эффективность")
plt.minorticks_on()
plt.grid()
current_dir = os.getcwd()
print_cyan(f"Saving graph to {current_dir}/efficiency.png")
plt.savefig('efficiency.png')

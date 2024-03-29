import os
import subprocess
import sys
from termcolor import colored
import matplotlib.pyplot as plt
import numpy as np


def print_cyan(str):
    print(colored(str, "cyan"))


def main() -> int:
    os.system("cmake . -B build/")
    os.chdir("build")
    print_cyan("make")
    os.system("make")
    accelerations = []
    efficiencies = []
    sizes = []
    threads = 1

    cmd = "./Sequential"
    print_cyan(cmd)
    result = subprocess.check_output(cmd, shell=True, text=True)
    print("   threads = 1: time = %ss" % result)
    sizes.append(threads)
    accelerations.append(float(result))
    efficiencies.append(1.0)
    seq = float(result)
    threads = 1

    while threads < 32:
        cmd = "./Parallel " + str(threads)
        print_cyan(cmd)
        result = subprocess.check_output(cmd, shell=True, text=True)
        print("   threads = %s: time = %ss" % (str(threads), result))
        sizes.append(threads)
        accelerations.append(float(result))
        efficiencies.append(seq / (float(result) * threads))
        if threads == 1:
            threads += 1
        else:
            threads += 2

    plt.figure(figsize=[12, 5], dpi=100)
    plt.axhline(y=0, color='k')
    plt.plot(list(sizes), list(map(lambda val: seq / val, accelerations)), '-o', markersize=4, linewidth=2, label='y1', color=np.random.rand(3))

    plt.xlabel("Num of threads")
    plt.ylabel("time / seq_time")
    plt.title("Acceleration")
    plt.minorticks_on()
    plt.grid()
    os.chdir("../")
    if not os.path.exists("graphs"):
        os.mkdir("graphs")
    os.chdir("graphs")
    current_dir = os.getcwd()
    print_cyan(f"Saving graph to {current_dir}/Acceleration.png")
    plt.savefig('Acceleration.png')

    plt.figure(figsize=[12, 5], dpi=100)
    plt.axhline(y=0, color='k')
    plt.plot(list(sizes), efficiencies, '-o', markersize=4, linewidth=2, label='y1', color=np.random.rand(3))
    plt.xlabel("num of procs")
    plt.ylabel("time / (seq_time * threads)")
    plt.title("Efficiency")
    plt.minorticks_on()
    plt.grid()
    current_dir = os.getcwd()
    print_cyan(f"Saving graph to {current_dir}/Efficiency.png")
    plt.savefig('Efficiency.png')
    return 0


if __name__ == '__main__':
    sys.exit(main())

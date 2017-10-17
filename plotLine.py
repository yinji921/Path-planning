import numpy as np
import matplotlib.pyplot as plt
import math
import time


# dataStyle example:
#   start
#   x1 y1
#   end
#   x1 y1
#   obstacle
#   100 200 1 50 x x        (circle  x y kind r a b)
#   obstacle
#   300 200 0 x 390 20      (rectangle  x y kind r a b)
#   line
#   255                     (weight [0, 255])
#   x1 y1
#   x2 y2
#   ...
#   endline
#   line
#   200
#   x1 y1
#   ...
#   endline
#
#   ...
#
#   clear           (if you want to clear previous lines)
#   line
#   128                     (weight [0, 255])
#   x1 y1
#   x2 y2
#   ...
#   endline
#   exit

#   Run code:
#   ./{编译后文件} | python3 plotLine.py


# Red color (from light to dark)
# 'lightpink':            '#FFB6C1'
# 'lightcoral':           '#F08080'
# 'tomato':               '#FF6347'
# 'red':                  '#FF0000'
# 'firebrick':            '#B22222'
# 'brown':                '#A52A2A'
# 'darkred':              '#8B0000'

mapSize = 500

def getColor(weight):   # weight small-> black, weight large-> red. weight range [0, 255]
    # frac = 255/math.log(255, math.e)
    # redColor = int(math.exp(weight/frac))
    redColor = weight

    if redColor <= 16:
        # print("Set redColor from %d to 16" % redColor)
        redColor = 16
    if redColor > 255:
        # print("Set redColor from %d to 255" % redColor)
        redColor = 255
    colorStr = hex(int(redColor)) + '0000'
    if redColor>100:
        width = (redColor-100) / 155 * 0.9 + 0.05
    else:
        width = 0.05
    return '#' + colorStr[2:], width


def plot_data(data):
    try:
        colorStr = getColor(data[2])
    except IndexError:
        print("No color weight input! Set as default RED!")
        colorStr = '#FF0000'
    plt.plot(data[0], data[1], color=colorStr, marker=',')
    plt.pause(0.0001)
    # print('continue computation')

def plot_line(lineData, colorStr, width):
    dataX = []
    dataY = []
    for data in lineData:
        dataX.append(data[0])
        dataY.append(data[1])
    ln, = plt.plot(dataX, dataY, color=colorStr, linewidth=width)
    lineSet.append(ln)
    plt.pause(0.0001)


def plot_circle(x, y, r):
    circle = plt.Circle((x, y), r, color='b')
    ax.add_artist(circle)


def plot_rec(x, y, a, b):
    rectangle = plt.Rectangle((x-b/2, y-a/2), a, b, color='b')
    ax.add_artist(rectangle)

def plot_start(x, y):
    circle = plt.Circle((x, y), 10, color='#98FB98')
    ax.add_artist(circle)

def plot_end(x, y):
    circle = plt.Circle((x, y), 10, color='#98FB98')
    ax.add_artist(circle)


if __name__ == "__main__":
    fig, ax = plt.subplots()
    # ax.axis("equal")
    fig.set_size_inches((5, 5))
    plt.ion()
    plt.xlim((0, mapSize))
    plt.ylim((0, mapSize))
    plt.xticks([])
    plt.yticks([])
    plt.show()

    inputStr = input()
    countLine = 0
    lineSet = []
    while True:
        if inputStr == 'line':
            countLine += 1
           # print("Ploting line number %d" % countLine)
            lineData = []
            weight = int(input())
            colorStr, width = getColor(weight)
            inputStr = input()
            while inputStr != 'endline':
                tmp = inputStr.strip().split()
                data = np.array(tmp, dtype=np.double)
                lineData.append(data)
                inputStr = input()
            plot_line(lineData, colorStr, width)
        elif inputStr == 'start':
            x, y = np.array(input().strip().split(), dtype=np.double)
            plot_start(x, y)
        elif inputStr == 'end':
            x, y = np.array(input().strip().split(), dtype=np.double)
            plot_end(x, y)
        elif inputStr == 'obstacle':
            infoObs = input().strip().split()
            x, y, kind, r, a, b = np.array(infoObs, dtype=np.double)
            if kind == 0:
                plot_rec(x, y, a, b)
            if kind == 1:
                plot_circle(x, y, r)
        elif inputStr == 'clear':
            print("Lines will be cleared in 1 second.")
            plt.pause(1)
            for line in lineSet:
                line.remove()
            lineSet.clear()
            countLine = 0
        try:
            inputStr = input()
            if inputStr == "exit":
                print("End of input! Exiting")
                break
        except EOFError:
            print("Input has terminated! Exiting")
            break

    print("The plot will be close in 10 seconds")
    #plt.savefig("data-%.8d.png" % counter)
    plt.savefig("data.png")
    plt.pause(10)

import serial
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation
portx = "COM4"
bps = 9600
timex = 5
# 串口执行到这已经打开 再用open命令会报错
ser = serial.Serial(portx, int(bps), timeout=1, parity=serial.PARITY_NONE,stopbits=1)
# 创建图形对象
fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)

# 设置图形标题和坐标轴标签
ax.set_title('Real-time Data Plot')
ax.set_xlabel('Time')
ax.set_ylabel('Volt Value')

# 设置坐标轴范围
ax.set_xlim(0, 100)  # 根据你的需求调整x轴范围
ax.set_ylim(0.80005, 1.1100)  # 根据你的数据范围调整y轴范围

# 存储接收到的数据点
data_points = []


# 更新图形的函数
def update(frame):
    global data_points  # 声明为全局变量
    # 从串口读取数据
    data = ser.readline().decode('utf-8').strip()
    print(data)
    # 转换数据为整数并添加到数据列表
    data_point = float(data)
    data_points.append(data_point)

    # 仅保留最近的100个数据点，可以根据需要调整
    data_points = data_points[-1000:]

    # 更新线的数据
    line.set_data(range(len(data_points)), data_points)

    return line,


# 创建动画
ani = FuncAnimation(fig, update,frames=100, blit=False)

plt.show()

# 关闭串口
ser.close()

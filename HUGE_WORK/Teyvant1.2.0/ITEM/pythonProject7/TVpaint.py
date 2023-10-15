import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit

# 读取数据
with open('VV.txt', 'r', encoding='utf-8') as f:
    lines = f.readlines()

# 提取电压值
voltage_values = []
for line in lines:
    if "相应电压值为:" in line:
        voltage_str = line.split("相应电压值为:")[1].strip()
        voltage_values.append(float(voltage_str))

# 数据拟合
x_data = np.arange(len(voltage_values))

def sine_func(x, A, frequency, phase, offset):
    return A * np.sin(2 * np.pi * frequency * x + phase) + offset

params, covariance = curve_fit(sine_func, x_data, voltage_values)

# 生成拟合后的数据
fit_data = sine_func(x_data, *params)

# 创建图表
plt.figure(figsize=(10, 4))
plt.plot(x_data, voltage_values, label='Voltage', color='blue')

plt.xlabel('Data Point')
plt.ylabel('Voltage')
plt.title('Voltage Data and Fitted Curve')
plt.grid(True)
plt.legend()
plt.tight_layout()
plt.savefig("out.png")
# 显示图表
plt.show()

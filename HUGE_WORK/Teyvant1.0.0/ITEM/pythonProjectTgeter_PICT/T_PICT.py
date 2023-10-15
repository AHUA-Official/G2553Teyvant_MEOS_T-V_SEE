import matplotlib.pyplot as plt
import re

# 读取文本文件
with open('your_temperature_data.txt', 'r', encoding='utf-8') as file:
    lines = file.readlines()

# 提取温度值
temperature_values = []
for line in lines:
    match = re.search(r'Temperature\(C\)：([\d.]+)', line)
    if match:
        temperature = float(match.group(1))
        temperature_values.append(temperature)

# 设置图表大小
fig, ax = plt.subplots(figsize=(len(temperature_values) / 10, 5))

# 绘制温度值
ax.plot(range(len(temperature_values)), temperature_values, marker='o')

# 设置Y轴范围和刻度
ax.set_ylim([10, 30])
ax.set_yticks([i for i in range(10, 31, 1)])  # 设置Y轴刻度
ax.set_yticklabels([f"{i}°C" for i in range(10, 31, 1)])  # 显示刻度标签

# 设置X轴标签
ax.set_xticks(range(0, len(temperature_values), 10))
ax.set_xticklabels([f"[{i}]" for i in range(0, len(temperature_values), 10)], rotation=45)

# 设置图表标题
ax.set_title('AHUA_T_PICT')

# 保存图表
plt.savefig('temperature_plot.png', dpi=500)

# 显示图表
plt.show()

import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize  import curve_fit

if __name__ == "__main__":
    with open('input.txt', 'r',encoding='utf-8') as f:
        context = f.readlines()
        # print(context)
        # print(type(context))
        result = context[2::2]
        # print(result)

        pre = []
        tem = []

        for item in result:
            if item[2] == '电':
                # print(item)
                colon_index = item.index("：")
                voltage_value = item[colon_index + 1:]
                # print(voltage_value)
                pre.append(voltage_value)
            else:
               # print(item)
               colon_index = item.index("：")
               voltage_value = item[colon_index + 1:]
               # print(voltage_value)
               tem.append(voltage_value)

        cleaned_tem = [item.strip() for item in tem]
        cleaned_pre = [item.strip() for item in pre]
        float_data1 = [float(item) for item in cleaned_tem]
        float_data2 = [float(item) for item in cleaned_pre]


        # 正弦函数作为拟合函数
        def sine_func(x, A, frequency, phase, offset):
            return A * np.sin(2 * np.pi * frequency * x + phase) + offset


        x_data1 = np.arange(len(float_data1))
        # 使用 curve_fit 进行数据拟合
        params1, covariance1 = curve_fit(sine_func, x_data1, float_data1)

        # 生成拟合后的数据
        fit_data1 = sine_func(x_data1, *params1)

        plt.figure()
        # 绘制原始数据和拟合曲线
        plt.plot(x_data1, fit_data1, 'b-', label='temperature')
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.legend()
        # plt.show()


        # 正弦函数作为拟合函数
        def sine_func2(x, A, frequency, phase, offset):
            return A * np.sin(2 * np.pi * frequency * x + phase) + offset


        x_data2 = np.arange(len(float_data2))
        # 使用 curve_fit 进行数据拟合
        params2, covariance2 = curve_fit(sine_func, x_data2, float_data2)

        # 生成拟合后的数据
        fit_data2 = sine_func2(x_data2, *params2)
        plt.figure()
        # 绘制原始数据和拟合曲线
        plt.plot(x_data2, float_data2, 'b-', label='voltage')
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.legend()
        plt.show()




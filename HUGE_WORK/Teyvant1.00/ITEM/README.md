# README

```python
#@auther  AHUA
#@create   23/9/27
#@version Teyvant 1.0.0
```



####简介

Vget     串口获得电压

pythonProjectV_GETER_PICT          使用python绘图获得电压图片     每次要把串口调试助手的DAT文件改成txt文件使用

Tget     串口获得温度

pythonProjectT_GETER_PICT               使用python绘图获得电压图片     每次要把串口调试助手的DAT文件改成txt文件使用



画图程序需要引入matplotlib库及其依赖  如果你有pycharm的话导入会很方便,实在不去用的话

```
#$   python -m pip install --upgrade pip 


#$    ip install matplotlib
```



## 升级到1.1.0的TODO   LIST

- 更换串口的配置模式,使得使用更高更快的采样和发送速度
- 把温度串口和电压串口结合起来 采用模式转换的方法来改变发送数据的类型(是温度数据还是电压数据)  模式转换初步想要采用按键控制的方式,或者串口向从机单片机板子发送指令
- 串口发送的数据被实时保存在串口调试助手的DAT文件里面,画图要手动转txt然后再拿给python程序跑,不够自动化
- python程序要做到每间隔一段时间自动读取文件更新波形图
- python画图程序要使用非线性拟合方式,让图看起来更好
- ​






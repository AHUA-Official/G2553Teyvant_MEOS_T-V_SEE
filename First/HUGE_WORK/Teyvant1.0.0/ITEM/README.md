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

- 更换串口的配置模式,使得使用更高更快的采样和发送速度(我们发现数据发送的太慢还是什么,得到是电压波形数据很尖锐,不太好看)
- 把温度串口和电压串口结合起来 采用模式转换的方法来改变发送数据的类型(是温度数据还是电压数据)  模式转换初步想要采用按键控制的方式,或者串口向从机单片机板子发送指令
- 串口发送的数据被实时保存在串口调试助手的DAT文件里面,画图要手动转txt然后再拿给python程序跑,不够自动化
- python程序要做到每间隔一段时间自动读取文件更新波形图(不过我想这不是最好的办法)
- python画图程序要使用非线性拟合方式,让图看起来更好(在画图里面自己弄一下,解决尖锐的问题)
- ​


23/10/07

画图功能的重大更新意见

不要用numpy和matplotlib来画图了.他画的很慢,实时更新延迟太高,好几秒i刷新一张图片,不好的

我们可以使用python和动画,js相关的依赖来更新画图的功能 ,这样就会不止好一倍,或者不用python ,就用js 或者javame的一些类库把,我不懂这些东西的,我之前用umpy和matplotlib是因为上个学期有一门课用这个我有路径依赖,对不起.



关于温度功能画图的必要,或者之类的,我不知道怎么说

就是温度这个东西,他不怎么有变化的,我们可以隔很久在采样一次,其实,我们可不可以这样做,就是老师,我们拿LCD1602这样的显示外设,让他显示温度,我们就可以直接看到,然后i如果温度发生了很大变化,比如超过了某种限定范围,我们再触发中断告诉电脑之类的,如果这样做是可以的话,原本不是规定用串口传温度和电压吗,现在这样做可以吗?.







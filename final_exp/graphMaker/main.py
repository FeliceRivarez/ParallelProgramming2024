import matplotlib.pyplot as plt
import seaborn
import numpy as np

# x = np.arange(5)
# x1 = [10335, 28180, 83976, 118709, 183914]#mt time
# x2 = [17502, 51964, 80679, 224398, 335687]#memcpy
# x3 = [80865, 144675, 253234, 564926, 710803]#gpu time
# # 设置所需参数
# total_width, n = 0.8, 3  # （柱状图的默认宽度值为 0.8）
# width = total_width / n
# x = x - (total_width - width) / 2  # 现在的x是每个并列柱的第一柱的中心横坐标
# # 绘制图
# plt.bar(x, x1, width=width, label='mt_time', fc='tomato')
# plt.bar(x + width, x2, width=width, label='memcpy', fc='springgreen')
# plt.bar(x + 2 * width, x3, width=width, label='gpu_time', fc='cyan')
# plt.xticks(np.arange(5), ['2000', '4000', '6000', '8000', '10000'])  # 将横坐标修改为学生姓名
# plt.xlabel('Sequence Length')
# plt.ylabel('Time Elapsed (microsecond)')
# plt.legend()  # 添加图例
# plt.show()




baseline=open("./result-CUDAsuper.txt")
simd=open("./result-CUDAopenMPemb-super.txt")
# avx=open("./avx.txt")
x_base=[]
y_base=[]
x_simd=[]
y_simd=[]
# x_avx=[]
# y_avx=[]
horizon=4
for i in baseline:
    tmp_x,tmp_y=map(int,i.split(" "))
    y_base.append(tmp_y)
    x_base.append(tmp_x)
    horizon+=5
horizon=4
for i in simd:
    tmp_x,tmp_y=map(int,i.split(" "))
    y_simd.append(tmp_y)
    x_simd.append(tmp_x)
    horizon += 5
horizon=4
# for i in avx:
#     tmp_x,tmp_y=map(int,i.split(" "))
#     y_avx.append(tmp_y)
#     x_avx.append(horizon)
#     horizon += 5
print(y_simd)
plt.plot(x_base,y_base,color='r',label='CUDA')
plt.fill_between(x_base,y_base,y_simd,color='lightcoral',label='CUDA')
plt.plot(x_simd,y_simd,color='dodgerblue',label='CUDA+Embedded+openMP')
plt.fill_between(x_simd,y_simd,y2=0,color='cyan',label='CUDA+Embedded+openMP')
# plt.plot(x_avx,y_avx,color='darkorange')
# plt.fill_between(x_avx,y_avx,y2=0.8e7,color='orange',label='AVX-Improved')
# plt.fill_between(x_simd,y_simd,y2=0.8e7,color='cyan',label='SSE-Improved')
plt.xlabel('Sequence Length')
plt.ylabel('Time Elapsed (microsecond)')
plt.legend()
plt.show()

diff=[]
for i in range(len(y_base)):
    diff.append(float(y_base[i]-y_simd[i])/y_base[i])
plt.plot(x_base,diff,color='limegreen')
plt.xlabel('Sequence Length')
plt.ylabel('Acceleration Ratio')
plt.show()
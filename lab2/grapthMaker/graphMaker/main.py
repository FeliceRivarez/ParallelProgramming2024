import matplotlib.pyplot as plt
import seaborn

baseline=open("./baseline.txt")
simd=open("./simd.txt")
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
    x_base.append(horizon)
    horizon+=5
horizon=4
for i in simd:
    tmp_x,tmp_y=map(int,i.split(" "))
    y_simd.append(tmp_y)
    x_simd.append(horizon)
    horizon += 5
horizon=4
# for i in avx:
#     tmp_x,tmp_y=map(int,i.split(" "))
#     y_avx.append(tmp_y)
#     x_avx.append(horizon)
#     horizon += 5
print(y_simd)
plt.plot(x_base,y_base,color='r',label='Non-SIMD')
plt.fill_between(x_base,y_base,y_simd,color='lightcoral',label='Non-SIMD')
plt.plot(x_simd,y_simd,color='dodgerblue',label='SSE-Improved')
plt.fill_between(x_simd,y_simd,y2=0.8e7,color='cyan',label='SSE-Improved')
# plt.plot(x_avx,y_avx,color='darkorange')
# plt.fill_between(x_avx,y_avx,y2=0.8e7,color='orange',label='AVX-Improved')
plt.fill_between(x_simd,y_simd,y2=0.8e7,color='cyan',label='SSE-Improved')
plt.xlabel('Alphabet Size')
plt.ylabel('Time Elapsed (microsecond)')
plt.legend()

# diff=[]
# for i in range(len(y_simd)):
#     diff.append(float(y_base[i]-y_simd[i])/y_base[i])
# plt.plot(x_base,diff,color='limegreen')
# plt.xlabel('Alphabet Size')
# plt.ylabel('Acceleration Ratio')
plt.show()
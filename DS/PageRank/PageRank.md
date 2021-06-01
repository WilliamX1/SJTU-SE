## PageRank
###### id: 519021910861
###### name: 徐惠东
1. 简述PageRank中阻尼系数的作用
	1. 在现实场景中，为了使PageRank更加精确，引入一个阻尼系数d，d用来表示用户到达某个页面时继续浏览的概率，而 1 - d 就是用户停止点击的概率，于是可以据此修正PageRank公式使得计算更合理精确。
	2. 同时，引入阻尼系数可以平滑处理，保证了经过多次迭代后数据将会收敛。
2. 简述用PageRank算法评估网页链接的质量和数量的优缺点
	优点：
	1. PageRank算法是一个与查询无关的静态算法，所有网页的PageRank值通过离线计算获得，能够有效减少在线查询时的计算量，极大降低了查询响应时间。
	缺点：
	1. PageRank算法的主要缺点在于旧的页面的排名往往会比新页面高。因为即使是质量很高的新页面也往往不会有很多外链，除非它是某个已经存在站点的子站点。
	2. 人们的查询具有主题特征，PageRank忽略了主题相关性，导致结果的相关性和主题性降低。
3. 阅读并尝试运行课程材料中的pagerank的c++实现（不要求提交运行结果）
```Python
# 基于Python用矩阵实现PageRank算法
import numpy as np

def PageRank() -> None:
    row1, col1, row2, col2 = 6, 6, 6, 1
    iterations = 60
    H = np.array([[0,0,0.25,0,0,1],
                    [1,0,0.25,0,0,0.2],
                    [0,0,0,0.5,0,1],
                    [0,0,0.25,0,1,0.2],
                    [0,1,0.25,0.5,0,0.5],
                    [1,0,1,0,1,0]])
    # H矩阵"行归一化"处理
    sum_x = np.sum(H, 0)
    for i in range(0, row1):
        for j in range(0, row2):
            H[i][j] = H[i][j] / sum_x[i]
    R = np.matrix("0.2;0.2;0.2;0.2;0.2;0.2")

    for i in range(0, iterations):
        res = np.matmul(H, R)
        R = res
    print(res)
    return
PageRank()
```


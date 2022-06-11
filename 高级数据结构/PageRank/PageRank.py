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

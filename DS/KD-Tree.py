# 完成findNeighbors函数(python3),使得该函数返回距离点@point最近的前@k个点,并分析其时间复杂度

# 分析时间复杂度时,基于变量n和k,其中 n = len(set)

# 提示:使用scipy库的kd-tree实现

# @point:一个len(point)维空间的点

# @set: 一个len(point)维空间中的点集

# @k: 寻找距离@point最近的@k个点

from typing import List
from numpy.lib.function_base import append
from scipy import spatial

def findNeighbors(point: List[int], set: List[List[int]], k: int) -> List[List[int]]:
    tree = spatial.KDTree(set) # 构造KD-Tree
    [dis, idx] = tree.query(point, k, 0, 2) # dis表示距离，idx表示对应下标
    return idx

def main():
    print(findNeighbors([1,3,2],[[1,-2,-2],[1,-3,-3],[1,2,4],[1,4,3],[2,1,4]], 3))

main()

# 输出应当是[[1, 4, 3], [1, 2, 4], [2, 1, 4]] 这个列表中的内容,不要求顺序一样.请运行比对并提供截图
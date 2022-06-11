
struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};


void getArr(TreeNode* root, TreeNode** arr, int &idx){
    if (root == nullptr) return;

    getArr(root->left, arr, idx);
    arr[++idx] = root;
    getArr(root->right, arr, idx);
}
TreeNode* convertToBST(TreeNode **arr, int start, int end){
  if (start > end) return nullptr;

  int mid = (start + end) / 2;
  TreeNode* root = arr[mid];
  root->left = convertToBST(arr, start, mid - 1);
  root->right = convertToBST(arr, mid + 1, end);

  return root;
}
TreeNode* balanceBST(TreeNode* root) {

    const int N = 10000000;
    TreeNode* arr[N];
    int idx = -1;

    getArr(root, arr, idx);
    /*创建头结点*/
    TreeNode* new_root = convertToBST(arr, 0, idx);

    return new_root;
}
int main()
{
  TreeNode* root = new TreeNode(0);
  TreeNode* tmp = root;
  for (int i = 1; i < 10; i++){
      tmp = tmp->right = new TreeNode(i);
  }
  tmp = root;
  for (int i = -1; i > -10; i--){
      tmp = tmp->left = new TreeNode(i);
  }

  TreeNode* new_root = balanceBST(root);

  return 0;
}
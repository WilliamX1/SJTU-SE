
  struct ListNode {
      int val;
      ListNode *next;
      ListNode() : val(0), next(nullptr) {}
      ListNode(int x) : val(x), next(nullptr) {}
      ListNode(int x, ListNode *next) : val(x), next(next) {}
  };

  struct TreeNode {
      int val;
      TreeNode *left;
      TreeNode *right;
      TreeNode() : val(0), left(nullptr), right(nullptr) {}
      TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
      TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
  };

TreeNode* convertToBST(int *arr, int start, int end){
  if (start > end) return nullptr;

  int mid = (start + end) / 2;
  TreeNode* root = new TreeNode(arr[mid]);
  root->left = convertToBST(arr, start, mid - 1);
  root->right = convertToBST(arr, mid + 1, end);

  return root;
}
TreeNode* sortedListToBST(ListNode* head) {
  /*获得长度*/
  int len = 0;
  ListNode* tmp = head;
  while (tmp != nullptr) {
    len++;
    tmp = tmp->next;
  }
  /*转化成数组*/
  int* arr = new int[len];
  tmp = head;
  for (int i = 0; i < len; i++) {
    arr[i] = tmp->val;
    tmp = tmp->next;
  }

  /*创建头结点*/
  TreeNode* root = convertToBST(arr, 0, len - 1);

  return root;
}
int main()
{
  ListNode* head = new ListNode(0);
  ListNode* tmp = head;
  for (int i = 1; i < 10; i++){
    tmp = tmp->next = new ListNode(i);
  };

  TreeNode* root = sortedListToBST(head);

  return 0;
}
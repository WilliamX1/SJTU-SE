#include <iostream>
#include <stdio.h>
#include <cstring>
#include <vector>
#include <algorithm>

#include "lexicon.h"

using namespace std;

void init();
void showState();
bool getWord();
void AIinit();
void autoFind(int x, int y, string str, int len);
void deleteRepeat();
void AI();
string changeTo(const string& str, int len, int ch);

struct node
{
    string str;
    int len;
    bool in;
    node(string s, int l, bool i = true):str(s), len(l), in(i){};
    node();
    static bool compare(node a, node b){
        return a.str < b.str;
    }
    bool operator==(const node &b){
        return str == b.str;
    }
};

char A[1000][1000];
bool T[1000][1000];
int N, YourScore = 0, AIScore = 0;
Lexicon lexicon("EnglishWords.txt");
vector<node> V;
vector<string> YourWords;

void init()
{
    char tmp = '\n'; scanf("%d%c", &N, &tmp);
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
            scanf("%c", &A[i][j]);
        scanf("%c", &tmp);
    }
    AIinit();
    return;
}
void showState()
{
    for (int i = 1; i <= N; i++)
    {
        for (int j = 1; j <= N; j++)
            printf("%c", A[i][j]);
        printf("\n");
    }

    printf("Your Score: %d\n", YourScore);
    printf("Your Words: ");
    vector<string>::iterator iter = YourWords.begin();
    while(iter != YourWords.end())
    {
        cout << *iter << " ";
        iter++;
    }
    printf("\n");
    return;
}
bool getWord()
{
    string str; cin >> str;
    if (str == "???") {
        AI();
        return false;
    }

    vector<node>::iterator iter = find(V.begin(), V.end(), node(changeTo(str, str.length(), 1), str.length()));
    if (str.length() < 4)
        cout << str << " is too short.\n";
    else if (!lexicon.contains(str))
        cout << str << " is not a word.\n";
    else if (iter == V.end())
        cout << str << " is not on board.\n";
    else if (!iter->in)
        cout << str << " is already found.\n";
    else {
        YourWords.push_back(changeTo(iter->str, iter->len, 0));
        YourScore += iter->len - 3;
        AIScore -= iter->len - 3;
        iter->in = false;
    };

    return true;
}
void AIinit()
{
    memset(T, 0, sizeof (T));
    for (int i = 1; i <= N; i++)
        for (int j = 1; j <= N; j++)
            autoFind(i, j, "", 0);
    deleteRepeat();

    return;
}
void autoFind(int x, int y, string str, int len)
{
    str = str + A[x][y], len++;
    T[x][y] = true;

    if (len >= 4 && lexicon.contains(str))
    {
        V.push_back(node(str, len));
        AIScore += len - 3;
    }

    if (!lexicon.containsPrefix(str))
    {
        T[x][y] = false;
        return;
    }
    
    if (x > 1 && !T[x - 1][y]) autoFind(x - 1, y, str, len);
    if (x < N && !T[x + 1][y]) autoFind(x + 1, y, str, len);
    if (y > 1 && !T[x][y - 1]) autoFind(x, y - 1, str, len);
    if (y < N && !T[x][y + 1]) autoFind(x, y + 1, str, len);
    if (x > 1 && y > 1 && !T[x - 1][y - 1]) autoFind(x - 1, y - 1, str, len);
    if (x > 1 && y < N && !T[x - 1][y + 1]) autoFind(x - 1, y + 1, str, len);
    if (x < N && y > 1 && !T[x + 1][y - 1]) autoFind(x + 1, y - 1, str, len);
    if (x < N && y < N && !T[x + 1][y + 1]) autoFind(x + 1, y + 1, str, len);

    T[x][y] = false;
    return;
}
void deleteRepeat()
{
    sort(V.begin(), V.end(), node::compare);

    //for (int i = 1; i <= V.size(); i++)
         //cout << V[i].str << " ";

    vector<node>::iterator iter_cur = V.begin();
    vector<node>::iterator iter_last = V.begin();
    while(iter_cur != V.end())
    {
        iter_cur++;
        if (iter_last->str == iter_cur->str)
        {
            AIScore -= iter_last->len - 3;
            V.erase(iter_last);
            iter_cur--;
        }
        iter_last = iter_cur;
    }

    return;
}
void AI()
{
    printf("Computer Score: %d\n", AIScore);
    printf("Computer Words: ");

    vector<node>::iterator iter = V.begin();
    while(iter != V.end()){
        if (iter->in) 
            cout << iter->str << " ";
        iter++;
    }
    return;
}
string changeTo(const string& str, int len, int ch)  // ch = 0 转换成小写， ch = 1 转化成大写
{
    string res = str;
    if (ch)
    {
        while(--len >= 0){
            if ('a' <= str[len] && str[len] <= 'z')
                res[len] = char(str[len] - 'a' + 'A');
            else res[len] = str[len];
        }
    } else {
        while(--len >= 0){
            if ('A' <= str[len] && str[len] <= 'Z')
                res[len] = char(str[len] - 'A' + 'a');
            else res[len] = str[len];
        }
    }
    return res;
}

int main()
{
    init();

    //cout << endl;
    //for (int i = 1; i <= V.size(); i++)
         //cout << V[i].str << " ";

    do {
        showState();
    }while(getWord());

    return 0;
}

#include "common.h"

bool lab1(int states[], int n, int button[][2], int m, bool answer[]);

//init states of stones
static int states[1000];
//which button/switch controls which stones
static int button[1000][2];
//SAT answer
static bool answer[1000];
//the number of buttons/switches
static int m;
//the bumber of stones
static int n;
//the number of cases
static int iter;

void input_data()
{
    int i;
    memset(states, 0, sizeof(states));
    memset(button, 0, sizeof(button));
    memset(answer, 0, sizeof(answer));

    scanf("%d %d", &m, &n);


    for(i = 0; i < n; ++i)
    {
        scanf("%d", states + i);
    }

    for(i = 0; i<m; ++i)
    {
        int num;
        scanf("%d", &num);
        if(num == 1)
        {
            scanf("%d", &(button[i][0]) );
        }
        else
        {
            scanf("%d %d", &(button[i][0]), &(button[i][1]) );
        }
    }
    return;
}



int main()
{
    // open test file
    FILE* input_fh = freopen("test.txt", "r", stdin);
    if(input_fh == NULL)
    {
        printf("Open test file error!\n");
        exit(-1);
    }
    //create answer file
    FILE* output_fh = fopen("answer.txt", "w");
    if(output_fh == NULL)
    {
        printf("Write answer error!\n");
        exit(-1);
    }

    //input number of test cases
    scanf("%d", &iter);

    int i;
    //address every cases
    for(i=0; i<iter; ++i)
    {
        //init input data
        input_data();

        //use SAT solver to get answer
        bool sat = lab1(states, n, button, m, answer);

        if(sat)
        {
            //if the problem is satisfiable, output the answer
            int j;
            for(j = 0; j<m; ++j)
                fprintf(output_fh, "%d ", answer[j]);
        }
        else
        {
            //if the problem is unsatisfiable, output UNSAT
            fprintf(output_fh, "UNSAT");
        }
        fprintf(output_fh, "\n");
    }

    //close input and output
    fclose(stdin);
    fclose(output_fh);

    return 0;
}




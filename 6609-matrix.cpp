#include <iostream>
using namespace std;
#include <fstream>
#include <pthread.h>
#include <ctime>
int r1,c1,r2,c2;
int** c;
typedef struct
{
    int rn,cn;
    int** a;
    int** b;

} nums;
void* element_multiplication(void* arg)
{
    nums *output;
    output = (nums *) arg;
    int rn= output->rn, cn = output->cn;
    int **a= output->a, **b= output->b;

    c[rn][cn]=0;
    for (int k = 0; k < c1 ; ++k)
    {
        c[rn][cn]+= a[rn][k]*b[k][cn];
    }
    pthread_exit(0);
}
void* row_multiplication(void * arg)
{
    nums *output;
    output = (nums*) arg;
    int rn= output->rn,cn= output->cn;
    int **a= output->a, **b= output->b;

    for (cn = 0; cn < c2; ++cn) {
        c[rn][cn]=0;
        for (int k = 0; k < c1 ; ++k) // or c1?
        {
            c[rn][cn] += a[rn][k] * b[k][cn];
        }
    }


    pthread_exit(0);
}

void ThreadR(int**a, int**b, int**c)
{   //cout << "Number of row threads"<< r1  <<endl;
    pthread_t R_thread[r1];
    // nums* obj = new nums[r1];
    for (int i = 0; i < r1; ++i)
    { nums* obj= (nums*)malloc(sizeof (nums));

        obj->a =a, obj->b=b,obj->rn=i;
        //obj->cn=j;
        pthread_create(&R_thread[i],NULL,row_multiplication,(void*) obj);
        //  cout << "Number of row threads"<< r1  <<endl;

    }
    for (int i = 0; i < r1; i++) {
        pthread_join(R_thread[i], NULL);
        //   cout << "Number of row threads"<< r1  <<endl;
    }

}
void ThreadE(int**a, int**b, int**c)
{  //cout << "Number of element threads"<< r1*c2  <<endl;
    pthread_t threadE[r1*c2];
    //cout << "Number of element threads"<< r1*c2  <<endl;
    for(int i=0; i<r1; i++)
    {
        for (int j = 0; j < c2; ++j)
        {
            nums* obj ;
            obj= (nums*)malloc(sizeof (nums));
            obj->a =a, obj->b=b,obj->rn=i,obj->cn=j;
            pthread_create(&threadE[i*c2+j],NULL,element_multiplication,(void*) obj);
            //cout << "Number of element threads"<< r1*c2  <<endl;
        }


    }for (int i = 0; i < r1; i++){
        for(int j=0;j<c2;j++){
            pthread_join(threadE[i*c2+j], NULL);
            // cout << "Number of element threads"<< r1*c2  <<endl;
        }
    }

}

int main(int argc,char ** argv)
{ if(argc <2) return -1;
    ifstream file(argv[1]);
    file >> r1 >> c1;

    int **a = (int **) malloc(r1 * sizeof (int*));
    for (int i = 0; i < r1; ++i)
    {
        a[i] = (int *) malloc(c1 * sizeof (int));
        for (int j = 0; j < c1; ++j)
        {
            file >> a[i][j];
            //cout << a[i][j]<< " ";
        } //cout<<"Hiii\n";

    }
    file >> r2 >> c2;
    int **b = (int **) malloc(r2 * sizeof (int*));
    for (int i = 0; i < r2; ++i)
    {
        b[i] = (int *) malloc(c2 * sizeof (int));
        for (int j = 0; j < c2; ++j) {
            file >> b[i][j];
            //  cout << b[i][j] << " ";
        } //cout<<endl;
    }
    if (r2 != c1)
        cout << "can't multiply";
    else
    {
        c = (int **) malloc(r1 * sizeof (int*));
        for (int i = 0; i < r1; ++i)
            c[i] = (int *) malloc(c2 * sizeof (int));

        ofstream outfile("outfile.txt");
        clock_t startR, endR, startE, endE;
        startE = clock();
        ThreadE(a, b, c);
        endE = clock();
        outfile << "Element Multiplication Time: " << (endE - startE) / (double)CLOCKS_PER_SEC << "\n";
        for (int i = 0; i < r1; ++i)
        {
            for (int j = 0; j < c2; j++)
                outfile << c[i][j] << " ";
            outfile << endl;
        }

        startR = clock();
        ThreadR(a, b, c);
        endR = clock();
        outfile << "Row Multiplication Time: " << (endR - startR) / (double)CLOCKS_PER_SEC << "\n";
        for (int i = 0; i < r1; ++i)
        {
            for (int j = 0; j < c2; j++)
                outfile << c[i][j] << " ";
            outfile << endl;
        }

        free(a);
        free(b);
        free(c);



    }
    return 0;
}
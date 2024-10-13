
#include <string.h>
// #include <ostream.h>
#include <stdio.h>
#include <iostream>

// using namespace std;
using std::string;
using namespace std;



void task1(){
    /*
    
    Increment a pointer
    
    */
    int n = 0;
    int* n2 = &n;
    printf("n is: %d\n", n);
    ++*n2;
    printf("n is: %d\n", n);

    for (int i = 0; i < 5; i++)
    {
        ++*n2;
        printf("n is: %d\n", n);
    }
    std::string stuff(2, '.');
    cout << stuff << endl;

    // task 2
    int x[]= {10, 30, 2000};
    int *y;
    y = x;

    printf("\nList[%d] == %d", *y, *y);
    ++y;
    printf("\nList[%d] == %d", *y, *y);
    ++y;
    printf("\nList[%d] == %d", *y, *y);
    printf("\n");
}
bool compare(int *pointer1, int *pointer2, int length){
    // if null return false
    if (pointer1 == nullptr || pointer2 == nullptr)
    {
        return false;
    }

    for (id_t i = 0; i < length; i++)
    {
        printf("Pointer 1:2 = %d:%d\n", *pointer1, *pointer2);
        if (*pointer1 != *pointer2)
        {
            return false;
        }
        
        
        *pointer1++;
        *pointer2++;
    }
    return true;
    
}

void readfile(){ // readfile task 1.4
    string test;
    ifstream file("test.txt");
    while (getline (file, test  )){
        cout << test << endl;
    }
    // printf("Hello");
}


// task 1.5
void swap2(void *x, void *y){
    // Swap x and y
    int temp = *(int*)x;
    *(int*)x = *(int*)y;
    *(int*)y = temp;
}

// task 1.6
void print_array(int *arr, int width, int height){
    for (int i = 0; i < width; i++)
    {
        for (int x = 0; x < height; x++)
        {
            printf("%d ", arr[i * height + x]);
        }
        printf("\n");
    }
}

int main(void) {
    // printf("Hello World\n");
    task1();

    int x [3] = {0,1,2};
    int y [3] = {1,1,3};
    bool test = compare(x, y, 3);
    cout << test << endl;

    readfile();
    printf("\nBefore Swap: %d, %d\n",*x, *y);
    swap2(x,y);
    printf("\nSwapped: %d, %d\n",*x, *y);

    int array [3][3] = {{0, 1, 2},{2,4,8}};
    print_array((int*)array, 2, 3);
    return 0;
}
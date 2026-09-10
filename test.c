
#include <stdio.h>


void printvec(unsigned long long state[25]){
    int i=3, j=3;
    for(int count =0 ; count < 5; count++){
        for(int count2 =0 ; count2 < 5; count2++){
            printf("%016llX,\t ",state[i*5+j]);
            j = (j+1)%5;
        }
        printf("\n");
        i = (i+1)%5;
    }
}


unsigned long long test[25] = {
    0x5DB0B09ADC42C701, 0xEA8BE351A3D425F9, 0x24E2943578ADA77C, 0x06F3838519B8D587, 0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x8000000000000000,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00
};



int main(){
    printvec(test);

    return 0;
}
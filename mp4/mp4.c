//this is code is based on the greedy algorithm, and made some improvements
//in each loop we will find the best value at this moment and save it, then compare the current best value with
//the next value 
#include <stdio.h>
#include <stdint.h>
#include "mp4.h"
int32_t print_stamps (int32_t amount, int32_t s1, int32_t s2, int32_t s3, int32_t s4) {
    // initialize the variables for the number of stamps and the total value
    int32_t n1 = 0, n2 = 0, n3 = 0, n4 = 0, value = 0, number=0,diff=0,i=0;
    // initialize the variables for the best combination so far
    int32_t best_n1 = 0, best_n2 = 0, best_n3 = 0, best_n4 = 0;
    // initialize a flag for exact match
    int32_t best_diff=1e7,best_number=1e7;
    for (n1 = 0; n1  <= ((amount+s1-2)/s1); n1++) {
        for (n2 = 0; n2  <= (amount - n1 * s1+s2-1)/s2; n2++) {                             //round up to an integer for each n
            for (n3 = 0; n3  <= (amount - n1 * s1 - n2 * s2+s3-1)/s3; n3++) {
                for(n4=0;n4 <=(amount - n1 * s1 - n2 * s2 - n3 * s3+s4-1)/s4; n4++){
                    value=n1*s1+n2*s2+n3*s3+n4*s4;
                    diff=value-amount;
                    number=n1+n2+n3+n4; 
                    if(diff>=0){
                                                                //this is for comparing the number and value
                        if(diff<best_diff){                     //try to find the most optimized solutions
                            best_n1=n1;                         
                            best_n2=n2;
                            best_n3=n3;
                            best_n4=n4;
                            best_number=number;
                            best_diff=diff;
                        }
                        if(diff==best_diff){
                            if(number<=best_number){
                                best_n1=n1;
                                best_n2=n2;
                                best_n3=n3;
                                best_n4=n4;     
                                best_number=number;
                                best_diff=diff;    
                                break;                          //back to the loop
                                }
                            }
                        }
                    }
                }
            }
        }

        // print the best combination and the total value

    printf("%d %d %d %d -> %d\n", best_n1, best_n2, best_n3, best_n4, best_n1*s1+best_n2*s2+best_n3*s3+best_n4*s4);
    // return the exact match flag
    i=(best_diff==0?1:0);
    return i;
}

                            

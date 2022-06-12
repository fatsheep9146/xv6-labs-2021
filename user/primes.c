#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void findprime(int prime, int infd){
    int n;
    int next = 0;
    int p[2];
    pipe(p);
    printf("prime %d\n", prime);
    while(read(infd, &n, sizeof(int)) == sizeof(int)) {
        if (n % prime != 0){
            // printf("prime %d got %d\n", prime, n);
            if (next == 0) {
                next = n;
            }
            write(p[1], &n, sizeof(int));         
        }
    }

    if (next != 0) {
        if (fork() == 0) {
            close(p[1]); // this should be considered again
            // If no data is available, 
            // a "read" on a pipe waits for either data to be written or for 
            // all file descriptors referring to the "write" end to be closed; in the latter case, read will return 0, just as if the end of a data file had been reached.
            findprime(next, p[0]);
        } else {
            close(p[0]);
            close(p[1]);
            wait(0);
        }
    }
    
    close(infd);
}


// version1, 结果不符合预期，输出
// prime 2
// prime 3
// prime 17 / 15 / 23 / 25
// 怀疑是因为 write(p[1]) 时，p[1] 空间理论无限大，所以可以一直写成功
// void findprime(int prime, int infd){
//     int n;
//     int neighbor = 0;
//     int p[2];
//     pipe(p);
//     close(p[0]);
//     // printf("prime %d\n", prime);
//     while(read(infd, &n, sizeof(int)) == sizeof(int)) {
//         if (n % prime != 0){
//             printf("%d\n", n);
//             write(p[1], &n, sizeof(int));         
//             if (neighbor == 0) {
//                 if (fork() == 0) {
//                     close(p[1]);
//                     findprime(n, p[0]);
//                 } else {
//                     neighbor = 1;
//                 }
//             } 
//         }
//     }
//     close(infd);
//     // close(p[1]);
//     wait(0);
// }

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    // printf("version 202206122216\n");

    if(fork() == 0) {
        close(p[1]);
        findprime(2, p[0]);
    } else {
        // wait for sub process
        close(p[0]);
        for (int i = 2; i <= 35; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        wait(0);
        // printf("subprocess exits");
    }
    exit(0);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <string.h>

// Tailor-ln converging in (0, 2]
double calc_ln(double x) {
    double y = x - 1.0;
    double sum = 0.0;
    double term = y;
    int k = 1;

    while (fabs(term) > 1e-6) {
        sum += term;
        k++;

        term = -term * y * (k - 1) / k;
    }
    return sum;
}

int main() {
    const char* filename = "temp_data.txt";

    // file creation or cleaning
    FILE *init_f = fopen(filename, "w");
    if(!init_f) {
        printf("[Parent PID: %d] open %s error.\n", getpid(), filename);
        exit(1);
    } else {
        fclose(init_f);
    }

    pid_t pid = fork();
    if (pid < 0) {
        printf("[Parent PID: %d] fork error", getpid());
        exit(1);
    } else if (pid == 0) {
        printf("[Child PID: %d]: Reading file %s...\n", getpid(), filename);

        FILE *f = fopen(filename, "r");
        if(!f) {
            printf("[Child PID: %d] open %s error.\n", getpid(), filename);
            exit(1);
        }

        char buffer[256];
        while (1) {
            if (fgets(buffer, sizeof(buffer), f) != NULL) {
                if (strncmp(buffer, "STOP", 4) == 0) {
                    printf("[Child PID: %d] STOP signal recieved.\n", getpid());
                    break;
                }
                printf("[Child PID: %d] Read: %s", getpid(), buffer);
            } else {
                clearerr(f);
            }
        }
        fclose(f);
        exit(0);
    } else {
        srand(time(NULL));
        int n = rand() % 20 + 6;

        printf("[Parent PID: %d] %d computations on the way.\n", getpid(), n);

        FILE *f = fopen(filename, "a");
        if (!f) {
            printf("[Parent PID: %d] open %s error.\n", getpid(), filename);
            exit(1);
        }

        for (int i = 0; i < n; i++) {
            double x = ((double)rand() / RAND_MAX) * 1.8 + 0.1;

            fprintf(f, "x = %.4f, ln(x) = %.6f\n", x, calc_ln(x));
            fflush(f);
        }

        fprintf(f, "STOP\n");
        fflush(f);
        fclose(f);

        int status;
        wait(&status);
    }

    return 0;
}
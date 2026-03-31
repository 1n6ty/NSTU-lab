#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

struct Message {
    int process_id;
    char text[40];
};

void sig_handler(int sign) {
    printf("[P1 PID: %d] Signal recieved.\n", getpid());
}

int main() {
    int K1[2];
    pid_t pid1, pid2;

    printf("[P0 PID: %d] Channel creating...\n", getpid());
    if (pipe(K1) == -1) {
        printf("[P0 PID: %d] pipe error.\n", getpid());
        exit(EXIT_FAILURE);
    }

    printf("[P0 PID: %d] Creating P1...\n", getpid());
    pid1 = fork();

    if (pid1 < 0) {
        printf("[P0 PID: %d] fork for P1 error.\n", getpid());
        exit(EXIT_FAILURE);
    } 
    else if (pid1 == 0) {
        close(K1[0]);

        signal(SIGUSR1, sig_handler);
        
        printf("[P1 PID: %d] Start waiting for signal...\n", getpid());
        pause();

        printf("[P1 PID: %d] Preparing data...\n", getpid());
        
        struct Message msg1;
        msg1.process_id = 1;
        strncpy(msg1.text, "Message from P1\0", 17);
        
        // Запись в канал
        write(K1[1], &msg1, sizeof(msg1));
        printf("[P1 PID: %d] Message sent. Exiting...\n", getpid());
        
        close(K1[1]);
        exit(EXIT_SUCCESS);
    }

    printf("[P0 PID: %d] Creating P2...\n", getpid());
    pid2 = fork();

    if (pid2 < 0) {
        printf("[P0 PID: %d] fork for P2 error.\n", getpid());
        exit(EXIT_FAILURE);
    } 
    else if (pid2 == 0) {
        close(K1[0]);
        
        printf("[P2 PID: %d] Preparing data...\n", getpid());
        
        struct Message msg2;
        msg2.process_id = 2;
        strncpy(msg2.text, "Message from P2\0", 17);
        
        write(K1[1], &msg2, sizeof(msg2));
        printf("[P2 PID: %d] Message sent. Sending signal to P1...\n", getpid());
        
        kill(pid1, SIGUSR1);
        
        printf("[P2 PID: %d] Exiting...\n", getpid());
        close(K1[1]);
        exit(EXIT_SUCCESS);
    }

    close(K1[1]);
    
    printf("[P0 PID: %d] Waiting for data...\n", getpid());
    
    struct Message received_msg;
    
    while (read(K1[0], &received_msg, sizeof(received_msg)) > 0) {
        printf("[P0 PID: %d] Message from P%d: '%s'.\n", getpid(), received_msg.process_id, received_msg.text);
    }
    
    close(K1[0]);
    
    printf("[P0 PID: %d] Waiting for child processes to finish.\n", getpid());
    wait(NULL);
    wait(NULL);
    
    printf("[P0 PID: %d] Exiting...\n", getpid());
    return EXIT_SUCCESS;
}
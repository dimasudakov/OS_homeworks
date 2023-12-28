#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>
#include <random>

using namespace std;

int exprCount = 0;

void handleSigusr1(int signalNumber) {
    if (signalNumber == SIGUSR1) {
        cout << "Produced: " << exprCount << endl;
    }
}

void printProduced() {
    random_device rd; 
    mt19937 gen(rd()); 
    uniform_int_distribution<> distr(3000, 7000); 

    while (true) {
        int waitTime = distr(gen); 
        this_thread::sleep_for(chrono::milliseconds(waitTime));
        cout << "Produced " << exprCount << endl;
    }
}

int main() {
    srand(time(nullptr));
    thread myThread(printProduced);

    int pipeProducer[2], pipeControllerToBc[2], pipeBcToController[2];

    if (pipe(pipeProducer) == -1 || pipe(pipeControllerToBc) == -1 || pipe(pipeBcToController) == -1) {
        cerr << "Pipe creation failed." << endl;
        return 1;
    }

    signal(SIGUSR1, handleSigusr1);

    pid_t pidProducer = fork();
    if (pidProducer == 0) {
        close(pipeProducer[0]);
        dup2(pipeProducer[1], STDOUT_FILENO);
        close(pipeProducer[1]);

        execl("../producer/producer", "producer", (char*)nullptr);
        exit(1);
    }
    pid_t pidBc = fork();
    if (pidBc == 0) {
        close(pipeControllerToBc[1]);
        dup2(pipeControllerToBc[0], STDIN_FILENO);
        close(pipeControllerToBc[0]);

        close(pipeBcToController[0]);
        dup2(pipeBcToController[1], STDOUT_FILENO);
        close(pipeBcToController[1]);

        execl("/usr/bin/bc", "bc", (char*)nullptr);
        exit(1);
    }

    cout << "Controller started with PID: " << getpid() << endl;
    close(pipeProducer[1]);

    char buffer[256];
    ssize_t bytesRead;

    while ((bytesRead = read(pipeProducer[0], buffer, sizeof(buffer))) > 0) {
        pid_t endedChild;
        int childStatus;

        buffer[bytesRead] = '\0';
        write(pipeControllerToBc[1], buffer, bytesRead);

        char resultBuffer[256];
        ssize_t resultBytesRead = read(pipeBcToController[0], resultBuffer, sizeof(resultBuffer));
        resultBuffer[resultBytesRead] = '\0';

        cout << buffer << "result: " << resultBuffer;
        exprCount++;

        while ((endedChild = waitpid(-1, &childStatus, WNOHANG)) > 0) {
            if (endedChild == pidProducer) {
                break;
            }
        }

        if (endedChild == pidProducer) {
            break;
        }
    }

    close(pipeControllerToBc[1]);
    close(pipeBcToController[0]);
    waitpid(pidProducer, nullptr, 0);

    return 0;
}

import os
import random
import sys
import time

def run_child(child_pid):
    print(f"Parent[{os.getpid()}]: I ran children process with PID {child_pid}.")

def wait_for_children():
    while True:
        try:
            child_pid, status = os.wait()
            if os.WIFEXITED(status):
                exit_status = os.WEXITSTATUS(status)
                print(f"Parent[{os.getpid()}]: Child with PID {child_pid} terminated. Exit Status {exit_status}.")
                if exit_status != 0:
                    create_child()
            elif os.WIFSIGNALED(status):
                print(f"Parent[{os.getpid()}]: Child with PID {child_pid} terminated by signal.")
                create_child()
        except OSError:
            break

def create_child():
    pid = os.fork()
    if pid == 0:  # Child process
        random_sleep = random.randint(5, 10)
        os.execlp('python3', 'python3', 'child.py', str(random_sleep))
    elif pid > 0:  # Parent process
        run_child(pid)
    else:
        sys.exit("Fork failed.")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("Usage: python3 parent.py N (N is the number of child processes)")

    try:
        N = int(sys.argv[1])
        for _ in range(N):
            create_child()

        wait_for_children()

    except ValueError:
        sys.exit("Invalid number of child processes.")

import os
import random
import sys
import time

def child_process():
    pid = os.getpid()
    parent_pid = os.getppid()
    print(f"Child[{pid}]: I am started. My PID {pid}. Parent PID {parent_pid}.")

    if len(sys.argv) != 2:
        sys.exit("Usage: python3 child.py S (S is the sleep duration)")

    try:
        sleep_duration = int(sys.argv[1])
        time.sleep(sleep_duration)

        exit_status = random.randint(0, 1)
        print(f"Child[{pid}]: I am ended. PID {pid}. Parent PID {parent_pid}.")
        sys.exit(exit_status)
    except ValueError:
        sys.exit("Invalid sleep duration.")

if __name__ == "__main__":
    child_process()

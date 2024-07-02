import cv2
import sys
import time
import functools
import threading

sys.path.append("C:/Users/gzyit/CLionProjects/pybind11/cpp/camera2/cmake-build-release")
import camera2


def log_execution_time(func):
    @functools.wraps(func)
    def wrapper(*args, **kwargs):
        start = time.perf_counter()
        res = func(*args, **kwargs)
        end = time.perf_counter()
        print('{} took {} ms'.format(func.__name__, (end - start) * 1000))
        return res

    return wrapper

# @log_execution_time
def sleep_10ms():
    time.sleep(0.01)



def thread_function(name):
    """线程函数示例，打印线程名和当前时间"""
    print(f"Thread {name}: starting")
    while True:
        sleep_10ms()


if __name__ == "__main__":
    # camera2.start()

    t1 = threading.Thread(target=thread_function, args=(1,))
    t1.start()

    t2 = threading.Thread(target=camera2.start)
    t2.start()

    t1.join()
    t2.join()


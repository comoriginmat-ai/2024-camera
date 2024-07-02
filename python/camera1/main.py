import cv2
import sys

sys.path.append("C:/Users/gzyit/CLionProjects/pybind11/cmake-build-release")
import camera

import time
import functools


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
def show_image(img):
    camera.display_image(img)


def init_camera():
    global cap
    cap = cv2.VideoCapture(0)
    cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter.fourcc('M', 'J', 'P', 'G'))

    cap.set(cv2.CAP_PROP_FRAME_WIDTH, 1920)
    cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 1080)
    cap.set(cv2.CAP_PROP_FPS, 30.0)

    fps = cap.get(cv2.CAP_PROP_FPS)
    size = (int(cap.get(cv2.CAP_PROP_FRAME_WIDTH)), int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT)))
    print(fps)
    print(size)

    # 获取FourCC
    fourcc = int(cap.get(cv2.CAP_PROP_FOURCC))
    # 将FourCC转换为对应的字符
    fourcc_char = chr((fourcc >> 0) & 0xFF) + chr((fourcc >> 8) & 0xFF) + chr((fourcc >> 16) & 0xFF) + chr(
        (fourcc >> 24) & 0xFF)
    print(f"Current FourCC: {fourcc_char}")


if __name__ == "__main__":
    cap = None
    init_camera()
    while (True):
        # start = time.perf_counter()
        ret, frame = cap.read()
        show_image(frame)
        # end = time.perf_counter()
        # print('took {} ms'.format((end - start) * 1000))


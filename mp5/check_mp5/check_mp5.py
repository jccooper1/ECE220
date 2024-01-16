import ctypes
import sys
import subprocess
import os
import random
import json

# check if os is linux
def check_os():
    if sys.platform != "linux":
        return False
    else:
        return True

# check if file exists
def check_file(file):
    if not os.path.exists(file):
        return False
    return True

# check if command exists
def check_command(command):
    if subprocess.getoutput(f"which {command}") == "":
        return False
    else:
        return True

# ERROR: print with red
def ERROR(msg, br=True):
    print("\033[0;31m{}\033[0m".format(msg), end="")
    if br:
        print()

# INFO: print with green
def INFO(msg, br=True):
    print("\033[0;32m{}\033[0m".format(msg), end="")
    if br:
        print()

def randomPoint():
    x = random.randint(0, 624)
    y = random.randint(0, 320)
    return x, y

def gen_near_horizontal(so, x1, y1, x2, y2, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.near_horizontal(x1, y1, x2, y2)
    so.make_png(path.encode())
    so.free_image_data()

def gen_near_vertical(so, x1, y1, x2, y2, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.near_vertical(x1, y1, x2, y2)
    so.make_png(path.encode())
    so.free_image_data()

def gen_draw_line(so, x1, y1, x2, y2, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.draw_line(x1, y1, x2, y2)
    so.make_png(path.encode())
    so.free_image_data()

def gen_draw_triangle(so, x1, y1, x2, y2, x3, y3, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.draw_triangle(x1, y1, x2, y2, x3, y3)
    so.make_png(path.encode())
    so.free_image_data()

def gen_draw_parallelogram(so, x1, y1, x2, y2, x3, y3, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.draw_parallelogram(x1, y1, x2, y2, x3, y3)
    so.make_png(path.encode())
    so.free_image_data()

def gen_draw_rect(so, x, y, w, h, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.draw_rect(x, y, w, h)
    so.make_png(path.encode())
    so.free_image_data()

def gen_draw_circle(so, x, y, inner_r, outer_r, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.draw_circle(x, y, inner_r, outer_r)
    so.make_png(path.encode())
    so.free_image_data()

def gen_rect_gradient(so, x, y, w, h, c1, c2, path: str):
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    so.rect_gradient(x, y, w, h, c1, c2)
    so.make_png(path.encode())
    so.free_image_data()

if __name__ == "__main__":
    if not check_os():
        ERROR("Only support Linux")
        exit(1)

    if os.path.exists("check_mp5"):
        os.chdir("check_mp5")

    if not os.path.exists("ans"):
        os.mkdir("ans")

    CC = None
    if check_command("gcc"):
        CC = "gcc"
    elif check_command("clang"):
        CC = "clang"
    else:
        ERROR("No C compiler found")
        exit(1)
    # build shared library
    os.system(f"{CC} ../mp5.c check_mp5.c -lpng -fPIC -shared -o yourmp5.so")
    
    zhyso = ctypes.cdll.LoadLibrary("./zhymp5.so")
    yourso = ctypes.cdll.LoadLibrary("./yourmp5.so")

    if not os.path.exists("data.json"):
        with open("data.json", "w") as f:
            json.dump({}, f)

    # load data from json
    with open("data.json", "r") as f:
        data = json.load(f)
        LINE_DATA = data.get("LINE_DATA")
        TRIANGLE_DATA = data.get("TRIANGLE_DATA")
        PARALLELOGRAM_DATA = data.get("PARALLELOGRAM_DATA")
        RECT_DATA = data.get("RECT_DATA")
        CIRCLE_DATA = data.get("CIRCLE_DATA")
        RECT_GRADIENT_DATA = data.get("RECT_GRADIENT_DATA")

    if LINE_DATA is None:
        LINE_DATA = []
        for _ in range(50):
            x1, y1 = randomPoint()
            x2, y2 = randomPoint()
            LINE_DATA.append((x1, y1, x2, y2))
        data["LINE_DATA"] = LINE_DATA
    if TRIANGLE_DATA is None:
        TRIANGLE_DATA = []
        for _ in range(50):
            x1, y1 = randomPoint()
            x2, y2 = randomPoint()
            x3, y3 = randomPoint()
            TRIANGLE_DATA.append((x1, y1, x2, y2, x3, y3))
        data["TRIANGLE_DATA"] = TRIANGLE_DATA
    if PARALLELOGRAM_DATA is None:
        PARALLELOGRAM_DATA = []
        for _ in range(50):
            x1, y1 = randomPoint()
            x2, y2 = randomPoint()
            x3, y3 = randomPoint()
            PARALLELOGRAM_DATA.append((x1, y1, x2, y2, x3, y3))
        data["PARALLELOGRAM_DATA"] = PARALLELOGRAM_DATA
    if RECT_DATA is None:
        RECT_DATA = []
        for _ in range(50):
            x, y = randomPoint()
            w = random.randint(0, 624 - x)
            h = random.randint(0, 320 - y)
            RECT_DATA.append((x, y, w, h))
        data["RECT_DATA"] = RECT_DATA
    if CIRCLE_DATA is None:
        CIRCLE_DATA = []
        for _ in range(50):
            x, y = randomPoint()
            inner_r = random.randint(0, 100)
            outer_r = random.randint(inner_r, 100)
            CIRCLE_DATA.append((x, y, inner_r, outer_r))
        data["CIRCLE_DATA"] = CIRCLE_DATA
    if RECT_GRADIENT_DATA is None:
        RECT_GRADIENT_DATA = []
        for _ in range(50):
            x, y = randomPoint()
            w = random.randint(0, 624 - x)
            h = random.randint(0, 320 - y)
            c1 = random.randint(0, 0xFFFFFFFF)
            c2 = random.randint(0, 0xFFFFFFFF)
            RECT_GRADIENT_DATA.append((x, y, w, h, c1, c2))
        data["RECT_GRADIENT_DATA"] = RECT_GRADIENT_DATA
    with open("data.json", "w") as f:
        json.dump(data, f)

    # test draw_line
    INFO("Testing draw_line")
    for i in range(len(LINE_DATA)):
        gen_draw_line(zhyso, *LINE_DATA[i], f"ans/zhy_draw_line{i}.png")
        gen_draw_line(yourso, *LINE_DATA[i], f"ans/your_draw_line{i}.png")
        if subprocess.getoutput(f"diff ans/zhy_draw_line{i}.png ans/your_draw_line{i}.png") != "":
            ERROR("Your draw_line is different from zhy's")
            ERROR("data: {}".format(LINE_DATA[i]))
            print()
        else:
            os.remove(f"ans/zhy_draw_line{i}.png")
            os.remove(f"ans/your_draw_line{i}.png")

    # test draw_triangle
    INFO("Testing draw_triangle")
    for i in range(len(TRIANGLE_DATA)):
        gen_draw_triangle(zhyso, *TRIANGLE_DATA[i], f"ans/zhy_draw_triangle{i}.png")
        gen_draw_triangle(yourso, *TRIANGLE_DATA[i], f"ans/your_draw_triangle{i}.png")
        if subprocess.getoutput(f"diff ans/zhy_draw_triangle{i}.png ans/your_draw_triangle{i}.png") != "":
            ERROR("Your draw_triangle is different from zhy's")
            ERROR("data: {}".format(TRIANGLE_DATA[i]))
            print()
        else:
            os.remove(f"ans/zhy_draw_triangle{i}.png")
            os.remove(f"ans/your_draw_triangle{i}.png")

    # test draw_parallelogram
    INFO("Testing draw_parallelogram")
    for i in range(len(PARALLELOGRAM_DATA)):
        gen_draw_parallelogram(zhyso, *PARALLELOGRAM_DATA[i], f"ans/zhy_draw_parallelogram{i}.png")
        gen_draw_parallelogram(yourso, *PARALLELOGRAM_DATA[i], f"ans/your_draw_parallelogram{i}.png")
        if subprocess.getoutput(f"diff ans/zhy_draw_parallelogram{i}.png ans/your_draw_parallelogram{i}.png") != "":
            ERROR("Your draw_parallelogram is different from zhy's")
            ERROR("data: {}".format(PARALLELOGRAM_DATA[i]))
            print()
        else:
            os.remove(f"ans/zhy_draw_parallelogram{i}.png")
            os.remove(f"ans/your_draw_parallelogram{i}.png")

    # test draw_rect
    INFO("Testing draw_rect")
    for i in range(len(RECT_DATA)):
        gen_draw_rect(zhyso, *RECT_DATA[i], f"ans/zhy_draw_rect{i}.png")
        gen_draw_rect(yourso, *RECT_DATA[i], f"ans/your_draw_rect{i}.png")
        if subprocess.getoutput(f"diff ans/zhy_draw_rect{i}.png ans/your_draw_rect{i}.png") != "":
            ERROR("Your draw_rect is different from zhy's")
            ERROR("data: {}".format(RECT_DATA[i]))
            print()
        else:
            os.remove(f"ans/zhy_draw_rect{i}.png")
            os.remove(f"ans/your_draw_rect{i}.png")

    # test draw_circle
    INFO("Testing draw_circle")
    for i in range(len(CIRCLE_DATA)):
        gen_draw_circle(zhyso, *CIRCLE_DATA[i], f"ans/zhy_draw_circle{i}.png")
        gen_draw_circle(yourso, *CIRCLE_DATA[i], f"ans/your_draw_circle{i}.png")
        if subprocess.getoutput(f"diff ans/zhy_draw_circle{i}.png ans/your_draw_circle{i}.png") != "":
            ERROR("Your draw_circle is different from zhy's")
            ERROR("data: {}".format(CIRCLE_DATA[i]))
            print()
        else:
            os.remove(f"ans/zhy_draw_circle{i}.png")
            os.remove(f"ans/your_draw_circle{i}.png")
    
    # test rect_gradient
    INFO("Testing rect_gradient")
    for i in range(len(RECT_GRADIENT_DATA)):
        gen_rect_gradient(zhyso, *RECT_GRADIENT_DATA[i], f"ans/zhy_rect_gradient{i}.png")
        gen_rect_gradient(yourso, *RECT_GRADIENT_DATA[i], f"ans/your_rect_gradient{i}.png")
        if subprocess.getoutput(f"diff ans/zhy_rect_gradient{i}.png ans/your_rect_gradient{i}.png") != "":
            ERROR("Your rect_gradient is different from zhy's")
            ERROR("data: {}".format(RECT_GRADIENT_DATA[i]))
            print()
        else:
            os.remove(f"ans/zhy_rect_gradient{i}.png")
            os.remove(f"ans/your_rect_gradient{i}.png")
        

import ctypes
import sys
import subprocess
import os
import random
import json

WIDTH = 624
HEIGHT = 320

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
    x = random.randint(0-10, WIDTH+10)
    y = random.randint(0-5, HEIGHT+5)
    return x, y

def gen_near_horizontal(so, x1, y1, x2, y2, path: str = None):
    if so is None:
        return os.system("../gold 1 {} {} {} {}".format(x1, y1, x2, y2))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.near_horizontal(x1, y1, x2, y2)
    so.make_png(path.encode())
    so.free_image_data()
    return result

def gen_near_vertical(so, x1, y1, x2, y2, path: str = None):
    if so is None:
        return os.system("../gold 2 {} {} {} {}".format(x1, y1, x2, y2))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.near_vertical(x1, y1, x2, y2)
    so.make_png(path.encode())
    so.free_image_data()
    return result

def gen_draw_line(so, x1, y1, x2, y2, path: str = None):
    if so is None:
        return os.system("../gold 3 {} {} {} {}".format(x1, y1, x2, y2))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.draw_line(x1, y1, x2, y2)
    so.make_png(path.encode())
    so.free_image_data()
    return result

def gen_draw_triangle(so, x1, y1, x2, y2, x3, y3, path: str = None):
    if so is None:
        return os.system("../gold 4 {} {} {} {} {} {}".format(x1, y1, x2, y2, x3, y3))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.draw_triangle(x1, y1, x2, y2, x3, y3)
    so.make_png(path.encode())
    so.free_image_data()
    return result

def gen_draw_parallelogram(so, x1, y1, x2, y2, x3, y3, path: str = None):
    if so is None:
        return os.system("../gold 5 {} {} {} {} {} {}".format(x1, y1, x2, y2, x3, y3))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.draw_parallelogram(x1, y1, x2, y2, x3, y3)
    so.make_png(path.encode())
    so.free_image_data()
    return result

def gen_draw_rect(so, x, y, w, h, path: str = None):
    if so is None:
        return os.system("../gold 6 {} {} {} {}".format(x, y, w, h))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.draw_rect(x, y, w, h)
    so.make_png(path.encode())
    so.free_image_data()
    return result

def gen_draw_circle(so, x, y, inner_r, outer_r, path: str = None):
    if so is None:
        return os.system("../gold 7 {} {} {} {}".format(x, y, inner_r, outer_r))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.draw_circle(x, y, inner_r, outer_r)
    so.make_png(path.encode())
    so.free_image_data()
    return result

def gen_rect_gradient(so, x, y, w, h, c1, c2, path: str = None):
    if so is None:
        return os.system("../gold 8 {} {} {} {} {} {}".format(x, y, w, h, c1, c2))
    so.set_color(0x00FFFFFF)
    so.initialize_and_clear_canvas()
    so.set_color(0)
    result = so.rect_gradient(x, y, w, h, c1, c2)
    so.make_png(path.encode())
    so.free_image_data()
    return result

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
    
    zhyso = None
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
        LINE_DATA.append([10, 10, 10, 10])
        for _ in range(50):
            x1, y1 = randomPoint()
            x2, y2 = randomPoint()
            LINE_DATA.append((x1, y1, x2, y2))
        data["LINE_DATA"] = LINE_DATA
    if TRIANGLE_DATA is None:
        TRIANGLE_DATA = []
        TRIANGLE_DATA.extend((
            [10, 10, 10, 10, 10, 10],
            [10, 10, 100, 10, 50, 10]
        ))
        for _ in range(50):
            x1, y1 = randomPoint()
            x2, y2 = randomPoint()
            x3, y3 = randomPoint()
            TRIANGLE_DATA.append((x1, y1, x2, y2, x3, y3))
        data["TRIANGLE_DATA"] = TRIANGLE_DATA
    if PARALLELOGRAM_DATA is None:
        PARALLELOGRAM_DATA = []
        PARALLELOGRAM_DATA.extend((
            [10, 10, 10, 10, 10, 10],
            [10, 10, 100, 10, 50, 10]
        ))
        for _ in range(50):
            x1, y1 = randomPoint()
            x2, y2 = randomPoint()
            x3, y3 = randomPoint()
            PARALLELOGRAM_DATA.append((x1, y1, x2, y2, x3, y3))
        data["PARALLELOGRAM_DATA"] = PARALLELOGRAM_DATA
    if RECT_DATA is None:
        RECT_DATA = []
        RECT_DATA.extend((
            [10, 10, 0, 100],
            [10, 10, 100, 0],
            [10, 10, 0, 0]
        ))
        for _ in range(50):
            x, y = randomPoint()
            w = random.randint(0, WIDTH + 10 - x)
            h = random.randint(0, HEIGHT + 5 - y)
            RECT_DATA.append((x, y, w, h))
        data["RECT_DATA"] = RECT_DATA
    if CIRCLE_DATA is None:
        CIRCLE_DATA = []
        CIRCLE_DATA.extend((
            [10, 10, 0, 5],
            [10, 10, 0, 0]
        ))
        for _ in range(50):
            x, y = randomPoint()
            inner_r = random.randint(0, 100)
            outer_r = random.randint(inner_r, 100)
            CIRCLE_DATA.append((x, y, inner_r, outer_r))
        data["CIRCLE_DATA"] = CIRCLE_DATA
    if RECT_GRADIENT_DATA is None:
        RECT_GRADIENT_DATA = []
        RECT_GRADIENT_DATA.extend((
            [10, 10, 0, 100, 0, 0],
            [10, 10, 100, 0, 0, 0],
            [10, 10, 0, 0, 0, 0]
        ))
        for _ in range(50):
            x, y = randomPoint()
            w = random.randint(0, WIDTH + 10 - x)
            h = random.randint(0, HEIGHT + 5 - y)
            c1 = random.randint(0, 0x00FFFFFF)
            c2 = random.randint(0, 0x00FFFFFF)
            RECT_GRADIENT_DATA.append((x, y, w, h, c1, c2))
        data["RECT_GRADIENT_DATA"] = RECT_GRADIENT_DATA
    with open("data.json", "w") as f:
        json.dump(data, f)

    # test near_horizontal
    # INFO("Testing near_horizontal")
    # for i in range(len(LINE_DATA)):
    #     zhyans = gen_near_horizontal(zhyso, *LINE_DATA[i], f"ans/zhy_near_horizontal{i}.png")
    #     yourans = gen_near_horizontal(yourso, *LINE_DATA[i], f"ans/your_near_horizontal{i}.png")
    #     diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_near_horizontal{i}.png")
    #     if diffresult != "":
    #         ERROR("Your near_horizontal is different from gold's")
    #         os.system(f"mv gold_out.png ans/gold_near_horizontal{i}.png")
    #         ERROR("data: {}".format(LINE_DATA[i]))
    #         print()
    #     else:
    #         os.remove("gold_out.png")
    #         os.remove(f"ans/your_near_horizontal{i}.png")

    # test near_vertical
    # INFO("Testing near_vertical")
    # for i in range(len(LINE_DATA)):
    #     zhyans = gen_near_vertical(zhyso, *LINE_DATA[i], f"ans/zhy_near_vertical{i}.png")
    #     yourans = gen_near_vertical(yourso, *LINE_DATA[i], f"ans/your_near_vertical{i}.png")
    #     diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_near_vertical{i}.png")
    #     if diffresult != "":
    #         ERROR("Your near_vertical is different from gold's")
    #         os.system(f"mv gold_out.png ans/gold_near_vertical{i}.png")
    #         ERROR("data: {}".format(LINE_DATA[i]))
    #         print()
    #     else:
    #         os.remove("gold_out.png")
    #         os.remove(f"ans/your_near_vertical{i}.png")

    # test draw_line
    INFO("Testing draw_line")
    for i in range(len(LINE_DATA)):
        zhyans = gen_draw_line(zhyso, *LINE_DATA[i], f"ans/zhy_draw_line{i}.png")
        yourans = gen_draw_line(yourso, *LINE_DATA[i], f"ans/your_draw_line{i}.png")
        diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_draw_line{i}.png")
        if diffresult != "":
            ERROR("Your draw_line is different from gold's")
            os.system(f"mv gold_out.png ans/gold_draw_line{i}.png")
            ERROR("data: {}".format(LINE_DATA[i]))
            print()
        else:
            os.remove("gold_out.png")
            os.remove(f"ans/your_draw_line{i}.png")

    # test draw_triangle
    INFO("Testing draw_triangle")
    for i in range(len(TRIANGLE_DATA)):
        zhyans = gen_draw_triangle(zhyso, *TRIANGLE_DATA[i], f"ans/zhy_draw_triangle{i}.png")
        yourans = gen_draw_triangle(yourso, *TRIANGLE_DATA[i], f"ans/your_draw_triangle{i}.png")
        diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_draw_triangle{i}.png")
        if diffresult != "":
            ERROR("Your draw_triangle is different from gold's")
            os.system(f"mv gold_out.png ans/gold_draw_triangle{i}.png")
            ERROR("data: {}".format(TRIANGLE_DATA[i]))
            print()
        else:
            os.remove("gold_out.png")
            os.remove(f"ans/your_draw_triangle{i}.png")

    # test draw_parallelogram
    INFO("Testing draw_parallelogram")
    for i in range(len(PARALLELOGRAM_DATA)):
        zhyans = gen_draw_parallelogram(zhyso, *PARALLELOGRAM_DATA[i], f"ans/zhy_draw_parallelogram{i}.png")
        yourans = gen_draw_parallelogram(yourso, *PARALLELOGRAM_DATA[i], f"ans/your_draw_parallelogram{i}.png")
        diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_draw_parallelogram{i}.png")
        if diffresult != "":
            ERROR("Your draw_parallelogram is different from gold's")
            os.system(f"mv gold_out.png ans/gold_draw_parallelogram{i}.png")
            ERROR("data: {}".format(PARALLELOGRAM_DATA[i]))
            print()
        else:
            os.remove("gold_out.png")
            os.remove(f"ans/your_draw_parallelogram{i}.png")

    # test draw_rect
    INFO("Testing draw_rect")
    for i in range(len(RECT_DATA)):
        zhyans = gen_draw_rect(zhyso, *RECT_DATA[i], f"ans/zhy_draw_rect{i}.png")
        yourans = gen_draw_rect(yourso, *RECT_DATA[i], f"ans/your_draw_rect{i}.png")
        diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_draw_rect{i}.png")
        if diffresult != "":
            ERROR("Your draw_rect is different from gold's")
            os.system(f"mv gold_out.png ans/gold_draw_rect{i}.png")
            ERROR("data: {}".format(RECT_DATA[i]))
            print()
        else:
            os.remove("gold_out.png")
            os.remove(f"ans/your_draw_rect{i}.png")

    # test draw_circle
    INFO("Testing draw_circle")
    for i in range(len(CIRCLE_DATA)):
        zhyans = gen_draw_circle(zhyso, *CIRCLE_DATA[i], f"ans/zhy_draw_circle{i}.png")
        yourans = gen_draw_circle(yourso, *CIRCLE_DATA[i], f"ans/your_draw_circle{i}.png")
        diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_draw_circle{i}.png")
        if diffresult != "":
            ERROR("Your draw_circle is different from gold's")
            os.system(f"mv gold_out.png ans/gold_draw_circle{i}.png")
            ERROR("data: {}".format(CIRCLE_DATA[i]))
            print()
        else:
            os.remove("gold_out.png")
            os.remove(f"ans/your_draw_circle{i}.png")

    # test rect_gradient
    INFO("Testing rect_gradient")
    for i in range(len(RECT_GRADIENT_DATA)):
        zhyans = gen_rect_gradient(zhyso, *RECT_GRADIENT_DATA[i], f"ans/zhy_rect_gradient{i}.png")
        yourans = gen_rect_gradient(yourso, *RECT_GRADIENT_DATA[i], f"ans/your_rect_gradient{i}.png")
        diffresult = subprocess.getoutput(f"diff gold_out.png ans/your_rect_gradient{i}.png")
        if diffresult != "":
            ERROR("Your rect_gradient is different from gold's")
            os.system(f"mv gold_out.png ans/gold_rect_gradient{i}.png")
            ERROR("data: {}".format(RECT_GRADIENT_DATA[i]))
            print()
        else:
            os.remove("gold_out.png")
            os.remove(f"ans/your_rect_gradient{i}.png")
        

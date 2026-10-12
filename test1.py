import keyboard
import pyautogui

# 紧急中止
pyautogui.FAILSAFE = True


# 1280 x 600

# 75,219,106
while True:
    if keyboard.is_pressed("q"):  # 按下 'q' 键时退出循环
        break
    color = pyautogui.pixel(120, 600)  # 获取指定位置的像素点颜色
    if color == (75, 219, 106):
        pyautogui.click(120, 600)  # 点击指定位置
    pyautogui.sleep(0.05)  # 等待0.1秒
    if keyboard.is_pressed("q"):  # 按下 'q' 键时退出循环
        break


# first commit for feature/due-date


# rebase start

# main first commit

# feature first commit

# feature second commit

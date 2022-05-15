# this is platformio's black magic, not my fault
Import("env")
print("Replace MKSPIFFSTOOL with mklittlefs.exe")
env.Replace(MKSPIFFSTOOL = "./mklittlefs.exe")
import subprocess
import os.path
import glob
from datetime import datetime
from PIL import Image


def recurs(walk_in):
    walk_result = os.walk(walk_in)
    
    for dir in walk_result:
        
        print("dir: " + dir[0])
        
        if(not "output" in dir[0]):

            files = glob.glob(os.path.join(dir[0], '*.jpg'))
            jpg = []

            for x in files:
                s = x.split('.')

                img = Image.open(x)

                str = "C:\\Users\\bossC\\Documents\\Cedric\\C++\\Repos\\MachineLearning_5DJV\\MLChampionsColorx5\\output";

                img.crop((0, 0, 340, 340)).save(str + s[1] + "1." + s[2])
                img.crop((40, 40, 380, 380)).save(str + s[1] + "2." + s[2])
                img.crop((0, 40, 340, 380)).save(str + s[1] + "3." + s[2])
                img.crop((40, 0, 380, 340)).save(str + s[1] + "4." + s[2])
                img.crop((20, 20, 360, 360)).save(str + s[1] + "." + s[2])

                print(x)

            

                    
recurs(".")
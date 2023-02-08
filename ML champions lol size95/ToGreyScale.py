import subprocess
import os.path
import glob
from datetime import datetime
from PIL import Image


def recurs(walk_in):
    walk_result = os.walk(walk_in)
    
    for dir in walk_result:
        
        print("dir: " + dir[0])
        
        files = glob.glob(os.path.join(dir[0], '*.jpg'))
        jpg = []

        for x in files:
            img = Image.open(x).convert('L').resize((95, 95))
            img.save(x)

            print(x)

            #tmp = x.split("\\")
            #jpg.append(tmp[len(tmp)-1])

        #for file in jpg:

            #print(file)

            

                    
recurs(".")
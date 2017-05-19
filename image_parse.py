import os, Image, sys
from scipy import misc
import PIL

path = os.getenv('HOME') + '/Documents/graduation_cap/'

def main(infile):
    global path
    name, extension = infile.split('.')
    fi = open(name+'.txt', 'w')
    #change file name as needed
    arr = misc.imread(infile) # 640x480x3 array
    lin_img = arr.flatten()
    pixel_list = lin_img.tolist()
    pixel_str_list = map(str, pixel_list)

    final_string = ""
    for string in pixel_str_list:
        number = oct(int(string))
        number = str(number).replace("0","")
        final_string = final_string + "\\" + str(number)
    fi.write(final_string)

if len(sys.argv)==1:
    print("please enter the file you wish to convert as system argument")
else:
    main(sys.argv[1])

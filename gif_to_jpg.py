import os, Image, sys, PIL
from scipy import misc

path = os.getenv('HOME') + '/Documents/graduation_cap/'
count = 0

def processImage(infile):
    global count
    name, f_ext = infile.split('.')
    try:
        im = Image.open(infile)
    except IOError:
        print "Cant load", infile
        sys.exit(1)
    i = 0
    mypalette = im.getpalette()
    try:
        os.system('mkdir ' + name)
        os.chdir(path + 'media/' + name)
        while 1:
            im.putpalette(mypalette)
            new_im = Image.new("RGBA", im.size)
            new_im.paste(im)
            new_im.save(name + str(i)+'.png')
            i += 1
            im.seek(im.tell() + 1)
            count += 1
    except EOFError:
        pass # end of sequence

    #this will now go through and conver teach file made above
    tempPath = path + 'media/' + name
    os.chdir(tempPath)
    print tempPath
    for infile in tempPath:
        if '.gif' in infile:
            continue
        else:
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

os.chdir(path + '/media')
if len(sys.argv) == 1:
    print("please enter the gif file you wish to convert as a system argument")
else:
    gif_name = sys.argv[1:][0]
    processImage(gif_name)
    print("Processed " + name + ".  " + count + ' files made')

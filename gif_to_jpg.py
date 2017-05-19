import os, Image, sys, PIL
from scipy import misc

path = os.getenv('HOME') + '/Documents/graduation_cap/'

def processImage(infile):
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
    except EOFError:
        pass # end of sequence

os.chdir(path + '/media')
if len(sys.argv) == 1:
    print("please enter the gif file you wish to convert as a system argument")
else:
    gif_name = sys.argv[1:][0]
    processImage(gif_name)

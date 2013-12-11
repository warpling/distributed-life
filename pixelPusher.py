import os, sys
import Image
# such Python

print "hey!"

for infile in sys.argv[1:]:
    f, e = os.path.splitext(infile)
    outfile = "out.jpg"

    try: # much whitespace
       image = Image.open(infile)#.save(outfile)
       print infile, image.format, "%dx%d" % image.size, image.mode
    except IOError:
        print "cannot convert", infile

    #box = (100,100,400,400)
    #region = image.crop(box)
    #region = region.transpose(Image.ROTATE_180)
    #image.paste(region, box)


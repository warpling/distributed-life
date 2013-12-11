import os, sys
import struct
#import Image
# such Python

def main():
   if len(sys.argv) < 2:
      print "Usage: pixelPusher <inputFile>"
      sys.exit(1)
   print "hey!"

   for infile in sys.argv[1:]:
#      name, e = os.path.splitext(infile)
#      print "name " + name +" e " + e
      ints = []
      f = open(infile, "rb")
      try:
         numElements = struct.unpack('i', f.read(4))[0]
         print "numElements = ", numElements 
         byte = f.read(1) 
         for i in range(numElements):
         # Do stuff with byte.
            if (ord(byte) & 0x01) == 0x01:
               ints.append(1)
            else:
               ints.append(0)
            byte = f.read(1)
      finally:
         f.close()
      print ints

   outfile = "out.jpg"

#    try: # much whitespace
#       image = Image.open(infile)#.save(outfile)
#       print infile, image.format, "%dx%d" % image.size, image.mode
#    except IOError:
#        print "cannot convert", infile

#box = (100,100,400,400)
#region = image.crop(box)
#region = region.transpose(Image.ROTATE_180)
#image.paste(region, box)

if __name__ == "__main__":
   main()

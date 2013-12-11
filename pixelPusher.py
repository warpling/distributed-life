import os, sys
import struct
import math
from PIL import Image
from bmp import BitMap, Color

def main():
   if len(sys.argv) < 2:
      print "Usage: pixelPusher <inputFile>"
      sys.exit(1)

   for infile in sys.argv[1:]:
#        name, e = os.path.splitext(infile)
#        print "name " + name +" e " + e
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
    
        # Make the pretty picture
        width = int(math.floor(math.sqrt(numElements)))
    
        PIXEL_SIZE = 5
        bmp = BitMap(PIXEL_SIZE*width, PIXEL_SIZE*width)
        for idx, bit in enumerate(ints):
            if (bit > 0):
              bmp.setPenColor(Color(220, 75, 55))
            else:
              bmp.setPenColor(Color(255, 255, 255))
            bmp.drawSquare((idx/width)*PIXEL_SIZE, (idx % width)*PIXEL_SIZE, PIXEL_SIZE, fill=False)

        bmp.saveFile(os.path.splitext(infile)[0] + ".bmp", "bmp")
    
if __name__ == "__main__":
   main()

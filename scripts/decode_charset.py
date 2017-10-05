import sys

VDG_BLACK = '\x00\x00\x00\xFF'
VDG_WHITE = '\xFF\xFF\xFF\xFF'
VDG_GREEN = '\x00\xFF\x0B\xFF'
VDG_AI_INV = '\x00A\x00\xff'

f = open("./screen.data", "rb")
charbuf = []
mymap = {}
key = 0

for i in range(12288):
    ch = f.read(4)
    if not mymap.has_key(ch):
        mymap[ch] = key
        key = key + 1

    charbuf.append(mymap[ch])

basex = 0
basey = 0

for j in range(4):
    for i in range(32):
        # print "{",
        sys.stdout.write("{ ")
        for y in range(12):
            # print "0b",
            if y > 0:
                sys.stdout.write("  ")
            sys.stdout.write("0b")
            for x in range(8):
                pixel = charbuf[(j * 256 * 12) + (y * 256) + (i * 8) + x]
                pixel = 1 - pixel
                # print pixel,
                sys.stdout.write(str(pixel))
            if y < 11:
                print ","
            else:
                print " },"

f.close()

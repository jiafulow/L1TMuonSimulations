#!/usr/bin/env python


def sanity_check(patterns):
    assert len(patterns) == 9, "%i" % len(patterns)
    for i in [0,1,3,5,7]:
        patt = patterns[i]
        if i == 0:
            mirror = patterns[i]
        else:
            mirror = patterns[i+1]
        assert len(patt) == 4 and len(mirror) == 4, "%i, %i" % (len(patt), len(mirror))
        for j in xrange(4):
            pair1 = patt[j]
            pair2 = mirror[j]
            assert len(pair1) == 2 and len(pair2) == 2, "%i, %i" % (len(pair1), len(pair2))
            assert pair1[0] <= pair1[1] and pair2[0] <= pair2[1], "(%i, %i), (%i, %i)" % (pair1[0], pair1[1], pair2[1], pair2[0])
            assert pair1[0] == -pair2[1] and pair1[1] == -pair2[0], "(%i, %i), (%i, %i)" % (pair1[0], pair1[1], pair2[1], pair2[0])
    return

def make_images(patterns):
    sanity_check(patterns)
    images = []
    for pattern in patterns:
        image = make_image(pattern)
        images.append(image)
    return images

def make_image(pattern):
    image = []
    origin = 15
    for j in xrange(len(pattern)):
        pair = pattern[j]
        kk = 0
        for k in xrange(pair[0], pair[1]+1):
            kk |= (1 << (origin-k))
        image.append(kk)
        image.append(0)
    return tuple(image)

def print_images(images, check=False):
    i = 1
    for image in images:
        printme = []
        for d in image:
            if d == 0:
                printme.append("0")
            else:
                printme.append("0x%x" % d)

        print "PhiMemoryImage pattern%i (%s);" % (i, ",".join(printme))
        i += 1
    print

    if check:
        original = [
            (0x8000,0,0x8000,0,0x8000,0,0x8000,0),
            (0x10000,0,0x8000,0,0xc000,0,0xc000,0),
            (0x4000,0,0x8000,0,0x18000,0,0x18000,0),
            (0x60000,0,0x8000,0,0xe000,0,0xe000,0),
            #(0x3000,0,0x8000,0,0x78000,0,0x78000,0),  # not consistent!
            (0x3000,0,0x8000,0,0x38000,0,0x38000,0),
            (0x780000,0,0x8000,0,0xff00,0,0xff00,0),
            (0xf00,0,0x8000,0,0x7f8000,0,0x7f8000,0),
            (0x7f800000,0,0x8000,0,0xff00,0,0xff00,0),
            (0xff,0,0x8000,0,0x7f8000,0,0x7f8000,0),
        ]
        assert images == original


# ______________________________________________________________________________
current_emtf_patterns = [
    (   (0,0), (0,0),   (0,0),   (0,0)),   # pattern 1
    ( (-1,-1), (0,0),  (0,+1),  (0,+1)),   # pattern 2
    ( (+1,+1), (0,0),  (-1,0),  (-1,0)),   # pattern 3
    ( (-3,-2), (0,0),  (0,+2),  (0,+2)),   # pattern 4
    ( (+2,+3), (0,0),  (-2,0),  (-2,0)),   # pattern 5
    ( (-7,-4), (0,0),  (0,+7),  (0,+7)),   # pattern 6
    ( (+4,+7), (0,0),  (-7,0),  (-7,0)),   # pattern 7
    ((-15,-8), (0,0),  (0,+7),  (0,+7)),   # pattern 8
    ((+8,+15), (0,0),  (-7,0),  (-7,0)),   # pattern 9
]

images = make_images(current_emtf_patterns)
print_images(images, check=True)


modified_emtf_patterns = [
    (   (0,0), (0,0), (-1,+1), (-1,+1)),   # pattern 1
    ( (-1,-1), (0,0),  (0,+2),  (0,+3)),   # pattern 2
    ( (+1,+1), (0,0),  (-2,0),  (-3,0)),   # pattern 3
    ( (-3,-2), (0,0),  (0,+2),  (0,+3)),   # pattern 4
    ( (+2,+3), (0,0),  (-2,0),  (-3,0)),   # pattern 5
    ( (-7,-4), (0,0),  (0,+3),  (0,+5)),   # pattern 6
    ( (+4,+7), (0,0),  (-3,0),  (-5,0)),   # pattern 7
    ((-15,-8), (0,0),  (0,+7),  (0,+12)),   # pattern 8
    ((+8,+15), (0,0),  (-7,0),  (-12,0)),   # pattern 9
]

images = make_images(modified_emtf_patterns)
print_images(images, check=False)


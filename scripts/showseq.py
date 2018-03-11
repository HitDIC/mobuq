import struct
import matplotlib.pyplot as plt
import numpy as np
from PIL import Image
import os
# plt.rcParams.update({'figure.autolayout': True})

def intervals(values, counts, incredible):
    # values: 
    # counts:
    if values.size != counts.size:
        print("Not Equal: %s != %s" % (values.size, counts.size))
        # return
    al = np.sum(counts)
    n = counts.size
    left = 0.0
    vleft = values[0]
    for i in range(n):
        left += 1.0 * counts[i] / al
        if left >= incredible/2.0:
            vleft = values[i]
            break
    right = 0.0
    vright = values[-1]
    for i in range(n-1, -1, -1):
        right += 1.0 * counts[i] / al
        if right >= incredible/2.0:
            vright = values[i]
            break
    return (vleft, vright)

def showseq(src, dest):
    data = np.genfromtxt(src)

    m, n = data.shape
    n = n-1
    imglist = []
    imgall = []

    for i in range(n):
        l = []
        for j in range(i+1, n):
            l.append("%d-%d.jpg" % (i, j))
            fig = plt.figure()
            ax = fig.add_subplot(111)
            ax.scatter(data[:, i], data[:, j])
            ax.set_xlabel("$\\theta_%d$" % i)
            ax.set_ylabel("$\\theta_%d$" % j)
            ax.set_xlim([-450, -50])
            ax.set_ylim([-450, -50])
            plt.savefig("%d-%d.jpg" % (i, j))
            # plt.clear()
        imglist.append(l)
        imgall.extend(l)

    histlist = []
    reals = [-340.0, -330.0, -320.0, -350.0]
    for i in range(n):
        fig = plt.figure()
        ax = fig.add_subplot(111)
        m, bins, patches = ax.hist(data[:, i], facecolor='white', histtype='step', bins=100)
        # print(bins)
        print(m)
        ax.set_xlabel("$\\theta_%d$" % i)
        ax.axvline(data[:, i].mean(), color='b', linestyle='dashed', linewidth=2)
        ax.axvline(reals[i], color='b', linestyle='-', linewidth=2)
        vleft, vright = intervals(bins, m, (1.0-0.95))
        print(vleft, vright)
        ax.axvline(vleft, color='r', linestyle='dashed', linewidth=2)
        ax.axvline(vright, color='r', linestyle='dashed', linewidth=2)
        ax.set_xlim([-450, -50])
        plt.savefig("%d-hist.jpg" % (i, ))
        histlist.append("%d-hist.jpg" % (i, ))


    # https://kanoki.org/2017/07/12/merge-images-with-python/
    imgs = dict(zip(imgall, [ Image.open(i) for i in imgall]))
    min_img_shape = imgs[imgall[0]].size
    empty = Image.new('RGB', min_img_shape, (255, 255, 255))
    
    imgs_hist = [ Image.open(i) for i in histlist]

    cols = []
    for i in range(n):
        ims = [empty for j in range(i)]
        ims.append(imgs_hist[i])
        if i < n-1:
            ims.extend([imgs[imn] for imn in imglist[i]])

        cols.append(
            np.vstack( (np.asarray( i.resize(min_img_shape, Image.ANTIALIAS) ) for i in ims ) )
        )

    for each in imgall:
        os.remove(each)
    
    for each in histlist:
        os.remove(each)

    img_merge = np.hstack( tuple(cols) )

    img_merge = Image.fromarray(img_merge)
    img_merge.save(dest)

if __name__ == "__main__":
    showseq("seq.txt", "merge.jpg")
import numpy as np
from PIL import Image

images_list = ['0-1.jpg', '0-2.jpg', '0-3.jpg', '1-2.jpg', '1-3.jpg', '2-3.jpg']
imgs = [ Image.open(i) for i in images_list ]
min_img_shape = sorted( [(np.sum(i.size), i.size ) for i in imgs])[0][1]

empty = Image.new('RGB', min_img_shape, (255, 255, 255))

# empty = imgs[0].resize(min_img_shape,Image.ANTIALIAS)

col_1 = np.vstack( (np.asarray( i.resize(min_img_shape,Image.ANTIALIAS) ) for i in [imgs[0], imgs[1], imgs[2]] ) )
col_2 = np.vstack( (np.asarray( i.resize(min_img_shape,Image.ANTIALIAS) ) for i in [empty, imgs[3], imgs[4]] ) )
col_3 = np.vstack( (np.asarray( i.resize(min_img_shape,Image.ANTIALIAS) ) for i in [empty, empty, imgs[5]] ) )

img_merge = np.hstack( (col_1, col_2, col_3) )

img_merge = Image.fromarray( img_merge)
img_merge.save( 'merge.jpg' )
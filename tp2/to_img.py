from netCDF4 import Dataset
import numpy as np
import cv2

opt = str(input("original(o) or filtered(f): "))

if opt=='f':
	output_file = 'filtrada.jpg'
	value = np.fromfile('./out.bin', dtype=np.float32)
	value = np.reshape(value, (21696,21696))
	cv2.normalize(value, value, 0, 255, cv2.NORM_MINMAX)
	value = value.astype(np.uint8)
	cv2.imwrite(output_file, value)

else:
	output_file = 'original.jpg'
	file_name = "OR_ABI-L2-CMIPF-M6C02_G16_s20191011800206_e20191011809514_c20191011809591.nc"
	output_file = "original.jpg"
	rootgrp = Dataset(file_name, "r+")
	value = rootgrp.variables['CMI'][:]	
	data = 255 * value
	img = data.astype(np.uint8)
	cv2.imwrite(output_file, img)

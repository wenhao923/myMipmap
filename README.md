这里是自己实现的MIPMAP

二重过滤采样了4个pixel，分别做线性插值。

三重过滤采样了2层，再做插值。

踩坑指南：

1. opencv的库，导入图片的类型存的像素点是char*，这里我转成32F。最终输出的时候转成int，输出ppm文件格式
2. ![pixel](.\pixel.png)
3. 当分辨率与原图相同时，选择层级是正确的，但是进行二重过滤时，是错的。（做了插值）所以图片demo即使分辨率与原图相同，但是却比原图模糊。

4. 传指针作为参数时，比如float uv[]，传了一个地址。函数里需要用指针接收，并解引用。然后++即可访问下一个地址

   ```
   void LookUp(float uv[], const cv::Mat &img) {
   	float *ptr = uv;
   	float row = (*ptr) * (float)img.rows;
   	ptr++;
   	float col = (*ptr) * (float)img.cols;
   }
   ```

   

5. 模板的使用：由于Mat类需要访问对应下标像素如

   ```
   img.at<cv::Vec3f>(i, j);
   img.at<cv::Vec3b>(i, j);
   //由于函数参数只传了cv::Mat无法带上模板，所以存放不同类型像素值的图像不能调用同名函数
   ```

   
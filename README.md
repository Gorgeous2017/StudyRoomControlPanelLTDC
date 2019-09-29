# LTDC版本的室内控制面板程序

## 开发日志 2019/09/26

  ### 遇到问题：图片出现`雪花点`和`错位`  
  找资料时发现有人用LTDC和DMA2D显示图片时出现了问题 [博客地址](http://firebbs.cn/thread-26999-1-1.html)  
  主要是显示的图片前面有雪花点，并且显示的图片有错位。  
  ![图片显示错位](doc/图片显示错位.png)  
    
  ### 雪花点的出现
  - 原因分析  
  LTDC的显存中被写入了“其他数据”，一些不是目标图片的数据。这些数据是其他语句在执行过程中所产生的（如变量、堆栈等等），它们并不是按照特定格式排列的（像素数据需要按RGB888、RGB565等格式排列）。  
  当这些数据被写到显存中的时候，LTDC将它看成颜色数据并显示出来，所以显示出来的是杂色雪花点（有颜色是因为该内存中的数据恰好三个字节连在一起组成了RGB颜色值，所以在显示屏中有显示颜色）

  ### 图片的错位显示
  仔细观察图片，会发现错位的图片中右边“C”上面的短黑线是比左边的长黑线要高一个像素的，这两条线**不是在同一行**。说明图片并不是单纯的将左半边的部分平移到右边来了而已（乍一看的话很容易产生这种错觉）。  
  而出现这种情况的原因，跟DMA2D的工作方式有关。  
  
  #### DMA2D
  DMA2D中关于图片的大小主要有三个关键的参数：
  ```
  DMA2D_InitStruct.DMA2D_PixelPerLine // 指定一行的像素数，即图片的宽度
  DMA2D_InitStruct.DMA2D_NumberOfLine // 指定行的数量，即图片的高度
  DMA2D_InitStruct.DMA2D_OutputOffset // 一行的末尾到下一行起始的偏移量，值为LCD的宽度减去图片的宽度
  ```
  其中，DMA2D_OutputOffset是比较难理解的。为什么需要这么一个参数？这跟像素数据的存储有关。像素数据是按照特定排列存储在显存里的，而内存都是按照顺序存储的，先是按序存储一行的数据，然后紧接着存储下一行的数据。  
  这就导致了与我们的固有观念产生了极大的冲突：在显示屏上相邻的像素点，在存储位置上并不一定相邻。
  
  > 例如：对于像素点p（x,y），与他相邻的像素点p1（x+1，y）和p2（x-1，y）在存储位置上是相邻的（因为在同一行）；但是相邻像素点p3（x，y+1）跟p不在同一行，它们在存储位置上相差了（`LCD_PIXEL_WIDTH - 1`）个距离。类比一下二维数组，更好理解。
  
  所以，画一条竖线，在内存中并不是像画横线一样操作了一片**连续的**的存储空间，而是操作了一个像素点便**跳转**（LCD_PIXEL_WIDTH - 1）个像素点去操作下一个像素点，各个“离散的”像素点在LCD屏幕有序地显示出来，最终组成了一条看起来“连续的”的竖线。  

  #### 导致错位的原因
  图片数据前面被“其他数据”占用了，导致图片数据的起始点不是原点！第一个像素点在某一行末尾的时候才被载入。由于DMA2D配置的是读取到一行图片的数据就换行（DMA2D_InitStruct.DMA2D_PixelPerLine），但是前面被“其他数据”占用了“一行图片”的一部分。虽然我们肉眼看起来图片没有读够一行，但是DMA2D认为它已经读够一行了，所以换行继续读取下一行的图片数据。  

  而第一行图片的数据已经被读取了一部分，这就导致DMA2D在读取第二行数据的时候，虽然我们肉眼看起来已经读完一行图片了，但是DMA2D没有读够“一行图片”长度的数据，于是他便继续读显存下一部分的数据，即第二行图片的前一部分（更好地说明了像素数据在显存中是一行一行连续存储的）。  

  如此重复多次，就出现了我们看到的“图片错位”的情况。  

  ### 归因：冗杂数据从何而来
  
  #### 试错：不断地踩坑
  一开始以为是LTDC、LTDC_Layer或DMA2D的存储空间没有配置好，导致“写入数据”的位置与“显示数据”的位置产生了偏差。以为是LTDC_Layer的显示区域往前挪，显示了“图片所在显存位置”之外的某些东西。但是查了很多配置，都没发现有什么问题。也改过内存位置，想将显示区域向后挪动，不显示前面的雪花点，结果都无功而返。  
  在写着这段文字的时候，我也才反应过来：根本不可能是显示区域错位的问题。我之所以会以为是显示错位的问题是因为我以为整个显示屏就那一小块显示图片的区域（图片在显示屏中间，其他地方都是黑的）。但实际上整个屏幕都在显示显存中的东西（其他区域也有数据，只不过是黑的而已，看起来就好像没数据一样！），所以不可能是显示区域错位的问题，要是错位的画其他黑色的地方也会错位，而不只是图片的那一小块区域。  

  #### 图片地址指向错误的可能
  既然不可能是图片显示错位，那么只可能是搬运了错误的数据到显存。  
  我一开始以为那一片雪花点是BMP图片的信息头，所以想着删掉图片数组的前一部分就能消掉雪花点了。但是我将图片数组的前一半都删掉了，前面雪花点区域的大小一点没变，而雪花点后面的图片倒是的确减少了一半，这让我百思不得其解 —— 这一部分雪花点究竟来源何处？

  #### 官方例程的对照
  最后，我在搜资料的时候发现原来ST官方的标准库例程中有关于利用DMA2D显示图片的（我当初咋不知道！！在网上找了好久才发现有一开头的那个博客！！）。  
  于是我将博客上的例程与官方的例程逐行进行对照，乍一看都没啥问题，差点崩溃。不过在第N次检查时发现了一些不一样的东西：  
  > 博客上的：
  ```
  const unsigned char gImage_color[27648];

  //...........

  LCD_DisplayPicture(100, 100, 300, 124,gImage_color);

  //............

  void LCD_DisplayPicture(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height,const uint8_t *image){

    /* Configure Input Address */
    DMA2D_FG_InitStruct.DMA2D_FGMA = (uint32_t)*image;

  //..........
  }
  ```

  > 官方例程上的：
  ```
  const uint32_t ARGB4444_150x150[11250];
  
  //............

  static void DMA2D_Config(void){

    /* Configure Input Address */
    DMA2D_FG_InitStruct.DMA2D_FGMA = (uint32_t)&ARGB4444_150x150;

    //..........
  }
  ```

  经过对比，我发现对DMA2D_FGMA成员的操作，博客上用的给的图片数组的第一个成员，而官方例程给的是图片数组的地址。博客上的做法封装了一层函数，内聚性好，使用起来比较方便。

  书到用时方恨少，又开始后悔指针没有认真学。然后我写了个小的测试程序：
  ![指针地址验证](doc/指针地址验证.png)

  结果发现是因为指向的地址不对- - 把其他地方的数据写到了显存中。  

  但是很奇怪，这两个地址居然相差不是很多。要是差很多的话就完全显示不出来图片了才对，这样我觉得反而更好排查。结果只差了一点，搞得我老是以为是图片错位、信息头之类的。服了。  

  将博客例程中的语句修改如下：
  ```
   DMA2D_FG_InitStruct.DMA2D_FGMA = (uint32_t)image;
  ```

  图片可以正常显示。

  ## 开发日志 2019/09/28

  ### 实现功能
  - **新增：** 触摸图标事件响应
  - **新增：** 编写用电器状态转换的宏
    - 采用`宏拼接`和`条件运算符`的方式实现用电器状态与用电器图标数组名的等价转换
    - 相较于传统的分支语句这种写法使得程序*更加简洁*，*可读性更好*
  - **改进：** 图标结构体增加*用电器类型*、*用电器状态*、*用电器编号*等成员
  - **改进：** 优化并精简触摸画板主程序
  - **改进：** 显示多个图标
  - **删除：** 触摸画板中关于笔刷、画刷的部分
  - **删除：** 触摸画板中关于触摸轨迹显示的部分
  
  ### 待完成
  - 字体的显示 
    - 大字体字模的制作
    - 文字的刷新（用空格清除原来的文字）
    - 文字空白范围与背景层的混合叠加
  - 前景层与背景层的叠加混合
    - ARGB8888格式的LCD_Layer的配置
    - 透明度混合因子的配置
  - 图片数组的生成
    - RLE编码格式的图片数组解码

  ### 今日总结

  #### 实用的宏展开
  之前编程的时候经常遇到这样的情况：
  - 只有0、1两种状态的分支语句
  - 两种表示同样东西的变量相互转换的麻烦
  eg：
  ```
  int i = 0;
  ...
  if( i == 0){
    ...
  } else if ( i == 1){
    ...
  }
  ```
  这种是比较蠢且无脑的写法，下面这种稍好一点：
  ```
  int i = 0;
  ...
  if( i ){
    ...
  } else {
    ...
  }
  ```
  后来我发现就两种情况的话还是条件表达式最方便：
  ```
  int i = 0;
  ...
  ( i ) ? {do something...} : { do another thing...}; 
  ```
  还有就是更加经常遇到的一种情况：
  ```
  #define LIGHT 1
  #define FAN   2
  ...
  int device = LIGHT; 
  int command_light[10];
  int command_fan[10];
  ...
  void do_command(int* command_device);
  ```
  device被改变时，传入do_command的command数组也要求改变。这是就遇到了很纠结的情况：  
  
  
  一种方法是分支判断后，每一个语句块都用不同的参数调用函数；  

  另一种是引入中间量，每个分支只是将不同的参数给中间量，然后在程序末以中间量作为参数调用函数；  
  
  这两种方法对函数来说比较友好，但是对于特殊的`执行语句`就不太方便了。以本项目为例：
  ```
  #define DEVICE_STATUS_TOGGLE(device, status) (status) ? (gImage_##device##_off) : (gImage_##device##_on)

  ...
  switch (ptr->device){
    case DEVICE_FAN:
        gImage = DEVICE_STATUS_TOGGLE(fan, ptr->status);
        break;
    case DEVICE_LIGHT:
        gImage = DEVICE_STATUS_TOGGLE(light, ptr->status);
        break;
    ...
  }
  ```
  如果不用宏展开的话还需要在每个case中多加一个判断，着实麻烦。

  #### 结构体与函数指针
  函数指针的引入让C拥有了类似“接口”一样的特性，使用起来很方便。  
  同时，在结构体中的函数指针能直接使用结构体中的成员。无需再给定额外的参数（也无法指定）。  
  对于参数很多的函数来说使用起来很方便。

  #### 灵感：图标图片数组作为成员
  在整理上述文档的时候，我发现之所以我需要不断地判断用电器类型、用电器状态，是因为各个变量是独立的，没有内聚性！  
  图标图片数组明明就应该是属于图标的一部分才对，根本没必要把它声明成外部数组，只需要在结构体中增加一个指针成员，然后在初始化图标结构体的时候将对应的数组首地址赋给指针就行了。这样在调用绘图函数的时候就不用再判断了，直接将这个指针作为参数调用绘图函数就行了。  
  然后我又想到绘图函数也没必要再封装一层了，直接将LCD_DisplayPicture()的内容放到绘图函数里面就行了，坐标什么的都是结构体里的成员，更加方便了。  
  但是这样又有一个问题，就是不同像素格式的时候切换起来会很麻烦。  

  #### 叹服：宏定义和预编译if的精妙
  如果没有宏定义的话，需要将一个变量声明成外部变量，并且每个用到的文件都要包含该头文件。  
  而如果不用预编译if直接用普通if来分支的话，程序每次执行都要有一个判断，但是对于`配置类`的设置来说，这个配置是在程序开始之前就设定好且在程序执行期间不会改动的。但是普通if仍需每次执行都要判断一次，这种地方多了的话不利于程序的高效运行。  
  使用预编译if的话就不会存在这种问题，在编译期间就将`不符合条件的语句块`给“删掉”了。在程序执行期间不会再次判断，提升了程序的执行效率。
  > 这种方法适用于`配置类`的语句块，特别是这种配置的语句穿插在`各个不同的文件`之中。
  

























### 介绍
把地图信息相关的公用操作，写成了扩展（支持c/c++）  

### geohash编码算法

比如：成都永丰立交经纬度(30.63578,104.031601)

1.1. 纬度范围(-90, 90)平分成两个区间(-90, 0)、(0, 90)， 如果目标纬度位于前一个区间，则编码为0，否则编码为1。  
由于30.625265属于(0, 90)，所以取编码为1。  
然后再将(0, 90)分成 (0, 45), (45, 90)两个区间，而39.92324位于(0, 45)，所以编码为0，  
然后再将(0, 45)分成 (0, 22.5), (22.5, 45)两个区间，而39.92324位于(22.5, 45)，所以编码为1，  
依次类推可得永丰立交纬度编码为101010111001001000100101101010。  

1.2. 经度也用同样的算法，对(-180, 180)依次细分，(-180，0)、(0,180) 得出编码110010011111101001100000000000  

1.3. 合并经纬度编码，从高到低，先取一位经度，再取一位纬度；得出结果   `111001001100011111101011100011000010110000010001010001000100`

1.4. 用0-9、b-z（去掉a, i, l, o）这32个字母进行base32编码，得到(30.63578,104.031601)的编码为wm3yr31d2524。  


### 安装
1. `git clone https://github.com/weedge/geo.git`
2. `cd geo && phpize && ./configure && make && make install`
3. 将生成的geo.so拷贝到PHP 扩展模块文件夹中,然后修改php.ini配置文件,加入扩展,比如：  
        extension_dir="/usr/lib64/php/modules/"  
        geo  
        extension=geo.so
4. `php -m | grep geo` 查看是否有geo模块


### 测试实例

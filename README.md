### 介绍
把地图信息相关的公用操作，写成了扩展（支持c/c++）

### 安装
1. git clone https://github.com/weedge/geo.git
2. cd geo && phpize && ./configure && make && make install
3. 将生成的geo.so拷贝到PHP 扩展模块文件夹中,然后修改php.ini配置文件,加入扩展,比如：  
  `extension_dir="/usr/lib64/php/modules/"`  
  `geo`
  `extension=geo.so`
4. php -m | grep geo 查看是否有geo模块

### 测试实例

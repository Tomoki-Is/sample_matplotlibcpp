# sample_matplotlibcpp

```
++ main.cpp config.cpp -std=c++11 -IC:\Users\tomok\AppData\Local\Programs\Python\Python312\include -IC:\Users\tomok\AppData\Local\Programs\Python\Python312\Lib\site-packages\numpy\core\include
 -LC:\Users\tomok\AppData\Local\Programs\Python\Python312\libs -lpython312 -Wno-deprecated-declarations
```

```
 python -c "import sysconfig; print(sysconfig.get_paths()['include'])" 
 >> C:\Users\tomok\AppData\Local\Programs\Python\Python312\Include 
```

```
 python -c "import numpy; print(numpy.get_include())"  
 >> C:\Users\tomok\AppData\Local\Programs\Python\Python312\Lib\site-packages\numpy\core\include   
```
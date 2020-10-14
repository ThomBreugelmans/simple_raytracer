# simple_raytracer
A for now simple raytracer, because I was interested and bored ;)

It has a CLI where you can choose to update certain parameters (defaults are: size 1024x768, sampleAmount 5, filename "test.bmp", fov 0.2), to render it, or just to quit.
If you choose to render, it will show a percentage of how far it has progressed (not an accurate representation because if the objects are clustered somewhere it will need to cast a lot of rays). 
After rendering it will save it to a bitmap image and automatically open it for you (it might happen that the image is completely white, if this is the case, close the image viewer and reopen the image).

An example of some primitives rendered using default settings:

![example](https://github.com/ThomBreugelmans/simple_raytracer/blob/master/example.bmp?raw=true)

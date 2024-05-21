# KoboExtraFunk

Kobo Extra Fun~~c~~Ktionalities is a simple Kobo Ereader library aiming to restore functionalities from the unmaintained [Rain92's Qt Platform Abstraction](https://github.com/Rain92/qt5-kobo-platform-plugin) that got removed in the more recent [Quill-OS's Qt Platform Abstraction](https://github.com/Quill-OS/qt5-kobo-platform-plugin/). 

Such functionalities include : 
- Support for physical buttons
- Access to screen luminosity and tint 
- Access to battery level and usb charging status
-  Sleep cover events
- (basic) led activation
- (basic) Wifi management functions

# How to use ? 
```cpp
KoboPlatformExtra const& extraFunk = KoboPlatformExtra::Instance();
// TODO : write a proper how to, later on
```
# Thanks 
Thanks to Rain92 for its original QPA and bearing with my questions at the time.

Thanks to Quill-OS's team for their effort maintaining a modern QPA and writing an alternative OS for kobo eareaders.

Thanks to Niluje for their efforts with koreader, fbink, etc and sharing part of his knowledge with me.

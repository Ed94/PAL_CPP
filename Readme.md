PAL_CPP : Platform Abstraction Layer for C++

I made this for future projects of mine where I want the most ideal substrate in.
It also can serve as a good reference for learning c++.

I use or reference the following code for a good portion of the library :
[EASTL](https://github.com/electronicarts/EASTL)
[ZPL](https://github.com/zpl-c/zpl)

## Notes:

**Forced global includes**:
These are includes that have to pollute the global symbol space of all files that use the library.

Ussually I prevent the pollution by having specific headers only allowed in backend translation units
which implement the desired interface I want the library to have.

### MSVC:

Name					Reason						API Location		Interface
vcruntime_string.h		Memory op intrinsics		API.Windows.hpp		OSAL.Platform.hpp

### Clang:


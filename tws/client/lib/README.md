# Installation of libbid.a (Intel® Decimal Floating-Point Math Library)

## Debug
### resource 1
Compiler may complain about linkage error when compiling using the included libbid.a library (Intel® Decimal Floating-Point Math Library) from IBKR.

The solution is to download the source code from [here](https://www.netlib.org/misc/intel/) and recompile with the right platform.

Two modifications are needed for the successful compilation of the library.

```c
#include <stdlib.h>
```
needs to be added to each of the `bid21/64/128_pow.c` files in the LIBRARY/src

```c
# include <signal.h> // line 134 from <sys/signal.h> 
```
needs to be changed in `LIBRARY/float128/dpml_exception.c`

### resouce 2
[discussion on debugging](https://groups.io/g/twsapi/topic/96140048?p=%2C%2C%2C20%2C0%2C0%2C0%3A%3Arecentpostdate%2Fsticky%2C%2C%2C20%2C2%2C180%2C96140048)

## Records
the `libbid.a` in this folder is `gcc100libbid.a` after running `RUNOSX`.
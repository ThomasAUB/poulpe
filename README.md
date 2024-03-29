[![Build Status](https://travis-ci.com/ThomasAUB/Poulpe.svg?branch=master)](https://travis-ci.com/ThomasAUB/Poulpe)
[![License](https://img.shields.io/github/license/ThomasAUB/poulpe.svg)](LICENSE)

# Poulpe

<p align="center">
  <img width="64" height="64" src="icon.png">
</p>

  Poulpe is a header-only lightweight compile-time observer design pattern (c++17 or above).

  It allows to send and receive data and events (called signals) from one class to others without other coupling than the signal type.

- A class can be an emitter and a receiver at the same time.
- A class can emit an unlimited number of signals to an unlimited number of receivers.
- A class can receive an unlimited number of signals from an unlimited number of emitters.
- Signals are passed by reference which means they can be modified on the fly if received as non const.
- A signal emitted as const cannot be received as non const reference.
- Signals can be template classes


## How-To

  In order to make a class able to receive a signal of type **MySignal**, it has to implement the function ***void pReceive(MySignal);***

  Signal definition

```cpp
/// my_signal.h ///
// Example of an empty signal definition
struct MySignal{};
```

  Receiver definition
  
```cpp
/// my_receiver.h ///
#include "my_signal.h"
struct MyReceiver{
    void pReceive(MySignal s);
};
```

  The classes reponsible for emitting signals must take a typename **emitter** in template parameter

```cpp
/// my_emitter.h ///
#include "my_signal.h"
template<typename emitter_t>
struct MyEmitter{
    void test(){
        // signal send : 
        // the line below will call every receivers that implement 
        // the function "void pReceive(MySignal)"
        emitter_t::pEmit(MySignal());
    }
};
```

  Then add the receiver to the Poulpe declaration :

```cpp
/// main.cpp ///
#include "poulpe.h"
#include "my_receiver.h"
// list of receiver's types,
// there is no limitation on the number of receivers
DEFINE_RECEIVERS(MyReceiver)

// instantiation of the receiver
MyReceiver sRX;

// list of receiver's instances,
// it has to be listed in the same order as in DEFINE_RECEIVERS
CREATE_POULPE(sRX)
```

After the lines above, a type **Emitter** is declared, it is used to send the signals from emitters to receivers.

```cpp
/// main.cpp ///

int main(){
  // instantiation of the emitter
  MyEmitter<poulpe::Emitter> e;
  e.test();
  return 0;
}

```


## Multiple signals

Poulpe automatically handles multiple signal types and calls only the receivers that implement the right pReceive function.

```cpp
// Example of empty signal definitions
struct MySignalA{};

struct MySignalB{};
```


```cpp
struct MyReceiverA{
    void pReceive(MySignalA s);
};

struct MyReceiverB{
    void pReceive(MySignalB s);
};
```

  Then add the receiver to the Poulpe declaration :

```cpp
// list of receiver's types,
// there is no limitation on the number of receivers
DEFINE_RECEIVERS(MyReceiverA, MyReceiverB)

// instantiation of the receivers
MyReceiverA sRXA;

MyReceiverB sRXB;

// list of receiver's instances,
// it has to be listed in the same order as in DEFINE_RECEIVERS
CREATE_POULPE(sRXA, sRXB)
```

```cpp
template<typename emitter_t>
struct MyEmitter{

    void testA(){        
        // the line below will call every receivers that implement 
        // the function "void pReceive(MySignalA)"
        emitter_t::pEmit(MySignalA());
    }

    void testB(){
        // the line below will call every receivers that implement 
        // the function "void pReceive(MySignalB)"
        emitter_t::pEmit(MySignalB());
    }
};

int main(){
  // instantiation of the emitter
  MyEmitter<poulpe::Emitter> e;

  e.testA();

  e.testB();

  return 0;
}
```

## Template signals
It's possible to send templated signals.

```cpp
template<int I>
struct MySignal{};
```

```cpp
struct MyReceiverA{
  // this function will be called for every MySignal<5> sent
  void pReceive(MySignal<5> s);
};

struct MyReceiverB{
  // this function will be called for every MySignal<I> sent
  template<int I>
  void pReceive(MySignal<I> s);
};

struct MyReceiverC{
  // this function will be called for any type sent
  template<typename signal_t>
  void pReceive(signal_t s);
};
```

## Modifying the signals on the fly
Signals are passed by reference, which means that they can be modified on the fly by receivers.

```cpp
struct GetValue{ bool m; };
```

```cpp
struct MyReceiver{
  void pReceive(GetValue& s) {
    s.m = true;
  }
};
```

```cpp
template<typename emitter_t>
struct MyEmitter{

    void getValue(){        

        // instantiation of the signal
        GetValue gv;

        // init the member
        gv.m = false;

        // emitt signal
        emitter_t::pEmit(gv);

        // gv.m is now true
        if(!gv.m) {
          // error
        }

    }
};

int main(){
  // instantiation of the emitter
  MyEmitter<poulpe::Emitter> e;

  e.getValue();

  return 0;
}
```

## Emitter/Receiver
A class can be an emitter and a receiver at the same time

```cpp
struct MySignalA{};

struct MySignalB{};
```


```cpp
template<typename emitter_t>
struct MyRxTx{

  void test(){
    emitter_t::pEmit(MySignalA());
  }

  void pReceive(MySignalB s){
    // do stuff
  }

};
```

```cpp
DEFINE_RECEIVERS(MyRxTx<poulpe::Emitter>)

MyRxTx<poulpe::Emitter> sRX;

CREATE_POULPE(sRX)
```

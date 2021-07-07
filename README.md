[![Build Status](https://travis-ci.com/ThomasAUB/Poulpe.svg?branch=master)](https://travis-ci.com/ThomasAUB/Poulpe)

# Poulpe

<p align="center">
  <img width="64" height="64" src="icon.png">
</p>

  Poulpe is a compile-time observer design pattern (c++17 or above).

  It allows to send and receive data and events from one class others without other coupling than the signal type.

## How-To

  In order to make a class able to receive a signal of type **MySignal**, it has to implement the function ***void pReceive(MySignal);***

```cpp
struct MyReceiver{
    void pReceive(MySignal s);
}
```

  Then add the receiver to the Poulpe declaration :

```cpp
// list of receivers,
// there is no limitation on the number of receivers
DEFINE_RECEIVERS(MyReceiver)

// instantiation of the receiver
MyReceiver sRX;

// instantiation of poulpe, 
// it has to be listed in the same order as in DEFINE_RECEIVERS
CREATE_POULPE(sRX)
```

After the lines above, a type **Emitter** is declared, it is used to send the signals from emitters to receivers.

```cpp
template<typename emitter_t>
struct MyEmitter{
    void test(){
        // instantiation of the signal
        MySignal s;
        // signal send
        emitter_t::pEmit(s);
    }
};

int main(){
  // instantiation of the emitter
  MyEmitter<Emitter> e;
  e.test();
  return 0;
}
```
** - A class can be an emitter and a receiver at the same time.**
** - A class can emit an unlimited number of signals to an unlimited number of receivers.**
** - A class can receive an unlimited number of signals from an unlimited number of emitters.**

The signals are sent by reference which means that if they are received as non const reference, the receivers are able to modify the signals on the fly.


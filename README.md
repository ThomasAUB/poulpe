[![Build Status](https://travis-ci.com/ThomasAUB/Poulpe.svg?branch=master)](https://travis-ci.com/ThomasAUB/Poulpe)

# Poulpe

<p align="center">
  <img width="64" height="64" src="icon.png">
</p>

  Poulpe is a compile-time observer design pattern.

  It allows to statically define receivers by implementing a **pReceive** function.

## How

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

// instancation of the receiver
MyReceiver sRX;

// instanciation of poulpe, 
// it has to be listed in the same order as in DEFINE_RECEIVERS
CREATE_POULPE(sRX)
```

After the lines above, a type **Emitter** is declared, it is used to send the signals from emitters to receivers, a class can be an emitter and a receiver at the same time.

```cpp
template<typename emitter_t>
struct MyEmitter{
    void test(){
        // instanciation of the signal
        MySignal s;
        // signal send
        emitter_t::pEmit(s);
    }
};

int main(){
  // instanciation of the emitter
  MyEmitter<Emitter> e;
  e.test();
  return 0;
}
```

The signals are sent by reference which means that if they are received as non const reference, the receivers are able to modify the signals on the fly.


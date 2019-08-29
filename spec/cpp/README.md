# This is libbft on C++

## Basic build

Just enter `build` and use cmake.

```
cd spec/cpp/build
cmake ..
make
```

To quickly test it:
```
cd build
./app_test
```

Quick test using RPC:
```
make cpp && ./spec/cpp/bin/runRPCtest.sh 
```

## Elaborate build: gRPC and protobuf

It's possible to build without gRPC, in a raw prototype version, however a real RPC can be build using gRPC and protobuf libraries.

### Dependencies for gRPC and protobuf

First advice: if you have `protobuf` installed, remove it... it can cause strange conflicts with gRPC (unfortunately this took many hours from me).
You can try to keep many versions, at your own risk, I had problems when managing `3.8.0` (from gRPC) together with `3.9.0` (local version).

Official instructions to install gRPC: https://github.com/grpc/grpc/blob/master/BUILDING.md

Things you may need (linux/ubuntu):
```
apt-get install build-essential autoconf libtool pkg-config
apt-get install libgflags-dev libgtest-dev
apt-get install clang libc++-dev
```

My advice is (it will take around 3 GB from your computer, be prepared!):

```
cd ../../
git submodule update --init --recursive   # get submodules for libbft (including grpc)
cd spec/cpp/grpc/third_party/protobuf     # install protobuf first
git submodule update --init --recursive   # get submodules (for protobuf)
./autogen.sh                              # generate installation files
./configure
make
make check                                # if this fails, something bad happened
sudo make install                         # this will install system-wide protobuf and protoc (compiler)
sudo ldconfig                             # refresh shared library cache
cd ../..                                  # back to grpc
make
sudo make install                         # install grpc (and also /usr/local/bin/grpc_cpp_plugin)
```

### Test it

`protoc --version`  should be `libprotoc 3.8.0`

`ls -la /usr/local/lib/libproto*` should have `/usr/local/lib/libprotobuf.so -> libprotobuf.so.19.0.0`

**Warning:** I had a **huge** problem when two versions existed here... `19.0.0` and `20.0.0`, so if this happens with you, get rid of `20.0.0` before it's too late!

If you want to test `protobuf`:
```
cd grpc/third_party/protobuf/examples
make clean
make
touch book.txt
./add_person_cpp book.txt   # put some data
./list_people_cpp book.txt  # get info
```

If you want to test `grpc`, do this simple test (_huge thanks to the authors of `https://medium.com/@andrewvetovitz/grpc-c-introduction-45a66ca9461f`_):
```
cd spec/cpp/test-grpc
make clean
make
./server   # open ./client in other window
```

If this works, you're good to go! \o/

## Final build: libbft + gRPC

```
cd spec/cpp/src/bftevents-grpc
make
./bftserver # open ./bftclient in another terminal

```


## License

libbft is MIT License (although some specific components can have distinct licenses...)

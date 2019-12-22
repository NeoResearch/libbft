LDFLAGS = -L/usr/local/lib `pkg-config --libs protobuf grpc++`\
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl

CXX = g++
CPPFLAGS1 += `pkg-config --cflags protobuf grpc`
CXXFLAGS1 += -std=c++17

GRPC_CPP_PLUGIN = grpc_cpp_plugin
GRPC_CPP_PLUGIN_PATH ?= `which $(GRPC_CPP_PLUGIN)`

all: bftclient bftserver

bftclient: bftevent.pb.o bftevent.grpc.pb.o bftclient.o
	$(CXX) $^ $(LDFLAGS) -o $@

bftserver: bftevent.pb.o bftevent.grpc.pb.o bftserver.o
	$(CXX) $^ $(LDFLAGS) -o $@

%.grpc.pb.cc: %.proto
	protoc --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN_PATH) $<

%.pb.cc: %.proto
	protoc --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h bftclient bftserver

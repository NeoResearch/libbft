projects_folder=spec

all: cpp_bazel # cpp_cmake
	
#	(cd ~/go/src/github.com/NeoResearch/libbft && dep ensure && go run main.go)

cpp_bazel:
	(cd $(projects_folder)/cpp/ && bazel build ...)
	valgrind $(projects_folder)/cpp/bazel-bin/app_test   # bazel run //:app_test

cpp_cmake:
	#export BFTEVENTS=`pwd`/spec/cpp/src/bftevents-grpc
	(mkdir -p $(projects_folder)/cpp/build && cd $(projects_folder)/cpp/build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && make)
	valgrind ./$(projects_folder)/cpp/build/app_test

cpp_test: cpp
	@echo "Performing basic tests now"
	@echo
	(cd $(projects_folder)/cpp/build/tests && make test)

go:
	(cd ~/go/src/github.com/NeoResearch/libbft && dep ensure && go build *.go)

go_test: go
	(cd ~/go/src/github.com/NeoResearch/libbft && go test ./...)

csharp:
	(cd $(projects_folder)/csharp && dotnet build)

test: cpp_test
	@echo
	@echo "Generating coverage (see tests/reports/)"
	@echo
	#(cd tests && make test-coverage)

	@echo
	@echo "Performing hard tests now... this will take a while!"
	@echo
	#(cd tests && make test-hard)

clean:
	(cd $(projects_folder)/cpp/build && make clean)

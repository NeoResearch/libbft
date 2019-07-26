all: cpp go
	valgrind ./src/cpp/build/app_test
	valgrind go run main.go

cpp:
	(mkdir -p src/cpp/build && cd src/cpp/build && cmake .. && make)

go: src/go/*
	(cd ~/go/src/github.com/libbft && dep ensure && go build *.go)

csharp:
	(cd src/csharp && dotnet build)

test: cpp
	@echo "Performing basic tests now"
	@echo
	(cd src/cpp/build/tests && make test)
	@echo
	@echo "Generating coverage (see tests/reports/)"
	@echo
	#(cd tests && make test-coverage)

	@echo
	@echo "Performing hard tests now... this will take a while!"
	@echo
	#(cd tests && make test-hard)

clean:
	rm src/cpp/build/*.so

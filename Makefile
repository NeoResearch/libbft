projects_folder=spec

all: cpp
	valgrind ./$(projects_folder)/cpp/build/app_test
#	(cd ~/go/src/github.com/libbft && dep ensure && go run main.go)

cpp:
	(mkdir -p $(projects_folder)/cpp/build && cd $(projects_folder)/cpp/build && cmake .. && make)

go:
	(cd ~/go/src/github.com/libbft && dep ensure && go build *.go)

csharp:
	(cd $(projects_folder)/csharp && dotnet build)

test: cpp
	@echo "Performing basic tests now"
	@echo
	(cd $(projects_folder)/cpp/build/tests && make test)
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

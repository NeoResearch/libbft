all: cpp
	valgrind ./src/cpp/build/app_test

cpp:
	(mkdir -p src/cpp/build && cd src/cpp/build && cmake .. && make)

go:
	(cd src/go && go build)

csharp:
	(cd src/csharp && dotnet build)

test:
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

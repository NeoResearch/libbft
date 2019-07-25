all:  base
	./bin/app_test

base:
	g++ -std=c++17 -Ofast src/cpp/main.cpp -o bin/app_test

test:
	@echo "Performing basic tests now"
	@echo
	(cd tests && make test)
	@echo
	@echo "Generating coverage (see tests/reports/)"
	@echo
	#(cd tests && make test-coverage)

	@echo
	@echo "Performing hard tests now... this will take a while!"
	@echo
	#(cd tests && make test-hard)
clean:
	rm build/*.so 

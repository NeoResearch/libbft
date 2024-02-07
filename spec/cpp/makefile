all: build_with_clang_tidy

build:
	bazel build //...

build_with_clang_tidy:
	# bazel build //:app_test --config clang-tidy # --verbose_failures
	bazel build //... --config clang-tidy

refresh_commands_json:
	bazel run @hedron_compile_commands//:refresh_all



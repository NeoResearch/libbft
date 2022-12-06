
package(default_visibility = ["//visibility:public"])

load("@rules_cc//cc:defs.bzl", "cc_binary")

# SETUP CLANG TIDY
filegroup(
       name = "clang_tidy_config",
       srcs = [".clang-tidy"],
       visibility = ["//visibility:public"],
)

####################
# binary:  app_test
####################

cc_binary(
    name = "app_test",
    srcs = ["src/demo/main.cpp"],
    defines = ["BAZEL_BUILD"],
    deps = [
        "//include:LibBFT",
    ],
)

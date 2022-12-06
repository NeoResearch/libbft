load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

local_repository(
    name = "LibBFT",
    path = "spec/cpp/",
)

http_archive(
    name = "com_github_grpc_grpc",
    urls = [
        "https://github.com/grpc/grpc/archive/0a82c02a9b817a53574994374dcff53f2e961df2.tar.gz",
    ],
    sha256 = "92fdb11e9bdc770e16caedf9c13d1688ce476a0c763c11d3871781af3881ac28",
    strip_prefix = "grpc-0a82c02a9b817a53574994374dcff53f2e961df2",
)
load("@com_github_grpc_grpc//bazel:grpc_deps.bzl", "grpc_deps")
grpc_deps()
load("@com_github_grpc_grpc//bazel:grpc_extra_deps.bzl", "grpc_extra_deps")
grpc_extra_deps()
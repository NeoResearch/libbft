0)for app_test (no grpc):
bazel build //src:app_test

0) testing only (before grpc and protobuf stuff)
bazel build @com_github_grpc_grpc//:grpc++
bazel build @com_google_protobuf//:protoc_lib

1)for bftevents-grpc:
bazel build //src/bftevents-grpc:cc_bftclient
bazel build //src/bftevents-grpc:cc_bftserver

2)for dbft-test-rpc:
bazel build //src/dbft-test-rpc:main_test_rpc


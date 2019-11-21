cc_library(
    name = "storage",
    hdrs = ["storage.h"],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "expression",
    hdrs = ["expression.h"],
    visibility = ["//visibility:public"],
    deps = [":storage"],
)

cc_library(
    name = "value",
    hdrs = ["value.h"],
    visibility = ["//visibility:public"],
    deps = [":expression"],
)

cc_library(
    name = "boolean",
    hdrs = ["boolean.h"],
    visibility = ["//visibility:public"],
    deps = [":value"],
)

cc_library(
    name = "natural",
    hdrs = ["natural.h"],
    visibility = ["//visibility:public"],
    deps = [":value"],
)

cc_library(
    name = "operation",
    hdrs = ["operation.h"],
    visibility = ["//visibility:public"],
    deps = [":expression"],
)

cc_test(
    name = "expression_test",
    srcs = ["expression_test.cc"],
    deps = [
        ":boolean",
        ":expression",
        ":natural",
        ":operation",
        "@googletest//:gtest_main",
    ],
)

load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "ray_tracer",
    srcs = ["main.cc"],
    deps = [
        "//common:ray",
        "//common:color",
        "//common:constants",
        "//common:sphere",
        "//common:camera",
        "//common:utils",
        "//common:thread_pool",
    ],
)
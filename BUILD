load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

cc_binary(
    name = "ray_tracer",
    srcs = ["main.cc"],
    deps = [
        "//canvas:ray",
        "//canvas:color",
        "//common:constants",
        "//canvas:sphere",
        "//canvas:camera",
    ],
)
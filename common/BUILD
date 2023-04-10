load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "vec3",
    hdrs = ["vec3.hh"],
)

cc_library(
    name = "ray",
    hdrs = ["ray.hh"],
    deps = [":vec3"],
)

cc_library(
    name = "color",
    hdrs = ["color.hh"],
    deps = [":vec3"],
)
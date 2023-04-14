load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "vec3",
    hdrs = ["vec3.hh"],
)

cc_library(
    name = "constants",
    hdrs = ["constants.hh"],
)

cc_library(
    name = "ray",
    hdrs = ["ray.hh"],
    deps = [":vec3"],
)

cc_library(
    name = "utils",
    hdrs = ["utils.hh"],
    deps = [":constants"],
)


cc_library(
    name = "color",
    hdrs = ["color.hh"],
    deps = [":vec3"],
)

cc_library(
    name = "object",
    srcs = ["object.cc"],
    hdrs = ["object.hh"],
    deps = [":ray"],
)

cc_library(
    name = "sphere",
    srcs = ["sphere.cc"],
    hdrs = ["sphere.hh"],
    deps = [":object"],
)


cc_library(
    name = "camera",
    hdrs = ["camera.hh"],
    deps = [
        ":constants", 
        ":ray", 
    ],
)


cc_library(
    name = "thread_pool",
    hdrs = ["thread_pool.hh"]
)
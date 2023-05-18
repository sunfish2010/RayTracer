load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_library")

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "ray",
    hdrs = ["ray.hh"],
    deps = ["//common:vec3"],
)

cc_library(
    name = "color",
    hdrs = ["color.hh"],
    deps = ["//common:vec3"],
)

cc_library(
    name = "object",
    srcs = ["object.cc"],
    hdrs = ["object.hh"],
    deps = [
        ":ray", 
        ":bbox",
    ],
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
        "//common:constants", 
        ":ray", 
    ],
)

cc_library(
    name = "bbox",
    hdrs = ["bbox.hh"],
    deps = [
        ":ray",
    ]
)


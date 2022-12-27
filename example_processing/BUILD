package(default_visibility = ["//visibility:public"])

cc_library(
    name = "example_processing",
    srcs = ["example_processing.c"],
    hdrs = ["example_processing.h"],
    deps = ["//:plugin_processor"],
)

cc_binary(
    name = "gen_parameter",
    srcs = ["gen_parameter.c"],
    deps = [":example_processing"],
)

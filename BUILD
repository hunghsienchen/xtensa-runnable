package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "run_example_processing",
    srcs = ["main.c"],
    deps = [
        ":plugin_processor",
        "//example_processing:example_processing",
    ],
)

cc_library(
    name = "plugin_processor",
    hdrs = ["plugin_processor.h"],
)

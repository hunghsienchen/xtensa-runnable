#ifndef CHROMEOS_AUDIO_EXAMPLE_PROCESSING_H_
#define CHROMEOS_AUDIO_EXAMPLE_PROCESSING_H_

#include "plugin_processor.h"

struct example_config {
    struct plugin_processor_config config;
    int int_config;
    float float_config;
};

struct example_parameter {
    int mute_duration;
    float volume;
};

struct example_processor {
    struct plugin_processor processor;
    struct plugin_processor_config config;
    struct example_parameter parameter;
    int num_processed_block;
    int mute_switch;
};

#endif  // CHROMEOS_AUDIO_EXAMPLE_PROCESSING_H_

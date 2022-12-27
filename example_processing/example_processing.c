#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "plugin_processor.h"
#include "example_processing.h"

static enum status run(struct plugin_processor* p,
                       const struct multi_slice* input,
                       struct multi_slice* output) {
  struct example_processor* processor = (struct example_processor*)p;
  output->channels = input->channels;
  output->num_frames = input->num_frames;
  output->data[0] = calloc(input->num_frames, sizeof(float));
  if (output->data[0] == NULL) {
    return ErrOutOfMemory;
  }

  int i;
  for (i = 0; i < input->num_frames; i++) {
    if (processor->mute_switch) {
      output->data[0][i] = 0.0f;
    } else {
      output->data[0][i] = input->data[0][i] * processor->parameter.volume;
    }
  }
  processor->num_processed_block++;
  if (processor->parameter.mute_duration > 0 && !(processor->num_processed_block % processor->parameter.mute_duration)) {
    processor->mute_switch = !(processor->mute_switch);
  }
  return StatusOk;
}

static enum status destroy(struct plugin_processor* p) {
  free((struct example_processor*)(p));
  return StatusOk;
}

static const struct plugin_processor_ops ops = {
    .run = run,
    .destroy = destroy,
};

enum status parse_parameter(struct example_processor *processor, const struct raw_parameter* raw_parameter) {
  if (raw_parameter == NULL) {
    processor->parameter.mute_duration = 0;
    processor->parameter.volume = 1.0f;
  } else {
    memcpy(&(processor->parameter), raw_parameter, sizeof(processor->parameter));
  }
  assert(processor->parameter.mute_duration >= 0);
  assert(processor->parameter.volume >= 0.0f && processor->parameter.volume <= 1.0f);
  return StatusOk;
}

enum status processor_create(
    struct plugin_processor** out,
    const struct plugin_processor_config* config,
    const struct raw_parameter* raw_parameter) {
  struct example_processor *processor = calloc(1, sizeof(*processor));
  processor->config = *config;

  enum status ret = parse_parameter(processor, raw_parameter);
  if (ret != StatusOk) {
    return ret;
  }

  *out = (struct plugin_processor*) processor;
  (*out)->ops = &ops;
  return StatusOk;
}

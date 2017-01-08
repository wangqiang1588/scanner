#include "scanner/eval/pipeline_description.h"
#include "scanner/evaluators/movie_analysis/histogram_evaluator.h"
#include "scanner/evaluators/video/decoder_evaluator.h"

namespace scanner {
namespace {
PipelineDescription get_pipeline_description(const DatasetInformation& info) {
  PipelineDescription desc;
  Sampler::all_frames(info, desc);

  DeviceType device_type;
  VideoDecoderType decoder_type;

#ifdef HAVE_CUDA
  device_type = DeviceType::GPU;
  decoder_type = VideoDecoderType::NVIDIA;
#else
  device_type = DeviceType::CPU;
  decoder_type = VideoDecoderType::SOFTWARE;
#endif

  std::vector<std::unique_ptr<EvaluatorFactory>>& factories =
      desc.evaluator_factories;

  factories.emplace_back(
    new DecoderEvaluatorFactory(DeviceType::CPU, VideoDecoderType::SOFTWARE));
  factories.emplace_back(new HistogramEvaluatorFactory(device_type));

  return desc;
}
}

REGISTER_PIPELINE(histogram, get_pipeline_description);
}

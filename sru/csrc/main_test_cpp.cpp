
#include <torch/script.h> // One-stop header.

#include <iostream>
#include <memory>
#include <typeinfo>

int main(int argc, const char* argv[]) {
  if (argc != 2) {
    std::cerr << "usage: example-app <path-to-exported-script-module>\n";
    return -1;
  }

  // Deserialize the ScriptModule from a file using torch::jit::load().
  torch::jit::script::Module module = torch::jit::load(argv[1]);

  auto input_size = module.attr("input_size").toInt();
  auto hidden_size = module.attr("hidden_size").toInt();
  
  std::vector<torch::jit::IValue> inputs;
  inputs.push_back(torch::ones({3, 2, input_size}));
  auto outputs = module.forward(std::move(inputs));
  auto h = outputs.toTuple()->elements()[0].toTensor();
  auto c = outputs.toTuple()->elements()[1].toTensor();

  for (int i = 0; i < 3; ++i)
      for (int j = 0; j < 2; ++j)
          for (int k = 0; k < hidden_size; ++k)
              std::cout << std::setprecision(4) << h[i][j][k].item<float>() << ' ';
  std::cout << std::endl;

  for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 2; ++j)
          for (int k = 0; k < hidden_size; ++k)
              std::cout << std::setprecision(4) << c[i][j][k].item<float>() << ' ';
  std::cout << std::endl;
}

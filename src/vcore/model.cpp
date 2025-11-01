#include <vcore/model.hpp>

namespace vcore {
  Model::Model() {
    
  }

  vobj::Operation &Model::addOp(std::source_location sloc, std::string content) {
    ops.emplace_back(ops.size(), sloc.line(), sloc.column(), content);
    return ops.back();
  }

  void Model::update() {
    // TODO
  }
}
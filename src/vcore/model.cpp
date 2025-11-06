#include <vcore/model.hpp>

namespace vcore {
  Model::Model() {
    
  }

  vobj::Operation &Model::addOp(std::source_location sloc, std::string content) {
    ops.emplace_back(ops.size(), sloc.line(), sloc.column(), content);
    return ops.back();
  }

  void Model::update(std::source_location sloc) {
    std::cout << "MODEL UPDATE " << sloc.line() << std::endl;
    ++vobj::Display::globalUpdateTick;
    ops.emplace_back(ops.size(), lastUpdateLine, lastUpdateOffset, sloc.line(), sloc.column(), "update values");
    root->update(ops.back());
    if (ops.back().comps.empty()) ops.pop_back();
    lastUpdateLine = sloc.line();
    lastUpdateOffset = sloc.column();
  }
}
#include <vcore/model.hpp>

namespace vcore {
  Model::Model() {

  }

  vobj::Operation &Model::addOp(std::source_location sloc, std::string content) {
    ops.emplace_back(ops.size(), sloc.line(), sloc.column(), sloc.file_name(), content);
    return ops.back();
  }

  bool Model::update(std::source_location sloc) {
    std::cout << "MODEL UPDATE " << sloc.line() << std::endl;
    ++vobj::Display::globalUpdateTick;
    ops.emplace_back(ops.size(), lastUpdateLine, lastUpdateOffset, lastUpdatedFileName, sloc.line(), sloc.column(), sloc.file_name(), "update values");
    root->update(ops.back());
    bool updated = !ops.back().comps.empty();
    if (!updated) ops.pop_back();
    lastUpdateLine = sloc.line();
    lastUpdateOffset = sloc.column();
    return updated;
  }
}
#include <vcore/model.hpp>

namespace vcore {
  Model::Model() {

  }

  vobj::Operation &Model::addOp(std::source_location sloc, std::string content) {
    ops.emplace_back(ops.size(), content, vobj::LocInfo(sloc));
    return ops.back();
  }

  bool Model::update(std::source_location sloc) {
    vobj::LocInfo loc(sloc);
    std::cout << "MODEL UPDATE " << sloc.line() << std::endl;
    ++vobj::Display::globalUpdateTick;
    ops.emplace_back(ops.size(), "update values", lastLoc, loc);
    root->update(ops.back());
    bool updated = !ops.back().comps.empty();
    if (!updated) ops.pop_back();
    lastLoc = loc;
    return updated;
  }
}
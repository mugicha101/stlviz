#include "vobj/root_display.hpp"
#include <iostream>

sf::FloatRect padbbox(sf::FloatRect bbox, float amt) {
  bbox.position -= {amt, amt};
  bbox.size += sf::Vector2f{amt, amt} * 2.f;
  return bbox;
};

namespace vobj {
  RootDisplay::RootDisplay() {
    alive = true; // always alive
  }

  std::vector<std::shared_ptr<Display>> RootDisplay::topLevelDisplays() const {
    std::vector<std::shared_ptr<Display>> tld;
    tld.reserve(getNumAlive());
    for (std::shared_ptr<Display> d : Display::displays) {
      if (d->parent || !d->isAlive() || d.get() == this) continue;

      tld.push_back(d);
    }
    std::sort(tld.begin(), tld.end(), [](const std::shared_ptr<Display> &a, const std::shared_ptr<Display> &b) {
      return a->priority == b->priority ? a->uid < b->uid : a->priority < b->priority;
    });
    return tld;
  }

  void RootDisplay::draw() {
    resetCanvas(size.x, size.y);
    auto tlds = topLevelDisplays();
    float invCamZoom = 1.f / camZoom;

    // first locate any new displays and assign them positions
    // do this by pushing away from others
    std::vector<std::shared_ptr<Display>> newDisplays;
    for (std::shared_ptr<Display> d : tlds) {
      if (d->needInitPos) {
        newDisplays.push_back(d);
      }
    }
    if (!newDisplays.empty()) {
      // move displays down and right until no overlaps
      for (size_t i = 0; i < newDisplays.size(); ++i) {
        bool done = false;
        while (!done) {
          done = true;
          auto nd = newDisplays[i];
          
          sf::FloatRect ndb = padbbox(static_cast<sf::FloatRect>(nd->getBBox()), 25.f);
          ndb.position += nd->pos;
          for (auto d : tlds) {
            if (nd == d || d->needInitPos) continue;
            
            sf::FloatRect db = padbbox(static_cast<sf::FloatRect>(d->getBBox()), 25.f);
            db.position += d->pos;
            if (!ndb.findIntersection(db)) continue;

            done = false;
            sf::Vector2f delta = db.position + db.size - ndb.position;
            if (delta.x <= delta.y) {
              nd->pos.x += delta.x;
            } else {
              nd->pos.y += delta.y;
            }
          }
          
          nd->needInitPos = false;
        }
      }
    }

    for (std::shared_ptr<Display> d : tlds) {
      bbox = d->getBBox();

      // draw name
      sf::Font &font = Display::getFont();
      sf::Text text(font);
      text.setString(d->name);
      text.setCharacterSize(FONT_SIZE);
      float charWidth = font.getGlyph('0', FONT_SIZE, false).advance;
      float nameWidth = charWidth * (float)text.getString().getSize();
      float nameHeight = (float)font.getLineSpacing(FONT_SIZE);
      text.setFillColor(sf::Color::Black);
      text.setPosition(world2screen({d->pos.x, d->pos.y - nameHeight * 0.5f}));
      text.setOrigin({0.f, nameHeight * 0.5f});
      text.setScale({invCamZoom, invCamZoom});
      canvas.draw(text);
      canvas.display();

      // draw display
      // don't use drawOn since doesn't support transforms
      sf::Sprite sprite(d->getTexture());
      sprite.setPosition(world2screen(d->pos));
      sprite.setScale({invCamZoom, invCamZoom});
      canvas.draw(sprite);
    }
    canvas.display();
  }

  bool RootDisplay::update(Operation &op) {
    for (std::shared_ptr<Display> d : Display::displays) {
      // Skip displays that have a parent, are not alive, or are this RootDisplay itself
      if (d->parent || !d->isAlive() || d.get() == this) continue;

      std::cout << "UPDATE TOP LEVEL " << d->uid << std::endl;
      d->update(op);
    }
    return false;
  }

  std::shared_ptr<Display> RootDisplay::at(sf::Vector2f worldPos) const {
    auto tlds = topLevelDisplays();
    for (auto it = tlds.rbegin(); it != tlds.rend(); ++it) {
      std::shared_ptr<Display> d = *it;
      sf::FloatRect bbox = static_cast<sf::FloatRect>(d->getBBox());
      if (bbox.contains(worldPos - d->pos)) return d;
    }
    return nullptr;
  }

  sf::Vector2f RootDisplay::screen2world(sf::Vector2f screenPos) const {
    return screenPos * camZoom + camPosition;
  }

  sf::Vector2f RootDisplay::world2screen(sf::Vector2f worldPos) const {
    return (worldPos - camPosition) / camZoom;
  }
}

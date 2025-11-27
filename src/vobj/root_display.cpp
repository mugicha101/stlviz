#include "vobj/root_display.hpp"
#include "vstd/base.hpp"
#include "vobj/primitive.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

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

  std::vector<std::shared_ptr<Display>> RootDisplay::topLevelDisplays(std::function<bool(std::shared_ptr<Display>)> filter) const {
    std::vector<std::shared_ptr<Display>> tld;
    tld.reserve(getNumAlive());
    for (std::shared_ptr<Display> d : Display::displays) {
      if (d->parent || !d->isAlive() || d.get() == this) continue;
      if (filter && !filter(d)) continue;

      tld.push_back(d);
    }
    std::sort(tld.begin(), tld.end(), [](const std::shared_ptr<Display> &a, const std::shared_ptr<Display> &b) {
      return a->priority == b->priority ? a->uid < b->uid : a->priority < b->priority;
    });
    return tld;
  }

  void RootDisplay::draw() {
    resetCanvas(size.x, size.y);
    auto tlds = displayFilter ? topLevelDisplays(displayFilter) : topLevelDisplays();
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
      // Position new displays using a grid-based approach to avoid overlaps
      // Only search in the bottom-right quadrant (visible area)
      for (size_t i = 0; i < newDisplays.size(); ++i) {
        auto nd = newDisplays[i];

        // Start from a visible position on screen (top-left corner with some padding)
        sf::Vector2f startPos = camPosition + sf::Vector2f{50.f, 50.f};

        // Try positions in a spiral pattern outward from starting position
        const float gridStep = 50.f;
        sf::FloatRect ndb = padbbox(static_cast<sf::FloatRect>(nd->getBBox()), 25.f);

        bool positionFound = false;
        int maxRadius = 20; // Maximum search radius

        for (int radius = 0; radius < maxRadius && !positionFound; ++radius) {
          // Try positions at this radius, but only in bottom-right quadrant
          int steps = std::max(1, radius * 8); // More positions at larger radii

          for (int step = 0; step < steps && !positionFound; ++step) {
            // Calculate candidate position in a circular pattern
            float angle = (float)step / (float)steps * 2.f * 3.14159265f;

            // Restrict to bottom-right quadrant only (angles 0 to π/2)
            // Map step to angle range [0, π/2] instead of [0, 2π]
            angle = (float)step / (float)steps * 3.14159265f * 0.5f;

            sf::Vector2f candidatePos = {
              startPos.x + std::cos(angle) * (float)radius * gridStep,
              startPos.y + std::sin(angle) * (float)radius * gridStep
            };

            // Check if this position causes any overlaps
            ndb.position = candidatePos;
            bool hasOverlap = false;

            for (auto d : tlds) {
              if (nd == d || d->needInitPos) continue;

              sf::FloatRect db = padbbox(static_cast<sf::FloatRect>(d->getBBox()), 25.f);
              db.position += d->pos;

              // Check if rectangles intersect
              bool intersects = !(ndb.position.x + ndb.size.x < db.position.x ||
                                 db.position.x + db.size.x < ndb.position.x ||
                                 ndb.position.y + ndb.size.y < db.position.y ||
                                 db.position.y + db.size.y < ndb.position.y);

              if (intersects) {
                hasOverlap = true;
                break;
              }
            }

            if (!hasOverlap) {
              nd->pos = candidatePos;
              positionFound = true;
            }
          }
        }

        nd->needInitPos = false;
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
      if (auto p = std::dynamic_pointer_cast<PrimitiveBase>(d)) {
        p->o->_vstd_update_values(op);
      } else {
        d->update(op);
      }
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

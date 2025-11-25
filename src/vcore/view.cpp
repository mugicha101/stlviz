#include <vcore/all.hpp>
#include <iostream>

namespace vcore {
  View::View(uint32_t width, uint32_t height) {
    window.create(sf::VideoMode({width, height}), "STLVIZ");
    window.setFramerateLimit(60);
  }

  void View::update(vcore::Controller &controller) {
    window.clear(sf::Color(200, 200, 255));

    auto root_display = controller.model.root;
    if (!root_display) {
      window.display();
      return;
    }
    root_display->size = static_cast<sf::Vector2i>(window.getSize());
    auto mousePos = controller.mousePos;
    auto mouseWorldPos = root_display->screen2world(mousePos);
    auto &ops = controller.model.ops;
    auto currOp = controller.currOp;
    auto hoverOp = opListHover(mousePos.x, mousePos.y);
    auto selectedDisplay = controller.selectedDisplay;
    auto hoveredDisplay = root_display->at(mouseWorldPos);
    
    const sf::Color hoveredDisplayColor(255, 0, 0);
    const sf::Color selectedDisplayColor(255, 255, 0);
    const sf::Color hoverOpColor(200, 200, 200);
    const sf::Color selectOpColor(255, 255, 0);
    const sf::Color normalColor(255, 255, 255);
    const sf::Color locSelectColor(255, 0, 0);
    const sf::Color locNormalColor(25, 25, 25);
    const sf::Color borderColor(0, 0, 0);
    const sf::Color textColor(0, 0, 0);
    const sf::Color locTextColor(255, 255, 255);

    // draw data window
    ++vobj::Display::globalDrawTick;
    uint64_t width = window.getSize().x;
    uint64_t height = window.getSize().y;
    opListX = width * 3u / 4u;
    root_display->size.x = opListX;
    root_display->size.y = height;
    root_display->drawOn(window, 0.f, 0.f);

    // draw last operation to window
    if (currOp > 0) ops[currOp-1].draw(root_display, window);
    
    // draw hovered and selected displays
    auto drawSelectBox = [this, root_display](std::shared_ptr<vobj::Display> display, sf::Color color) {
      sf::RectangleShape selectBox;
      selectBox.setFillColor(sf::Color::Transparent);
      selectBox.setOutlineThickness(3.f);
      sf::FloatRect bbox = static_cast<sf::FloatRect>(display->getBBox());
      selectBox.setSize(bbox.size / root_display->camZoom);
      selectBox.setPosition(root_display->world2screen(display->pos + bbox.position));
      selectBox.setOutlineColor(color);
      window.draw(selectBox);
    };
    if (hoveredDisplay && hoveredDisplay != selectedDisplay) {
      drawSelectBox(hoveredDisplay, hoveredDisplayColor);
    }
    if (selectedDisplay) {
      drawSelectBox(selectedDisplay, selectedDisplayColor);
    }

    // draw operations list
    sf::RectangleShape opListBg({(float)(width - opListX), (float)height});
    opListBg.setPosition({(float)opListX, 0.f});
    opListBg.setFillColor(sf::Color(220, 220, 220));
    window.draw(opListBg);
    sf::Font &font = vobj::Display::getFont();
    sf::Text text(font);
    sf::Text locText(font);
    uint64_t textSpacing = font.getLineSpacing(OP_FONT_SIZE);
    float charWidth = font.getGlyph('0', OP_FONT_SIZE, false).advance;
    text.setCharacterSize(OP_FONT_SIZE);
    locText.setCharacterSize(OP_FONT_SIZE);
    text.setFillColor(textColor);
    locText.setFillColor(locTextColor);
    sf::RectangleShape opBg({(float)(width - opListX), (float)textSpacing});
    opBg.setOutlineColor(borderColor);
    opBg.setOutlineThickness(-2.f);
    sf::RectangleShape locBg({(float)(width - opListX), (float)textSpacing});
    locBg.setOutlineColor(borderColor);
    locBg.setOutlineThickness(-2.f);
    float currOpY = (float)((int)currOp - 1) * (float)textSpacing;
    if (opListYOff + currOpY > (float)height * 0.75f) {
      opListYOff = (float)height * 0.75f - currOpY;
    }
    if (opListYOff + currOpY < (float)height * 0.25f) {
      opListYOff = std::min((float)height * 0.25f - currOpY, 0.f);
    }
    for (size_t i = 0; i < ops.size(); ++i) {
      float y = opListYOff + i * textSpacing;
      if (y + textSpacing < 0.f || y > (float)height) continue;

      // display background
      opBg.setPosition({(float)opListX, y});
      opBg.setFillColor(i == currOp - 1 ? selectOpColor : i == hoverOp - 1 ? hoverOpColor : normalColor);
      window.draw(opBg);

      // display loc text
      locText.setPosition({(float)(opListX + 5.f), y});
      bool isRange = ops[i].startLine != ops[i].endLine || ops[i].startOffset != ops[i].endOffset;
      std::string loc = isRange ? std::to_string(ops[i].startLine) + ":" + std::to_string(ops[i].startOffset) + "-" + std::to_string(ops[i].endLine) + ":" + std::to_string(ops[i].endOffset)
                                : std::to_string(ops[i].startLine) + ":" + std::to_string(ops[i].startOffset);
      locText.setString(loc);
      uint32_t locTextWidth = (uint32_t)std::ceil(charWidth * (float)locText.getString().getSize());
      locText.setFillColor(locTextColor);
      locBg.setPosition({(float)(opListX), y});
      locBg.setSize({(float)(locTextWidth + 10u), (float)textSpacing});
      locBg.setFillColor(i == currOp - 1 ? locSelectColor : locNormalColor);
      window.draw(locBg);
      window.draw(locText);
      
      // display text
      text.setPosition({(float)(opListX + locTextWidth) + 10.f, y});
      text.setString(ops[i].content);
      window.draw(text);
    }
    uint64_t opListHeight = std::max({1ULL, height, (uint64_t)ops.size() * textSpacing});
    float scrollBarScale = (float)height / (float)opListHeight;
    sf::RectangleShape opListScrollBar;
    opListScrollBar.setSize({3.f, (float)height});
    opListScrollBar.setPosition({(float)(opListX - 3u), 0});
    opListScrollBar.setFillColor(borderColor);
    window.draw(opListScrollBar);
    opListScrollBar.setSize({3.f, (float)height * scrollBarScale});
    opListScrollBar.setPosition({(float)(opListX - 3u), -opListYOff * scrollBarScale});
    opListScrollBar.setFillColor(normalColor);
    window.draw(opListScrollBar);
    opListScrollBar.setSize({3.f, (float)textSpacing * scrollBarScale});
    opListScrollBar.setPosition({(float)(opListX - 3u), currOpY * scrollBarScale});
    opListScrollBar.setFillColor(locSelectColor);
    window.draw(opListScrollBar);

    window.display();
  }

  int View::opListHover(float mx, float my) const {
    if (mx < (float)opListX) return -1;
    float y = my - opListYOff;
    int index = (int)(y / (float)vobj::Display::getFont().getLineSpacing(OP_FONT_SIZE)) + 1;
    return std::max(index, -1);
  }
}
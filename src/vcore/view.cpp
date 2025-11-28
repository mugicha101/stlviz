#include <vcore/all.hpp>
#include <vstd/base.hpp>
#include <iostream>

namespace vcore {
  View::View(uint32_t width, uint32_t height) {
    window.create(sf::VideoMode({width, height}), "STLVIZ");
    window.setFramerateLimit(60);
    initializeTabs();
  }

  void View::initializeTabs() {
    // "All" tab - shows everything
    tabs.emplace_back("All", [](std::shared_ptr<vobj::Display> d) {
      return true;
    });

    // "Vectors" tab - shows only vector displays
    tabs.emplace_back("Vectors", [](std::shared_ptr<vobj::Display> d) {
      if (!d->o) return false;
      std::string typeName = d->o->_vstd_type_name();
      return typeName.find("vector") != std::string::npos;
    });

    // "Stacks" tab
    tabs.emplace_back("Stacks", [](std::shared_ptr<vobj::Display> d) {
      if (!d->o) return false;
      std::string typeName = d->o->_vstd_type_name();
      return typeName.find("stack") != std::string::npos;
    });

    // "Deques" tab
    tabs.emplace_back("Deques", [](std::shared_ptr<vobj::Display> d) {
      if (!d->o) return false;
      std::string typeName = d->o->_vstd_type_name();
      return typeName.find("deque") != std::string::npos;
    });

    // "Trees" tab - shows tree and BST structures
    tabs.emplace_back("Trees", [](std::shared_ptr<vobj::Display> d) {
      if (!d->o) return false;
      std::string typeName = d->o->_vstd_type_name();
      return typeName.find("tree") != std::string::npos ||
             typeName.find("binary") != std::string::npos;
    });

    // "Sets" tab
    tabs.emplace_back("Sets", [](std::shared_ptr<vobj::Display> d) {
      if (!d->o) return false;
      std::string typeName = d->o->_vstd_type_name();
      return typeName.find("set") != std::string::npos ||
             typeName.find("bitset") != std::string::npos;
    });
  }

  int View::tabBarHover(float mx, float my) const {
    if (my >= tabBarHeight) return -1;

    for (size_t i = 0; i < tabs.size(); ++i) {
      float tabX = tabSpacing + i * (tabWidth + tabSpacing);
      if (mx >= tabX && mx < tabX + tabWidth) {
        return (int)i;
      }
    }
    return -1;
  }

  void View::update(vcore::Controller &controller) {
    window.clear(sf::Color(200, 200, 255));

    auto root_display = controller.model.root;
    if (!root_display) {
      window.display();
      return;
    }

    // Apply the active tab's filter to the root display
    if (activeTabIndex < tabs.size()) {
      root_display->displayFilter = tabs[activeTabIndex].filter;
    }

    root_display->size = static_cast<sf::Vector2i>(window.getSize());
    auto mousePos = controller.mousePos;
    // Adjust mouse position for tab bar offset
    sf::Vector2f adjustedMousePos = {mousePos.x, mousePos.y - (float)tabBarHeight};
    auto mouseWorldPos = root_display->screen2world(adjustedMousePos);
    auto &ops = controller.model.ops;
    auto currOp = controller.currOp;
    auto hoverOp = opListHover(mousePos.x, mousePos.y);
    auto hoverTab = tabBarHover(mousePos.x, mousePos.y);
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
    root_display->size.y = height - tabBarHeight;

    // Draw tab bar

    // Calculate dynamic tab width to fit all tabs on screen
    // Reserve space for operation list (25% of screen width)
    tabHeight = (float)tabBarHeight - 5.f;
    float availableWidth = opListX - tabSpacing * 2.f; // Account for spacing on both sides
    tabWidth = (availableWidth - tabSpacing * (tabs.size() - 1)) / (float)tabs.size();

    // Ensure minimum tab width for readability
    const float minTabWidth = 80.f;
    const float maxTabWidth = 150.f;
    tabWidth = std::max(minTabWidth, std::min(maxTabWidth, tabWidth));

    sf::Font &font = vobj::Display::getFont();
    const uint32_t tabFontSize = 16; // Consistent font size for all tabs

    for (size_t i = 0; i < tabs.size(); ++i) {
      float tabX = tabSpacing + i * (tabWidth + tabSpacing);

      // Tab background
      sf::RectangleShape tabBg({tabWidth, tabHeight});
      tabBg.setPosition({tabX, 5.f});

      if (i == activeTabIndex) {
        tabBg.setFillColor(sf::Color(100, 150, 255)); // Active tab color
        tabBg.setOutlineThickness(-2.f);
        tabBg.setOutlineColor(sf::Color(50, 100, 200));
      } else if (hoverTab == (int)i) {
        tabBg.setFillColor(sf::Color(180, 200, 255)); // Hover tab color
      } else {
        tabBg.setFillColor(sf::Color(220, 220, 220)); // Inactive tab color
      }

      window.draw(tabBg);

      // Tab text
      sf::Text tabText(font);
      tabText.setString(tabs[i].name);
      tabText.setCharacterSize(tabFontSize);
      tabText.setFillColor(i == activeTabIndex ? sf::Color::White : sf::Color::Black);

      // Center text in tab
      sf::FloatRect textBounds = tabText.getLocalBounds();
      tabText.setPosition({
        tabX + (tabWidth - textBounds.size.x) / 2.f - textBounds.position.x,
        10.f + (tabHeight - textBounds.size.y) / 2.f - textBounds.position.y
      });

      window.draw(tabText);
    }

    // Draw data view below tab bar
    root_display->drawOn(window, 0.f, (float)tabBarHeight);

    // draw last operation to window
    if (currOp > 0) ops[currOp-1].draw(root_display, window);
    
    // draw hovered and selected displays
    auto drawSelectBox = [this, root_display](std::shared_ptr<vobj::Display> display, sf::Color color) {
      sf::RectangleShape selectBox;
      selectBox.setFillColor(sf::Color::Transparent);
      selectBox.setOutlineThickness(3.f);
      sf::FloatRect bbox = static_cast<sf::FloatRect>(display->getBBox());
      selectBox.setSize(bbox.size / root_display->camZoom);
      selectBox.setPosition(root_display->world2screen(display->pos + bbox.position) + sf::Vector2f{0.f, (float)tabBarHeight});
      selectBox.setOutlineColor(color);
      window.draw(selectBox);
    };
    if (hoveredDisplay && hoveredDisplay != selectedDisplay) {
      drawSelectBox(hoveredDisplay, hoveredDisplayColor);
    }
    if (selectedDisplay) {
      drawSelectBox(selectedDisplay, selectedDisplayColor);
    }

    
    // draw operations list scrollbar
    uint64_t textSpacing = font.getLineSpacing(OP_FONT_SIZE);
    float charWidth = font.getGlyph('0', OP_FONT_SIZE, false).advance;
    uint64_t opListHeight = std::max({(uint64_t)1, height, (uint64_t)ops.size() * textSpacing});
    float currOpY = (float)((int)currOp - 1) * (float)textSpacing;
    if (opListYOff + currOpY > (float)height * 0.75f) {
      opListYOff = (float)height * 0.75f - currOpY;
    }
    if (opListYOff + currOpY < (float)height * 0.25f) {
      opListYOff = std::min((float)height * 0.25f - currOpY, 0.f);
    }
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

    // draw operations list
    sf::RectangleShape opListBg({(float)(width - opListX), (float)height});
    opListBg.setPosition({(float)opListX, 0.f});
    opListBg.setFillColor(sf::Color(220, 220, 220));
    window.draw(opListBg);
    sf::Text text(font);
    sf::Text locText(font);
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
    uint32_t maxLineChars = (uint32_t)std::floor(((float)(width - opListX) - 5.f) / charWidth);
    for (size_t i = 0; i < ops.size(); ++i) {
      float y = opListYOff + i * textSpacing;
      float x = opListX;
      if (y + textSpacing < 0.f || y > (float)height) continue;

      // display background
      opBg.setPosition({x, y});
      opBg.setFillColor(i == currOp - 1 ? selectOpColor : i == hoverOp - 1 ? hoverOpColor : normalColor);
      window.draw(opBg);

      // display loc text
      std::string locStr = ops[i].startLoc.prettyCombine(ops[i].endLoc);
      uint32_t maxLocStrChars = maxLineChars - std::min(maxLineChars, (uint32_t)ops[i].content.size());
      bool cutOff = false;
      if (maxLocStrChars < locStr.size()) {
        if (hoverOp - 1 == i) {
          x -= charWidth * (float)(locStr.size() - maxLocStrChars);
        } else {
          locStr = locStr.substr(locStr.size()-maxLocStrChars);
          cutOff = true;
        }
      }
      locText.setPosition({x + 5.f, y});
      locText.setString(locStr);
      uint32_t textWidth = ops[i].content.size();
      uint32_t locTextWidth = (uint32_t)std::ceil(charWidth * (float)locText.getString().getSize());
      locText.setFillColor(locTextColor);
      locBg.setPosition({x, y});
      locBg.setSize({(float)(locTextWidth + 10u), (float)textSpacing});
      locBg.setFillColor(i == currOp - 1 ? locSelectColor : locNormalColor);
      window.draw(locBg);
      window.draw(locText);

      // draw arrow indicating more text
      if (cutOff) {
        sf::ConvexShape tri;
        tri.setPointCount(3);
        float sideLen = textSpacing * 0.3f;
        float dx = sideLen * std::sin(sf::degrees(30).asRadians());
        float dy = sideLen * std::cos(sf::degrees(30).asRadians());
        float cx = x + 2.5f;
        float cy = y + (float)textSpacing * 0.5f;
        tri.setPoint(0, sf::Vector2f{cx - dx * 0.5f, cy});
        tri.setPoint(1, sf::Vector2f{cx + dx * 0.5f, cy + dy});
        tri.setPoint(2, sf::Vector2f{cx + dx * 0.5f, cy - dy});
        tri.setFillColor(normalColor);
        window.draw(tri);
      }
      
      // display text
      text.setPosition({x + (float)locTextWidth + 10.f, y});
      text.setString(ops[i].content);
      window.draw(text);
    }

    window.display();
  }

  int View::opListHover(float mx, float my) const {
    if (mx < (float)opListX) return -1;
    float y = my - opListYOff;
    int index = (int)(y / (float)vobj::Display::getFont().getLineSpacing(OP_FONT_SIZE)) + 1;
    return std::max(index, -1);
  }
}
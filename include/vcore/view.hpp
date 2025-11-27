#pragma once

// render screen using SFML
#include <SFML/Graphics.hpp>
#include "vobj/all.hpp"
#include <memory>
#include <vector>
#include <string>
#include <functional>

namespace vcore {
  struct Model;
  struct Controller;

  // Tab structure to organize displays
  struct Tab {
    std::string name;
    std::function<bool(std::shared_ptr<vobj::Display>)> filter;

    Tab(std::string name, std::function<bool(std::shared_ptr<vobj::Display>)> filter)
      : name(name), filter(filter) {}
  };

  struct View {
    sf::RenderWindow window;
    uint32_t opListX = 0u; // x position of operation list
    float opListYOff = 0.f; // vertical offset of operation list scrolling
    uint32_t tabBarHeight = 40u; // height of the tab bar

    std::vector<Tab> tabs;
    size_t activeTabIndex = 0;

    View(uint32_t width = 0, uint32_t height = 0);
    View(sf::Vector2u size) : View(size.x, size.y) {}

    void update(vcore::Controller &controller);
    int opListHover(float mouseX, float mouseY) const;
    int tabBarHover(float mouseX, float mouseY) const;
    void initializeTabs();
  };
}
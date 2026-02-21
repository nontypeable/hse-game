#pragma once

#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable {
public:
  ~Entity() override = default;

  virtual void update(float dt) = 0;

  virtual sf::FloatRect getLocalBounds() const = 0;
  sf::FloatRect getGlobalBounds() const {
    return this->getTransform().transformRect(this->getLocalBounds());
  }

  bool isAlive() const noexcept { return this->alive_; }
  void markForRemoval() noexcept { this->alive_ = false; }

  bool isActive() const noexcept { return this->active_; }
  void setActive(bool active) noexcept { this->active_ = active; }

  bool isVisible() const noexcept { return this->visible_; }
  void setVisible(bool visible) noexcept { this->visible_ = visible; }

  bool intersects(const Entity &other) const noexcept {
    return this->getGlobalBounds()
        .findIntersection(other.getGlobalBounds())
        .has_value();
  }
  bool contains(sf::Vector2f point) const noexcept {
    return this->getGlobalBounds().contains(point);
  }

protected:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const final {
    if (!this->visible_ || !this->alive_) {
      return;
    }
    states.transform *= this->getTransform();
    this->onDraw(target, states);
  }

  virtual void onDraw(sf::RenderTarget &target,
                      sf::RenderStates states) const = 0;

private:
  bool alive_ = true;
  bool active_ = true;
  bool visible_ = true;
};

#pragma once
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

class QuadTree;
using QuadTreeRef = std::shared_ptr<QuadTree>;

struct ITreeVisitorCallback {
  virtual void BeforVisit(QuadTree *qt) {}
  virtual void AfterVisit(QuadTree *qt) {}
  virtual void OnLeaf(QuadTree *qt, bool is_last, int level) {}
  virtual void BeforeRecursioCall(QuadTree *qt, bool is_last, int level) {}
  virtual void OnRecursioCall(QuadTree *qt, bool is_last, int level) {}
  virtual void AfterRecursioCall(QuadTree *qt, bool is_last, int level) {}
};

struct color3 {
  double r, g, b;
  color3(double r, double g, double b) : r(r), g(g), b(b) {}
  color3() = default;
};

struct IQuadTreeRender {
  virtual void draw_plane(double ox, double oy, double size, color3 color) = 0;
};

static color3 ltc = color3(1, 0, 0);
static color3 rtc = color3(0, 0, 1);
static color3 lbc = color3(0, 1, 0);
static color3 rbc = color3(0, 1, 1);

class QuadTree {
  struct Quad {
    double ox, oy;
    color3 color;
    Quad(double ox, double oy, color3 color) : ox(ox), oy(oy), color(color) {}
    Quad() = default;
  };

public:
  QuadTree(int depth, double size, double x, double y, color3 color)
      : m_depth(depth), m_size(size), m_x(x), m_y(y), m_color(color) {}
  auto get_offset_by_index(int i) {
    std::tuple<int, int> offset;
    switch (i) {
    case 0:
      offset = std::make_tuple<int, int>(-1, -1);
      break;
    case 1:
      offset = std::make_tuple<int, int>(-1, 1);
      break;
    case 2:
      offset = std::make_tuple<int, int>(1, -1);
      break;
    case 3:
      offset = std::make_tuple<int, int>(1, 1);
      break;
    default:
      offset = std::make_tuple<int, int>(0, 0);
      break;
    }
    return offset;
  }

  auto get_node_size(double parent_size) { return 0.5 * parent_size; }

  auto get_origin(QuadTree *parent, int n) {
    auto offset = get_offset_by_index(n);
    return std::make_tuple<double, double>(
        parent->m_x + 0.5 * get_node_size(parent->m_size) * std::get<0>(offset),
        parent->m_y +
            0.5 * get_node_size(parent->m_size) * std::get<1>(offset));
  }

  bool need_split(double x, double y, double ox, double oy, double L,
                  double k) {
    if (m_depth > 0) {
      auto d = std::max(std::min(std::abs(x - ox), std::abs(x - ox - L)),
                        std::min(std::abs(y - oy), std::abs(y - oy - L)));
      return d < k * L;
    }
    return false;
  }

  Quad get_quad(int i) {
    color3 color;
    auto origin = get_origin(this, i);
    switch (i) {
    case 0:
      color = lbc;
      break;
    case 1:
      color = ltc;
      break;
    case 2:
      color = rtc;
      break;
    case 3:
      color = rbc;
      break;
    }
    return Quad(std::get<0>(origin), std::get<1>(origin), color);
  }

  QuadTreeRef make_child(int i) {
    auto quad = get_quad(i);
    return std::make_shared<QuadTree>(m_depth - 1, 0.5 * m_size, quad.ox,
                                      quad.oy, quad.color);
  }

  void split(double px, double py, double k) {
    if (need_split(px, py, m_x - 0.5 * m_size, m_y - 0.5 * m_size, m_size, k)) {
      m_children.resize(4);
      for (int i = 0; i < 4; i++) {
        m_children[i] = make_child(i);
        m_children[i]->split(px, py, k);
      }
    }
  }

  void visit(ITreeVisitorCallback *callback, double ox, double oy, int level) {
    callback->BeforVisit((this));
    visit_recursive(callback, ox, oy, level + 1, true);
    callback->AfterVisit((this));
  }

  void visit_recursive(ITreeVisitorCallback *callback, double ox, double oy,
                       int level, bool is_last) {
    using namespace std;

    if (m_children.size() == 0) {
      callback->OnLeaf((this), is_last, level);
    } else {
      callback->BeforeRecursioCall((this), is_last, level);
      for (int i = 0; i < 4; i++) {
        auto offset = get_offset_by_index(i);
        auto origin = get_origin(this, i);
        m_children[i]->visit_recursive(callback, std::get<0>(origin),
                                       std::get<0>(origin), level + 1, i == 3);
      }
      callback->AfterRecursioCall((this), is_last, level);
    }
  }

public:
  int m_depth;
  double m_size;
  double m_x, m_y;
  color3 m_color;
  std::vector<std::shared_ptr<QuadTree>> m_children;
};

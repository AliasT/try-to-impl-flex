

#include <algorithm>
#include <cstddef>

#include "SDL2/SDL_stdinc.h"
#ifndef SDL_h_
#include <SDL2/SDL.h>
#endif

enum AlignType {
  FlexStart,
  Center,
  FlexEnd,
};

struct Box {
  int flex_basis = 0;
  int flex_grow = 0;
  int flex_shrink = 0;

  AlignType align_items = AlignType::FlexStart;

  Box *parent = 0;
  Box *sibiling = 0;

  // firstChild
  Box *child = 0;

  Box *last_child = 0;

  int height = 0;
  int width = 0;

  int offset_x = 0;
  int offset_y = 0;

  Uint8 background[4];
};

void flex_add_child(Box *box, Box *child) {
  if (box->child == nullptr) {
    box->child = child;
    box->last_child = child;
  } else {
    box->last_child->sibiling = child;
    box->last_child = child;
  }
}

void layout_box(Box *box) {
  int total = 0;
  int offset_x = box->offset_x;
  int offset_y = box->offset_y;
  int maxHeight = 0;
  int acc = 0;

  Box *child = box->child;

  if (box->child == nullptr) return;

  if (child != nullptr) {
    layout_box(child);
  }

  while (child != nullptr) {
    total += child->flex_grow;
    child = child->sibiling;
  }

  // 重置指针
  child = box->child;

  while (child != nullptr) {
    if (child->sibiling == nullptr) {
      child->width = box->width - acc;
    } else {
      child->width = (child->flex_grow) / (total * 1.0) * box->width;
      acc += child->width;
    }
    maxHeight = std::max(maxHeight, child->height);

    child->offset_x = offset_x;
    offset_x += child->width;
    child = child->sibiling;
  }

  child = box->child;

  while (child != nullptr) {
    if (box->align_items == AlignType::Center) {
      child->offset_y = box->offset_y + (maxHeight - child->height) / 2;
    } else if (box->align_items == AlignType::FlexEnd) {
      child->offset_y = box->offset_y + maxHeight - child->height;
    } else {
      child->offset_y = box->offset_y;
    }

    child = child->sibiling;
  }

  // 计算父级的高度
  box->height = maxHeight;
}

void flex_render(SDL_Renderer *renderer, Box *container) {
  Box *current = container;

  int height = 0;
  int width = 0;
  int offset_x = 0;
  int offset_y = 0;

  Uint8 *background;

  layout_box(container);

  while (current != nullptr) {
    // 绘制矩形
    width = current->width;
    height = current->height;
    offset_x = current->offset_x;
    offset_y = current->offset_y;

    // 在这里需要计算出所有 children 的宽高

    background = current->background;

    SDL_Rect rect = {offset_x, offset_y, width, height};
    SDL_SetRenderDrawColor(renderer, *background, *(background + 1),
                           *(background + 2), *(background + 3));

    SDL_RenderFillRect(renderer, &rect);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
    // offset += 10;
    if (current->sibiling == nullptr) {
      current = current->child;
      continue;
    }

    current = current->sibiling;
  }
}

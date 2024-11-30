

#include <cstddef>

#include "SDL2/SDL_stdinc.h"
#ifndef SDL_h_
#include <SDL2/SDL.h>
#endif

struct Box {
  int flex_basis = 0;
  int flex_grow = 0;
  int flex_shrink = 0;

  Box *parent = 0;
  Box *sibiling = 0;

  // firstChild
  Box *child = 0;

  Box *lastChild = 0;

  int height = 0;
  int width = 0;

  Uint8 background[4];
};

void flex_add_child(Box *box, Box *child) {
  if (box->child == nullptr) {
    box->child = child;
    box->lastChild = child;
  } else {
    box->lastChild->sibiling = child;
    box->lastChild = child;
  }
}

void layout_box(Box *box) {
  int total = 0;

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
    child->width = (child->flex_grow) / (total * 1.0) * box->width;
    child = child->sibiling;
  }
}

void flex_render(SDL_Renderer *renderer, Box *container) {
  Box *current = container;

  int height;
  int width;
  int offset = 0;

  Uint8 *background;

  layout_box(container);

  while (current != nullptr) {
    // 绘制矩形
    width = current->width;
    height = current->height;

    // 在这里需要计算出所有 children 的宽高

    background = current->background;

    SDL_Rect rect = {offset, offset, width, height};
    SDL_SetRenderDrawColor(renderer, *background, *(background + 1),
                           *(background + 2), *(background + 3));

    SDL_RenderFillRect(renderer, &rect);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
    offset += 10;
    if (current->sibiling == nullptr) {
      current = current->child;
      continue;
    }

    current = current->sibiling;
  }
}

#include <SDL2/SDL.h>
#include <stdio.h>

#include "./flex.cc"

int main(int argc, char *argv[]) {
  // 初始化 SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return 1;
  }

  // 创建窗口
  SDL_Window *window =
      SDL_CreateWindow("SDL2 Example",          // 窗口标题
                       SDL_WINDOWPOS_CENTERED,  // 窗口位置：屏幕中央
                       SDL_WINDOWPOS_CENTERED,
                       800,              // 窗口宽度
                       600,              // 窗口高度
                       SDL_WINDOW_SHOWN  // 显示窗口
      );

  if (!window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  // 创建渲染器
  SDL_Renderer *renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  // 主循环标志
  int running = 1;
  SDL_Event event;

  Box box1{
      .flex_grow = 1,
      .height = 100,
      .width = 800,
      .background =
          {
              255,
              0,
              10,
              144,
          },
  };

  Box box2{.flex_grow = 2, .height = 20, .background = {100, 0, 0, 255}};
  Box box3{.flex_grow = 3, .height = 30, .background = {0, 255, 0, 255}};

  flex_add_child(&box1, &box2);
  flex_add_child(&box1, &box3);
  // SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

  while (running) {
    // 事件处理
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {  // 点击关闭按钮退出
        running = 0;
      }
    }

    // 设置渲染颜色为黑色（清屏颜色）
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // 设置绘制颜色为红色
    // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    flex_render(renderer, &box1);

    // 显示渲染内容
    SDL_RenderPresent(renderer);

    // 延迟一点时间以降低 CPU 使用率
    SDL_Delay(16);
  }

  // 清理资源
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}

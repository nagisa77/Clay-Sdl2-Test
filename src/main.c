#define CLAY_IMPLEMENTATION
#include "renderer/renderer.h"
#include <windows.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// 错误处理函数
void HandleClayErrors(Clay_ErrorData errorData) {
  printf("Clay Error: %s\n", errorData.errorText.chars);
}

// 定义颜色常量
const Clay_Color PRIMARY_COLOR = {70, 130, 180, 255};     // Steel Blue
const Clay_Color SECONDARY_COLOR = {106, 90, 205, 255};   // Slate Blue
const Clay_Color ACCENT_COLOR = {220, 20, 60, 255};       // Crimson
const Clay_Color BACKGROUND_COLOR = {245, 245, 245, 255}; // White Smoke
const Clay_Color CARD_COLOR = {255, 255, 255, 255};       // White
const Clay_Color TEXT_COLOR = {51, 51, 51, 255};          // Dark Gray

// 定义组件
void CardComponent(Clay_String title, Clay_String content) {
  CLAY({.layout =
            {
                .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(150)},
                .padding = CLAY_PADDING_ALL(20),
                .childGap = 12,
            },
        .backgroundColor = CARD_COLOR,
        .cornerRadius = CLAY_CORNER_RADIUS(10),
        .border = {.color = {200, 200, 200, 255}, .width = {1, 1, 1, 1, 0}}}) {
    CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(24)}}}) {
      CLAY_TEXT(title,
                CLAY_TEXT_CONFIG(
                    {.fontId = 0, .fontSize = 18, .textColor = PRIMARY_COLOR}));
    }
    CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)}}}) {
      CLAY_TEXT(content,
                CLAY_TEXT_CONFIG(
                    {.fontId = 0, .fontSize = 14, .textColor = TEXT_COLOR}));
    }
  }
}

void ButtonComponent(Clay_String text, Clay_Color backgroundColor) {
  CLAY(
      {.layout = {.sizing = {CLAY_SIZING_FIXED(120), CLAY_SIZING_FIXED(40)},
                  .padding = CLAY_PADDING_ALL(10),
                  .childAlignment = {CLAY_ALIGN_X_CENTER, CLAY_ALIGN_Y_CENTER}},
       .backgroundColor = backgroundColor,
       .cornerRadius = CLAY_CORNER_RADIUS(6)}) {
    CLAY_TEXT(text, CLAY_TEXT_CONFIG({.fontId = 0,
                                      .fontSize = 14,
                                      .textColor = {255, 255, 255, 255}}));
  }
}

void HeaderComponent(Clay_String title) {
  CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(80)},
                   .padding = {20, 20, 0, 0},
                   .childAlignment = {CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER}},
        .backgroundColor = PRIMARY_COLOR}) {
    CLAY_TEXT(title, CLAY_TEXT_CONFIG({.fontId = 0,
                                       .fontSize = 28,
                                       .textColor = {255, 255, 255, 255}}));
  }
}

// 响应式卡片网格组件
void ResponsiveCardGrid() {
  // 根据窗口宽度决定卡片布局方式
  Clay_Context *context = Clay_GetCurrentContext();
  int windowWidth = (int)context->layoutDimensions.width;

  // 在小屏幕上使用垂直布局，在大屏幕上使用水平布局
  Clay_LayoutDirection direction =
      (windowWidth < 768) ? CLAY_TOP_TO_BOTTOM : CLAY_LEFT_TO_RIGHT;
  uint16_t gap = (windowWidth < 768) ? 15 : 20;

  CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(180)},
                   .layoutDirection = direction,
                   .childGap = gap}}) {
    // 在小屏幕上卡片占满宽度，大屏幕上各占一半
    Clay_SizingAxis cardWidth =
        (windowWidth < 768) ? CLAY_SIZING_GROW(0) : CLAY_SIZING_PERCENT(0.5f);

    CLAY({.layout = {.sizing = {cardWidth, CLAY_SIZING_GROW(0)},
                     .padding = CLAY_PADDING_ALL(15)},
          .backgroundColor = CARD_COLOR,
          .cornerRadius = CLAY_CORNER_RADIUS(10)}) {
      CLAY({.layout = {
                .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(24)},
                .childAlignment = {CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER}}}) {
        CLAY_TEXT(
            CLAY_STRING("响应式设计"),
            CLAY_TEXT_CONFIG(
                {.fontId = 0, .fontSize = 18, .textColor = PRIMARY_COLOR}));
      }
      CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                       .childGap = 8}}) {
        CLAY_TEXT(CLAY_STRING("当前窗口宽度:"),
                  CLAY_TEXT_CONFIG(
                      {.fontId = 0, .fontSize = 14, .textColor = TEXT_COLOR}));
        CLAY_TEXT(
            Clay__IntToString(windowWidth),
            CLAY_TEXT_CONFIG(
                {.fontId = 0, .fontSize = 14, .textColor = ACCENT_COLOR}));
      }
    }

    CLAY({.layout = {.sizing = {cardWidth, CLAY_SIZING_GROW(0)},
                     .padding = CLAY_PADDING_ALL(15)},
          .backgroundColor = CARD_COLOR,
          .cornerRadius = CLAY_CORNER_RADIUS(10)}) {
      CLAY({.layout = {
                .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(24)},
                .childAlignment = {CLAY_ALIGN_X_LEFT, CLAY_ALIGN_Y_CENTER}}}) {
        CLAY_TEXT(
            CLAY_STRING("自适应布局"),
            CLAY_TEXT_CONFIG(
                {.fontId = 0, .fontSize = 18, .textColor = PRIMARY_COLOR}));
      }
      CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                       .childGap = 8}}) {
        CLAY_TEXT(CLAY_STRING("布局方向:"),
                  CLAY_TEXT_CONFIG(
                      {.fontId = 0, .fontSize = 14, .textColor = TEXT_COLOR}));
        CLAY_TEXT(
            (direction == CLAY_LEFT_TO_RIGHT) ? CLAY_STRING("横向")
                                              : CLAY_STRING("纵向"),
            CLAY_TEXT_CONFIG(
                {.fontId = 0, .fontSize = 14, .textColor = ACCENT_COLOR}));
      }
    }
  }
}

// 加载中文字体的函数
TTF_Font *LoadChineseFont(int fontSize) {
  TTF_Font *font = NULL;

  // Windows系统中文字体路径
  const char *chineseFontPaths[] = {
      "C:\\Windows\\Fonts\\msyh.ttc",     // 微软雅黑 (常规)
      "C:\\Windows\\Fonts\\msyh.ttf",     // 微软雅黑 (常规, TTF格式)
      "C:\\Windows\\Fonts\\msyhbd.ttc",   // 微软雅黑 (粗体)
      "C:\\Windows\\Fonts\\simhei.ttf",   // 黑体
      "C:\\Windows\\Fonts\\simsun.ttc",   // 宋体
      "C:\\Windows\\Fonts\\simkai.ttf",   // 楷体
      "C:\\Windows\\Fonts\\fangsong.ttf", // 仿宋
      "C:\\Windows\\Fonts\\STXIHEI.TTF",  // 华文细黑
      "C:\\Windows\\Fonts\\STKAITI.TTF",  // 华文楷体
      "C:\\Windows\\Fonts\\STSONG.TTF",   // 华文宋体
      NULL};

  // 首先尝试加载系统中文字体
  for (int i = 0; chineseFontPaths[i] != NULL && font == NULL; i++) {
    font = TTF_OpenFont(chineseFontPaths[i], fontSize);
    if (font) {
      printf("成功加载中文字体: %s\n", chineseFontPaths[i]);
    }
  }

  // 如果系统字体加载失败，尝试相对路径下的字体文件
  if (!font) {
    const char *relativeChineseFonts[] = {"msyh.ttf",     // 微软雅黑
                                          "simhei.ttf",   // 黑体
                                          "simsun.ttc",   // 宋体
                                          "simkai.ttf",   // 楷体
                                          "arialuni.ttf", // Arial Unicode MS
                                          "arial.ttf",    // Arial (作为备选)
                                          NULL};

    for (int i = 0; relativeChineseFonts[i] != NULL && font == NULL; i++) {
      font = TTF_OpenFont(relativeChineseFonts[i], fontSize);
      if (font) {
        printf("成功加载相对路径字体: %s\n", relativeChineseFonts[i]);
      }
    }
  }

  return font;
}

int main(int argc, char *argv[]) {
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  // 1. 初始化SDL2
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL初始化失败: %s\n", SDL_GetError());
    return 1;
  }

  if (TTF_Init() < 0) {
    printf("SDL_ttf初始化失败: %s\n", TTF_GetError());
    SDL_Quit();
    return 1;
  }

  // 2. 创建窗口和渲染器
  SDL_Window *window = SDL_CreateWindow(
      "Clay SDL2 示例", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024,
      768, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

  if (!window) {
    printf("窗口创建失败: %s\n", SDL_GetError());
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  // 设置窗口的最小宽度和高度
  SDL_SetWindowMinimumSize(window, 440, 300);

  // 设置抗锯齿属性
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // 线性插值
  SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");         // 垂直同步

  // 创建支持多重采样抗锯齿的渲染器
  SDL_Renderer *renderer = NULL;

  // 尝试创建支持多重采样的渲染器 (4x MSAA)
  SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
  renderer = SDL_CreateRenderer(
      window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  // 如果失败，尝试2x MSAA
  if (!renderer) {
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  }

  // 如果还失败，创建不带MSAA的渲染器
  if (!renderer) {
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  }

  if (!renderer) {
    printf("渲染器创建失败: %s\n", SDL_GetError());
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  // 检查是否成功启用了多重采样
  SDL_RendererInfo info;
  if (SDL_GetRendererInfo(renderer, &info) == 0) {
    printf("渲染器: %s\n", info.name);
    if (info.flags & SDL_RENDERER_TARGETTEXTURE) {
      printf("支持渲染到纹理\n");
    }
  }

  // 3. 加载字体
  TTF_Font *font = NULL;

  // 优先加载支持中文的字体
  font = LoadChineseFont(16);

  // 如果中文字体加载失败，尝试获取系统默认字体
  if (!font) {
    HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
    if (hFont) {
      LOGFONT lf;
      if (GetObject(hFont, sizeof(LOGFONT), &lf)) {
        char fontName[LF_FACESIZE];
        // 将wchar_t转换为char
        WideCharToMultiByte(CP_UTF8, 0, (LPCWCH)lf.lfFaceName, -1, fontName,
                            LF_FACESIZE, NULL, NULL);
        printf("系统默认字体: %s\n", fontName);

        // 构造可能的字体文件路径
        char fontPath[512];
        snprintf(fontPath, sizeof(fontPath), "C:\\Windows\\Fonts\\%s.ttf",
                 fontName);
        font = TTF_OpenFont(fontPath, 16);

        if (!font) {
          // 尝试.ttc扩展名
          snprintf(fontPath, sizeof(fontPath), "C:\\Windows\\Fonts\\%s.ttc",
                   fontName);
          font = TTF_OpenFont(fontPath, 16);
        }
      }
    }
  }

  // 如果所有字体都加载失败，尝试常用字体
  if (!font) {
    const char *fallbackFonts[] = {"C:\\Windows\\Fonts\\arial.ttf", // Arial
                                   NULL};

    // 尝试加载系统字体
    for (int i = 0; fallbackFonts[i] != NULL && font == NULL; i++) {
      font = TTF_OpenFont(fallbackFonts[i], 16);
      if (font) {
        printf("成功加载备选字体: %s\n", fallbackFonts[i]);
      }
    }
  }

  if (!font) {
    printf("字体加载失败: %s\n", TTF_GetError());
    printf("请确保系统中有可用字体或在程序目录下放置字体文件\n");
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 1;
  }

  // 设置字体渲染样式以提高中文显示质量
  TTF_SetFontHinting(font, TTF_HINTING_LIGHT);
  TTF_SetFontKerning(font, 1);

  SDL2_Font fonts[1] = {{0, font}};

  // 4. 初始化Clay
  uint32_t memorySize = Clay_MinMemorySize();
  Clay_Arena arena =
      Clay_CreateArenaWithCapacityAndMemory(memorySize, malloc(memorySize));
  Clay_Initialize(arena, (Clay_Dimensions){1024, 768},
                  (Clay_ErrorHandler){HandleClayErrors});
  Clay_SetMeasureTextFunction(SDL2_MeasureText, fonts);

  // 5. 主循环
  bool running = true;
  SDL_Event event;
  int windowWidth = 1024;
  int windowHeight = 768;

  SDL_StartTextInput();

  while (running) {
    // 处理事件
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = false;
        break;
      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
            event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
          SDL_Log("Window event: %d\n", event.window.event);
          windowWidth = event.window.data1;
          windowHeight = event.window.data2;
          // 更新Clay的布局尺寸
          Clay_SetLayoutDimensions(
              (Clay_Dimensions){windowWidth, windowHeight});
        }
        break;
      default:
        break;
      }
    }

    // 获取鼠标状态
    int mouseX, mouseY;
    Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
    Clay_SetPointerState((Clay_Vector2){mouseX, mouseY},
                         mouseState & SDL_BUTTON(SDL_BUTTON_LEFT));

    // 开始布局
    Clay_BeginLayout();

    // 定义UI
    CLAY({.id = CLAY_ID("MainContainer"),
          .layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                     .layoutDirection = CLAY_TOP_TO_BOTTOM},
          .backgroundColor = BACKGROUND_COLOR}) {
      // 标题栏
      HeaderComponent(CLAY_STRING("Clay 响应式 UI 示例"));

      // 内容区域
      CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                       .padding = {20, 20, 20, 20},
                       .childGap = 20,
                       .layoutDirection = CLAY_TOP_TO_BOTTOM}}) {
        // 响应式卡片网格
        ResponsiveCardGrid();

        // 按钮区域
        CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(60)},
                         .layoutDirection = CLAY_LEFT_TO_RIGHT,
                         .childGap = 15,
                         .childAlignment = {CLAY_ALIGN_X_LEFT,
                                            CLAY_ALIGN_Y_CENTER}}}) {
          ButtonComponent(CLAY_STRING("主要按钮"), PRIMARY_COLOR);
          ButtonComponent(CLAY_STRING("次要按钮"), SECONDARY_COLOR);
          ButtonComponent(CLAY_STRING("强调按钮"), ACCENT_COLOR);
        }

        // 信息区域
        CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                         .padding = {20, 20, 20, 20},
                         .childGap = 15,
                         .layoutDirection = CLAY_TOP_TO_BOTTOM},
              .backgroundColor = CARD_COLOR,
              .cornerRadius = CLAY_CORNER_RADIUS(10)}) {
          CLAY({.layout = {
                    .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(30)}}}) {
            CLAY_TEXT(
                CLAY_STRING("响应式特性"),
                CLAY_TEXT_CONFIG(
                    {.fontId = 0, .fontSize = 20, .textColor = PRIMARY_COLOR}));
          }

          // 根据窗口大小动态调整布局
          Clay_Context *context = Clay_GetCurrentContext();
          int currentWidth = (int)context->layoutDimensions.width;

          // 在小屏幕上使用垂直布局，在大屏幕上使用水平布局
          Clay_LayoutDirection featureLayoutDirection =
              (currentWidth < 768) ? CLAY_TOP_TO_BOTTOM : CLAY_LEFT_TO_RIGHT;

          CLAY({.layout = {.sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_GROW(0)},
                           .layoutDirection = featureLayoutDirection,
                           .childGap = 15}}) {
            CLAY({.layout = {
                      .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(20)},
                      .layoutDirection = CLAY_LEFT_TO_RIGHT,
                      .childGap = 10}}) {
              CLAY({.layout = {.sizing = {CLAY_SIZING_FIXED(10),
                                          CLAY_SIZING_FIXED(10)}},
                    .backgroundColor = ACCENT_COLOR,
                    .cornerRadius = CLAY_CORNER_RADIUS(5)}) {}
              CLAY_TEXT(
                  CLAY_STRING("基于窗口大小的自适应布局"),
                  CLAY_TEXT_CONFIG(
                      {.fontId = 0, .fontSize = 14, .textColor = TEXT_COLOR}));
            }

            CLAY({.layout = {
                      .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(20)},
                      .layoutDirection = CLAY_LEFT_TO_RIGHT,
                      .childGap = 10}}) {
              CLAY({.layout = {.sizing = {CLAY_SIZING_FIXED(10),
                                          CLAY_SIZING_FIXED(10)}},
                    .backgroundColor = ACCENT_COLOR,
                    .cornerRadius = CLAY_CORNER_RADIUS(5)}) {}
              CLAY_TEXT(
                  CLAY_STRING("百分比尺寸支持"),
                  CLAY_TEXT_CONFIG(
                      {.fontId = 0, .fontSize = 14, .textColor = TEXT_COLOR}));
            }

            CLAY({.layout = {
                      .sizing = {CLAY_SIZING_GROW(0), CLAY_SIZING_FIXED(20)},
                      .layoutDirection = CLAY_LEFT_TO_RIGHT,
                      .childGap = 10}}) {
              CLAY({.layout = {.sizing = {CLAY_SIZING_FIXED(10),
                                          CLAY_SIZING_FIXED(10)}},
                    .backgroundColor = ACCENT_COLOR,
                    .cornerRadius = CLAY_CORNER_RADIUS(5)}) {}
              CLAY_TEXT(
                  CLAY_STRING("动态组件重排"),
                  CLAY_TEXT_CONFIG(
                      {.fontId = 0, .fontSize = 14, .textColor = TEXT_COLOR}));
            }
          }
        }
      }
    }

    // 结束布局并获取渲染命令
    Clay_RenderCommandArray commands = Clay_EndLayout();

    // 清屏
    SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR.r, BACKGROUND_COLOR.g,
                           BACKGROUND_COLOR.b, BACKGROUND_COLOR.a);
    SDL_RenderClear(renderer);

    // 设置渲染器绘制混合模式
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // 渲染Clay UI
    Clay_SDL2_Render(renderer, commands, fonts);

    // 显示
    SDL_RenderPresent(renderer);
  }

  // 6. 清理资源
  TTF_CloseFont(font);
  free(arena.memory);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  SDL_Quit();

  return 0;
}
#ifndef CLAY_SDL_RENDERER_H
#define CLAY_SDL_RENDERER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <clay.h>

// 字体结构体定义
typedef struct {
  uint32_t fontId;
  TTF_Font *font;
} SDL2_Font;

// 函数声明
void SDL_RenderFillRoundedRect(SDL_Renderer *renderer, const SDL_FRect rect,
                               const float cornerRadius,
                               const Clay_Color _color);

void Clay_SDL2_Render(SDL_Renderer *renderer,
                      Clay_RenderCommandArray renderCommands, SDL2_Font *fonts);

Clay_Dimensions SDL2_MeasureText(Clay_StringSlice text,
                                 Clay_TextElementConfig *config,
                                 void *userData);

#endif // CLAY_SDL_RENDERER_H
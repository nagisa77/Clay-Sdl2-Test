#include "renderer.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
void SDL_RenderFillRoundedRect(SDL_Renderer *renderer, const SDL_FRect rect,
                               const float cornerRadius,
                               const Clay_Color _color) {
  const SDL_Color color = (SDL_Color){
      .r = (Uint8)_color.r,
      .g = (Uint8)_color.g,
      .b = (Uint8)_color.b,
      .a = (Uint8)_color.a,
  };

  const float maxRadius = SDL_min(rect.w, rect.h) / 2.0f;
  const float clampedRadius = SDL_min(cornerRadius, maxRadius);

  // 对于非常小的圆角半径，直接绘制普通矩形
  if (clampedRadius <= 0.1f) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &rect);
    return;
  }

  // 增加顶点数量以获得更平滑的圆角
  const int segmentsPerCorner = 8;
  const int totalVertices = 4 + 4 * segmentsPerCorner;
  const int totalIndices = (4 + 4 * segmentsPerCorner) * 3;

  SDL_Vertex vertices[512];
  int indices[1536]; // 512 * 3
  int vertexCount = 0;
  int indexCount = 0;

  // 中心矩形
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + clampedRadius, rect.y + clampedRadius}, color, {0, 0}};
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w - clampedRadius, rect.y + clampedRadius}, color, {1, 0}};
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + rect.w - clampedRadius, rect.y + rect.h - clampedRadius},
      color,
      {1, 1}};
  vertices[vertexCount++] = (SDL_Vertex){
      {rect.x + clampedRadius, rect.y + rect.h - clampedRadius}, color, {0, 1}};

  indices[indexCount++] = 0;
  indices[indexCount++] = 1;
  indices[indexCount++] = 3;
  indices[indexCount++] = 1;
  indices[indexCount++] = 2;
  indices[indexCount++] = 3;

  // 绘制圆角
  //   const float pi_over_2 = (float)M_PI / 2.0f;

  //   // 左上角
  //   int topLeftCenter = 0;
  //   for (int i = 0; i < segmentsPerCorner; i++) {
  //     float angle = pi_over_2 * (float)i / (float)segmentsPerCorner;
  //     float x = rect.x + clampedRadius - clampedRadius * cosf(angle);
  //     float y = rect.y + clampedRadius - clampedRadius * sinf(angle);
  //     vertices[vertexCount] = (SDL_Vertex){{x, y}, color, {0, 0}};

  //     if (i > 0) {
  //       indices[indexCount++] = topLeftCenter;
  //       indices[indexCount++] = vertexCount - 1;
  //       indices[indexCount++] = vertexCount;
  //     }
  //     vertexCount++;
  //   }

  //   if (segmentsPerCorner > 0) {
  //     indices[indexCount++] = topLeftCenter;
  //     indices[indexCount++] = vertexCount - 1;
  //     indices[indexCount++] = 1; // 连接到第一个顶点
  //   }

  //   // 右上角
  //   int topRightCenter = 1;
  //   for (int i = 0; i < segmentsPerCorner; i++) {
  //     float angle = pi_over_2 * (float)i / (float)segmentsPerCorner;
  //     float x = rect.x + rect.w - clampedRadius + clampedRadius *
  //     sinf(angle); float y = rect.y + clampedRadius - clampedRadius *
  //     cosf(angle); vertices[vertexCount] = (SDL_Vertex){{x, y}, color, {0,
  //     0}};

  //     if (i > 0) {
  //       indices[indexCount++] = topRightCenter;
  //       indices[indexCount++] = vertexCount - 1;
  //       indices[indexCount++] = vertexCount;
  //     }
  //     vertexCount++;
  //   }

  //   if (segmentsPerCorner > 0) {
  //     indices[indexCount++] = topRightCenter;
  //     indices[indexCount++] = vertexCount - 1;
  //     indices[indexCount++] = 2; // 连接到第一个顶点
  //   }

  //   // 右下角
  //   int bottomRightCenter = 2;
  //   for (int i = 0; i < segmentsPerCorner; i++) {
  //     float angle = pi_over_2 * (float)i / (float)segmentsPerCorner;
  //     float x = rect.x + rect.w - clampedRadius + clampedRadius *
  //     cosf(angle); float y = rect.y + rect.h - clampedRadius + clampedRadius
  //     * sinf(angle); vertices[vertexCount] = (SDL_Vertex){{x, y}, color, {0,
  //     0}};

  //     if (i > 0) {
  //       indices[indexCount++] = bottomRightCenter;
  //       indices[indexCount++] = vertexCount - 1;
  //       indices[indexCount++] = vertexCount;
  //     }
  //     vertexCount++;
  //   }

  //   if (segmentsPerCorner > 0) {
  //     indices[indexCount++] = bottomRightCenter;
  //     indices[indexCount++] = vertexCount - 1;
  //     indices[indexCount++] = 3; // 连接到第一个顶点
  //   }

  //   // 左下角
  //   int bottomLeftCenter = 3;
  //   for (int i = 0; i < segmentsPerCorner; i++) {
  //     float angle = pi_over_2 * (float)i / (float)segmentsPerCorner;
  //     float x = rect.x + clampedRadius - clampedRadius * sinf(angle);
  //     float y = rect.y + rect.h - clampedRadius + clampedRadius *
  //     cosf(angle); vertices[vertexCount] = (SDL_Vertex){{x, y}, color, {0,
  //     0}};

  //     if (i > 0) {
  //       indices[indexCount++] = bottomLeftCenter;
  //       indices[indexCount++] = vertexCount - 1;
  //       indices[indexCount++] = vertexCount;
  //     }
  //     vertexCount++;
  //   }

  //   if (segmentsPerCorner > 0) {
  //     indices[indexCount++] = bottomLeftCenter;
  //     indices[indexCount++] = vertexCount - 1;
  //     indices[indexCount++] = 0; // 连接到第一个顶点
  //   }

  // 连接角之间的边缘
  // 顶部边缘
  //   indices[indexCount++] = 0;
  //   indices[indexCount++] = vertexCount - 4 * segmentsPerCorner;
  //   indices[indexCount++] = 1;

  //   // 右侧边缘
  //   indices[indexCount++] = 1;
  //   indices[indexCount++] = vertexCount - 3 * segmentsPerCorner;
  //   indices[indexCount++] = 2;

  //   // 底部边缘
  //   indices[indexCount++] = 2;
  //   indices[indexCount++] = vertexCount - 2 * segmentsPerCorner;
  //   indices[indexCount++] = 3;

  //   // 左侧边缘
  //   indices[indexCount++] = 3;
  //   indices[indexCount++] = vertexCount - segmentsPerCorner;
  //   indices[indexCount++] = 0;

  // 渲染几何图形
  SDL_RenderGeometry(renderer, NULL, vertices, vertexCount, indices,
                     indexCount);
}

void Clay_SDL2_Render(SDL_Renderer *renderer,
                      Clay_RenderCommandArray renderCommands,
                      SDL2_Font *fonts) {
  for (uint32_t i = 0; i < renderCommands.length; i++) {
    Clay_RenderCommand *renderCommand =
        Clay_RenderCommandArray_Get(&renderCommands, i);
    Clay_BoundingBox boundingBox = renderCommand->boundingBox;

    switch (renderCommand->commandType) {
    case CLAY_RENDER_COMMAND_TYPE_RECTANGLE: {
      Clay_RectangleRenderData *config = &renderCommand->renderData.rectangle;
      Clay_Color color = config->backgroundColor;
      SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
      SDL_FRect rect = (SDL_FRect){
          .x = boundingBox.x,
          .y = boundingBox.y,
          .w = boundingBox.width,
          .h = boundingBox.height,
      };
      if (config->cornerRadius.topLeft > 0) {
        SDL_RenderFillRoundedRect(renderer, rect, config->cornerRadius.topLeft,
                                  color);
      } else {
        SDL_RenderFillRectF(renderer, &rect);
      }
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_TEXT: {
      Clay_TextRenderData *config = &renderCommand->renderData.text;
      char *cloned = (char *)calloc(config->stringContents.length + 1, 1);
      memcpy(cloned, config->stringContents.chars,
             config->stringContents.length);
      TTF_Font *font = fonts[config->fontId].font;
      TTF_SetFontSize(font, config->fontSize);

      // 优化中文字体渲染质量
      TTF_SetFontHinting(font, TTF_HINTING_LIGHT); // 轻量级字体提示
      TTF_SetFontKerning(font, 1);                 // 启用字距调整

      // 根据字体大小选择最佳渲染模式
      SDL_Surface *surface = NULL;
      if (config->fontSize >= 18) {
        // 对于较大字体使用抗锯齿渲染
        surface = TTF_RenderUTF8_Blended(font, cloned,
                                         (SDL_Color){
                                             .r = (Uint8)config->textColor.r,
                                             .g = (Uint8)config->textColor.g,
                                             .b = (Uint8)config->textColor.b,
                                             .a = (Uint8)config->textColor.a,
                                         });
      } else {
        // 对于较小字体使用_LCD渲染以提高清晰度
        surface = TTF_RenderUTF8_Blended(font, cloned,
                                         (SDL_Color){
                                             .r = (Uint8)config->textColor.r,
                                             .g = (Uint8)config->textColor.g,
                                             .b = (Uint8)config->textColor.b,
                                             .a = (Uint8)config->textColor.a,
                                         }); // 背景色
      }

      // 如果_LCD渲染失败，回退到抗锯齿渲染
      //   if (!surface) {
      //     surface = TTF_RenderUTF8_Blended(font, cloned,
      //                                      (SDL_Color){
      //                                          .r =
      //                                          (Uint8)config->textColor.r, .g
      //                                          = (Uint8)config->textColor.g,
      //                                          .b =
      //                                          (Uint8)config->textColor.b, .a
      //                                          = (Uint8)config->textColor.a,
      //                                      });
      //   }
      // 如果渲染失败，再次尝试使用抗锯齿渲染
      if (!surface) {
        surface = TTF_RenderUTF8_Blended(font, cloned,
                                         (SDL_Color){
                                             .r = (Uint8)config->textColor.r,
                                             .g = (Uint8)config->textColor.g,
                                             .b = (Uint8)config->textColor.b,
                                             .a = (Uint8)config->textColor.a,
                                         });
      }

      if (!surface) {
        free(cloned);
        break;
      }

      SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

      // 设置纹理混合模式，确保文字背景透明
      SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

      SDL_Rect destination = (SDL_Rect){
          .x = boundingBox.x,
          .y = boundingBox.y,
          .w = boundingBox.width,
          .h = boundingBox.height,
      };
      SDL_RenderCopy(renderer, texture, NULL, &destination);

      SDL_DestroyTexture(texture);
      SDL_FreeSurface(surface);
      free(cloned);
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_START: {
      SDL_Rect currentClippingRectangle = (SDL_Rect){
          .x = boundingBox.x,
          .y = boundingBox.y,
          .w = boundingBox.width,
          .h = boundingBox.height,
      };
      SDL_RenderSetClipRect(renderer, &currentClippingRectangle);
      break;
    }
    case CLAY_RENDER_COMMAND_TYPE_SCISSOR_END: {
      SDL_RenderSetClipRect(renderer, NULL);
      break;
    }
    default: {
      // Ignore unhandled render commands
      break;
    }
    }
  }
}

// 文本测量函数 - 来自SDL2渲染器
Clay_Dimensions SDL2_MeasureText(Clay_StringSlice text,
                                 Clay_TextElementConfig *config,
                                 void *userData) {
  SDL2_Font *fonts = (SDL2_Font *)userData;
  TTF_Font *font = fonts[config->fontId].font;

  char *chars = (char *)calloc(text.length + 1, 1);
  memcpy(chars, text.chars, text.length);

  // 优化字体测量
  TTF_SetFontHinting(font, TTF_HINTING_LIGHT);

  int width = 0, height = 0;
  if (TTF_SizeUTF8(font, chars, &width, &height) < 0) {
    fprintf(stderr, "Error measuring text: %s\n", TTF_GetError());
    width = text.length * 12; // 备用值
    height = 16;
  }

  free(chars);
  return (Clay_Dimensions){.width = (float)width, .height = (float)height};
}
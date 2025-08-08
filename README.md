## 编译须知
- 本项目现在支持 macOS 等平台，不再依赖 Windows 专有头文件
- 本项目使用了zig进行打包编译~~(ps: make系列作者实在是用不来)~~
- 本项目不提供clay.h头文件，请自行到[这里](https://github.com/nicbarker/clay/releases/tag/v0.14)下载
- 本项目使用了SDL2库作为渲染后端，请在`build.zig`配置好对本机SDL2及其依赖库的链接路径
- 本项目.clangd只作为用于适配clangd语言服务器示范
```bash
# 编译命令
zig build
```

---

~~AI写C居然还不错~~
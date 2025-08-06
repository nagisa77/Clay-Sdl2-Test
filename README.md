## 编译须知
本项目使用了windows.h头文件，请在Win环境下编译
本项目使用了zig进行打包编译~~(ps: make系列作者实在是用不来)~~
本项目不提供clay.h头文件，请自行到[这里](https://github.com/nicbarker/clay/releases/tag/v0.14)下载
本项目使用了SDL2库作为渲染后端，请再build.zig配置好对本机SDL2的链接路径，以及SDL2的所依赖的其他库
本项目.clangd只作为用于适配clangd语言服务器示范
```bash
# 编译命令
zig build
```

---

~~AI写C居然还不错~~
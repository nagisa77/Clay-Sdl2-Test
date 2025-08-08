const std = @import("std");

// Although this function looks imperative, note that its job is to
// declaratively construct a build graph that will be executed by an external
// runner.

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    var name = std.ArrayList(u8).init(b.allocator);

    if (target.query.cpu_arch) |arch| {
        name.appendSlice(arch.genericName()) catch |err| {
            // 在构建脚本中，内存不足通常是致命错误，可以直接 panic
            std.debug.panic("Failed to append arch name: {}", .{err});
        };
    } else {
        name.appendSlice("x86_64") catch |err| {
            std.debug.panic("Failed to append default arch name: {}", .{err});
        };
    }

    const exe = b.addExecutable(.{ .name = name.items, .target = target, .optimize = optimize });

    const cFiles = [_][]const u8{ "src/main.c", "src/renderer/renderer.c" };

    const cFlags = [_][]const u8{
        "-std=c99",
        // UTF-8 编码
        "-D_UNICODE",
        "-DUNICODE",
    };

    exe.addCSourceFiles(.{
        .files = &cFiles,
        .flags = &cFlags,
    });

    exe.addIncludePath(b.path("include"));

    switch (target.result.os.tag) {
        .windows => {
            // 链接 ucrt64 lib // 我这里是 msys2的 ucrt64，如果你的libsdl2.dll 在其他地方，请修改这里
            const UCRT64_LIB = std.process.getEnvVarOwned(b.allocator, "UCRT64_LIB") catch |err| switch (err) {
                error.EnvironmentVariableNotFound => "d:/Software/Dev/msys64/ucrt64/lib",
                else => std.debug.panic("Failed to get UCRT64_LIB: {}", .{err}),
            };
            exe.addLibraryPath(.{ .cwd_relative = UCRT64_LIB });

            // 引入 ucrt64 Include // 我这里是 msys2的 ucrt64，如果你的libsdl2.dll 在其他地方，请修改这里
            const UCRT64_INCLUDE = std.process.getEnvVarOwned(b.allocator, "UCRT64_INCLUDE") catch |err| switch (err) {
                error.EnvironmentVariableNotFound => "d:/Software/Dev/msys64/ucrt64/include",
                else => std.debug.panic("Failed to get UCRT64_INCLUDE: {}", .{err}),
            };
            exe.addIncludePath(.{ .cwd_relative = UCRT64_INCLUDE });

            // 添加 Windows 系统库
            exe.linkSystemLibrary("gdi32");
            exe.linkSystemLibrary("user32");
            exe.linkSystemLibrary("kernel32");
            exe.linkSystemLibrary("comctl32");
            exe.linkSystemLibrary("comdlg32");
            exe.linkSystemLibrary("shell32");
            exe.linkSystemLibrary("ole32");
            exe.linkSystemLibrary("oleaut32");
            exe.linkSystemLibrary("imm32");
            exe.linkSystemLibrary("winmm");
            exe.linkSystemLibrary("setupapi");
            exe.linkSystemLibrary("version");
            exe.linkSystemLibrary("cfgmgr32");
            exe.linkSystemLibrary("msimg32");
            exe.linkSystemLibrary("rpcrt4");
            exe.linkSystemLibrary("winspool");
        },
        .macos => {
            // macOS 需要链接的框架和库
            exe.linkFramework("Cocoa");
            exe.linkFramework("Carbon");
            exe.linkFramework("CoreVideo");
            exe.linkFramework("SDL2");
            exe.linkFramework("SDL2_ttf");
            exe.linkSystemLibrary("freetype");
            exe.linkSystemLibrary("harfbuzz");
            exe.linkSystemLibrary("png");
            exe.linkSystemLibrary("z");
            exe.linkSystemLibrary("bz2");
            exe.linkSystemLibrary("brotlicommon");
            exe.linkSystemLibrary("brotlidec");
        },
        else => {
            // 其他系统：假设库已在系统路径中
            exe.linkSystemLibrary("SDL2");
            exe.linkSystemLibrary("SDL2_ttf");
            exe.linkSystemLibrary("freetype");
            exe.linkSystemLibrary("harfbuzz");
            exe.linkSystemLibrary("png");
            exe.linkSystemLibrary("z");
            exe.linkSystemLibrary("bz2");
            exe.linkSystemLibrary("brotlicommon");
            exe.linkSystemLibrary("brotlidec");
        },
    }

    exe.linkSystemLibrary("c++");

    exe.linkLibC();

    // SDL2特定设置
    exe.root_module.addCMacro("SDL_MAIN_HANDLED", "1");
    exe.subsystem = .Console;

    b.installArtifact(exe);
}


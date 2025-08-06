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

    // 添加 FreeType 库及相关依赖
    exe.linkSystemLibrary("freetype");

    // 添加 SDL2_ttf 依赖的库
    exe.linkSystemLibrary("harfbuzz");
    exe.linkSystemLibrary("graphite2"); // 添加Graphite2库支持
    exe.linkSystemLibrary("png");
    exe.linkSystemLibrary("z");
    exe.linkSystemLibrary("bz2");
    exe.linkSystemLibrary("brotlicommon");
    exe.linkSystemLibrary("brotlidec");

    // 链接 SDL2 相关库
    exe.linkSystemLibrary("SDL2");
    exe.linkSystemLibrary("SDL2_ttf");

    exe.linkSystemLibrary("c++");

    exe.linkLibC();

    // SDL2特定设置
    exe.root_module.addCMacro("SDL_MAIN_HANDLED", "1");
    exe.subsystem = .Console;

    b.installArtifact(exe);
}

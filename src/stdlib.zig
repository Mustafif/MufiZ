const std = @import("std");
const Value = core.value_h.Value;
const vm = @cImport(@cInclude("vm.h"));
const conv = @import("conv.zig");
const core = @import("core");

pub const math = @import("stdlib/math.zig");
pub const time = @import("stdlib/time.zig");
pub const types = @import("stdlib/types.zig");
pub const fs = @import("stdlib/fs.zig");

pub const NativeFn = *const fn (c_int, [*c]Value) callconv(.C) Value;

pub const NativeFunctions = struct {
    map: std.StringArrayHashMap(NativeFn),

    const Self = @This();

    pub fn init(allocator: std.mem.Allocator) Self {
        return .{ .map = std.StringArrayHashMap(NativeFn).init(allocator) };
    }

    pub fn deinit(self: *Self) void {
        self.map.deinit();
    }

    pub fn append(self: *Self, name: []const u8, comptime func: NativeFn) !void {
        try self.map.put(name, func);
    }

    pub fn addMath(self: *Self) !void {
        try self.append("log2", &math.log2);
        try self.append("log10", &math.log10);
        try self.append("pi", &math.pi);
        try self.append("sin", &math.sin);
        try self.append("cos", &math.cos);
        try self.append("tan", &math.tan);
        try self.append("asin", &math.asin);
        try self.append("acos", &math.acos);
        try self.append("atan", &math.atan);
        try self.append("complex", &math.complex);
        try self.append("abs", &math.abs);
        try self.append("phase", &math.phase);
        try self.append("rand", &math.rand);
        try self.append("pow", &math.pow);
        try self.append("sqrt", &math.sqrt);
        try self.append("ceil", &math.ceil);
        try self.append("floor", &math.floor);
        try self.append("round", &math.round);
    }

    pub fn addFs(self: *Self) !void {
        try self.append("create_file", &fs.create_file);
        try self.append("read_file", &fs.read_file);
        try self.append("write_file", &fs.write_file);
        try self.append("delete_file", &fs.delete_file);
        try self.append("create_dir", &fs.create_dir);
        try self.append("delete_dir", &fs.delete_dir);
    }

    pub fn addTypes(self: *Self) !void {
        try self.append("double", &types.double);
        try self.append("int", &types.int);
        try self.append("str", &types.str);
    }

    pub fn addTime(self: *Self) !void {
        try self.append("now", &time.now);
        try self.append("now_ns", &time.now_ns);
        try self.append("now_ms", &time.now_ms);
    }

    pub fn addOthers(self: *Self) !void {
        try self.append("what_is", &what_is);
    }

    pub fn names(self: Self) []const []const u8 {
        return self.map.keys();
    }
    pub fn functions(self: Self) []const NativeFn {
        return self.map.values();
    }
    pub fn define(self: Self) void {
        for (self.names(), self.functions()) |n, f| {
            vm.defineNative(@ptrCast(n), @ptrCast(f));
        }
    }
};

pub fn what_is(argc: c_int, args: [*c]Value) callconv(.C) Value {
    if (argc != 1) return stdlib_error("what_is() expects 1 argument!", .{ .argn = argc });

    const str = conv.what_is(args[0]);
    std.debug.print("Type: {s}\n", .{str});

    return conv.nil_val();
}

const Got = union(enum) {
    value_type: []const u8,
    argn: i32,
};

pub fn stdlib_error(message: []const u8, got: Got) Value {
    std.log.err("{s}", .{message});
    switch (got) {
        .value_type => |v| std.log.err("Got a {s} type...", .{v}),
        .argn => |n| std.log.err("Got {d} arguments...", .{n}),
    }
    return conv.nil_val();
}

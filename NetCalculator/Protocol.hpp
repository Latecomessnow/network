#pragma once

// 请求
struct Request
{
    int x;   // 左操作数
    int y;   // 有操作数
    char op; // 操作符
};

// 响应
struct Response
{
    int code;   // 状态码
    int result; // 结果
};
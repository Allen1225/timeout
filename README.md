# PHP函数执行超时控制扩展 (For PHP7+)
控制PHP函数的执行时间，超时终止执行，支持毫秒级配置，在call_user_func()基础上加了超时控制机制。

### 环境&依赖
* OS: linux
* Version: PHP 7.0.0及以上
* SAPI: php-fpm or cli

### 特性
* 通用：函数级控制超时
* 支持毫秒级配置

### 函数
    
    int call_func_with_timeout(mixed callable, int interval [, array args [, mixed retval]]


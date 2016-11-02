# PHP函数执行超时控制扩展 (For PHP7+)
[![Build Status](https://travis-ci.org/pangudashu/timeout.svg?branch=master)](https://travis-ci.org/pangudashu/timeout)

控制PHP函数的执行时间，超时终止执行，支持毫秒级配置，在call_user_func()基础上加了超时控制机制。

### 版本
v1.0.0

### 环境&依赖
* OS: linux
* Version: PHP 7.0.0及以上
* SAPI: php-fpm or cli

### 特性
* 通用：函数级控制超时
* 支持毫秒级配置

### 函数
    
    int *call_func_with_timeout*(mixed callable, int interval [, array args [, mixed retval]]

    Parames：
    $callable array|string 调用函数名称，普通function传函数名，类方法传数组:array(class|object, method)
    $interval int          超时时间，单位:ms
    $args     array        (可选)调用函数的参数数组
    $retval   mixed        (可选)调用函数的返回值，此值以引用传入

    Return:
    TT_CALL_FUNCTION_SUCC: 执行成功(未超时)
    TT_CALL_FUNCTION_TIMEOUT: 执行超时
    TT_SET_FUNCTION_CALL_INFO_ERROR: 调用函数不合法(如：类或者对象不存在)
    TT_FUNCTION_CALL_FAILURE: 调用失败(如：方法不存在、调用私有方法等)
    TT_SET_SIGNAL_ERROR: 设置signal handler错误(系统不支持此扩展)
    TT_SET_TIMER_ERROR: 同上

### 安装

    git clone git@github.com:pangudashu/timeout.git
    cd timeout
    phpize
    ./configure
    make && make install

    add "extension=timeout.so" to php.ini

### Demo

    <?php

    class sync {

        public function send($a,$b,$c,$d){
            sleep(2);

            print_r(get_defined_vars());

            return 1024;
        }
    }

    $sync_send_ret = null;
    $r = call_func_with_timeout(array("sync", "send"), 1200, array(1,2,3,4), $sync_send_ret);

    echo "sync::send() return value:" . $sync_send_ret . "\n";
    switch($r)
    {
        case TT_CALL_FUNCTION_SUCC:
            echo 'call function success';
            break;
        case TT_SET_FUNCTION_CALL_INFO_ERROR:
            echo 'set function call info error';
            break;
        case TT_FUNCTION_CALL_FAILURE:
            echo 'call function failure';
            break;
        case TT_SET_SIGNAL_ERROR:
            echo 'set signal handler error';
            break;
        case TT_SET_TIMER_ERROR:
            echo 'set timer error';
            break;
        case TT_CALL_FUNCTION_TIMEOUT:
            echo 'call function timeout';
            break;
    }
    ?>

执行结果：

    sync::send() return value:
    call function timeout
     
如果将sleep(2)注释掉则输出：
    
    Array
    (
        [a] => 1
        [b] => 2
        [c] => 3
        [d] => 4
    )
    sync::send() return value:1024
    call function success 



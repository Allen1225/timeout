# PHP extension to control function's excution time (For PHP7+)
[![Build Status](https://travis-ci.org/pangudashu/timeout.svg?branch=master)](https://travis-ci.org/pangudashu/timeout)

[![中文]](https://github.com/pangudashu/timeout/blob/master/README_ZH.md)

It's an extension to control PHP function's excution time.In fact,it just have one function:call_func_with_timeout() which similar to function `call_user_func()`.I add timeout control to it.

Why written this extesion?In my work,I find many circumstances that function executed long time and block the process,so I need a method to control the execution time of function.

When call a function, we can set timeout on it and it will be forced stoped if it's still in executing over the limit time.

This exetension use sigsetjmp() and siglongjmp() to implement the jump of running stack which like PHP's `try-catch` implement.

### Version
v1.0.0

### Environment Requirements
* OS: linux
* Version: PHP 7.0.0 or newer
* SAPI: php-fpm or cli

### Features
* timeout control on function level
* support millisecond-level config

### Function

```c
int call_func_with_timeout(mixed callable, int interval [, array args [, mixed retval]]

Parames：

$callable array|string  [required]  //call function name or array(class|object, method)
$interval int           [required]  //timeout interval，unit:ms
$args     array         [optional]  //function request args
$retval   mixed         [optional]


Return:[type int]

TT_CALL_FUNCTION_SUCC:           //execute success
TT_CALL_FUNCTION_TIMEOUT:        //execute timeout
TT_SET_FUNCTION_CALL_INFO_ERROR: //the function called invalid,eg:class or object not exist
TT_FUNCTION_CALL_FAILURE:        //execute failed,the same as TT_SET_FUNCTION_CALL_INFO_ERROR,eg:function not exist
TT_SET_SIGNAL_ERROR:             //set signal handler error
TT_SET_TIMER_ERROR:              //set timer error
```

### Install

    git clone git@github.com:pangudashu/timeout.git
    cd timeout
    phpize
    ./configure
    make && make install

    After install, add "extension=timeout.so" to php.ini

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

Result output：

    sync::send() return value:
    call function timeout
     
If annotate `sleep(2)`,result output:
    
    Array
    (
        [a] => 1
        [b] => 2
        [c] => 3
        [d] => 4
    )
    sync::send() return value:1024
    call function success 



--TEST--
Check for try-catch function
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
function func_exception(){
    throw new Exception("function exception");
}

function test(){
    echo "start\n";

    try{
        func_exception();
    }catch(Exception $e){
        echo $e->getMessage(),"\n";
    }

    sleep(2);
    echo "execute success\n";
}

$res = call_func_with_timeout("test", 2000);

var_dump(TT_CALL_FUNCTION_TIMEOUT == $res);

?>
--EXPECT--
start
function exception
bool(true)

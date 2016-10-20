--TEST--
Check for call user class undefined method
--SKIPIF--
<?php if (!extension_loaded("timeout")) print "skip"; ?>
--FILE--
<?php
class Test {
    public function called_func($id){
        echo $id,"\n";
        sleep(1);
        return "Test::called_func";
    }
}

$obj = new Test;

$retval = null;
$r = call_func_with_timeout(array('tEst', 'called_funC'), 2000, array(1024), $retval);
echo $r,"\n";
echo $retval;
?>
--EXPECT--
1024
0
Test::called_func


--TEST--
Boyer-Moore Usage variation - Studied needles
--SKIPIF--
<?php if (!extension_loaded("boyermoore")) print "skip"; ?>
--FILE--
<?php 

$needle = boyermoore_study('hello world');
var_dump($needle);

var_dump(boyermoore_indexof('i like writing hello world programs', $needle));

var_dump(boyermoore_indexof('hello world i like hello world', $needle, 10));

?>
--EXPECTF--
resource(%d) of type (Boyer-Moore Studied Needle)
int(15)
int(19)

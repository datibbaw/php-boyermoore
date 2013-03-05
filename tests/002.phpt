--TEST--
Boyer-Moore Error conditions
--SKIPIF--
<?php if (!extension_loaded("boyermoore")) print "skip"; ?>
--FILE--
<?php 

var_dump(boyermoore_study(array(1,2,3)));

var_dump(boyermoore_indexof(null, null));
var_dump(boyermoore_indexof(null, ''));

var_dump(boyermoore_indexof(null, '1'));

$f = fopen(__FILE__, 'r');
var_dump(boyermoore_indexof('', $f));

?>
--EXPECTF--
Warning: boyermoore_study() expects parameter 1 to be string, array given in %s on line %d
NULL

Warning: boyermoore_indexof(): Needle must be a string or studied pattern in %s on line %d
bool(false)

Warning: boyermoore_indexof(): Empty needle in %s on line %d
bool(false)
bool(false)

Warning: boyermoore_indexof(): supplied resource is not a valid Boyer-Moore Studied Needle resource in %s on line %d
bool(false)


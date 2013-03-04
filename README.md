php-boyermoore
==============

Optimized searching for larger haystacks.

Basic usage:
```
    $pos = boyermoore_indexof('i like writing hello world programs', 'hello world');
```
This works exactly like `strpos()`, except that it's optimized for a larger haystack by skipping more than one characters while searching for the needle.

Advanced usage:
```
    $needle = boyermoore_study('hello world');
    $pos = boyermoore_indexof('i like writing hello world programs', $needle);
```
This option caches the meta information for the needle and then uses it to search the haystack. It's designed to be applied multiple times onto a new haystack.


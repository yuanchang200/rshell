#!/bin/sh
(echo yuan && echo chang) || (echo y && echo c)
((echo yuan && echo chang) || echo c) && test yuan
(echo yuan && echo chang) || echo u
echo yuan && (echo chang || echo u)
(echo yuan || echo chang) && echo u
echo yuan || (echo chang && echo u)
(echo yuan; echo chang && echo u) || echo y
(echo yuan || (echo chang && echo o)) && test yuan
(test yuan && (echo yuan || echo o)) && ls -a
( test yuan && echo yuan ) || ( echo y && echo c )
( test yuan && echo yuan) || (echo y && echo c )
test yuan && (echo yuan; echo chang) && echo o
(test chang || echo o) && (test yuan || echo u)
(test yuan; echo u) && (exit; echo i)
test yuan; (echo yuan || echo yuan); echo i
(((echo yuan && test yuan) || test o) && echo o) && exit
(((echo yuan || test chang); test o) && echo p) || exit
(((echo yuan; test yuan); echo i) || echo p); echo i
echo yuan || (test yuan && (ls -a || exit;(echo u && echo i)))
test yuan && (test u || ls -a || (exit && (echo p; echo i)))
echo yuan && (test yuan; (echo u || echo y) || (echo yuan; echo u)) && exit
(test yuan && test chang) && test i || (exit && echo u)
(test yuan; test chang; test o) && ( echo y || echo u) && exit

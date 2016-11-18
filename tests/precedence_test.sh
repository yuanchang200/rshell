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


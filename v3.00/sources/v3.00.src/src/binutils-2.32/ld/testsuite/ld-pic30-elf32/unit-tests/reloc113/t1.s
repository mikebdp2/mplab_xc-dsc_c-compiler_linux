        .text
        .global _main
_main:
        call secure(0)
        call secure(1)
        call secure(2)
        call secure(3)
        call secure(4)
        call secure(5)
        call secure(6)
        call secure(7)
        call secure(8)
        call secure(9)
        call secure(10)
        call secure(11)
        call secure(12)
        call secure(13)
        call secure(14)
        call secure(15)
        ;; slot 16 is reserved
        call secure(17)
        call secure(18)
        call secure(19)
        call secure(20)
        call secure(21)
        call secure(22)
        call secure(23)
        call secure(24)
        call secure(25)
        call secure(26)
        call secure(27)
        call secure(28)
        call secure(29)
        call secure(30)
        call secure(31)
        ;;
        goto secure(0)
        goto secure(1)
        goto secure(2)
        goto secure(3)
        goto secure(4)
        goto secure(5)
        goto secure(6)
        goto secure(7)
        goto secure(8)
        goto secure(9)
        goto secure(10)
        goto secure(11)
        goto secure(12)
        goto secure(13)
        goto secure(14)
        goto secure(15)
        ;; slot 16 is reserved
        goto secure(17)
        goto secure(18)
        goto secure(19)
        goto secure(20)
        goto secure(21)
        goto secure(22)
        goto secure(23)
        goto secure(24)
        goto secure(25)
        goto secure(26)
        goto secure(27)
        goto secure(28)
        goto secure(29)
        goto secure(30)
        goto secure(31)
        
        .end
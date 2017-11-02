(DEFUN MEM (X LIST) 
    (COND 
        ((NULL LIST) NIL) 
        (T (COND 
            ((EQ X (CAR LIST)) T) 
            (T (MEM X (CDR LIST)))))))

(MEM 2 (QUOTE (2 3 4)))
(MEM (PLUS 1 2) (QUOTE (2 3 4)))
(MEM (MINUS 10 6) (QUOTE (2 3 4)))

(MEM 5 (QUOTE (2 3 4)))
(MEM 6 (QUOTE (2 3 4)))

(MEM A (QUOTE (2 3 4)))
(MEM B (QUOTE (2 3 4)))
(ATOM (1))
(ATOM (1 2))

(INT (1))
(INT (A))
(INT (1 A))

(MINUS (1) 1)
(PLUS (PLUS (1) 2) 3)
(TIMES (MINUS (5) 2) 9)

(LESS (5) 10)
(LESS (5) 2)
(GREATER (10) 5)
(GREATER (5) 10)
(EQ (5) 5)
(EQ (6) 10)

(CAR (1 2 3))
(CDR (1 2 3))

(CONS (1 2 3) (4 5 6))

(QUOTE (CDR (1 2 3)))
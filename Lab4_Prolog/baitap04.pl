men('John').
men('Fred').
men('Harry').
women('Mary').
women('Julie').
women('Susan').
women('Anne').

blonde_hair('John').
dark_hair('Fred').
dark_hair('Harry').

blonde_hair('Julie').
blonde_hair('Susan').
brunette_hair('Mary').
brunette_hair('Anne').

rich(X) :- have(X, 'gold').
have('Fred', 'gold').
have('Julie', 'gold').

like('John', X) :- rich(X), women(X).
like('Harry', X) :- rich(X), women(Y).
like('John', X) :- blonde_hair(X), women(X).
like('Fred', X) :- brunette_hair(X), women(X).

like('Mary', X) :- dark_hair(X), men(X).
like('Julie', X) :- dark_hair(X), men(X).
like('Julie', X) :- rich(X), men(X).

have('Anne', 'house').
have('John', 'car').

%men(X).
%like('John', X).
%like('Mary', X).
%like('Julie', X).
%have(X, Y).
%like(X, Y), like(Y, X).
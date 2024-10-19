% Moi con thu nuoi nho deu la thu nuoi trong nha.
indoorpet(X) :- smallpet(X).
% Cho va meo deu la thu nuoi.
pet(X) :- dog(X).
pet(X) :- cat(X).
% Cho long xu la cho nho.
smallpet(X) :- dog(X), fluffy(X).
% Fluto la cho nho
dog('Fluto').
fluffy('Fluto').

%indoorpet('Fluto').

% Định nghĩa ai hạnh phúc
happy(X) :- no_poor(X), smart(X).

% Định nghĩa ai thông minh
smart(X) :- read_book_regularly(X).

% Định nghĩa ai thường xuyên đọc sách
read_book_regularly('An').

% Định nghĩa ai không nghèo
no_poor('An').

% Định nghĩa ai có cuộc sống sôi động
vibrant_life(X) :- happy(X).


% smart('An').
% happy(X).
% vibrant_life('An').
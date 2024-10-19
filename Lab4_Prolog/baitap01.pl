% a. Bieu dien cac menh de bang ngon ngu Prolog
% Mary thich thuc an.
like('Mary', 'food').
% Mary thich ruou vang.
like('Mary', 'wine').
% John thich ruou vang.
like('John', 'wine').
% John thich Mary.
like('John', 'Mary').

/*
b. Thanh lap truy van va ghi nhan ket qua
like('Mary', 'food').
like('John', 'wine').
like('John', 'food').
*/

% c. Bo sung cac luat
% John thich nhung gi ma Mary thich.
like('John', X) :- like('Mary', X).
% John thich nguoi thich ruou vang.
like('John', X) :- like(X, 'wine').
% John thich nhung nguoi thich chinh ban than ho.
like('John', X) :- like(X, X).

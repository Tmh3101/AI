like('Tuan', X) :- easy_subject(X).
hard_subject('Tri tue nhan tao').
easy_subject(X) :- using_technology_subject(X).
using_technology_subject('TC333').

% like('Tuan', X).
% Marcus la nguoi.
person('Marcus').
% Marcus la nguoi Pompeian
pompeian_person('Marcus').
% Nguoi Pompeian la nguoi la ma
roman_person(X) :- pompeian_person(X).
% Ceasar la lanh chua
lord('Ceasar').
% Mot nguoi la ma neu khong trung thanh voi lanh chua se am sat ong ta
kill_person(X, Y) :- lord(Y), disloyal(X, Y), roman_person(X).
% Marcus khong trung thanh voi Ceasar
disloyal('Marcus', 'Ceasar').

%kill_person('Marcus', 'Ceasar').
%roman_preson(X).
%lord(X).
%kill_person(X, Y).
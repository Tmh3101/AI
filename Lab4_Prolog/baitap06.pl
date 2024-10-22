tenis_club_member('Long').
tenis_club_member('Trinh').
tenis_club_member('Hung').
tenis_club_member('Anh').
wife('Trinh', 'Long').
brother('Hung', 'Anh').
tenis_club_member(X) :- wife(X, Y), tenis_club_member(Y).
most_recent_meeting_at_house('Long').
most_recent_meeting_at_house(X) :- (wife(X, Y); wife(Y, X)), most_recent_meeting_at_house(Y).

% wife(X, Y).
% most_recent_meeting_at_house('Trinh').

Requirements
=========

This are the requirements from user's perspective:

* This should be a desktop application.
* It should be a game of cruce, as described in docs/game-rules_en.md.
* It should allow a number of 2, 3 or 4 players, at least 1 human.
* It should allow observers. They should not be able to interact with the game
by any means. They should see the cards from all users and their choices.
* The players and the observers may interact using an in-game chat.
* A player must not be allowed to cheat, i.e. to make an illegal move, as
described in docs/game-rules.md.
* If there is more than one player, they should play over the network: one
player creates a host and the other ones, using some login details, join the
host.
* Observers may observe the game using the game by joining it. An observer may
enter the game at any moment, while players may not join the game after it
started.
* The players should be noticed when an observer enters or leaves the game.
For noticing the chat may be used.
* When a player leaves, his position is taken by an AI, for the other players
to continue the game. It is possible to rejoin the game. However, if the host
leaves, the game should end.
* A user should not be allowed to run 2 instances of the application.
* There may be more UIs for this application and it might be supported over
multiple platforms. It should be possible to play the game using any of them.
* The application must run on linux operating system. If possible, it should
be cross-platform.


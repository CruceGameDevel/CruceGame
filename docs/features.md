Features
========

Document all not-so-obvious features (e.g. multiplayer, AI, networked game,
planned UIs, etc)

- The game will be multiplayer, networked and it will allow the players to form
  teams, as described in docs/game-rules_en.md.
- The game will have an AI module that may replace as many players as needed.
  For a game to be possible, a least one human is needed.
- The game will be multithreaded.
- The primary OS is linux. Use as many cross-platform features as possible.
- The code should respect C99 standard and be at least POSIX compliant, if not
  POSIX conformant.
- The cutter framework will be used for unit testing.
- gcc should be used as the main compiler.
- Use make to write a compile script for the game. When you write a module,
  do not forget to add build rules for it in the Makefile.
- The UI interface should be developed in 2 stages: a CLI interface, using
  ncurses, and a GUI interface using GTK.
- There must be an API, to allow other developers to build other interfaces.
  The interface should not interfere with the game. It's only purpose is user
  interaction.
- The business logic of the application must be packaged as a library (or more,
  if it is needed) in order to use it for UI development.
- The build must be tested with valgrind.


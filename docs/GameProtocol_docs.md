Game protocol commands
=========

**REQUEST_BID**
Syntax:
```
$REQUEST_BID <username>
```
This command is used by host to request the bid of a player. <username>
represents the player name to whom the bid is required.

**SEND_BID**
Syntax:
```
$SEND_BID <value>
```
This command send the bid of a player. <value> represents the bid value.

**REQUEST_CARD**
Syntax:
```
$REQUEST_CARD <username>
```
This command is used by host to request the card of a player. <username>
represents the player name to whom the card is required.

**SEND_CARD**
Syntax:
```
$SEND_CARD <value> <suit>
```
This command send the card of a player. <value> represents the card value and
<suit> represents the suit card.

**DISTRIBUTE_CARD**
Syntax:
```
$DISTRIBUTE_CARD <recipient> <value> <suit>
```
This command is used by host to distribute the cards deck. <recipient>
represents the name of the recipient of the card, <value> represents the card
value and <suit> represents the card suit.

**UPDATE_SCORE**
Syntax:
```
$UPDATE_SCORE <name1> <name2> <score1> , <name3> <name4> <scoare2>
```
This command is used by host to send the new score. <name1> and <name2>
represents the players name from the first team and <score1> represents the
score of the first team. <name3> and <name4> represents the players name from
the second team and <score2> represents the score of the second team.
If the game is not on teams then the syntax is:
```
$UPDATE_SCORE <name1> <score1> , <name2> <score2> , <name3> <score3> ...
```

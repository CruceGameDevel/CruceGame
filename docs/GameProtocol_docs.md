Game protocol commands
=========

**REQUEST_BID**
<br>Syntax:<br/>
```
$REQUEST_BID <username>
```
This command is used by host to request the bid of a player. username
represents the player name to whom the bid is required.



<br>**SEND_BID**<br/>
Syntax:
```
$SEND_BID <value>
```
This command send the bid of a player. value represents the bid value.



<br>**REQUEST_CARD**<br/>
Syntax:
```
$REQUEST_CARD <username>
```
This command is used by host to request the card of a player. username
represents the player name to whom the card is required.



<br>**SEND_CARD**<br/>
Syntax:
```
$SEND_CARD <value> <suit>
```
This command send the card of a player. value represents the card value and
suit represents the suit card.



<br>**DISTRIBUTE_CARD**<br/>
Syntax:
```
$DISTRIBUTE_CARD <recipient> <value> <suit>
```
This command is used by host to distribute the cards deck. recipient
represents the name of the recipient of the card, value represents the card
value and suit represents the card suit.



<br>**UPDATE_SCORE**<br/>
Syntax:
```
$UPDATE_SCORE <name1> <name2> <score1> , <name3> <name4> <scoare2>
```
This command is used by host to send the new score. name1 and name2
represents the players name from the first team and score1 represents the
score of the first team. name3 and name4 represents the players name from
the second team and score2 represents the score of the second team.
If the game is not on teams then the syntax is:
```
$UPDATE_SCORE <name1> <score1> , <name2> <score2> , <name3> <score3> ...
```

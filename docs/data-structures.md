These are the basic data structures to be used in this project
=======

### Card
* color
* value

The values for color and value should be constants.

### Player
* id
* name
* score
* cards (in his hand at the moment)
* socketfd (file descriptor of the socket used to communicate with player)
* type (human or AI)

### Team
* id
* players (array of Player)

### Round
* id
* trump
* hands (array of Hand data structures)
* bids (association between players and their bids)

### Hand
* cards (association between players and cards they put in this round)

### Game
* teams (array of Team)
* noOfRounds


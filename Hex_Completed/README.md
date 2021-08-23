This a HEX Game.
HEX is a 2 player game which cannot end in Tie.
The code in this can be used to generate and size "n" board. 
The most commmon version is 11 * 11 board.
![HEX GAME Image](https://github.com/Memester2112/CPP-Programs/blob/master/Hex_Completed/Hex_Game.png)

---

The objective of the game is to build a bridge from one boundary to its opposite boundary. Thus according to the diagram one player needs to build the bridge from the blue boundary to the other and player 2 needs to build a bridge from one red boundary to the other.  

---

There is also a PIE Rule in this game. Research has shown that there are positions, which when played in the 1st move increase chances of success drastically. Thus after Player 1 has played their first move, Player 2 has a choice of switching colour.

---

One can play the game in 2 modes:

a) Player 1 vs Player 2
b) Player 1 vs AI

To choose a move one must give a tuple of 2 number (x,y) where x ∈ [0,10] and y ∈ [0,10]

---

The AI player uses a probablistic model to decide upon its next move.

Consider a particular state in the game and the AI has to make a move. Say it has to choose from 101 possible moves.
Now the AI has to choose between these 101 alternatives and needs a scoring mechanism to rank the moves.

Lets say the AI is evaluating moving at the position (3,5), then the AI will play its move on the current board and then proceed to alternatively play for the Player and itself randomly until one of them wins. It repeats the same strategy of randomly filling a 1000 times. Then the score for the move (3,5) is the number of times the AI wins. Say if the AI wins for 600/1000 cases the score is 600.

This process is repeated for all valid moves that the AI needs to evaluate. So for 101 valid moves, a total of 101000 games are played in advance by the AI.

Thus the AI effectively looks into possible futures to make the best move.

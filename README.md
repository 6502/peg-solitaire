# peg-solitaire
A solver for the peg solitaire problem

The peg solitaire is based on a 33-places board where there are pegs and holes and in the start configuration there is only one hole in the middle of the board and all other places are filled with a peg.

At each move the player must select a peg and "jump over" a 4-adjacent peg ending up in a hole; the "jumped over" peg is removed fromthe board. The english starting position is:

                             o o o             o = peg
                             o o o             . = hole
                         o o o o o o o
                         o o o . o o o
                         o o o o o o o
                             o o o
                             o o o

and there are therefore 4 possibile initial moves.

At each move one peg is removed from the board and one is moved. Goal of the solitaire is to end up with a single peg in the central position of the board.

The solver uses a simple propagation graph traversal algorithm representing each position as a 33-bit number. The table of all visited position is kept in a packed bit array (that alone requires 1Gb of memory).

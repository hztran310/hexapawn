#include <iostream>
#include <vector>
#include <cctype>
#pragma clang diagnostic ignored "-Wc++11-extensions"

using namespace std;

const int BOARD_SIZE = 3;

enum Player
{
    Empty,
    X,
    O
};

class HexapawnGame
{
private:
    vector<vector<Player> > board;
    Player currentPlayer;
    bool gameOver; 

public:
    HexapawnGame() : board(BOARD_SIZE, vector<Player>(BOARD_SIZE, Player::Empty)), currentPlayer(Player::X), gameOver(false)
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            board[0][i] = Player::X;
            board[BOARD_SIZE - 1][i] = Player::O;
        }
    }

    void play()
    {
        cout << "----- Hexapawn Game -----" << endl;
        cout << "Instructions: Enter the row and column of the pawn you want to move (e.g., 1 2)." << endl;
        cout << "Player 1: X, Player 2: O" << endl;

        while (true)
        {
            cout << endl;
            printBoard();

            if (currentPlayer == Player::X)
                cout << "Player 1 (X) turn: ";
            else
                cout << "Player 2 (O) turn: ";

            if (!hasValidMove())
            {
                cout << "No valid moves available. Player " << static_cast<int>(getOpponentPlayer(currentPlayer)) << " wins!" << endl;
                break;
            }

            if (gameOver) // Check if the game is over before accepting the opponent's move
            {
                cout << "Game over! Player " << static_cast<int>(getOpponentPlayer(currentPlayer)) << " wins!" << endl;
                break;
            }

            cout << "From: ";
            int fromRow, toRow;
            char fromCol, toCol;
            cin >> fromRow >> fromCol;

            cout << "Move to: ";
            cin >> toRow >> toCol;

            fromRow--;
            fromCol = tolower(fromCol) - 'a';
            toRow--;
            toCol = tolower(toCol) - 'a';

            if (isValidMove(fromRow, fromCol, toRow, toCol))
            {
                makeMove(fromRow, fromCol, toRow, toCol);

                switchPlayer();

                if (isGameOver())
                {
                    cout << endl;
                    printBoard();
                    cout << "Player " << static_cast<int>(getOpponentPlayer(currentPlayer)) << " wins" << endl;
                    break;
                }
            }
            else
            {
                cout << "Invalid move! Try again." << endl;
            }
        }
    }
    bool hasValidMove()
    {
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == currentPlayer)
                {
                    if (currentPlayer == Player::X)
                    {
                        if (isValidMove(i, j, i + 1, j) || isValidMove(i, j, i + 1, j - 1) || isValidMove(i, j, i + 1, j + 1))
                            return true;
                    }
                    if (currentPlayer == Player::O)
                    {
                        if (isValidMove(i, j, i - 1, j) || isValidMove(i, j, i - 1, j - 1) || isValidMove(i, j, i - 1, j + 1))
                            return true;
                    }
                }
            }
        }

        return false;
    }

private:
    void printBoard()
    {
        cout << "  A B C" << endl;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            cout << i + 1 << " ";
            for (int j = 0; j < BOARD_SIZE; j++)
            {

                switch (board[i][j])
                {
                case Player::X:
                    cout << "X ";
                    break;
                case Player::O:
                    cout << "O ";
                    break;
                case Player::Empty:
                    cout << ". ";
                    break;
                }
            }
            cout << endl;
        }
    }

    bool isInside(int pos)
    {
        return pos >= 0 && pos < BOARD_SIZE;
    }

    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol)
    {
        // Check if the coordinates are within bounds
        if (!isInside(fromRow) || !isInside(fromCol) || !isInside(toRow) || !isInside(toCol))
            return false;

        // Check if the current player is moving their own pawn
        if (board[fromRow][fromCol] != currentPlayer)
            return false;

        // Check if the target position is empty or contains the opponent's pawn
        if (board[toRow][toCol] == Player::Empty && ((currentPlayer == Player::X && toRow == fromRow + 1 && toCol == fromCol) 
            || (currentPlayer == Player::O && toRow == fromRow - 1 && toCol == fromCol)))
            return true;

        // Check if the move is a capture
        if (abs(toCol - fromCol) == 1 && ((currentPlayer == Player::X && toRow == fromRow + 1 && board[toRow][toCol] == getOpponentPlayer(currentPlayer)) 
            || (currentPlayer == Player::O && toRow == fromRow - 1 && board[toRow][toCol] == getOpponentPlayer(currentPlayer))))
            return true;

        return false;
    }

    Player getOpponentPlayer(Player player)
    {
        if (player == Player::X)
            return Player::O;
        else
            return Player::X;
    }

    void makeMove(int fromRow, int fromCol, int toRow, int toCol)
    {
        Player opponentPlayer = getOpponentPlayer(currentPlayer);

        // Check if the move is a capture
        if (abs(toCol - fromCol) == 1)
        {
            // Diagonal capture
            board[toRow][toCol] = board[fromRow][fromCol];
            board[fromRow][fromCol] = Player::Empty;

            // Replace opponent pawn if captured
            if (board[toRow][toCol] == opponentPlayer)
            {
                int captureCol = fromCol + (toCol - fromCol) / 2;
                board[toRow][captureCol] = currentPlayer;
            }
        }
        else
        {
            // Forward move
            board[toRow][toCol] = board[fromRow][fromCol];
            board[fromRow][fromCol] = Player::Empty;
        }

        // Check if the current player has no pawns left or if the opponent has no valid moves left
        if (isGameOver() || currentPlayerReachedOpponentEnd())
        {
            gameOver = true; 
        }
    }

    bool currentPlayerReachedOpponentEnd()
    {
        if (currentPlayer == Player::X)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[BOARD_SIZE - 1][j] == Player::X)
                    return true; // Current player's pawn reached the opponent's end
            }
        }
        else if (currentPlayer == Player::O)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[0][j] == Player::O)
                    return true; // Current player's pawn reached the opponent's end
            }
        }

        return false; // Current player's pawn hasn't reached the opponent's end
    }
    void switchPlayer()
    {
        currentPlayer = (currentPlayer == Player::X) ? Player::O : Player::X;
    }

    bool isGameOver()
    {
        Player opponentPlayer = getOpponentPlayer(currentPlayer);

        // Check if the opponent has no pawns left
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == opponentPlayer)
                    return false; // There is still an opponent's pawn left
            }
        }

        // Check if the opponent's pawns are unable to move
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[i][j] == currentPlayer)
                {
                    // Check if any valid move is available for the opponent's pawn
                    if (isValidMove(i, j, i + 1, j) || isValidMove(i, j, i + 1, j - 1) || isValidMove(i, j, i + 1, j + 1))
                        return false; // At least one valid move is available for the opponent
                }
            }
        }

        // Check if the current player's pawn reached the end of the opponent's end
        if (currentPlayer == Player::X)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[BOARD_SIZE - 1][j] == Player::X)
                    return true; 
            }
        }
        else if (currentPlayer == Player::O)
        {
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (board[0][j] == Player::O)
                    return true; 
            }
        }

        return true;
    }
};

int main()
{
    HexapawnGame game;
    game.play();
    return 0;
}

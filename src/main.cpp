#include <iostream>
#include <string>
#include <cmath>

int isKingNextToKing(int blackKingLetter,int blackKingNumber,int  whiteKingLetter, int whiteKingNumber);
int blackKingInCheck(int blackKingLetter, int blackKingNumber);
int whiteKingInCheck(int whiteKingLetter, int whiteKingNumber);

enum PieceType
{                                                                                                                 
    PW = 0, // pawn white
    RW = 1, // rook white
    NW = 2, // (K)Night
    BW = 3,
    QW = 4,
    KW = 5, // King white
    PB = 6,
    RB = 7,
    NB = 8,
    BB = 9,
    QB = 10,
    KB = 11,
    XX = 12 // Freies feld
};
char pieceToChar(PieceType piece)
{ // wandelt die enmu int i ein char fuer lesbarkeit beim printen
    switch (piece)
    {
    case PW:
        return 'P';
    case RW:
        return 'R';
    case NW:
        return 'N';
    case BW:
        return 'B';
    case QW:
        return 'Q';
    case KW:
        return 'K';
    case PB:
        return 'p';
    case RB:
        return 'r';
    case NB:
        return 'n';
    case BB:
        return 'b';
    case QB:
        return 'q';
    case KB:
        return 'k';
    case XX:
        return '.';
    default:
        return '?';
    }
}

PieceType board[8][8] = {
    {RB, XX, XX, XX, KB, XX, XX, RB}, // Reihe 0 = Schwarz
    {XX, XX, XX, XX, XX, XX, XX, XX},
    {XX, XX, XX, XX, XX, XX, XX, XX},
    {XX, XX, XX, XX, XX, XX, XX, XX},
    {XX, XX, XX, XX, XX, XX, XX, XX},
    {XX, XX, XX, XX, XX, XX, XX, XX},
    {XX, XX, XX, XX, XX, XX, XX, XX},
    {RW, XX, XX, XX, KW, XX, XX, RW}  // Reihe 7 = Weiß
};




void copyBoard(PieceType source[8][8], PieceType copy[8][8]) { //kopiert source in copy
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            copy[i][j] = source[i][j];
        }
    }
}




void printBoard(PieceType arr[8][8])
{ // printet das Board inklusive notation aus kleinbuchstaben = Schwarz; Gross = Weiss
    std::cout << "\n";
    for (int i = 0; i < 8; i++)
    {
        std::cout << 8 - i << "|" << "  "; // notations hilfe
        for (int k = 0; k < 8; k++)
        {
            std::cout << pieceToChar(arr[i][k]) << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << " |_________________" << std::endl; // notationshilfe
    std::cout << "    a b c d e f g h";              // notationshilfe
};

bool checkPiecesAreIllegal(PieceType arr[8][8], int blackKingLetter, int blackKingNumber,int whiteKingLetter,int whiteKingNumber)
{ // zaehlt die menge an Bauern und Königen weil diese figuren begrenzt sind. Andere theoretische auch durch umwandlung max n+8;
    int counterBlackPawns = 0;
    int counterBlackKings = 0;
    int counterWhitePawns = 0;
    int counterWhiteKings = 0;
    bool pawnFirstRow = false;
    bool illegalLineup = false;
    for (int i = 0; i < 8; i++)
    {

        for (int k = 0; k < 8; k++)
        {
            switch (arr[i][k])
            {
            case PB:
                counterBlackPawns++;
                break;
            case KB:
                counterBlackKings++;
                break;
            case PW:
                counterWhitePawns++;
                break;
            case KW:
                counterWhiteKings++;
                break;
            }
            if ((i == 0 || i == 8) && (arr[i][k] == PW || arr[i][k] == PB))
            { // letzte und erste Reihe Bauer = nicht möglich
                pawnFirstRow = true;
                illegalLineup = true;
            }
        }
    }
    if (counterBlackKings > 1)
    {
        std::cout << "\nEs herrscht ein Koenig zuviel (Schwarz) !\n";
        illegalLineup = true;
    }
    if (counterWhiteKings > 1)
    {
        std::cout << "\nEs herrscht ein Koenig zuviel (Weiss)) !\n";
        illegalLineup = true;
    }
    if (counterBlackPawns > 8)
    {
        std::cout << "\nNur Acht Schwarze Bauern duerfen fuer ihren Koenig Kaempfen !\n";
        illegalLineup = true;
    }
    if (counterWhitePawns > 8)
    {
        std::cout << "\nNur Acht Weisse Bauern stehe duerfen fuer ihren Koenig Kaempfen!\n";
        illegalLineup = true;
    }
    if(isKingNextToKing( blackKingLetter, blackKingNumber,  whiteKingLetter, whiteKingNumber) == 1){
        illegalLineup = true;
    }
    if(blackKingInCheck(blackKingLetter, blackKingNumber) == true && whiteKingInCheck(whiteKingLetter, whiteKingNumber) == true){
        illegalLineup = true;
    }
    return illegalLineup; // Ensure a return value for all code paths
}

bool checkCorrectNotation(std::string notation)
{

    if (notation.length() != 2)
    {
        std::cout << "\n Ungueltige Eingabe! Ein Buchstabe und eine Zahl zb. B2";
        return false;
        ;
    }
    char letter = toupper(notation[0]); // toupper() wandelt Kleinbuchstaben in Grossbuchstaben um
    char number = notation[1];
    if (letter < 65 || letter > 72)
    {
        std::cout << "\n Ungueltige Eingabe! Ein Buchstabe und eine Zahl zb. B2";
        return false;
    }
    if (number < 49 || number > 56)
    {
        std::cout << "\n Ungueltige Eingabe! Ein Buchstabe und eine Zahl zb. B2";
        return false;
    }
    return true;
}

int getNotationInput(int &startLetter, int &startNumber, int &endLetter, int &endNumber)
{ // übergabe mit & gibt direkt denn Speicherort der Originalen Variable an. Macht keine Kopie
    std::cout << "\nGibt das Feld an auf welchem die Figur steht die du bewegen moechtest. (z.b A4)" << std::endl;
    std::string startNotation;
    std::cin >> startNotation;

    if (checkCorrectNotation(startNotation) != true)
    {
        return -1;
    }
    startLetter = toupper(startNotation[0]) - 'A'; // toupper() wandelt kleinbuchstaben in Grossbuchstaben um

    startNumber = 8 - (startNotation[1] - '0'); //'2' - '1' (ASCII) ->  50 - 49 = 1 bei input A1 muss der vektor [0][0] sein darum -1.

    std::cout << "\nGibt das Feld an wo du die Figur hinbewegen moechtest. (z.b B4)" << std::endl;
    std::string endNotation;
    std::cin >> endNotation;

    if (checkCorrectNotation(endNotation) != true)
    {
        return -1;
    }
    endLetter = toupper(endNotation[0]) - 'A'; // toupper() wandelt Kleinbuchstaben in Grossbuchstaben um
    endNumber = 8 - (endNotation[1] - '0');    //'2' - '1' (ASCII) ->  50 - 49 = 1 bei input A1 muss der vektor [0][7] sein. -'0' für typumwandlung denn '0' ascii 48 und zb '2' = 50

    return 1;
}

int checkIfLineIsClear(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber) // return -1 nicht selbe Linie. Bei return -2 figur im Weg. Bei return 1 keine figur im Weg
{
    if ((startPosLetter != endPosLetter) && (startPosNumber != endPosNumber))
    {
        // die Positionen sind nicht in der Selben Reihe / Spalte. Bei Dame evtl möglich
        return -1;
    }
    int squaresToMove = 0;
    if (startPosLetter == endPosLetter)
    { // vertikale
        squaresToMove = startPosNumber - endPosNumber;
        if (squaresToMove < 0) // bewegung nach Unten
        {
            squaresToMove = squaresToMove * -1; // macht squares to Move Positiv
            for (int i = 1; i <= squaresToMove - 1; i++)
            { // Die anfangs und end position wird anders geprüft darum squaresToMove-1
                if (board[startPosNumber + i][startPosLetter] != XX)
                {
                    return -2;
                }
            }
            return 1;
        }
        if (squaresToMove > 0) // bewegung nach Oben
        {
            for (int i = 1; i <= squaresToMove - 1; i++)
            { // Die anfangs und end position wird anders geprüft darum squaresToMove-1
                if (board[startPosNumber - i][startPosLetter] != XX)
                {
                    return -2;
                }
            }
            return 1;
        }

        return 1;
    }

    if (startPosNumber == endPosNumber)
    { // horizontale bewegung
        squaresToMove = startPosLetter - endPosLetter;
        if (squaresToMove < 0) // bewegung nach Rechts
        {
            squaresToMove = squaresToMove * -1; // macht squares to Move Positiv
            for (int i = 1; i <= squaresToMove - 1; i++)
            { // Die anfangs und end position wird anders geprüft darum squaresToMove-1
                if (board[startPosNumber][startPosLetter + i] != XX)
                {
                    return -2;
                }
            }
            return 1;
        }
        if (squaresToMove > 0) // bewegung nach Links
        {
            for (int i = 1; i <= squaresToMove - 1; i++)
            { // Die anfangs und end position wird anders geprüft darum squaresToMove-1
                if (board[startPosNumber][startPosLetter - i] != XX)
                {
                    return -2;
                }
            }
            return 1;
        }
    }
    return 1; // vergleich anfangs und endposition und  guckt ob dazwischen figuren stehen und ob beide position inder selben reihe sind
}
int returnColor(int positionLetter, int positionNumber)
{ // ret 1 = weiss; ret 2 = Schwarz; ret 3 = Keine Figur
    PieceType piece = board[positionNumber][positionLetter];
    if (piece >= PW && piece <= KW)
    {
        return 1;
    }
    else if (piece >= PB && piece <= KB)
    {
        return 2;
    }
    else
    {
        return 3;
    }
}

int checkStartAndEndPosition(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{ // return -1 wenn man eigene Figur Schlagen würde und return -2 wenn auf der Startposition keine Figur steht
    if (board[startPosNumber][startPosLetter] == XX)
    {
        std::cout << "\nWaehlen Sie ein Feld aus auf dem eine Figur steht.";
        return -2;
    }
    bool startColorIsWhite;
    if (board[startPosNumber][startPosLetter] <= KW && board[startPosNumber][startPosLetter] >= PW)
    { // Das KW /PW wird als Zahl aus den Enum Interpretiert
        startColorIsWhite = true;
    }
    else
    {
        startColorIsWhite = false;
    }

    if (board[endPosNumber][endPosLetter] == XX)
    { // Endfeld ist frei
        return 1;
    }
    bool endColorIsWhite;
    if (board[endPosNumber][endPosLetter] <= KW && board[startPosNumber][startPosLetter] >= PW)
    {
        endColorIsWhite = true;
    }
    else
    {
        endColorIsWhite = false;
    }
    // Die Farbe der End und Anfangsposition wurde ermittelt. Sind Sie identisch ist der Zug illegal
    if (startColorIsWhite == endColorIsWhite)
    {
        std::cout << "\nAuf Ihrem Zielfeld befindet sich eine Figur der eigenen Farbe! \n";
        return -1;
    }
    else
    {
        return 1;
    }
}

int checkIfDiagonalIsClear(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{ // return -1 wenn es NICHT Diagonal ist return -2 wenn etwas im weg Steht

    if (abs(endPosLetter - startPosLetter) != abs(endPosNumber - startPosNumber))
    {
        return -1; // Zug ist nicht Diagonal
    }
    int movementLetter = 1; // bewegung nach Rechts
    int movementNumber = 1; // bewegung nach Unten
    if ((startPosLetter - endPosLetter) > 0)
    { // links
        movementLetter = -1;
    }
    if ((startPosNumber - endPosNumber) > 0)
    {
        movementNumber = -1; // oben
    }

    int squaresToMove = abs(startPosLetter - endPosLetter);
    for (int i = 1; i < squaresToMove; i++)
    {
        if (board[startPosNumber + (i * movementNumber)][startPosLetter + (i * movementLetter)] != XX)
        {
            return -2; // figur ist im Weg.
        }
    }
    return 1;
}

int rook(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{

    if (checkIfLineIsClear(startPosLetter, startPosNumber, endPosLetter, endPosNumber) < 0)
    {
        std::cout << "\n Ihr Zug ist nicht legal.";
        return -1;
    }
    else
    {
        return 1;
    }
}

int queen(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{

    if (checkIfLineIsClear(startPosLetter, startPosNumber, endPosLetter, endPosNumber) == 1 || checkIfDiagonalIsClear(startPosLetter, startPosNumber, endPosLetter, endPosNumber) == 1)
    {
        // Wenn Zug nicht Diagonal oder Grade ist rückgabe -2. Wenn der Zug eine der Zugbedingungen erfüllt wird getestet.
        return 1;
    }
    else
    {
        std::cout << "\n Ihr Zug ist nicht legal.";
        return -1;
    }
}

int bishop(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)

{

    if (checkIfDiagonalIsClear(startPosLetter, startPosNumber, endPosLetter, endPosNumber) == 1)
    {
        return 1;
    }
    else
    {
        std::cout << "\n Ihr Zug ist nicht legal.";
        return -1;
    }
}

int castling(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber){

    int castlingPossible = 0;

    
    if((board[startPosNumber][startPosLetter] == KW) && (startPosNumber == 7) && (startPosLetter == 4)){ //wenn weisser Koenig am Startpunkt steht
        //koenigsseite rochade
        if((endPosNumber == 7) && (endPosLetter == 6) && (board[7][7] == RW) && (board[7][6]== XX) && (board[7][5]== XX)){
            for(int i = 0; i<3; i++){
                if(whiteKingInCheck(i+4, 7) != 0){ // wenn nicht im Schach sind alle Bedingungen fuer eine Rochade erfuellt
                    return 0;
                    
                }
            }
            castlingPossible = 1;
                    board[7][7] = XX; //verschiebt den Turm
                    board[7][5] = RW;
                    return castlingPossible;
        }
        //damenseite rochade
        if((endPosNumber == 7) && (endPosLetter == 2) && (board[7][0] == RW) && (board[7][1]== XX) && (board[7][2]== XX) && (board[7][3]== XX)){
            for(int i = 0; i<4; i++){
                if(whiteKingInCheck(4-i, 7) != 0){ // wenn nicht im Schach sind alle Bedingungen fuer eine Rochade erfuellt
                    return 0;
                }
            }
                    castlingPossible = 1;
                    board[7][0] = XX; //verschiebt den Turm
                    board[7][3] = RW;
                    return castlingPossible;
        } 
    }else if((board[startPosNumber][startPosLetter] == KB) && (startPosNumber == 0) && (startPosLetter == 4)){ //wenn schwarzer Koenig am Startpunkt steht
        //koenigsseite rochade
        if((endPosNumber == 0) && (endPosLetter == 6) && (board[0][7] == RB) && (board[0][6]== XX) && (board[0][5]== XX)){
            for(int i = 0; i<3; i++){
                if(blackKingInCheck(i+4, 0) != 0){ // wenn nicht im Schach sind alle Bedingungen fuer eine Rochade erfuellt
                 return 0;
                }
            }
            castlingPossible = 1;
                    board[0][7] = XX; //verschiebt den Turm
                    board[0][5] = RB;
                    return castlingPossible;
        }  else if((endPosNumber == 0) && (endPosLetter == 2) && (board[0][0] == RB) && (board[0][1]== XX) && (board[0][2]== XX) && (board[0][3]== XX)){
            for(int i = 0; i<4; i++){ //damenseite rochade
                if(blackKingInCheck(4-i, 0) != 0){ // wenn nicht im Schach sind alle Bedingungen fuer eine Rochade erfuellt
                return 0;
                }
            }
                castlingPossible = 1;
                    board[0][0] = XX; //verschiebt den Turm
                    board[0][3] = RB;
                    return castlingPossible;
        }
    }
    return 0;
}
int knight(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{
    int differencePosLetter = abs(startPosLetter - endPosLetter); // betrag von der differenz muss 1 && 2 ergeben
    int differencePosNumber = abs(startPosNumber - endPosNumber);

    if ((differencePosLetter == 1 && differencePosNumber == 2) || (differencePosLetter == 2 && differencePosNumber == 1))
    {
        return 1;
    }
    else
    {
        std::cout << "\n Ihr Zug ist nicht legal.";
        return -1;
    }
}
int king(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{
    if(castling(startPosLetter,startPosNumber,endPosLetter,endPosNumber) == 1){
        return 1;
    }
    
    int differencePosLetter = abs(startPosLetter - endPosLetter);
    int differencePosNumber = abs(startPosNumber - endPosNumber);

    if (differencePosLetter <= 1 && differencePosNumber <= 1)
    {
        return 1;
    }
    else
    {
        std::cout << "\n Ihr Zug ist nicht legal.";
        return -1;
    }
}

int pawnWhite(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{
    if (startPosLetter == endPosLetter && board[endPosNumber][endPosLetter] == XX)
    { // Fürs grade ziehen ohne Schlagen. Finales feld darf nur XX sein
        if ((startPosNumber - endPosNumber) == 1)
        {
            return 1;
        }
        else if ((startPosNumber - endPosNumber) == 2 && board[startPosNumber - 1][startPosLetter] == XX && startPosNumber == 6)
        {
         // Wenn der Bauer auf der Anfangspositon steht und zwei nach vorne geht
            return 1;
        }
        else
        {
            std::cout << "\n Ihr Zug ist nicht legal.";
            return -1;
        }
    }

    if ((startPosNumber - 1 == endPosNumber) && (abs(startPosLetter - endPosLetter) == 1)) // wenn Zug diagonal nach vorne-rechts oder vorne links  ausgeführt wird
    {                                                                                      // geht nur wenn eine Figur geschlagen wird darum testen ob != XX. Eigene Figuren wird schon vorher getestet
        if (board[endPosNumber][endPosLetter] != XX)
        {
            // normales Schlagen
            return 1;    
        }
        if ((board[endPosNumber + 1][endPosLetter] == PB) && (endPosNumber == 2) && board[endPosNumber][endPosLetter] == XX)
        {
            std::cout << "\n Info: En passent geht nur wenn der gegner im letzten Zug mit dem Bauern zwei Felder lief!"; // wenn neben dem gezogenen Bauer eine andere Bauer steht der letzten zug zwei felder ging ist en passent moeglich
            board[endPosNumber + 1][endPosLetter] = XX; //schlaegt denn Bauern hinter endposition
            return 1;
        }
    }
    std::cout << "\nIhr Zug ist nicht legal."; // alle anderen Bauern bewegungen sind illegal
    return -1;
}
int pawnBlack(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber)
{
    if (startPosLetter == endPosLetter && board[endPosNumber][endPosLetter] == XX)
    { // Fürs grade ziehen ohne Schlagen. Finales feld darf nur XX sein
        if ((startPosNumber - endPosNumber) == -1)
        {
            return 1;
        }
        else if ((startPosNumber - endPosNumber) == -2 && board[startPosNumber + 1][startPosLetter] == XX && startPosNumber == 1)
        {
             // Wenn der Bauer auf der Anfangspositon steht und zwei nach vorne geht
            return 1;
        }
        else
        {
    
            return -1;
        }
    }

    if ((startPosNumber + 1 == endPosNumber) && (abs(startPosLetter - endPosLetter) == 1)) // wenn Zug diagonal nach vorne-rechts oder vorne links  ausgeführt wird
    {                                                                                      // geht nur wenn eine Figur geschlagen wird darum testen ob nicht XX. Eigene Figuren wurde schon getestet
        if (board[endPosNumber][endPosLetter] != XX)
        {
            //normales Schlagen
            return 1;
        }
        if ((board[endPosNumber - 1][endPosLetter] == PB) && (endPosNumber == 5) && board[endPosNumber][endPosLetter] == XX)
        {
            // wenn neben dem gezogenen Bauer eine andere Bauer steht der letzten zug zwei felder ging ist en passent moeglich
            board[endPosNumber - 1][endPosLetter] = XX;
            return 1;
        }
    }
    // alle anderen Bauern bewegungen sind illegal
    return -1;
}
int findPositionKings(int &blackKingLetter, int &blackKingNumber, int &whiteKingLetter, int &whiteKingNumber)
{

    for (int i = 0; i <= 7; i++)
    {
        for (int k = 0; k <= 7; k++)
        {
            if (board[i][k] == KW)
            {
                whiteKingLetter = k;
                whiteKingNumber = i;
            }
            if (board[i][k] == KB)
            {
                blackKingLetter = k;
                blackKingNumber = i;
            }
        }
    }
    return 1;
};

bool isInBounds(int row, int col) // return = true wenn row und col innerhalb vom board
{
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

int blackKingInCheck(int blackKingLetter, int blackKingNumber) //return 1 wenn im Schach
{ //Sobald koening im Schach steht wird nicht direkt returnt. Evtl für weitere prüfungen . Wahrscheinlich irrelevant kann ggf geändert werden.
    int i = 1;
    int leftSquares = blackKingLetter;
    int rightSquares = 7 - blackKingLetter;
    int upSquares = blackKingNumber;
    int downSquares = 7 - blackKingNumber;                          // Geben die freien felder an in die jeweilige Richtung
    int leftUpSquares = std::min(blackKingNumber, blackKingLetter); // std::min gibt die niedriger Zahl der beiden zurück
    int rightUpSquares = std::min(blackKingNumber, 7 - blackKingLetter);
    int leftDownSquares = std::min(7 - blackKingNumber, blackKingLetter);
    int rightDownSquares = std::min(7 - blackKingNumber, 7 - blackKingLetter);
    int blackKingInCheck = 0;
    for (i = 1; i <= leftSquares; i++)
    { // for loop Schach von links
        if ((board[blackKingNumber][blackKingLetter - i] == RW) || (board[blackKingNumber][blackKingLetter - i] == QW))
        {
            blackKingInCheck = 1;
            break;
        }
        else if ((board[blackKingNumber][blackKingLetter - i] != XX))
        { // es steht eine Figur im Weg darum kann der König nicht durch Turm oder Dame im Schach sein
            break;
        }
    }
    if (blackKingInCheck != 1)
    {
        for (i = 1; i <= rightSquares; i++)
        { // for loop Schach von Rechts
            if ((board[blackKingNumber][blackKingLetter + i] == RW) || (board[blackKingNumber][blackKingLetter - i] == QW))
            {
                blackKingInCheck = true;
                break;
            }
            else if ((board[blackKingNumber][blackKingLetter + i] != XX))
            { // es steht eine Figur im Weg darum kann der König nicht durch Turm oder Dame im Schach sein
                break;
            }
        }
    }
    if (blackKingInCheck != 1)
    {
        for (i = 1; i <= upSquares; i++)
        { // for loop Schach von Oben
            if ((board[blackKingNumber - i][blackKingLetter] == RW) || (board[blackKingNumber][blackKingLetter - i] == QW))
            {
                blackKingInCheck = 1;
                break;
            }
            else if ((board[blackKingNumber - i][blackKingLetter] != XX))
            { // es steht eine Figur im Weg darum kann der König nicht durch Turm oder Dame im Schach sein
                break;
            }
        }
    }
    if (blackKingInCheck != 1)
    {
        for (i = 1; i <= downSquares; i++)
        { // for loop Schach von Unten
            if ((board[blackKingNumber - i][blackKingLetter] == RW) || (board[blackKingNumber][blackKingLetter - i] == QW))
            {
                blackKingInCheck = 1;
                break;
            }
            else if ((board[blackKingNumber - i][blackKingLetter] != XX))
            { // es steht eine Figur im Weg darum kann der König nicht durch Turm oder Dame im Schach sein
                break;
            }
        }
    }
    if (blackKingInCheck != 1)
    {
        for (i = 1; i <= leftUpSquares; i++)
        { // for loop Schach von Obenlinks
            if ((board[blackKingNumber - i][blackKingLetter - i] == BW) || (board[blackKingNumber][blackKingLetter - i] == QW))
            {
                blackKingInCheck = 1;
                break;
            }
            else if ((board[blackKingNumber - i][blackKingLetter - i] != XX))
            { // es steht eine Figur im Weg darum kann der König nicht durch Läufer oder Dame im Schach sein
                break;
            }
        }
    }
    if (blackKingInCheck != 1)
    {
        for (i = 1; i <= rightUpSquares; i++)
        { // for loop Schach von Obenrechts
            if ((board[blackKingNumber - i][blackKingLetter + i] == BW) || (board[blackKingNumber][blackKingLetter - i] == QW))
            {
                blackKingInCheck = 1;
                break;
            }
            else if ((board[blackKingNumber - i][blackKingLetter + i] != XX))
            { // es steht eine Figur im Weg darum kann der König nicht durch Läufer oder Dame im Schach sein
                break;
            }
        }
    }
    if (blackKingInCheck != 1)
    {
        for (i = 1; i <= leftDownSquares; i++)
        { // for loop Schach von Untenlinks
            if ((board[blackKingNumber + i][blackKingLetter - i] == BW) || (board[blackKingNumber][blackKingLetter - i] == QW))
            {
                blackKingInCheck = 1;
                break;
            }
            else if ((board[blackKingNumber + i][blackKingLetter - i] != XX))
            { // es steht eine Figur im Weg darum kann der König nicht durch Läufer oder Dame im Schach sein
                break;
            }
        }
    }
    if (blackKingInCheck != 1)
    {
        for (i = 1; i <= rightDownSquares; i++)
        { // for loop Schach von Untenrechts
            if ((board[blackKingNumber + i][blackKingLetter + i] == BW) || (board[blackKingNumber][blackKingLetter - i] == QW))
            {
                blackKingInCheck = 1;
                break;
            }
            else if ((board[blackKingNumber + i][blackKingLetter + i] != XX))
            { // es steht eine Figur im Weg darum kann der König nicht durch Läufer oder Dame im Schach sein
                break;
            }
        }
    }
    // Mögliche Züge des Springers idee von ChatGPT
    if(blackKingInCheck != 1){
        int knightMoves[8][2] = {
            {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
    
        for (i = 0; i < 8; i++)
        {
            int newRow = blackKingNumber + knightMoves[i][0];
            int newCol = blackKingLetter + knightMoves[i][1];
    
            if (isInBounds(newRow, newCol))
            {
                if (board[newRow][newCol] == NW)
                { // wenn das feld bestehend aus newRow und newCol innerhalb des board 8x8 liegt und dort NW ist.
                    blackKingInCheck = 1;
                    break; // kein Grund weiter zu prüfen
                }
            }
        }
    }
    
    //Bauern 
    if(blackKingInCheck != 1){
        if (isInBounds(blackKingNumber +1, blackKingLetter -1)){
            if (board[blackKingNumber+1][blackKingLetter-1] == PW){
                blackKingInCheck = 1;
            }
        }
        if (isInBounds(blackKingNumber +1, blackKingLetter +1)){
            if (board[blackKingNumber+1][blackKingLetter+1] == PW){
                blackKingInCheck = 1;
            }
        }


    }




    return blackKingInCheck;
}

int whiteKingInCheck(int whiteKingLetter, int whiteKingNumber) //return 1 wenn im Schach sonst 0
{
    int i = 1;
    int leftSquares = whiteKingLetter;
    int rightSquares = 7 - whiteKingLetter;
    int upSquares = whiteKingNumber;
    int downSquares = 7 - whiteKingNumber;

    int leftUpSquares = std::min(whiteKingNumber, whiteKingLetter);
    int rightUpSquares = std::min(whiteKingNumber, 7 - whiteKingLetter);
    int leftDownSquares = std::min(7 - whiteKingNumber, whiteKingLetter);
    int rightDownSquares = std::min(7 - whiteKingNumber, 7 - whiteKingLetter);

    int whiteKingInCheck = 0;

    // Von links (Turm oder Dame)
    for (i = 1; i <= leftSquares; i++) {
        if (board[whiteKingNumber][whiteKingLetter - i] == RB || board[whiteKingNumber][whiteKingLetter - i] == QB) {
            whiteKingInCheck = 1;
            break;
        } else if (board[whiteKingNumber][whiteKingLetter - i] != XX) break;
    }

    // Von rechts
    if (!whiteKingInCheck) {
        for (i = 1; i <= rightSquares; i++) {
            if (board[whiteKingNumber][whiteKingLetter + i] == RB || board[whiteKingNumber][whiteKingLetter + i] == QB) {
                whiteKingInCheck = 1;
                break;
            } else if (board[whiteKingNumber][whiteKingLetter + i] != XX) break;
        }
    }

    // Von oben
    if (!whiteKingInCheck) {
        for (i = 1; i <= upSquares; i++) {
            if (board[whiteKingNumber - i][whiteKingLetter] == RB || board[whiteKingNumber - i][whiteKingLetter] == QB) {
                whiteKingInCheck = 1;
                break;
            } else if (board[whiteKingNumber - i][whiteKingLetter] != XX) break;
        }
    }

    // Von unten
    if (!whiteKingInCheck) {
        for (i = 1; i <= downSquares; i++) {
            if (board[whiteKingNumber + i][whiteKingLetter] == RB || board[whiteKingNumber + i][whiteKingLetter] == QB) {
                whiteKingInCheck = 1;
                break;
            } else if (board[whiteKingNumber + i][whiteKingLetter] != XX) break;
        }
    }

    // Diagonal: oben links
    if (!whiteKingInCheck) {
        for (i = 1; i <= leftUpSquares; i++) {
            if (board[whiteKingNumber - i][whiteKingLetter - i] == BB || board[whiteKingNumber - i][whiteKingLetter - i] == QB) {
                whiteKingInCheck = 1;
                break;
            } else if (board[whiteKingNumber - i][whiteKingLetter - i] != XX) break;
        }
    }

    // oben rechts
    if (!whiteKingInCheck) {
        for (i = 1; i <= rightUpSquares; i++) {
            if (board[whiteKingNumber - i][whiteKingLetter + i] == BB || board[whiteKingNumber - i][whiteKingLetter + i] == QB) {
                whiteKingInCheck = 1;
                break;
            } else if (board[whiteKingNumber - i][whiteKingLetter + i] != XX) break;
        }
    }

    // unten links
    if (!whiteKingInCheck) {
        for (i = 1; i <= leftDownSquares; i++) {
            if (board[whiteKingNumber + i][whiteKingLetter - i] == BB || board[whiteKingNumber + i][whiteKingLetter - i] == QB) {
                whiteKingInCheck = 1;
                break;
            } else if (board[whiteKingNumber + i][whiteKingLetter - i] != XX) break;
        }
    }

    // unten rechts
    if (!whiteKingInCheck) {
        for (i = 1; i <= rightDownSquares; i++) {
            if (board[whiteKingNumber + i][whiteKingLetter + i] == BB || board[whiteKingNumber + i][whiteKingLetter + i] == QB) {
                whiteKingInCheck = 1;
                break;
            } else if (board[whiteKingNumber + i][whiteKingLetter + i] != XX) break;
        }
    }

    // Springer (NB = Knight Black)
    if (!whiteKingInCheck) {
        int knightMoves[8][2] = {
            {2, 1}, {1, 2}, {-1, 2}, {-2, 1},
            {-2, -1}, {-1, -2}, {1, -2}, {2, -1}};
        for (i = 0; i < 8; i++) {
            int newRow = whiteKingNumber + knightMoves[i][0];
            int newCol = whiteKingLetter + knightMoves[i][1];
            if (isInBounds(newRow, newCol)) {
                if (board[newRow][newCol] == NB) {
                    whiteKingInCheck = 1;
                    break;
                }
            }
        }
    }

    // Bauern (PB greifen "nach oben", also -1 Zeile)
    if (!whiteKingInCheck) {
        if (isInBounds(whiteKingNumber - 1, whiteKingLetter - 1)) {
            if (board[whiteKingNumber - 1][whiteKingLetter - 1] == PB) {
                whiteKingInCheck = 1;
            }
        }
        if (isInBounds(whiteKingNumber - 1, whiteKingLetter + 1)) {
            if (board[whiteKingNumber - 1][whiteKingLetter + 1] == PB) {
                whiteKingInCheck = 1;
            }
        }
    }
    

    return whiteKingInCheck;
}

int isKingNextToKing(int whiteKingLetter, int whiteKingNumber, int blackKingLetter, int blackKingNumber){ //return 1 wenn koenige nebeneinander
    int diffLetter = abs(whiteKingLetter - blackKingLetter);
    int diffNumber = abs(whiteKingNumber - blackKingNumber);
    if (diffLetter < 2 && diffNumber < 2){ //
        return 1; 
    }else{
        return 0;
    }
}
int checkLegalPieceMovement(int startPosLetter, int startPosNumber, int endPosLetter, int endPosNumber) //return >= 1 dann legale bewegungen
{

    if (checkStartAndEndPosition(startPosLetter, startPosNumber, endPosLetter, endPosNumber) < 1)
    {
        return -1;
    }

    PieceType movingPiece = board[startPosNumber][startPosLetter];
    if (movingPiece == PB)
    {
        
        return pawnBlack(startPosLetter, startPosNumber, endPosLetter, endPosNumber);
    }
    else if (movingPiece == PW)
    {
        
        return pawnWhite(startPosLetter, startPosNumber, endPosLetter, endPosNumber);
    }
    else if (movingPiece == RB || movingPiece == RW)
    {
        
        return rook(startPosLetter, startPosNumber, endPosLetter, endPosNumber);
    }
    else if (movingPiece == NB || movingPiece == NW)
    {
        return knight(startPosLetter, startPosNumber, endPosLetter, endPosNumber);
    }
    else if (movingPiece == BB || movingPiece == BW)
    {
        return  bishop(startPosLetter, startPosNumber, endPosLetter, endPosNumber);
    }
    else if (movingPiece == QB || movingPiece == QW)
    {
        return queen(startPosLetter, startPosNumber, endPosLetter, endPosNumber);

    }
    else if (movingPiece == KB || movingPiece == KW)
    {

        return  king(startPosLetter, startPosNumber, endPosLetter, endPosNumber);
    }
    else
    {
        return -1;
    }
}

int main(int argc, char *argv[])
{
    PieceType backUpBoard[8][8];
    int blackKingLetter, blackKingNumber, whiteKingLetter, whiteKingNumber; // pos
    int startPosLetter, startPosNumber, endPosLetter, endPosNumber; //positionen fuer die Bewegung
    findPositionKings(blackKingLetter, blackKingNumber, whiteKingLetter, whiteKingNumber);
    if(checkPiecesAreIllegal(board, blackKingLetter, blackKingNumber, whiteKingLetter, whiteKingNumber)== true){
        std::cout << "\nIhre Stellung ist nicht moeglich, bitte aendern Sie diese im Code!";
        return 0;
    }
    int breakLoop = 1;

    while (breakLoop == 1)
    {
        printBoard(board);
        copyBoard (board, backUpBoard); //kopiert board in backUpBoard fals neue Position auf Board nichtnmoeglich sind
        while (getNotationInput(startPosLetter, startPosNumber, endPosLetter, endPosNumber) != true)
        { // leauft solange bis korrekter input gegeben wurde
            std::cout << "\n\nGeben Sie ihren Zug erneut ein!\n";
        }
        if(checkLegalPieceMovement(startPosLetter, startPosNumber, endPosLetter, endPosNumber) < 1){
            std::cout << "\n\nGeben Sie eine Ihrer Figur entsprechenden Bewegung ein!\n";
            continue;
        }; // uberprüft nur ob die bewegungen legal sind ohne Schachgebote zu beachten

        //jetzt wird das Brett aktualisiert und geguckt ob die neue Position in hinsicht der Schachgebote legal sind.
        int colorMovingPiece = returnColor(startPosLetter, startPosNumber); // color =  1 -> weiss; color = 2 -> schwarz; color = 3 -> keine Figur */
        PieceType movingPieceType = board[startPosNumber][startPosLetter]; //Das board wird angepasst. Da die Funktionemit der Globalen variable board[][] geschriebne wurde muss das selbe verwendet werden
        board[startPosNumber][startPosLetter] = XX;
        board[endPosNumber][endPosLetter] = movingPieceType; //geschlagener en passant Bauer wird ggf in der Funktion black/whitePawn auf XX gesetzt
        
        //neue Koenig Position muessen angepasst werden Fals er bewegt wurde
        int newBlackKingNumber = blackKingNumber;
        int newWhiteKingNumber = whiteKingNumber;
        int newBlackKingLetter = blackKingLetter;
        int newWhiteKingLetter = whiteKingLetter;

        if(movingPieceType ==  KB || movingPieceType == KW){ //wenn koenig bewegt wurde, neue Position der Koenige in eigenen Variablen speichern
            findPositionKings(newBlackKingLetter, newBlackKingNumber, newWhiteKingLetter,newWhiteKingNumber );
        }

        if (colorMovingPiece == 1 && whiteKingInCheck(newWhiteKingLetter, newWhiteKingNumber) == 1)
        {
            printf("\nDer Zug ist nicht moeglich, Weiss steht nach dem Zug im Schach.");
            copyBoard(backUpBoard, board); //wenn nicht moeglich wieder alte Position verwenden
        }else if((colorMovingPiece == 2 && blackKingInCheck(newBlackKingLetter, newBlackKingNumber) == 1)){
            copyBoard(backUpBoard, board); //wenn nicht moeglich wieder alte Position verwenden
            printf("\nDer Zug ist nicht moeglich, Schwarz steht nach dem Zug im Schach.");
        }
        else if(isKingNextToKing( newWhiteKingLetter, newWhiteKingNumber, newBlackKingLetter, newBlackKingNumber) == 1){
            copyBoard(backUpBoard, board); //wenn nicht moeglich wieder alte Position verwenden
            printf("\nDieser Zug ist Illegal da beide Koenige nicht nebeneinander stehen duerfen!");
        };

        printBoard(board);


        
        std::cout << "\nWillst du fortfahren gib 1 ein.";
        std::cin >> breakLoop;
    }

    return 0;
}

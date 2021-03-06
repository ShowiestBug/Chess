#include "Screen.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Board.h"
#include <algorithm>

using namespace std;

Screen::Screen() : m_window(NULL), m_renderer(NULL), m_texture(NULL), m_buffer1(NULL), selectedSquare(-1), prevSquare(-1), mouseClick(false), squareHolded(-1), avaiableMoves(30,-1) {
    updateRectBoard.h = 100;
    updateRectBoard.w = 100;
    updateRectPiece.h = 80;
    updateRectPiece.w = 80;
}

Screen::~Screen() {

}

bool Screen::init() {

    // Screen initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    m_window = SDL_CreateWindow("Chess", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (m_window == NULL) {
        SDL_Quit;
        return false;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);

    if (m_renderer == NULL) {
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
    tempTexture1 = SDL_CreateTexture(m_renderer,SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 100, 100);

    if (tempTexture1 == NULL)   {
        cout << "Cannot create temp Texture!" << endl;
    }
    if (m_texture == NULL) {
        SDL_DestroyRenderer(m_renderer);
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "Failed to initialise SDL_image for PNG" << endl;
        return false;
    }
    // Setting the board
    SetBoard();
    return true;
}
void Screen::close() {
    delete[] m_buffer1;
    delete[] tempBuffer;
    SDL_DestroyTexture(bB);
    SDL_DestroyTexture(bQ);
    SDL_DestroyTexture(bK);
    SDL_DestroyTexture(bP);
    SDL_DestroyTexture(bR);
    SDL_DestroyTexture(bN);
    SDL_DestroyTexture(wB);
    SDL_DestroyTexture(wQ);
    SDL_DestroyTexture(wK);
    SDL_DestroyTexture(wP);
    SDL_DestroyTexture(wR);
    SDL_DestroyTexture(wN);
    SDL_DestroyTexture(m_texture);
    SDL_DestroyTexture(tempTexture1);
    SDL_DestroyTexture(tempTexture2);
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
void Screen::clear() {
    memset(m_buffer1, 0, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
    update();
}
void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
    if (x < 0 || x > SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT) {
        return;
    }
    m_buffer1[(y * SCREEN_WIDTH) + x] = Color(red,green,blue);
}

Uint32 Screen::Color(Uint8 r, Uint8 g, Uint8 b) {
    Uint32 color = 0;
    color += r;
    color <<= 8;
    color += g;
    color <<= 8;
    color += b;
    color <<= 8;
    color += 0xFF;
    return color;
    //Orange 255,151,53
    //Green 80,151,53
    //Marron 128,0,0
    //Grey 191,191,191
}
void Screen::update() {
    SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    SDL_RenderPresent(m_renderer);
}

void Screen::SetBoard() {
    tempBuffer = new Uint32[10000];
    Uint32 g = Color(191,191,191);
    Uint32 b = Color(0,0,0);
    for (int i = 0; i < 10000; i++) {
        if((0 <= i && i < 100) || (9900 <= i && i < 10000) || (i % 100 == 0) || ((i+1) % 100 == 0) ) {
            tempBuffer[i] = b;
        }
        else {
            tempBuffer[i] = g;
        }
        
    }
    //m_buffer1[(y * SCREEN_WIDTH) + x] = Color(red,green,blue);

    //Graphical initialization of the board
    m_buffer1 = new Uint32[SCREEN_HEIGHT * SCREEN_WIDTH];
    Uint32 col = Color(80,151,53);
    for (int i = 0; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++) {
        m_buffer1[i] = col;
    }
    // Maroon borders
    for (int i = 10; i < 821; i += 10) {
        drawSquare(i,10,10,128,0,0);
        drawSquare(i,820,10,128,0,0);
    }
    for (int i = 20; i < 820; i += 10) {
        drawSquare(10,i,10,128,0,0);
        drawSquare(820,i,10,128,0,0);
    }

    //Board squares
    for(int i = 20; i < SCREEN_WIDTH; i += 200) {
        for(int j = 20; j < SCREEN_HEIGHT; j += 200) {
            drawSquare(i,j,100,255,151,53);
        }
    }
    for(int i = 120; i < SCREEN_WIDTH; i += 200) {
        for(int j = 120; j < SCREEN_HEIGHT; j += 200) {
            drawSquare(i,j,100,255,151,53);
        }
    }
    SDL_UpdateTexture(m_texture, NULL, m_buffer1, SCREEN_WIDTH * sizeof(Uint32));
    SDL_UpdateTexture(tempTexture1, NULL, tempBuffer, 100 * sizeof(Uint32));
    SDL_RenderClear(m_renderer);
    SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
    loadPNG();
    renderPieces();
    SDL_RenderPresent(m_renderer);
}

void Screen::drawSquare(int x, int y, int size, Uint8 red, Uint8 green, Uint8 blue) {
    if (x < 0 || x+size > SCREEN_WIDTH || y < 0 || y+size >= SCREEN_HEIGHT || size < 0) {
        return;
    }
    else {
        for (int i = x; i < x + size; i++)  {
            for (int j = y; j < y + size; j++) {
                setPixel(i,j,red,green,blue);
            }
        }   
    }
}

void Screen::updateSelection(int x, int y) {
    prevSquare = selectedSquare;
    selectedSquare = board.returnSpot(x,y);
    if(selectedSquare != -1) {
        if(selectedSquare != prevSquare) {
            if (!mouseClick) {
                drawSquare2(board.panel[selectedSquare].x,board.panel[selectedSquare].y,1);
                drawPiece(updateRectBoard.x,updateRectBoard.y,getPieceTexture(board.panel[selectedSquare].occpuiedBy));
            
                if(prevSquare != -1) {
                    drawSquare2(board.panel[prevSquare].x,board.panel[prevSquare].y,2);
                    drawPiece(updateRectBoard.x,updateRectBoard.y,getPieceTexture(board.panel[prevSquare].occpuiedBy));
                }
            }
            else {
                if(inMoveRange(selectedSquare)) {
                    drawSquare2(board.panel[selectedSquare].x,board.panel[selectedSquare].y,1);
                    drawPiece(board.panel[selectedSquare].x,board.panel[selectedSquare].y,getPieceTexture(board.panel[squareHolded].occpuiedBy));
                }

                if(inMoveRange(prevSquare)) {
                    drawSquare2(board.panel[prevSquare].x,board.panel[prevSquare].y,1);
                    if(board.panel[prevSquare].occpuiedBy != 0) {
                        drawPiece(board.panel[prevSquare].x,board.panel[prevSquare].y,getPieceTexture(board.panel[prevSquare].occpuiedBy));
                    }
                }
            }
            SDL_RenderPresent(m_renderer);
        }
    }
    else if(selectedSquare == -1 && prevSquare != -1) {
        if(!mouseClick) {
            drawSquare2(board.panel[prevSquare].x,board.panel[prevSquare].y,2);
            drawPiece(updateRectBoard.x,updateRectBoard.y,getPieceTexture(board.panel[prevSquare].occpuiedBy));  
        }
        else {
            if(inMoveRange(prevSquare)) {
                drawSquare2(board.panel[prevSquare].x,board.panel[prevSquare].y,1);
            }
        }
        SDL_RenderPresent(m_renderer);
    }
}

void Screen::movePiece(int x, int y) {
    int tempSpot = board.returnSpot(x,y);
    if(tempSpot != -1) {
        if(mouseClick) {
            if(inMoveRange(tempSpot) || tempSpot == squareHolded) {
                //delete highlight of all moves
                for(int i = 0; i < 30; i++) {
                    if(avaiableMoves[i] != -1) {
                        if(avaiableMoves[i] != tempSpot) {
                            drawSquare2(board.panel[avaiableMoves[i]].x, board.panel[avaiableMoves[i]].y,2);
                            if(board.panel[avaiableMoves[i]].occpuiedBy != 0 ) {
                                drawPiece(board.panel[avaiableMoves[i]].x, board.panel[avaiableMoves[i]].y, getPieceTexture(board.panel[avaiableMoves[i]].occpuiedBy));
                            }
                        }
                        avaiableMoves[i] = -1;
                    }
                    else {
                        break;
                    }
                }

                if (tempSpot != squareHolded) {
                    //draw/set new square
                    // drawSquare2(board.panel[tempSpot].x,board.panel[tempSpot].y,1);
                    // drawPiece(board.panel[tempSpot].x,board.panel[tempSpot].y,getPieceTexture(board.panel[squareHolded].occpuiedBy));
                    board.panel[tempSpot].occpuiedBy = board.panel[squareHolded].occpuiedBy;

                    //overdraw/erase old square
                    drawSquare2(board.panel[squareHolded].x,board.panel[squareHolded].y,2);
                    board.panel[squareHolded].occpuiedBy = 0;    
                }

                //render all
                SDL_RenderPresent(m_renderer);

                //set piece holded to 0/null
                squareHolded = -1;
                mouseClick = !mouseClick;   
            }
        }
        else if(board.panel[tempSpot].occpuiedBy != 0) {
            squareHolded = tempSpot;
            fitMoves(board.avaiableMoves(squareHolded,board.panel[squareHolded].occpuiedBy));
            for(int i = 0; i < 30; i++) {
                if(avaiableMoves[i] < 0  || avaiableMoves[i] > 63) {
                    if(avaiableMoves[i] != -1) {
                        cout << "Nonposible move" << endl;
                    }
                    break;
                }
                else {
                    cout << avaiableMoves[i] << ' ';
                    drawSquare2(board.panel[avaiableMoves[i]].x, board.panel[avaiableMoves[i]].y,1);
                    if(board.panel[avaiableMoves[i]].occpuiedBy != 0 ) {
                        drawPiece(board.panel[avaiableMoves[i]].x, board.panel[avaiableMoves[i]].y, getPieceTexture(board.panel[avaiableMoves[i]].occpuiedBy));
                    }
                }
            }
            cout << endl;
            SDL_RenderPresent(m_renderer);
            mouseClick = !mouseClick;
        }
    }
}

void Screen::drawPiece(int x, int y, SDL_Texture *texture) {
    if (texture == NULL) {
        return;
    }
    else {
        updateRectPiece.x = x + 10;
        updateRectPiece.y = y + 10;
        SDL_RenderCopy(m_renderer,texture,NULL,&updateRectPiece);
    }
}

// 1 - gray, 2 - orange/green
void Screen::drawSquare2(int x, int y, int type) {

    updateRectBoard.x = x;
    updateRectBoard.y = y;
    if (type == 1) { // draw gray square
        SDL_RenderCopy(m_renderer, tempTexture1, NULL, &updateRectBoard);
    }
    else { // draw orange/green square
        SDL_RenderCopy(m_renderer, m_texture, &updateRectBoard, &updateRectBoard);
    }
}

SDL_Texture* Screen::getPieceTexture(int type) {
    switch (type)
    {
    case 1:
        return wK;
        break;
    case 2:
        return wQ;
        break;
    case 3:
        return wB;
        break;
    case 4:
        return wN;
        break;
    case 5:
        return wR;
        break;
    case 6:
        return wP;
        break;
    case -1:
        return bK;
        break;  
    case -2:
        return bQ;
        break;
    case -3:
        return bB;
        break;
    case -4:
        return bN;
        break;
    case -5:
        return bR;
        break;
    case -6:
        return bP;
        break;  
    default:
        return NULL;
        break;
    }
}

void Screen::renderPieces() {

    //rendering black pieces
    SDL_Rect m_destination;
    m_destination.h = 80;
    m_destination.w = 80;

    m_destination.x = 30;
    m_destination.y = 30;
    SDL_RenderCopy(m_renderer, bR, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bN, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bB, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bQ, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bK, NULL, &m_destination);
    
    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bB, NULL, &m_destination);
    
    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bN, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bR, NULL, &m_destination);

    m_destination.x = 30;
    m_destination.y = 130;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, bP, NULL, &m_destination);


    //rendering white pieces
    m_destination.x = 30;
    m_destination.y = 730;
    SDL_RenderCopy(m_renderer, wR, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wN, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wB, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wQ, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wK, NULL, &m_destination);
    
    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wB, NULL, &m_destination);
    
    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wN, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wR, NULL, &m_destination);

    m_destination.x = 30;
    m_destination.y = 630;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);

    m_destination.x += 100;
    SDL_RenderCopy(m_renderer, wP, NULL, &m_destination);
}

void Screen::loadPNG() {
    SDL_Surface *imageSurface1 = IMG_Load("pngs/bB.png");
    bB = SDL_CreateTextureFromSurface(m_renderer, imageSurface1);

    SDL_Surface *imageSurface2 = IMG_Load("pngs/bK.png");
    bK = SDL_CreateTextureFromSurface(m_renderer, imageSurface2);

    SDL_Surface *imageSurface3 = IMG_Load("pngs/bN.png");
    bN = SDL_CreateTextureFromSurface(m_renderer, imageSurface3);

    SDL_Surface *imageSurface4 = IMG_Load("pngs/bP.png");
    bP = SDL_CreateTextureFromSurface(m_renderer, imageSurface4);

    SDL_Surface *imageSurface5 = IMG_Load("pngs/bQ.png");
    bQ = SDL_CreateTextureFromSurface(m_renderer, imageSurface5);

    SDL_Surface *imageSurface6 = IMG_Load("pngs/bR.png");
    bR = SDL_CreateTextureFromSurface(m_renderer, imageSurface6);

    SDL_Surface *imageSurface7 = IMG_Load("pngs/wB.png");
    wB = SDL_CreateTextureFromSurface(m_renderer, imageSurface7);

    SDL_Surface *imageSurface8 = IMG_Load("pngs/wK.png");
    wK = SDL_CreateTextureFromSurface(m_renderer, imageSurface8);

    SDL_Surface *imageSurface9 = IMG_Load("pngs/wN.png");
    wN = SDL_CreateTextureFromSurface(m_renderer, imageSurface9);

    SDL_Surface *imageSurface10 = IMG_Load("pngs/wP.png");
    wP = SDL_CreateTextureFromSurface(m_renderer, imageSurface10);

    SDL_Surface *imageSurface11 = IMG_Load("pngs/wQ.png");
    wQ = SDL_CreateTextureFromSurface(m_renderer, imageSurface11);

    SDL_Surface *imageSurface12 = IMG_Load("pngs/wR.png");
    wR = SDL_CreateTextureFromSurface(m_renderer, imageSurface12);
   
    if(imageSurface1 == NULL || imageSurface2 == NULL || 
    imageSurface3 == NULL || imageSurface4 == NULL || 
    imageSurface5 == NULL || imageSurface6 == NULL || 
    imageSurface7 == NULL || imageSurface8 == NULL || 
    imageSurface9 == NULL || imageSurface10 == NULL || 
    imageSurface11 == NULL || imageSurface12 == NULL) {
        cout << "Problem with loading pngs" << endl;
        return;
    }

    SDL_FreeSurface(imageSurface1);
    SDL_FreeSurface(imageSurface2);
    SDL_FreeSurface(imageSurface3);
    SDL_FreeSurface(imageSurface4);
    SDL_FreeSurface(imageSurface5);
    SDL_FreeSurface(imageSurface6);
    SDL_FreeSurface(imageSurface7);
    SDL_FreeSurface(imageSurface8);
    SDL_FreeSurface(imageSurface9);
    SDL_FreeSurface(imageSurface10);
    SDL_FreeSurface(imageSurface11);
    SDL_FreeSurface(imageSurface12);
}

void Screen::fitMoves(vector<int> moves) {
    for(int i = 0; i < moves.size(); i++) {
        avaiableMoves[i] = moves[i];
    }
}

bool Screen::inMoveRange(int square) {
    for(int i = 0; i < 30; i++) {
        if(square == avaiableMoves[i] && square != -1)
            return true;
    }
    return false;
}
